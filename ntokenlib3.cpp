#include <iostream>
using namespace std;
#include <fstream>
#include <string.h>
#include "ntoken3.h"

//Constructor, überwiegend aus Vorlesung übernommen
ClToken::ClToken()
{
*tokenName='\0';
tokenChild=NULL;
tokenSibling=NULL;
tokenInhalt=new char[1];
*tokenInhalt='\0';
ausgegeben=false;
}


//Liest die XML-Datei aus, validiert diese und erstellt den Tokenbaum, überwiegend aus Vorlesung übernommen
int  ClToken::laden(ifstream &datei, ClElement *element, ClElement *wurzel, bool *gueltig)
{
int zaehler=0;
enum zustand zustand;
char zeichen;
char *ziel, *quelle;
char puffer[100];
ClToken *child;
int isChild;

for (zaehler=0;;)
    {
    datei.get(zeichen);
    if (datei.eof())
       {
       if (*tokenName == '\0' && tokenChild == NULL && tokenInhalt == NULL)
          return 0;
       return 1;
       }
    switch(zeichen)
       {
    case '<':
       datei.get(zeichen);
       if (zeichen=='/')
          {
          zustand = istEndTag;
          if (zaehler!=0)
             {
             puffer[zaehler]='\0';
             tokenInhalt = new char[zaehler+1];
             strcpy(tokenInhalt,puffer);
             }
          }
       else
          {
          datei.putback(zeichen);
          if (*tokenName!='\0')
             {
             datei.putback('<');
	     if (tokenChild==NULL)
	        {
                tokenChild=new ClToken;
                if (tokenChild->laden(datei,element,wurzel,gueltig)==0)
                   return 0;
		}
	     else
	        {
		for (child=tokenChild;;child=child->tokenSibling)
		    {
		    if (child->tokenSibling==NULL)
		       {
		       child->tokenSibling=new ClToken;
                       if (child->tokenSibling->laden(datei,element,wurzel, gueltig)==0)
                  return 0;
		       break;
		       }
		    }
		}
             }
          else zustand=istStartTag;
          }
       zaehler=0;
       break;
    case '>':
       puffer[zaehler]='\0';
       if (zustand==istEndTag)
          {
	  if (strcmp(tokenName,puffer))
	     {
         cout << "Fehlerhaftes End Tag. Erwartet: </"
              << tokenName << ">" << endl;
         *gueltig=false;
             return 0;
	     }
      return 1;
	  }
       if (zustand==istStartTag)
          {
          for (ziel=tokenName,quelle=puffer;*quelle!=' ' && *quelle!= '\0';
               ziel++,quelle++) *ziel=*quelle;
          *ziel='\0';
          if (!strcmp(tokenName,wurzel->getName())) element=wurzel;
          else
             {   
	     if (element==NULL) isChild=-1;
	     else isChild=element->elementIstErlaubt(tokenName);
	     if (isChild < 0)
	        {
	        cout << "Fehlerhaftes Start Tag: '" << tokenName << "'" << endl; 
            *gueltig=false;
                return 0;
                }
	     element=element->sucheElement(tokenName,wurzel);
             }
          att.getAttList(puffer,element);
          }
       zaehler=0;
       break;
    case '\n':
       break;
    default:
       puffer[zaehler]=zeichen;
       zaehler++;
       break;
       }
    }
}


//gibt alle Informationen des aktuellen Spielers aus, in Anlehnung an druckeToken() aus der Vorlesung
void ClToken::zeigeXYZ(int ebene)
{
    druckeTokenEbene2(ebene);
    cout << name();
    if (att.zahlAtt() > 0)
       {
        cout << " (";
       for (int i=0;i<att.zahlAtt();i++)
           {
           if (i>0) cout << ", ";
           cout << att.zeigeAttName(i) << ": "
                << att.zeigeAttWert(i);
           }
       cout << "):";
       }
    else cout << ": ";
    cout << inhalt() << endl;
    if (tokenChild!=NULL) tokenChild->zeigeXYZ(ebene+1);

    if (strcmp(tokenName,"Spieler")){                                           //Abfrage stellt sicher, dass nur ein einziger Spieler ausgegeben wird
        if (tokenSibling!=NULL) tokenSibling->zeigeXYZ(ebene);
    }
    else ausgegeben=true;
}


