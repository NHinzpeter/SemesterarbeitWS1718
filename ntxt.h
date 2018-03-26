#ifndef NTXT_H
#define NTXT_H
using namespace std;

enum txtzustand {nr, ein, tore};

class Cltxt {
private:
    int Spielernr;
    char NrChar[3];
    int Einsaetze;
    int Tore;
    Cltxt *naechstes;
public:
    void laden(ifstream& datei);
    void berechne(char nrn[23][3], int *anzahlSpieler, bool *modus, int *ergebnis);
    int gibSpielernr() {return Spielernr;}
    int gibEinsaetze() {return Einsaetze;}
    int gibTore() {return Tore;}
    char *gibNrChar() {return NrChar;}
    Cltxt *gibnaechstes() {return naechstes;}
};

#endif // NTXT_H
