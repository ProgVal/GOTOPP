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
Les fonctions de l'interpr�teur en ligne de commande					 
*/
#ifndef _GPP_GPP_H
#define _GPP_GPP_H

namespace GotoPP
{
	class CInstruction;
	class BoutCode;
	class Programme;
}

namespace Gpp
{
	void Desass(GotoPP::Programme * programme,bool UneLigne, GotoPP::BoutCode & Ins);
	void ListeVars(GotoPP::Programme * programme);
	void ContenuPile(GotoPP::Programme * programme);
	void FonctionsDefaut(GotoPP::Programme * programme);
	void AjouterArgs(GotoPP::Programme * programme, int argc,GotoPP::carac**argv, int DebutArg);
}

#endif
