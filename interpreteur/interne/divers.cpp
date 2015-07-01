/*

Interpr�teur GOTO++
Copyright 2002 Sidoine de Wispelaere

Ce programme est un logiciel libre ; vous pouvez le redistribuer
et/ou le modifier sous les termes de la GNU General Public Licence
telle que publi�e par la Free Software Fundation ; soit dans la
version 2 de la Licence, soit (� votre choix) dans une version
ult�rieure.

Ce programme est distribu� dans l'espoir qu'il sera utile mais
SANS AUCUNE GARANTIE ; sans m�me la garantie implicite qu'il soit
COMMERCIALISABLE ou ADEQUAT POUR UN USAGE PARTICULIER. Voir la
GNU General Public Licence pour plus de d�tails.

Vous devriez avoir re�u une copie de la GNU General Public Licence
avec ce programme ; dans le cas contraire, �crivez � :
Free Software Fundation, Inc.
59 Temple Place - Suite 330, Boston, MA  02111-1307, US

                          ****
					 
Des fonctions int�gr�es en vrac.
*/

#ifdef _WIN32
#include <windows.h>
#include <fcntl.h>
#include <io.h>
#else
#include <unistd.h>
#include <pthread.h>
#endif

#include <gotopp/base.h>
#include <gotopp/compilateur.h>
#include <gotopp/programme.h>
#include <gotopp/mutex.h>

#include "defbib.h"


using namespace GotoPP;

namespace DefBib
{
/*AIDE DODO
Fait une pause d'un certain nombre de millisecondes.
\param attente entier Nombre de millisecondes � attendre.
*/
void biDodo(int temps)
{
#ifdef _WIN32
	Sleep(temps);
#else
	int t=temps;
	int s=t/1000;
	int ms=t%1000;
	if (s)
		sleep(s);
	if (ms)
		usleep(ms*1000);
#endif
}

/*AIDE BigCrunch
Attend la fin d'un univers parall�le.
\param univers universparallele L'univers parall�le � attendre, tel qu'il a �t� renvoy� par GOTOUNIVERSPARALLELEouiz.
*/
void biBigCrunch(CTache* tache)
{
#ifdef _WIN32
	WaitForSingleObject( (HANDLE)tache->t, INFINITE ); 
#else
	pthread_join(tache->t,NULL);
#endif
}

/*AIDE INITIALISATION
Initialise une ou plusieurs variables � une valeur al�atoire entre 0 et 100.
\param reference reference Une r�f�rence sur la variable � initialiser.
\etc
\desc
Il est possible que vous soyiez amen� � modifier des variables pour leur
donner des valeurs honteusement logiques et pr�d�termin�es. Pour leur
rendre leur nature chaotique originelle, utilisez cette fonction.
\exemple
GOTO Affiche un nombre al�atoire entre 0 et 100 :
GOTOPRINTDUTEXTE() ; Nombre
Nombre=*(10)
GOTO Affiche un b�te et d�terministe 10 :
GOTOPRINTDUTEXTE() ; Nombre
INITIALISATION() Nombre
GOTO Affiche � nouveau un nombre al�atoire (ouf !)
GOTOPRINTDUTEXTE() ; Nombre
*/
void biInitialisation(IInterpreteur * interpreteur)
{
	CInterpreteur * itp=static_cast<CInterpreteur*>(interpreteur);
	valeur var;
	while (!itp->Pile.Vide())
	{
		itp->Pile.Depiler(var);
		if (var.Type==TYPE_DEBUTPARAM)
			break;
		if (var.Type!=TYPE_REFERENCE)
			throw  TXT("variable attendue");
		valeur *v=itp->Deref(var.v.p);
		*v=float(rand())/(RAND_MAX+1.0f)*100;
	}
	itp->Pile.AjouterIndef();
}

/*AIDE GOTOerreur
Se rend � l'endroit indiqu� par un encasderreurGOTO pr�c�dent, au besoin
en faisant d'abord des REGOTO.
\param erreur chaine La variable (optionnelle) contenant la description de l'erreur. En principe du texte.
*/

void biGotoErreur(IInterpreteur * interpreteur)
{
	CInterpreteur * itp=static_cast<CInterpreteur*>(interpreteur);
	valeur Texte;
	itp->Pile.Depiler(Texte);
	if (Texte.Type!=TYPE_DEBUTPARAM)
	{
		itp->programme->varGlob[0]=Texte;
		itp->Pile.FinParametres();
	}
	itp->Exception();
}


/*AIDE nombreCHEPA
Renvoie le nombre d'appel de fonction qui ont d�j� �t� faits.
\retour entier Nombre d'appels de fonctions effectu�s.
*/
void biNombreChepa(IInterpreteur * interpreteur)
{
	CInterpreteur * itp=static_cast<CInterpreteur*>(interpreteur);
	itp->Pile.Ajouter(itp->nAppels);
}


/*AIDE boum
G�n�re une erreur d'ex�cution. Au cas o� vous trouveriez que votre
programme n'a pas assez de bugs.
*/

void biBoum()
{
	throw TXT("boum !");
}

void biGotoDansTexte(IInterpreteur * interpreteur)
{
	CInterpreteur * itp=static_cast<CInterpreteur*>(interpreteur);
	valeur Code;
	itp->Pile.Depiler(Code);
	itp->Pile.FinParametres();
	Code.EnChaine();
	BoutCode * bc=itp->programme->compilateur->Compiler(Code.v.c->c);
	CInterpreteur interp(itp->programme);
	interp.Executer(bc->depart);
	itp->Pile.AjouterIndef();
}

}



