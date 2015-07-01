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
					 
Des fonctions de Compilateur que je mets ici parce que sinon hein heu.
*/

#include <gotopp/base.h>
#include <gotopp/compilateur.h>
#include <gotopp/erreur.h>
#include <gotopp/instruction.h>
#include <gotopp/global.h>

#include "compriv.h"
#include "time.h"

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif
using namespace GotoPP;
using namespace std;

void Compilateur::LireType(CType &Type)
{
	while(1)
	{
		SauterEspaces();
		switch(*Source)
		{
		case SEP_TAB_O:
			if (Source[1]!=SEP_TAB_F)
				throw "> attendu apr�s <";
			Source+=2;
			Type.Empiler(symboleTableau);
			continue;
		case SEP_HAC_O:
			if (Source[1]!=SEP_HAC_F)
				throw "] attendu apr�s [";
			Source+=2;
			Type.Empiler(symboleTableHachage);
			continue;
		case '&':
			Source++;
			Type.Empiler(symboleReference);
			continue;
		}
		break;
	}
}


void Compilateur::AfficherType(CType & t)
{
	for (int i=0; i<t.l; i++)
	{
		if (i)
			clog << " ";
		clog << t.t[i]->nom;
	}
}

void Compilateur::PremierePasse()
{
	//Cherche les �tiquettes de fonction, les d�finitions de classe, les modules
	//et les biblioth�ques
	Ligne=1;
	while (1)
	{
		SauterEspaces();
		//if (*Source=='�')
	}
}

void GotoPP::Compilateur::AfficherErreur(const carac *c)
{
	::AfficherErreur(c,AcFichier(),AcLigne(),false);
}

void GotoPP::Compilateur::AfficherErreur(GotoPP::CErreur&e)
{
	AfficherErreur(e.Texte());
}

Compilateur::Compilateur(Programme * _programme):programme(_programme),AcFichierSource(0)
{

}


GotoPP::BoutCode* Compilateur::Charger(const carac*fichier)
{
	FichierSource * fichierSource=AcFichierSource=new FichierSource();
    programme->fichierSource.Etendre(fichierSource);
	
	if (! fichierSource->Charger(fichier,CheminModules))
		throw TXT("chargement des fichiers binaires non support�");
			//ChargerBinaire(ProchainFichier());
	
	Source=programme->fichierSource.Dernier()->Source;
	fichierSource->Parent=0;
	BoutCode * ret=Compiler();
	fichierSource->DebutCode=ret->depart;
	fichierSource->FinCode=ret->fin;
	return ret;
}

GotoPP::BoutCode* Compilateur::Compiler(const carac *CodeSource)
{
	Source=(carac*)CodeSource;
	return Compiler();
}

ICompilateur * ICompilateur::Factory(Programme * _programme)
{
    return new Compilateur(_programme);
}

