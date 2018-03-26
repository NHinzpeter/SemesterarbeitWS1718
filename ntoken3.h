#include "natt3.h"

class ClToken
   {
public:
   ClToken();
   ClattToken att;
   char *name() { return tokenName; }
   ClToken *child() { return tokenChild; } 
   ClToken *sibling() { return tokenSibling; }
   char *inhalt() { return tokenInhalt; }
   bool laden(ifstream &datei,ClElement *element,ClElement *wurzel);
   void addSibling(char *tkname, char const *tkinhalt);
   void sucheXYZ(string *category1, string *keyword1, bool *gefunden, string *category2, string *keyword2, bool *UND);
   void exportiere(int ebene, bool modus, ofstream &ausgabe);
   void exportiereNamen(char nrn[23][3], ofstream &ausgabe, int *anzahlSpieler);
private:
   char tokenName[64];
   ClToken *tokenChild;
   ClToken *tokenSibling;
   char *tokenInhalt;
   bool ausgegeben;
   void druckeTokenEbene(int ebene, ofstream &ausgabe);
   void druckeTokenEbene2(int ebene);
   void zeigeXYZ(int ebene);
   } ;

enum zustand { istStartTag, istEndTag } ;
