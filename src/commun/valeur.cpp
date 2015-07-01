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
					 
Impl�mentation de la classe valeur
*/
#include <gotopp/base.h>

using namespace GotoPP;

void GotoPP::valeur::EcrireBin(FILE *F)
{
	switch(Type)
	{
	case TYPE_ENTIER:
		fwrite(&v.i,sizeof(int),1,F);
		break;
	case TYPE_CHANCES:
		fwrite(&v.f,sizeof(float),1,F);
		break;
	case TYPE_CARAC:
		{
			carac c=(carac)v.i;
			fwrite(&c,sizeof(carac),1,F);
			break;
		}
	case TYPE_CHAINE:
		fwrite(v.c->c,v.c->l*sizeof(carac),1,F);
		break;
	}
}

void GotoPP::valeur::Ecrire(FILE * F)
{
	switch(Type)
	{
	case TYPE_ENTIER:
		fprintf(F,"%d",v.i);
		break;
	case TYPE_ENTIER64:
		fprintf(F,"%ld",v.i64);
		break;
	case TYPE_CHANCES:
		fprintf(F,"%f",v.f);
		break;
	case TYPE_CARAC:
		fputc(v.i,F);
		break;
	case TYPE_VARIABLE:
		fprintf(F,"%d",v.i);
		break;
	case TYPE_CHAINE:
		czfputs(v.c->c,F);
		break;
	case TYPE_TABLEAU:
		fputs("[tableau]",F);
		break;
	case TYPE_REFERENCE:
		switch(v.p.Type)
		{
		case RefType::Global:
			fprintf(F,"r�f�rence vers globale %d",v.p.Decalage);
			break;
		case RefType::Local:
			fprintf(F,"r�f�rence vers locale %d",v.p.Decalage);
			break;
		case RefType::Tableau:
			fprintf(F,"r�f�rence vers indice %d d'un tableau",v.p.Decalage);
			break;
		case RefType::Hachage:
			fprintf(F,"r�f�rence vers paire %d d'un hachage",v.p.Decalage);
			break;
		case RefType::Bebe:
			fputs("r�f�rence vers un b�b� pingouin",F);
			break;
		default:
			fputs("r�f�rence de type inconnu",F);
		}
		break;
	case TYPE_HACHAGE:
		fputs("[hachage]",F);
		break;
	case TYPE_OBJETC:
		fputs("[objetc]",F);
		break;
	case TYPE_INCONNU:
		fputs("[ind�fini]",F);
		break;
	default:
		fprintf(F,"[non affichable %d]",Type);
	}
}

void GotoPP::valeur::EcrireU(FILE * F)
{
#ifdef _UNICODE
	switch(Type)
	{
	case TYPE_ENTIER:
		fwprintf(F,L"%d",v.i);
		break;
	case TYPE_ENTIER64:
		fwprintf(F,L"%ld",v.i64);
		break;
	case TYPE_CHANCES:
		fwprintf(F,L"%f",v.f);
		break;
	case TYPE_CARAC:
		fputwc(v.i,F);
		break;
	case TYPE_VARIABLE:
		fwprintf(F,L"%d",v.i);
		break;
	case TYPE_CHAINE:
		czfputws(v.c->c,F);
		break;
	case TYPE_TABLEAU:
		fputws(L"[tableau]",F);
		break;
	case TYPE_REFERENCE:
		switch(v.p.Type)
		{
		case RefType::Global:
			fwprintf(F,L"r�f�rence vers globale %d",v.p.Decalage);
			break;
		case RefType::Local:
			fwprintf(F,L"r�f�rence vers locale %d",v.p.Decalage);
			break;
		case RefType::Tableau:
			fwprintf(F,L"r�f�rence vers indice %d d'un tableau",v.p.Decalage);
			break;
		case RefType::Hachage:
			fwprintf(F,L"r�f�rence vers paire %d d'un hachage",v.p.Decalage);
			break;
		case RefType::Bebe:
			fputws(L"r�f�rence vers un b�b� pingouin",F);
			break;
		default:
			fputws(L"r�f�rence de type inconnu",F);
		}
		break;
	case TYPE_HACHAGE:
		fputws(L"[hachage]",F);
		break;
	case TYPE_OBJETC:
		fputws(L"[objetc]",F);
		break;
	case TYPE_INCONNU:
		fputws(L"[ind�fini]",F);
		break;
	default:
		fwprintf(F,L"[non affichable %d]",Type);
	}
#endif
}

/*std::basic_ostream<carac>& GotoPP::operator<<(std::basic_ostream<carac> &o,const GotoPP::valeur &v)
{
	switch(v.Type)
	{
	case TYPE_ENTIER:
		return o<<v.v.i;
	case TYPE_CHANCES:
		return o<<v.v.f;
	case TYPE_CARAC:
		return o<<char(v.v.i);
	case TYPE_VARIABLE:
		return o<<v.v.i;//Variable[v.v.i].Nom;
	case TYPE_CHAINE:
		return o<<v.v.c;
	case TYPE_FONCTION:
		return o<<"[fonction "<<v.v.i<<']';
	case TYPE_TABLEAU:
		return o<<"[tableau]";
	case TYPE_REFERENCE:
		return o<<"[reference]";
	case TYPE_HACHAGE:
		return o<<"[hachage]";
	case TYPE_OBJETC:
		return o<<"[objetc]";
	case TYPE_INCONNU:
		return o<<"[ind�fini]";
	default:
		return o<<"[non affichable]";
	}
}
*/
