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
					 
Une table de hachage de "valeur"s avec d�compte des r�f�rences.
*/

#ifndef _HACHVAL_H
#define _HACHVAL_H

namespace GotoPP
{
	struct indexhachage
	{
		uint i;
		uint hache;
	};

	struct pairehachage;
	struct valeur;
	struct reference;

	struct hachageval: public gc
	{
		size_t l,a;
		pairehachage * v;				
		indexhachage * Index; //Index vers les clefs tri�es
		uint * PLettre; //Pour les grosses tables
		short Classe;
		byte Dimensions;

		valeur * Get(const chaineval * clef);
		int ObtId(const chaineval * clef);
		void ObtRef(reference & r, const chaineval * clef);
		void Dim(index_t t);
		hachageval * Copier();
	};
}
#include "valeur.h"
namespace GotoPP
{
	struct pairehachage
	{
		const chaineval *Clef;
		valeur Val;
	};
}
#endif