//druckeTokenEbene schreibt in die Ausgabedatei, während DruckeTokenEbene2 in das Ausgabefenster schreibt.
void ClToken::druckeTokenEbene(int ebene, ofstream &ausgabe)
{
while (ebene > 0)
      {
      ausgabe << "    ";
      ebene = ebene - 1;
      }
}

void ClToken::druckeTokenEbene2(int ebene)                              //überwiegend aus der Vorlesung übernommen
{
while (ebene > 0)
      {
      cout << "    ";
      ebene = ebene - 1;
      }
}


bool paar1, paar2;
void ClToken::sucheXYZ(string *category1, string *keyword1, bool *gefunden, string *category2, string *keyword2, bool *UND) {
    string Name =tokenName;
    string Inhalt=tokenInhalt;
    ausgegeben=false;

    if (!category1->empty() || !keyword1->empty()) {
    if (Name.find(*category1)!= string::npos)
        if(Inhalt.find(*keyword1)!=string::npos)
            paar1=true;
    }
    if (!category2->empty() || !keyword2->empty()) {
        if (Name.find(*category2)!= string::npos)
            if(Inhalt.find(*keyword2)!=string::npos)
                paar2=true;
    }

    if (tokenChild!=NULL) tokenChild->sucheXYZ(category1, keyword1, gefunden, category2, keyword2, UND);

   if (!strcmp(tokenName,"Spieler")){
       if ((category2->empty() && keyword2->empty())||(category1->empty() && keyword1->empty())) {
            if (paar1==true || paar2==true) {
                cout << endl;
                zeigeXYZ(0);
                *gefunden=true;
            }
       }
       else {
           if (*UND==true){
               if (paar1==true && paar2==true) {
                   cout << endl;
                   zeigeXYZ(0);
                   *gefunden=true;
               }
           }
           else if (paar1==true || paar2==true){
               cout << endl;
               zeigeXYZ(0);
               *gefunden=true;
           }
       }

       paar1=false;
       paar2=false;
   }
    if (tokenSibling!=NULL) tokenSibling->sucheXYZ(category1, keyword1, gefunden, category2, keyword2, UND);
    return;
}


void ClToken::exportiere(int ebene, bool modus, ofstream &ausgabe){
    char vergleich=' ';

    if (modus==0 || (!strcmp(tokenName,"Spieler") && ausgegeben==true) || strcmp(tokenName,"Spieler")) {
    //Ausgabe der Starttags
    druckeTokenEbene(ebene, ausgabe);
    ausgabe << "<" << tokenName;
    //Ausgabe der Attribute
    if (att.zahlAtt() > 0)
       {
        ausgabe << " ";
       for (int i=0;i<att.zahlAtt();i++)
           {
           if (i>0) ausgabe << " ";
           ausgabe << att.zeigeAttName(i) << "=\""
                << att.zeigeAttWert(i) << "\"";
           }
       }
    ausgabe << ">";
    vergleich=tokenInhalt[0];
    if (vergleich ==' ' || *tokenInhalt=='\0') ausgabe << endl;            //Wenn kein Inhalt vorhanden ist, kommt ein Zeilenumbruch, ansonsten der Inhalt (rein optische Verbesserung)
    else ausgabe << tokenInhalt;

    if (tokenChild!=NULL) tokenChild->exportiere(ebene+1, modus, ausgabe);

    //Erstellung der Endtags
    if (vergleich ==' ') druckeTokenEbene(ebene, ausgabe);
    ausgabe << "</" << tokenName << ">" << endl;
    }
    if (tokenSibling!=NULL) tokenSibling->exportiere(ebene, modus, ausgabe);
}


//fügt dem aktuellen Token einen Sibling hinzu und füllt diesen mit dem übergebenen Namen und Inhalt.
void ClToken::addSibling(char *tkname, char const *tkinhalt) {
    tokenSibling= new ClToken;
    strcpy(tokenSibling->tokenName,tkname);
    tokenSibling->tokenInhalt=new char[strlen(tkinhalt)+1];
    strcpy(tokenSibling->tokenInhalt, tkinhalt);

}
