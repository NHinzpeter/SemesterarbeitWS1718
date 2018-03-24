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
    int gibSpielernr() {return Spielernr;}
    int gibEinsaetze() {return Einsaetze;}
    int gibTore() {return Tore;}
    char *gibNrChar() {return NrChar;}
    Cltxt *gibnaechstes() {return naechstes;}
};

#endif // NTXT_H
