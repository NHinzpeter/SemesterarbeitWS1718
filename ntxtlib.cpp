#include <iostream>
using namespace std;
#include <string.h>
#include <fstream>
#include "ntxt.h"

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

