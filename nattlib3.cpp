#include <iostream>
using namespace std;
#include <fstream>
#include <string.h>
#include "natt3.h"

//Attributlibrary aus Vorlesung übernommen
int ClattToken::getAttList(
char                      *eingabe,
ClElement                         *element)
{
char puffer[100];
int zaehler;
enum attzustand { zwischenTags, inNamen, erwarteAttributNamen, erwarteAttributWert,
               verarbeiteAttributWert} ;
enum attzustand zustand;

for (zaehler=0,zustand=inNamen,anzahlAtt=0;*eingabe!='\0';
     eingabe = eingabe + 1)
    {
   switch(*eingabe)
      {
   case ' ':
      if (zustand == inNamen)
         {
         zustand = erwarteAttributNamen;
         *eingabe='\0';
         zaehler=0;
         }
      else if (zustand == verarbeiteAttributWert)
         {
         puffer[zaehler] = *eingabe;
         zaehler++;
         }
      break;

   case '=':
      if (zustand == erwarteAttributNamen)
         {
         zustand = erwarteAttributWert;
         puffer[zaehler] = '\0';
         if (element->attributeIstErlaubt(puffer)<0)
            cout << "Fehlerhaftes Attribut: " << puffer << endl;
         attName[anzahlAtt] = new char[zaehler+1];
         strcpy(attName[anzahlAtt],puffer);
         zaehler=0;
         }
      else if (zustand == verarbeiteAttributWert)
         {
         puffer[zaehler] = *eingabe;
         zaehler++;
         }
      else cout << "Fehlerhaftes Zeichen! '='" << endl;
      break;

   case '"':
      if (zustand == erwarteAttributWert) 
         {
         zustand = verarbeiteAttributWert;
         zaehler = 0;
         }
      else if (zustand == verarbeiteAttributWert)
         {
         zustand = erwarteAttributNamen;
         puffer[zaehler] = '\0';
         attValue[anzahlAtt] = new char[zaehler+1];
         strcpy(attValue[anzahlAtt],puffer);
         zaehler=0;
         anzahlAtt++;
         }
      else cout << "Fehlerhaftes Zeichen! '\"'" << endl;
      break;

   default:
      if (zustand >= erwarteAttributNamen)
         {
         puffer[zaehler] = *eingabe;
         zaehler++;
         }
      break;
      }
   }

return 1;
}
