#include <iostream>
using namespace std;
#include <fstream>
#include <istream>

#include "ntoken3.h"

bool read(ClToken *token, Cltxt *txt);              //liest die drei Dateien ein und speichert sie in Objekten; Startprozedur, die nur einmal ausgeführt werden muss.
void convert(ClToken *token, Cltxt *txt);
void suchen(ClToken *token);
void exportieren(ClToken *token);
void menu();

int main()
{
    /* VORWORT
     * Übernahme der Tokenklasse, 'Datensätze' im Sinne der Aufgabenstellung sind Token mit dem Namen 'Spieler', inklusive aller Token, die in der Hierarchie des imaginären Baumes unter diesen stehen (Spielernummer, Name, usw).
     * Zunächst wird die txt-Datei in eine eigene Klasse eingelesen, durch die Konvertierung werden die Daten aus dieser Klasse in die Datensätze aufgenommen.
     * Zu Beginn erfolgt eine Validierung auf Basis der DTD. Sobald ein Fehler in der XML-Datei auftaucht, wird das Programm beendet, da es sonst nicht ordnungsgemäß funktioniert.
     * Benutzung vieler Strings, da sich diese in vielen Fällen als weitaus praktischer erwiesen haben, vor allem für Funktionen aus string.h
     * Die Suchfunktion erlaubt es, Felder leer zu lassen
     *
     * TO DO
     * Validierung: während laden() wird direkt überprüft, ob alles gültig ist. Idee: Rekursion mit XML und DTD, wo Childs, Siblings und Attribute verglichen werden. Hierzu muss Struktur der ClElement ClToken ähneln.
     */

ClToken *token=new ClToken;
Cltxt *txt=new Cltxt;
string command;

if (read(token, txt)==false){
    cout << "XML-Datei ung\x81ltig. Programm wird beendet." << endl;
    return 0;
}

cout << "**********************************************" << endl << "Willkommen beim Spielerdatenbanken-CLI!";
menu();
getline(cin, command);
if (command.empty()) command=" ";
cout << endl;

while (command[0]!='V'){
    switch (command[0]) {
    case 'V':
        break;
    case 'K':
        convert(token, txt);
        cout << "Die beiden Dateien wurden erfolgreich in eine neue Datei konvertiert." << endl;
        break;
    case 'E':
        exportieren(token);
        break;
    case 'S':
        suchen(token);
        break;
    default:
        cout << "Bitte geben sie den entsprechenden Grossbuchstaben an." << endl;
        break;
    }
    menu();
    getline(cin, command);
    if (command.empty()) command=" ";
    cout << endl;
}
}
