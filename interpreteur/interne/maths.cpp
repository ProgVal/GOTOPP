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
					 
Fonctions math�matiques int�gr�es.
*/

#include <gotopp/base.h>
#include "defbib.h"

using namespace GotoPP;

/*AIDE Carotte
Renvoie la racine carr�e d'un nombre.
\param a chances Le nombre.
\retour chances La racine carr�e de �a�.*/

/*AIDE CentMegaWatts
�l�ve un nombre � une puissance.
\param a chances Un nombre...
\param b chances et un autre.
\retour chances �a� puissance �b�.*/
namespace DefBib
{
/*AIDE tOUTeNpLUS
Renvoie la valeur absolue du nombre.
\param nombre pingouin Nombre dont on veut d�terminer la valeur absolue.
\retour pingouin Un nombre positif.
\desc 
Si �nombre� est positif ou nul, cette fonction renvoie
la valeur inchang�e, sinon le sign� est invers�.
*/
	void tOUTeNpLUS(IInterpreteur * i)
	{
		CInterpreteur * itp=static_cast<CInterpreteur *>(i);
		valeur val;
		itp->Pile.Depiler(val);
		itp->FinParametres();
		val.Absolue();
		itp->Pile.Ajouter(val);
	}		
}