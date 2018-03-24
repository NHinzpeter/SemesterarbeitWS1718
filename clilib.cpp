#include <iostream>
using namespace std;
#include <fstream>
#include <string.h>
#include <istream>
#include "ntoken3.h"

ClElement *verarbeite(ifstream& datei);

bool read(ClToken *token, Cltxt *txt) {
    ClElement *jetzt=NULL, *wurzel;
    ifstream eingabe;
    bool gueltig=true;

    eingabe.open("daten.dtd");
    wurzel=verarbeite(eingabe);
    jetzt=wurzel;
    eingabe.close();

    eingabe.open("daten.xml");
    token->laden(eingabe,NULL,wurzel,&gueltig);
    if (gueltig==false) return false;
    eingabe.close();

    eingabe.open("daten.txt");
    txt->laden(eingabe);
    eingabe.close();
    return true;
}

void menu() {
    cout << endl << "W\x84hlen sie einen Men\x81punkt:" << endl << "(K)onvertieren" << endl << "(S)uchen"<< endl << "(E)xportieren" << endl << "(V)erlassen" << endl;
}


void convert(ClToken *token, Cltxt *txt) {
    Cltxt *aktuell=txt;




    //Hinzufügen der Einsätze und Tore in den Tokenbaum
    if (!strcmp(token->name(),"Spieler")) {
        //Suchfunktion auf Basis der Trikotnummern, um die Daten aus der txt den Spielern zuzuordnen
        while (strcmp(aktuell->gibNrChar(),token->child()->inhalt())) {
            if (aktuell->gibnaechstes()==NULL) break;
            aktuell=aktuell->gibnaechstes();
        }
    string s = to_string(aktuell->gibEinsaetze());                          //Umwandlung des Integers in eine char const mit Hilfe von string.h
    char const *pchar = s.c_str();
    token->child()->sibling()->sibling()->addSibling("Einsaetze",pchar);

    s = to_string(aktuell->gibTore());                                      //Umwandlung des Integers in eine char const mit Hilfe von string.h
    pchar = s.c_str();
    token->child()->sibling()->sibling()->sibling()->addSibling("Tore",pchar);

    }


    if (token->child()!=NULL) convert(token->child(), txt);

    if (token->sibling()!=NULL) convert(token->sibling(), txt);

}

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
    if ((!category1.empty() || !keyword1.empty()) && (!category2.empty() || !keyword2.empty())) {
        cout << "Wie sollen die Paare verknüpft werden?" << endl << "(U)nd" << endl << "(O)der" << endl;
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
            cout << "Bitte geben sie den entsprechenden Grossbuchstaben an." << endl;
            getline(cin, verkn);
            if (verkn.empty()) verkn=" ";
            break;
        }
    }
    token->sucheXYZ(&category1, &keyword1, &gefunden, &category2, &keyword2, &UND);
    if (gefunden==false)
        cout << "Es wurde kein Datensatz mit der gesuchten Kombination gefunden." << endl;
}

void exportieren(ClToken *token){
    string pfad;
    string dateiname;
    string modus;
    cout << "Bitte geben sie einen Dateinamen an. Beispiel: ausgabe.xml" << endl;
    getline(cin, dateiname);
    pfad="../../"+dateiname;
    ofstream ausgabe (pfad);
    while(!(modus[0]=='1' || modus[0]=='2')) {
        cout << "Was soll exportiert werden?" << endl << "(1)Der gesamte XML-Baum" << endl << "(2)Die letzte Suchanfrage" << endl;
        getline(cin, modus);
        if (modus[0]=='1') token->exportiere(0, 0, ausgabe);
        else if (modus[0]=='2') token->exportiere(0, 1, ausgabe);
    }
    cout << endl << dateiname << " wurde erfolgreich erstellt." << endl;
}