/*AIDE TuerOgre%berger
Cr�e une expression r�guli�re � partir d'une cha�ne de caract�res.
\param chaine chaine La cha�ne � transformer en expression r�guli�re.
*/



/*AIDE TuerOgre%Valeur
Renvoie la cha�ne qui se trouvait entre certaines parenth�ses lors de la
derni�re utilisation de l'expression r�guli�re.
\param numero entier Le num�ro, de 0 � 9.
\retour chaine La cha�ne trouv�e.
\desc
Bon, hum, une description plus claire s'impose. Dans les expressions r�guli�res
on peut  mettre des trucs entre parenth�ses, lesquelles choses seront num�rot�es
de 1 � 9 de gauche � droite. Cette fonction peut retrouver les cha�nes
qui correspondent. En passant 0 en param�tre, on r�cup�re la totalit� de
ce qui a �t� trouv�. Oui... Bon, un petit exemple sera plus clair.
\exemple
TuerOgre ogre(�Monsieur (\w+)�)
OUQUILESTLETEXTE(�Bonjour Monsieur Ducon, comment-allez vous ?� &ogre)
GOTOPRINTDUTEXTE(;ogre@Valeur(*(1))
GOTO Affiche Ducon
GOTOPRINTDUTEXTE(;ogre@Valeur(*(0))
GOTO Affiche Monsieur Ducon
*/

/*AIDE TuerOgre%DerValeur
Renvoie la cha�ne qui se trouvait entre certaines parenth�ses lors de la
derni�re utilisation de la derni�re expression r�guli�re constante � avoir �t� mise dans la pile.
\static
\param numero entier Le num�ro, de 0 � 9.
\retour chaine La cha�ne trouv�e.
\desc
Bon, hum, une description plus claire s'impose (je me r�p�te). 
Dans les expressions r�guli�res
on peut  mettre des trucs entre parenth�ses, lesquelles choses seront num�rot�es
de 1 � 9 de gauche � droite. Cette fonction peut retrouver les cha�nes
qui correspondent. En passant 0 en param�tre, on r�cup�re la totalit� de
ce qui a �t� trouv�. Oui... Bon, un petit exemple sera plus clair (copier/coller).
\exemple
OUQUILESTLETEXTE(�Bonjour Monsieur Ducon, comment-allez vous ?� ^/Monsieur (\w+)/)
GOTOPRINTDUTEXTE(;TuerOgre%DerValeur(*(1))
GOTO Affiche Ducon
GOTOPRINTDUTEXTE(;TuerOgre%DerValeur(*(0))
GOTO Affiche Monsieur Ducon
*/

