#include <iostream>
using namespace std;
#include <string.h>
#include <fstream>
#include "ntxt.h"


//rekursive Funktion, parsed die TXT-Datei und schreibt sie in Cltxt-Objekte. Auf Basis des Zustandsenums des Typs txtzustand wird entschieden, ob es sich
//bei der eingelesenen Datei um eine Spielernummer, um die Menge an Einsätzen oder die Menge von Toren handelt. Die Objekte werden durch den Pointer
//'naechstes' miteinander verknüpft.
void Cltxt::laden(ifstream& datei){
    char zeichen;
    char puffer[3];
    int pufferint=0, zaehler=0;
    enum txtzustand zustand=nr;

    for (datei.get(zeichen);!datei.eof();datei.get(zeichen))
        {
        switch (zeichen) {
        case ' ':
            puffer[zaehler]='\0';
            pufferint=(puffer[0]-'0')*10+(puffer[1]-'0');
            switch (zustand){
            case nr:
                Spielernr=pufferint;
                strcpy(NrChar,puffer);
                zaehler=0;
                zustand=ein;
                break;
            case ein:
                Einsaetze=pufferint;
                zaehler=0;
                zustand=tore;
                break;
            }

            break;
        case '\n':
            puffer[zaehler]='\0';
            pufferint=(puffer[0]-'0')*10+(puffer[1]-'0');
            Tore=pufferint;
            naechstes=new Cltxt;
            naechstes->laden(datei);
            break;
        default:
            puffer[zaehler]=zeichen;
            zaehler++;
            break;
        }
    }
    return;
}


//rekursive Funktion, vergleicht die Spielernummer des aktuellen Spielers mit allen gesuchten Nummern. Wenn eine der Nummern mit der Spielernummer übereinstimmt, wird
//(je nacg Modus) der gesuchte Wert zur Summe addiert. Der break in der for-Schleife sorgt dafür, dass der Wert eines Spielers nur einmal in der Summe berücksichtigt wird,
//auch wenn dessen Spielernummer mehrmals eingegeben wurde.
void Cltxt::berechne(char nrn[23][3], int *anzahlSpieler, bool *modus, int *ergebnis){
    for (int i=0; i<*anzahlSpieler; i++){
        if (!strcmp(nrn[i],NrChar)){
            if (*modus==false) *ergebnis+=Einsaetze;
            else *ergebnis+=Tore;
            break;
        }
    }
    //Rekursion
    if (naechstes!=NULL) naechstes->berechne(nrn, anzahlSpieler, modus, ergebnis);
}
