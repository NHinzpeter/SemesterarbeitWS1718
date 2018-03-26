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


//Liest die XML-Datei aus, validiert diese und erstellt den Tokenbaum, überwiegend aus Vorlesung übernommen.
//Funktion gibt false zurück, wenn die XML-Datei nicht valide ist.
bool  ClToken::laden(ifstream &datei, ClElement *element, ClElement *wurzel)
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
                if (tokenChild->laden(datei,element,wurzel)==false)
                   return false;
		}
	     else
	        {
		for (child=tokenChild;;child=child->tokenSibling)
		    {
		    if (child->tokenSibling==NULL)
		       {
		       child->tokenSibling=new ClToken;
                       if (child->tokenSibling->laden(datei,element,wurzel)==false)
                  return false;
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
             return false;
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
                return false;
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


//rekursive Funktion, die jeden Token auf die Suchbegriffe überprüft und mit Hilfe der Methode zeigeXYZ() die Spieler ausgibt, auf die die Suchanfrage zutrifft
//die globalen Variablen paar1 und paar2 werden immer dann auf true gesetzt, wenn ein Kriterium erfüllt wurde, bevor zu einem weiteren Spieler gegangen wird, werden diese
//wieder auf false gesetzt. Die boolean gefunden gibt der aufrufenden Funktion zurück, ob überhaupt ein Spieler gefunden wurde (für eine etwaige Meldung, falls nicht).
bool paar1, paar2;
void ClToken::sucheXYZ(string *category1, string *keyword1, bool *gefunden, string *category2, string *keyword2, bool *UND) {
    //Umwandlung von Name und Inhalt in Strings, um die Methoden dieser Klasse verwenden zu können
    string Name =tokenName;
    string Inhalt=tokenInhalt;
    ausgegeben=false;

    //Wenn mindestens eine Bedingung im ersten Paar ausgefüllt wurde, werden Name und Inhalt mit den entsprechenden Suchbegriffen verglichen.
    //Nutzung der Methode find(), welche überprüft, ob der Suchbegriff an beliebiger Stelle im Namen oder Inhalt des Tokens vorkommt.
    //Wenn ein Feld leer gelassen wurde, ist das kein Problem, da die Abfrage auch dann wahr ist.
    if (!category1->empty() || !keyword1->empty()) {
    if (Name.find(*category1)!= string::npos)
        if(Inhalt.find(*keyword1)!=string::npos)
            paar1=true;
    }
    //Die selbe Abfrage geschieht auch für das zweite Paar, hierdurch kann auch Paar1 leer gelassen worden sein und nur Paar2 ausgefüllt.
    if (!category2->empty() || !keyword2->empty()) {
        if (Name.find(*category2)!= string::npos)
            if(Inhalt.find(*keyword2)!=string::npos)
                paar2=true;
    }

    //Rekursion
    if (tokenChild!=NULL) tokenChild->sucheXYZ(category1, keyword1, gefunden, category2, keyword2, UND);

    // Nachdem alle Kinder des Tokens überprüft wurden, wird gecheckt, ob man sich aktuell auf der Ebene eines Spielers befindet.
    //Falls ja, nur in einem Paar wurde mindestens eine Bedingung ausgefüllt und eine der globalen Variablen paar1 und paar2 wurde zuvor auf true gesetzt,
    //wird der gesamte Spieler mittels zeigeXYZ() ausgegeben.
    if (!strcmp(tokenName,"Spieler")){
       if ((category2->empty() && keyword2->empty())||(category1->empty() && keyword1->empty())) {
            if (paar1==true || paar2==true) {
                cout << endl;
                zeigeXYZ(0);
                *gefunden=true;
            }
       }
       //wenn in beiden Paaren mindestens eine Bedingung ausgefüllt wurde, werden paar1 und paar2 mit dem entsprechenden Operator verknüpft,
       //sofern dann ein wahre Aussage entsteht, wird der Spieler ausgegeben
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
    //Rekursion
    if (tokenSibling!=NULL) tokenSibling->sucheXYZ(category1, keyword1, gefunden, category2, keyword2, UND);
    return;
}


//rekursive Funktion, schreibt den aktuellen Token in XML-konformer Form in die Ausgabedatei. Der Modus legt fest, ob alle Spieler ausgegeben werden sollen, oder nur jene Spieler,
//deren boolean 'ausgegeben' bei der letzte Suchanfrage auf true gesetzt wurde.
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

        //Rekursion
        if (tokenChild!=NULL) tokenChild->exportiere(ebene+1, modus, ausgabe);

        //Erstellung der Endtags
        if (vergleich ==' ') druckeTokenEbene(ebene, ausgabe);
        ausgabe << "</" << tokenName << ">" << endl;
    }

    //Rekursion
    if (tokenSibling!=NULL) tokenSibling->exportiere(ebene, modus, ausgabe);
}


//fügt dem aktuellen Token einen Sibling hinzu und füllt diesen mit dem übergebenen Namen und Inhalt.
void ClToken::addSibling(char *tkname, char const *tkinhalt){
    tokenSibling= new ClToken;
    strcpy(tokenSibling->tokenName,tkname);
    tokenSibling->tokenInhalt=new char[strlen(tkinhalt)+1];
    strcpy(tokenSibling->tokenInhalt, tkinhalt);

}

//rekursive Funktion, schreibt Nachnamen und Vornamen eines Spieler in die Ausgabedatei
void ClToken::exportiereNamen(char nrn[23][3], ofstream &ausgabe, int *anzahlSpieler){
    if (!strcmp(tokenName,"Spieler")) {
        for (int i=0; i<*anzahlSpieler; i++){
            if (!strcmp(nrn[i],tokenChild->tokenInhalt)){
                ausgabe << tokenChild->tokenSibling->tokenChild->tokenSibling->tokenInhalt << ", " << tokenChild->tokenSibling->tokenChild->tokenInhalt << "; ";
                break;
            }
        }
    }
    //Rekursion
    if (tokenChild!=NULL) tokenChild->exportiereNamen(nrn, ausgabe, anzahlSpieler);
    if (tokenSibling!=NULL) tokenSibling->exportiereNamen(nrn, ausgabe, anzahlSpieler);
}
