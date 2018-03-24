#include "ntxt.h"

enum status {frei, inBearbeitung};

class ClElement
   {
public:
   ClElement(char *string);
   char *getName(void) { return name; }
   void addTag(char * string);
   void addAttribute(char * string);
   int getAnzahlTags() { return zahl; }
   char *getTagName(int slot) { return tags[slot]; }
   ClElement *getNext(void) { return naechstes; }
   void setNext(ClElement *neu) { naechstes = neu; }
   ClElement *sucheElement(char *suche,ClElement *element);
   int elementIstErlaubt(char *name);
   int attributeIstErlaubt(char *name);
private:
   int zahl;
   char name[64];
   char tags[10][64];
   ClElement *naechstes;
   enum status status;
   int zahlAttribute;
   char attributes[10][64];
   } ;
