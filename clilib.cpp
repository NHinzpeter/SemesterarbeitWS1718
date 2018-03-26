#include <iostream>
using namespace std;
#include <fstream>
#include <string.h>
#include <istream>
#include "ntoken3.h"

ClElement *verarbeite(ifstream& datei);


//liest die drei Dateien ein und speichert sie in Objekten. Startprozedur, die nur einmal ausgeführt werden muss.
//Gibt false zurück, wenn beim Einlesen der Dateien ein Fehler aufgetreten ist.
bool read(ClToken *token, Cltxt *txt) {
    ClElement *wurzel;
    ifstream eingabe;
    bool gueltig=true;

    //laden und verarbeiten der DTD
    eingabe.open("daten.dtd");
    if (eingabe.is_open()) wurzel=verarbeite(eingabe);
    else {
        cout << "DTD-Datei konnte nicht ge\x94ffnet werden." << endl;
        gueltig=false;
    }
    eingabe.close();

    //laden und verarbeiten der XML
    eingabe.open("daten.xml");
    if (eingabe.is_open()) gueltig=token->laden(eingabe,NULL,wurzel);
    else {
        cout << "XML-Datei konnte nicht ge\x94ffnet werden." << endl;
        gueltig=false;
    }
    eingabe.close();

    //laden und verarbeiten der TXT
    eingabe.open("daten.txt");
    if (eingabe.is_open()) txt->laden(eingabe);
    else {
        cout << "TXT-Datei konnte nicht ge\x94ffnet werden." << endl;
        gueltig=false;
    }
    eingabe.close();

    if (gueltig==false) return false;

    return true;
}


//Gibt das Menü aus
void menu() {
    cout << endl << "W\x84hlen sie einen Men\x81punkt:" << endl << "(K)onvertieren" << endl << "(S)uchen"<< endl << "(E)xportieren" << endl << "(B)erechnen" << endl << "(V)erlassen" << endl;
}


//Konvertierung von XML und TXT, rekursiv
void convert(ClToken *token, Cltxt *txt) {
    Cltxt *aktuell=txt;

    if (!strcmp(token->name(),"Spieler")) {
        //Suchfunktion auf Basis der Trikotnummern, um die Daten aus der txt den Spielern zuzuordnen
        while (strcmp(aktuell->gibNrChar(),token->child()->inhalt())) {
            if (aktuell->gibnaechstes()==NULL) break;
            aktuell=aktuell->gibnaechstes();
        }

        //Wenn der richtige Spieler gefunden wurde, werden mit addSibling() die Einsätze und Tore in den Token-Baum hinzugefügt
        string s = to_string(aktuell->gibEinsaetze());                          //Umwandlung des Integers in eine char const mit Hilfe von string.h
        char const *pchar = s.c_str();
        token->child()->sibling()->sibling()->addSibling("Einsaetze",pchar);

        s = to_string(aktuell->gibTore());                                      //Umwandlung des Integers in eine char const mit Hilfe von string.h
        pchar = s.c_str();
        token->child()->sibling()->sibling()->sibling()->addSibling("Tore",pchar);

    }

    //Rekursion
    if (token->child()!=NULL) convert(token->child(), txt);
    if (token->sibling()!=NULL) convert(token->sibling(), txt);

}


//Liest die zwei Paare aus Kategorie und Begriff ein und fragt ggf nach dem logischen Operator, mit denen die Paare verknüpft werden sollen
//Ruft anschließend die Methode sucheXYZ von ClToken auf
//Wenn nichts gefunden wurde, gibt die Funktion eine entsprechende Meldung aus.
void suchen(ClToken *token) {
    string category1, keyword1, category2, keyword2, verkn;
    bool gefunden=false, UND=false;
    cout << "Suchen sie nach bis zu zwei Paaren aus Kategorie und Begriff." << endl << "Sie k\x94nnen bis zu drei Felder leer lassen." << endl;
    cout << "Kategorie: ";
    getline(cin, category1);
    cout << "Begriff: ";
    getline(cin, keyword1);
    cout << "Kategorie 2: ";
    getline(cin, category2);
    cout << "Begriff 2: ";
    getline(cin, keyword2);

    //Wenn mindestens eine Bedingung in jedem Paar angegeben wurde, wird nach einer Verknüpfung gefragt.
    //Anschließend wird die Eingabe ausgewertet.
    if ((!category1.empty() || !keyword1.empty()) && (!category2.empty() || !keyword2.empty())) {
        cout << "Wie sollen die Paare verknüpft werden?" << endl << "(U)nd" << endl << "(O)der" << endl;
        while (!(verkn[0]=='U' || verkn[0]=='O')){
            getline(cin, verkn);
            if (verkn.empty()) verkn=" ";
            switch (verkn[0]) {
            case 'U':
                UND=true;
                break;
            case 'O':
                UND=false;
                break;
            default:
                cout << "Bitte geben sie den entsprechenden Gro\xE1buchstaben an." << endl;
                break;
            }
        }
    }

    token->sucheXYZ(&category1, &keyword1, &gefunden, &category2, &keyword2, &UND);

    if (gefunden==false)
        cout << "Es wurde kein Datensatz mit der gesuchten Kombination gefunden." << endl;
}


