

/* VORWORT
 * Bearbeitung aller Anforderungen bis einschließlich Stufe 4.
 * main.cpp dient ausschließlich der Menüführung. Bei einer gültigen Eingabe wird die entsprechende Funktion in clilib.cpp aufgerufen, diese ruft dann die benötigten Methoden der Klassen auf. Jede Klasse hat hierfür eine weitere Library.
 * Übernahme der Token-, Element- und Attributklassen aus der Vorlesung.
 * 'Datensätze' im Sinne der Aufgabenstellung sind Token mit dem Namen 'Spieler', inklusive aller Token, die in der Hierarchie des imaginären Baumes unter diesen stehen (Spielernummer, Name, usw).
 * Zunächst wird die txt-Datei in eine eigene Klasse eingelesen, durch die Konvertierung werden die Daten aus dieser Klasse in die Datensätze aufgenommen.
 * Um die konvertierten Daten ausgegeben zu bekommen, kann anschließend der Menupünkt 'Exportieren' gewählt werden (ein automatischer Export nach dem Konvertieren wird deshalb nicht mehr durchgeführt). Wenn dieser Menüpunkt vor der Konvertierung gewählt wird, wird nur die originale XML-Datei rekonstruiert.
 * Durch die Konvertierung wird somit auch die Suchfunktion beeinflusst: Nach der Konvertierung lassen sich auch nach den Daten aus der TXT suchen und jene Daten werden bei einer erfolgreichen Suche mit ausgegeben. Vor der Konvertierung ist dies nicht der Fall.
 * Benutzung vieler Strings, da sich diese in vielen Fällen als weitaus praktischer als Chars erwiesen haben, vor allem aufgrund der vielen praktischen Methoden der Klasse.
 * Für die Darstellung von Umlauten wurde hex escape Sequenzen genutzt, dies führte in meinem Compiler zu einer Warnung, funktionierte jedoch wie erwartet.
 * Die durch das Programm erstellten Dateien werden in das Verzeichnis abgelegt, in welchem auch das Buildverzeichnis liegt. (Relative Adresse vom Buildverzeichnis aus: "../dateiname"
 */

#include <iostream>
#include <fstream>
#include <istream>
#include "ntoken3.h"
using namespace std;

bool read(ClToken *token, Cltxt *txt);
void convert(ClToken *token, Cltxt *txt);
void suchen(ClToken *token);
void exportieren(ClToken *token);
void berechnen(ClToken *token, Cltxt *txt);
void menu();

int main()
{
ClToken *token=new ClToken;
Cltxt *txt=new Cltxt;
string command;

//Zu Beginn werden alle drei Dateien eingelesen. Hierbei erfolgt eine Validierung auf Basis der DTD.
//Sobald ein Fehler in der XML-Datei auftaucht oder eine der drei Dateien nicht geöffnet werden kann, wird das Programm beendet, da es sonst nicht ordnungsgemäß funktioniert.
if (read(token, txt)==false){
    cout << "Eingabedateien fehlerhaft. Programm wird beendet." << endl << endl;
    return 0;
}

//Ein Kommando wird eingelesen und anschließend ausgewertet.
//Wenn nichts oder etwas ungültiges eingegeben wird, erscheint ein entsprechender Hinweis.
//Dies wird so lange wiederholt, bis der Benutzer den Menüpunkt "Verlassen" wählt.
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
    case 'B':
        berechnen(token, txt);
        break;
    default:
        cout << "Bitte geben sie den entsprechenden Gro\xE1buchstaben an." << endl;
        break;
    }
    menu();
    getline(cin, command);
    if (command.empty()) command=" ";
    cout << endl;
}
return 0;
}