//liest einen Dateinamen und den gewünschten Modus ein, erstellt den ofstream
//ruft anschließend die Methode exportiere() auf und gibt am Ende eine Bestätigung aus.
void exportieren(ClToken *token){
    string pfad;
    string dateiname;
    string modus;

    cout << "Bitte geben sie einen Dateinamen an. Beispiel: ausgabe.xml" << endl;
    getline(cin, dateiname);
    pfad="../"+dateiname;
    ofstream ausgabe (pfad);

    while(!(modus[0]=='1' || modus[0]=='2')) {
        cout << "Was soll exportiert werden?" << endl << "(1)Der gesamte XML-Baum" << endl << "(2)Die letzte Suchanfrage" << endl;
        getline(cin, modus);
        if (modus[0]=='1') token->exportiere(0, 0, ausgabe);
        else if (modus[0]=='2') token->exportiere(0, 1, ausgabe);
    }

    cout << endl << dateiname << " wurde erfolgreich erstellt." << endl;
}


//fragt zunächst nach dem gewünschten Modus und anschließend nach den Spielernummern, deren Werte addiert werden sollen.
//Diese Eingabe wird geparsed und in ein Character Array geschrieben.
//Nachdem das Ergebnis durch die Methode berechne() zurückgegeben wurde, wird gefragt, ob dieses Ergebnis exportiert werden soll.
//Falls ja, wird nach einem Dateinamen gefragt, ein ofstream erstellt und das Ergebnis mitsamt der betroffenen Spielernamen durch exportiereNamen() in die Datei geschrieben.
void berechnen(ClToken *token, Cltxt *txt){
    string eingabe, pfad;
    bool modus;
    char nrn[23][3];
    int anzahlSpieler=0, ergebnis=0;

    //Bestimmung des Modus
    while(!(eingabe[0]=='1' || eingabe[0]=='2')) {
    cout << "Was m""\x94""chten sie addieren? (1) Eins\x84tze oder (2) Tore" << endl;
    getline(cin, eingabe);
    }
    if (eingabe[0]=='1') modus=false;
    else modus=true;

    eingabe.clear();

    //Einlesen der Trikotnummern
    while (eingabe.empty()){
    cout << "Die Werte welcher Spieler sollen addiert werden?" << endl << "Geben sie die 2-Stelligen Trikotnummern an und trennen sie diese durch ein Leerzeichen." << endl << "Beispiel: 01 02 03 04" << endl;
    getline(cin, eingabe);
    }

    //Parsing der Eingabe
    for (int i=0; i<=eingabe.length()/3 && i<23; i++){
        nrn[i][0]=eingabe[i*3];
        nrn[i][1]=eingabe[i*3+1];
        nrn[i][2]='\0';
        anzahlSpieler++;
    }

    //Addition der Werte
    txt->berechne(nrn, &anzahlSpieler, &modus, &ergebnis);
    cout << endl << "Ergebnis: " << ergebnis << endl << endl;

    //ggf Ausgabe
    eingabe.clear();
    cout << "Soll das berechnete Ergebnis exportiert werden? (J)a oder (N)ein" << endl;
    while (!(eingabe[0]=='J' || eingabe[0]=='N')) getline(cin, eingabe);
    if (eingabe[0]=='J'){
        eingabe.clear();
        cout << endl << "Geben sie einen Dateinamen an. Beispiel: Summe.txt" << endl;
        while (eingabe.empty()) getline(cin, eingabe);
        pfad="../"+eingabe;
        ofstream ausgabe(pfad);
        ausgabe << "Summe der ";
        if (modus==false) ausgabe << "Einsätze von ";
        else ausgabe << "Tore von ";
        token->exportiereNamen(nrn, ausgabe, &anzahlSpieler);
        ausgabe << endl << ">>>" << ergebnis << "<<<";
        cout << "Das Ergebnis wurde erfolgreich exportiert." << endl;
    }
}
