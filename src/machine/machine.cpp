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
					 
Ceci est un gros switch. C'est la machine virtuelle du GOTO++. 
Pr�parez vous � avoir les cheveux qui se dressent sur la t�te.
*/
#include <gotopp/base.h>
#include <gotopp/erreur.h>
#include <gotopp/machine.h>
#include <gotopp/mutex.h>
#include <gotopp/programme.h>

/*bool CInterpreteur::Executer()
{
	nAppels=0;
	Appel[0].EnErreur=-1;
	Pile.Vider();
	Pile2.Vider();
//	PileF.Init();
	This.Init();
	DebutLigne=Ins.i;
	return Executer2();
}*/

using namespace GotoPP;
using namespace std;

void GotoPP::CInterpreteur::Executer()
{
	int AppelDepart=nAppels;

	programme->mutex.Prendre();

	while (Ins.i)
	{
		switch (Ins.LireCode())
		{
		case I_STOP:
#ifdef _DEBUG
	/*		clog << "Taille de PileF :" << uint(PileF.l)<<"\n";
			clog << "Nombre d'appels :" << nAppels<<"\n";*/
#endif
			programme->mutex.Liberer();
			return;
		case I_DEFINIR:
			{
				valeur val;Pile.Depiler(val);
				valeur var;Pile.Depiler(var);
				if (var.Type!=TYPE_REFERENCE)
					throw TXT("on ne peut modifier que des r�f�rences");
				DerVar=var;
				Deref(var.v.p)->CopieDe(val);
				/*if (Tracer)
					printf("D�finir %s=%d\n",Variable[var].Nom,val);*/
				break;
			}
		case I_INSTANCEDE:
			{
				valeur val;Pile.Depiler(val);
				valeur var;Pile.Depiler(var);
				if (var.Type!=TYPE_REFERENCE)
					throw TXT("on ne peut instancier que vers des r�f�rences");
				DerVar=var;
				*Deref(var.v.p)=val;
				/*if (Tracer)
					printf("D�finir %s=%d\n",Variable[var].Nom,val);*/
				break;
			}
		case I_INCREMENTER:
			{
				valeur val;Pile.Depiler(val);
				valeur var;Pile.Depiler(var);
				if (var.Type!=TYPE_REFERENCE)
					throw TXT("vous devez incr�menter des r�f�rences");
				DerVar=var;
				*Deref(var.v.p)+=val;
				break;
			}
		case I_DECREMENTER:
			{
				valeur val;Pile.Depiler(val);
				valeur var;Pile.Depiler(var);
				if (var.Type!=TYPE_REFERENCE)
					throw TXT("vous devez d�cr�menter des r�f�rences");
				DerVar=var;
				*Deref(var.v.p)-=val;
				break;
			}
		case I_MULTIPLIEREGAL:
			{
				valeur val;Pile.Depiler(val);
				valeur var;Pile.Depiler(var);
				if (var.Type!=TYPE_REFERENCE)
					throw TXT("vous devez multiplier des r�f�rences");
				DerVar=var;
				*Deref(var.v.p)*=val;
				break;
			}
		case I_DIVISEREGAL:
			{
				valeur val;Pile.Depiler(val);
				valeur var;Pile.Depiler(var);
				if (var.Type!=TYPE_REFERENCE)
					throw TXT("vous devez diviser des r�f�rences");
				DerVar=var;
				*Deref(var.v.p)/=val;
				break;
			}

		case I_EXEC:
			{
				AppelerEtiquette(Ins.LireEtiquette());
				break;
			}

		case I_EXECVAL:
			{
				valeur var;Pile.Depiler(var);
				if (var.Type==TYPE_ETIQUETTE)
					AppelerEtiquette(var.v.et);
				else if (var.Type==TYPE_FONCTIONC)
					AppelerFonctionC(var.v.fonctionC);
				else if (var.Type==TYPE_FONCTIONC_THIS)
					AppelerMethodeC(var.v.fonctionC);
				else if (var.Type==TYPE_ETIQUETTE_THIS)
					AppelerEtiquette(var.v.et,true);
				else
					throw TXT("d'habitude c'est des fonctions qu'on ex�cute");
				break;
			}

		case I_EXECC:
			{
				AppelerFonctionC(Ins.LireFonctionC());
				break;
			}

		case I_EXECTHIS:
			{
				AppelerEtiquette(Ins.LireEtiquette(),true);
				break;
			}

		case I_EXECMEMETHISVAL:
			{
				valeur var;Pile.Depiler(var);
				if (var.Type==TYPE_ETIQUETTE_THIS)
					AppelerEtiquette(var.v.et,false);
				else
					throw TXT("seul l'appel d'�tiquettes est support� en this");
				break;
			}

		case I_EXECTHISC:
			{
				throw TXT("EXECTHISC non impl�ment�");
				break;
			}
		case I_EXECTHISMODULE:
			{
				throw TXT("EXECTHISMODULE non impl�ment�");
				break;
			}

		case I_EXPREG:
			{
				DerExpReg=(regexp*) Ins.LireObjet();
				Pile.Ajouter(DerExpReg);
				break;
			}
		case I_CONSTANTE:
			{
				valeur val;
				Ins.Lire(val);
				Pile.Ajouter(val);
				break;
			}
		case I_ENTIER:
			{
				Pile.Ajouter(Ins.LireEntier());
				break;
			}
		case I_CARAC:
			{
				Pile.Ajouter(Ins.LireCarac());
				break;
			}
		case I_ETIQUETTE:
			{
				Pile.AjouterEtiquette(Ins.LireEtiquette());
				break;
			}
		case I_ETIQUETTELOC:
			{
				Pile.AjouterEtiquette(Ins.i+Ins.LireEntier());
				break;
			}
		case I_VARIABLELOC:
			{
				int n=Ins.LireVariable();
/*				if (n>=int(PileF.l))
					PileF.l=size_t(n)+1;*/
/*				ASSERT(DebutVarLoc<10000);
				ASSERT(n<10000);*/
				PileF.RedimMin(DebutVarLoc+n+1);
				areference r;
				r.Type=RefType::Local;
				r.Decalage=DebutVarLoc+n;
/*#ifdef _DEBUG
				clog << "Ecriture dans la variable locale "<<r.Decalage<<".\n";
#endif*/
				Pile.Ajouter(r);
				break;
			}
		case I_VALEURVARLOC:
			{
				int n=Ins.LireVariable();
	/*			if (n>int(PileF.l))
					PileF.l=size_t(n);*/
				PileF.RedimMin(DebutVarLoc+n+1);
				Pile.Ajouter(PileF[DebutVarLoc+n]);
/*#ifdef _DEBUG
				clog << "Lecture de la variable locale "<<DebutVarLoc+n<<".\n";
#endif*/

				break;
			}
		case I_PARAMREF:
			{
				int n=Ins.LireVariable();
/*				if (n>=int(PileF.l))
					PileF.l=size_t(n)+1;*/
				PileF.RedimMin(DebutParam+n+1);
				areference r;
				r.Type=RefType::Local;
				r.Decalage=DebutParam+n;
/*#ifdef _DEBUG
				clog << "Ecriture dans la variable locale "<<r.Decalage<<".\n";
#endif*/
		                Pile.Ajouter(r);
				break;
			}
		case I_PARAMVAL:
			{
				int n=Ins.LireVariable();
	/*			if (n>int(PileF.l))
					PileF.l=size_t(n);*/
				Pile.Ajouter(PileF[DebutParam+n]);
/*#ifdef _DEBUG
				clog << "Lecture de la variable locale "<<DebutVarLoc+n<<".\n";
#endif*/

				break;
			}
		case I_VARIABLE:
			{
				areference r;
				r.Type=RefType::Global;
				r.Decalage=Ins.LireVariable();
				Pile.Ajouter(r);
				break;
			}
		case I_VALEURVAR:
			{
				Pile.Ajouter(varGlob[Ins.LireVariable()]);
				break;
			}
		case I_VALEUR:
			{
				valeur v;Pile.Depiler(v);
				if (v.Type!=TYPE_REFERENCE)
					throw TXT("il faut prendre la valeur d'une variable");
				
				Pile.Ajouter(*Deref(v.v.p));
				break;
			}
			
		case I_DEBUTLIGNE:
			{
				programme->mutex.Liberer();
				DebutLigne=Ins.i-sizeof(code);
				Pile.Vider();
				//ASSERT(Pile.Pos==0);
				Ligne=Ins.LireEntier();
				programme->mutex.Prendre();
				//clog  << Ligne << '\n';
				break;
			}
		case I_CHANCES:
			{
				Pile.Ajouter(Ins.LireChances());
				break;
			}
		case I_CONVTYPE:
			{
				valeur * val=&Pile.Dernier();
				type premier=Ins.LireType();
				val->Convertir(Ins.LireType());
				switch (val->Type)
				{
				case TYPE_VARIABLE:
					{
						if (val->v.i>=int(programme->varGlob.l))
							val->v.i=int(programme->varGlob.l)-1;
						if (val->v.i<0)
							val->v.i=0;
						break;						
					}
				case TYPE_FONCTIONC:
					{
						break;
					}
				}
				break;
			}
		case I_AJOUTER:
			{
				valeur a;Pile.Depiler(a);
				valeur b;Pile.Depiler(b);
				Pile.Ajouter(a+b);
				/*if (Tracer)
					printf("Ajouter %d+%d\n",a,b);*/
				break;
			}
		case I_AJOUTERE:
			{
				valeur a;Pile.Depiler(a);
				Pile.Dernier().v.i+=a.v.i;
				break;
			}
		case I_SOUSTRAIREE:
			{
				valeur a;Pile.Depiler(a);
				Pile.Dernier().v.i-=a.v.i;
				break;
			}
		case I_FOISE:
			{
				valeur a;Pile.Depiler(a);
				Pile.Dernier().v.i*=a.v.i;
				break;
			}
		case I_DIVISERE:
			{
				valeur a;Pile.Depiler(a);
				Pile.Dernier().v.i/=a.v.i;
				break;
			}
		case I_SUPERPLUS:
			{
				valeur a;Pile.Depiler(a);
				valeur b;Pile.Depiler(b);
				b.SuperPlus(a.Entier());
				Pile.Ajouter(b);
				break;
			}
		case I_SOUSTRAIRE:
			{
				valeur a;Pile.Depiler(a);
				valeur b;Pile.Depiler(b);
				Pile.Ajouter(b-a);
				break;
			}
		case I_FOIS:
			{
				valeur a;Pile.Depiler(a);
				valeur b;Pile.Depiler(b);
				Pile.Ajouter(a*b);
				break;
			}
		case I_DIVISE:
			{
				valeur a;Pile.Depiler(a);
				valeur b;Pile.Depiler(b);
				Pile.Ajouter(b/a);
				break;
			}
		case I_EGAL:
			{
				valeur a;Pile.Depiler(a);
				valeur b;Pile.Depiler(b);
				Pile.Ajouter(b==a);
				break;
			}
		case I_DIFFERENT:
			{
				valeur a;Pile.Depiler(a);
				valeur b;Pile.Depiler(b);
				Pile.Ajouter(b!=a);
				break;
			}
		case I_SUPERIEUR:
			{
				valeur a;Pile.Depiler(a);
				valeur b;Pile.Depiler(b);
				Pile.Ajouter(b>a);
				break;
			}
		case I_INFERIEUR:
			{
				valeur a;Pile.Depiler(a);
				valeur b;Pile.Depiler(b);
				Pile.Ajouter(a>b);
				break;
			}
		case I_SUPEGAL:
			{
				valeur a;Pile.Depiler(a);
				valeur b;Pile.Depiler(b);
				Pile.Ajouter(b>=a);
				break;
			}
		case I_INFEGAL:
			{
				valeur a;Pile.Depiler(a);
				valeur b;Pile.Depiler(b);
				Pile.Ajouter(a>=b);
				break;
			}
		case I_NON:
			{
				valeur a;Pile.Depiler(a);
				Pile.Ajouter(!a);
				break;
			}
		case I_OPPOSE:
			{
				Pile.Dernier().Oppose();
				break;
			}
		case I_NONOPPOSE:
			{
				Pile.Dernier().EnNombre();
				break;
			}
		case I_RETOUR:
			{
				Retour();
				if (nAppels<AppelDepart)
				{
					programme->mutex.Liberer();
					return;
				}
				break;
			}
		case I_CONCAT:
			{
				valeur a;Pile.Depiler(a);a.EnChaine();
				valeur b;Pile.Depiler(b);b.EnChaine();
				valeur c;
				c.Type=TYPE_CHAINE;
				c.v.c=chaineval::Creer(a.v.c,b.v.c);
				Pile.Ajouter(c);
				/*a.v.c+=b.v.c; plus lent !!
				Pile.Ajouter(a);*/
				break;
			}
		case I_HACHAGEREF:
			{
				valeur i;
				Pile.Depiler(i);
				i.EnChaine();
				valeur var;
				Pile.Depiler(var);
				if (var.Type!=TYPE_REFERENCE)
					throw TXT("Esp�ce de demeur�, j'attends un hachage");
				valeur * v=Deref(var.v.p);
				v->EnTableHachage();
				areference r;
				v->v.h->ObtRef(r,i.v.c);
				Pile.Ajouter(r);
				break;
			}
		case I_HACHAGEVAL:
			{
				valeur i;
				Pile.Depiler(i);
				i.EnChaine();
				valeur var;
				Pile.Depiler(var);
				if (var.Type!=TYPE_REFERENCE)
					throw TXT("Esp�ce de demeur�, j'attends un hachage");
				valeur * v=Deref(var.v.p);
				if (v->Type==TYPE_HACHAGE)
				{
					valeur * ret=v->v.h->Get(i.v.c);
					if (ret)
						Pile.Ajouter(*ret);
					else
						Pile.AjouterIndef();
				}
				else
					Pile.AjouterIndef();
				break;
			}
		case I_THISREF:
			{
				areference r;
				This->ObtRef(r,Ins.LireEntier());
				Pile.Ajouter(r);
				break;
			}
		case I_THISVAL:
			{
				if (This->v)
					Pile.Ajouter(This->Get(Ins.LireEntier()));
				else
					throw TXT("pas de this");
				break;
			}
		case I_TABLEAUREF:
			{
				valeur i;
				Pile.Depiler(i);
				/*if (i.Type!=TYPE_ENTIER)
					throw CErreur(TXT("l'indice d'un tableau dont on prend la r�f�rence doit �tre un entier et non un %s"),
						NomType(i.Type));*/
				i.EnEntier();
				valeur var;
				Pile.Depiler(var);
				if (var.Type!=TYPE_REFERENCE)
					throw CErreur(TXT("esp�ce de demeur�, j'attends une r�f�rence et nom un %s"),
						NomType(i.Type));
				valeur * v=Deref(var.v.p);
				v->EnTableau();
				areference r;
				v->v.t->ObtRef(r,i.v.i);
				Pile.Ajouter(r);
				break;
			}
		case I_TABLEAUVAL:
			{
				valeur i;
				Pile.Depiler(i);
				/*if (i.Type!=TYPE_ENTIER)
					throw TXT("l'indice d'un tableau doit �tre un entier (val)");*/
				i.EnEntier();
				valeur var;
				Pile.Depiler(var);
				if (var.Type!=TYPE_REFERENCE)
					throw TXT("esp�ce de demeur�, j'attends un tableau");
				valeur * v=Deref(var.v.p);
				if (v->Type==TYPE_TABLEAU)
				{
					valeur * el=v->v.t->ObtPtr(i.v.i);
					if (!el)
						Pile.AjouterIndef();
					else
						Pile.Ajouter(*el);
				}
				else
					Pile.AjouterIndef();
				break;
			}
		case I_CTABLEAUREF:
			{
				int i=Ins.LireEntier();
				valeur var;
				Pile.Depiler(var);
				if (var.Type!=TYPE_REFERENCE)
					throw TXT("esp�ce de demeur�, j'attends un tableau");
				valeur * v=Deref(var.v.p);
				v->EnTableau();
				areference r;
				v->v.t->ObtRef(r,i);
				Pile.Ajouter(r);
				break;
			}
		case I_CTABLEAUVAL:
			{
				int i=Ins.LireEntier();
				valeur var;
				Pile.Depiler(var);
				if (var.Type!=TYPE_REFERENCE)
					throw TXT("esp�ce de demeur�, j'attends un tableau");
				valeur * v=Deref(var.v.p);
				if (v->Type==TYPE_TABLEAU)
				{
					valeur * el=v->v.t->ObtPtr(i);
					if (el)
						Pile.Ajouter(*el);
					else
						Pile.AjouterIndef();
				}
				else
					Pile.AjouterIndef();
				break;
			}
		case I_DEBUTPARAM:
			{
				Pile.AjouterDebutParam();
				break;
			}
		case I_DUPLIQUER:
			{
				Pile.Ajouter(Pile.Dernier());
				break;
			}
		case I_SUPPRIMER:
			{
				Pile.EffacerDernier();
				break;
			}
		case I_GOTO:
			{
				Pile.Vider();
				Ins.Goto(Ins.LireEtiquette());
				break;
			}
		case I_GOTOR:
			{
				Pile.Vider();
				Ins.GotoRel(Ins.LireEntier());
				break;
			}
		case I_GOTONONZERO:
			{
				valeur var;
				Pile.Depiler(var);
				code * cible=Ins.LireEtiquette();
				if (var.Entier()!=0)
				{
					Pile.Vider();
					Ins.Goto(cible);
				}
				break;
			}
		case I_GOTONONZEROR:
			{
				valeur var;
				Pile.Depiler(var);
				int cible=Ins.LireEntier();
				if (var.Entier()!=0)
				{
					Pile.Vider();
					Ins.GotoRel(cible);
				}
				break;
			}
		case I_GOTOZERO:
			{
				valeur var;
				Pile.Depiler(var);
				code * cible=Ins.LireEtiquette();
				if (var.Entier()==0)
				{
					Pile.Vider();
					Ins.Goto(cible);
				}
				break;
			}
		case I_GOTOZEROR:
			{
				valeur var;
				Pile.Depiler(var);
				int cible=Ins.LireEntier();
				if (var.Entier()==0)
				{
					Pile.Vider();
					Ins.GotoRel(cible);
				}
				break;
			}
		case I_TAILLE:
			{
				valeur var;
				Pile.Depiler(var);
				int Taille;
				if (var.Type!=TYPE_REFERENCE)
					throw TXT("on doit prendre la taille en utilisant une r�f�rence");
				valeur *v=Deref(var.v.p);
				switch(v->Type)
				{
				case TYPE_TABLEAU:
					Taille=int(v->v.t->l);
					break;
				case TYPE_HACHAGE:
					Taille=int(v->v.h->l);
					break;
				case TYPE_CHAINE:
					if (v->v.c)
						Taille=int(v->v.c->l);
					else
						Taille=0;
					break;
				default:
					Taille=0;
				}
				Pile.Ajouter(Taille);
				break;
			}
		case I_VALEURCLEF:
			{
				valeur var;Pile.Depiler(var);
				if (var.Type!=TYPE_REFERENCE ||
					var.v.p.Type!=RefType::Hachage)
					throw TXT("on ne peut prendre la valeur d'une clef que pour une r�f�rence � une valeur d'un hachage (je sais pas si vous avez suivi)");
				reference * r=&var.v.p;
				if (r->Decalage>=r->BaseH->l)
					throw TXT("gourou � hache 2");
				Pile.Ajouter(((pairehachage*)r->BaseH->v)[r->Decalage].Clef);
				break;
			}
		case I_PROBASAUTERLIGNE:
			{
				float Proba=Ins.LireChances();
				code *Cible=Ins.LireEtiquette();
				if (float(rand())/(RAND_MAX+1.0f)*100<Proba)
					Ins.Goto(Cible);
				break;
			}
		case I_NOUVEAU:
			{
				areference r;
				r.Type=RefType::Bebe;
				r.BaseB=new (UseGC) valeur();
				Pile.Ajouter(r);
				break;
			}

		case I_GOTOTACHE:
			{
				code * Cible=Ins.LireEtiquette();
				Pile.Ajouter((IObjet*)NouvelleTache(Cible));
				break;
			}
		case I_GOTOTACHER:
			{
				int Cible=Ins.LireEntier();
				Pile.Ajouter(NouvelleTache(Ins.i+Cible));
				break;
			}

		case I_FINTACHE:
			{
				programme->mutex.Liberer();
				return;
			}
		case I_DEFCONSTR:
			{
				Symbole * c=Ins.LireSymbole();				
				int n=Ins.LireVariable();
				if (n>int(PileF.l))
					PileF.l=index_t(n);
				PileF.Redim(DebutVarLoc+n+1);
				PileF[DebutVarLoc+n].EnTableau();
				PileF[DebutVarLoc+n].v.t->Classe=c;
				PileF[DebutVarLoc+n].v.t->Dimensions=0;
				break;
			}
		case I_THIS:
			{
				valeur MonThis;
				MonThis.Type=TYPE_TABLEAU;
				MonThis.v.t=This;
				Pile.Ajouter(MonThis);
				break;
			}
		case I_ENERREUR:
			{
				Appel[nAppels].EnErreur=Ins.LireEtiquette();				
				break;
			}
		case I_ENERREURR:
			{
				Appel[nAppels].EnErreur=Ins.i+Ins.LireEntier();				
				break;
			}
	
		case I_OU:
			{
				int Suite=Ins.LireEntier();
				valeur v;
				Pile.Depiler(v);
				if (v.Entier())
				{
					Pile.Ajouter(1);
					Ins.GotoRel(Suite);
				}
				break;
			}
		case I_ET:
			{
				int Suite=Ins.LireEntier();
				valeur v;
				Pile.Depiler(v);
				if (v.Entier()==0)
				{
					Pile.Ajouter(0);
					Ins.GotoRel(Suite);
				}
				break;
			}
		case I_FINOUET:
			{
				valeur v;
				Pile.Depiler(v);
				if (v.Entier())
				{
					Pile.Ajouter(1);
				}
				else
				{
					Pile.Ajouter(0);
				}
				break;
			}
		case I_TYPEDE:
			{
				valeur v;
				Pile.Depiler(v);
				if (v.Type!=TYPE_REFERENCE)
					throw TXT("r�f�rence attendue pour obtenir le type");
				valeur * w=Deref(v.v.p);
				if (w->Type==TYPE_TABLEAU && w->v.t->Dimensions==0
						&& w->v.t->Classe)
					Pile.Ajouter(w->v.t->Classe);
				else
					Pile.AjouterIndef();
				break;
			}
		case I_SWITCH:
			{
				valeur v;
				Pile.Depiler(v);
				int i=v.Entier();
				int s=Ins.LireEntier();
				CSwitch * c=Switch[s];
				CCasSwitch * res=(CCasSwitch * )bsearch(&i,c->Cas.t,c->Cas.l,sizeof(CCasSwitch),
					CompCasSwitch);
				if (res)
					Ins.Goto(res->Pos);
				else
					Ins.Goto(c->PosDefaut);
				break;
			}
		case I_ESTREFVALIDE:
			{
				valeur p;
				Pile.Depiler(p);
				if (p.Type!=TYPE_REFERENCE)
					throw TXT("seule une r�f�rence peut �tre une r�f�rence valide");
				switch(p.v.p.Type)
				{
				case RefType::Global:
					Pile.Ajouter(p.v.p.Decalage<programme->varGlob.l);
					break;
				case RefType::Local:
					Pile.Ajouter(p.v.p.Decalage<PileF.l);
					break;
				case RefType::Hachage:
					Pile.Ajouter(p.v.p.Decalage<p.v.p.BaseH->l);
					break;
				case RefType::Tableau:
					Pile.Ajouter(p.v.p.Decalage<p.v.p.Base->l);
					break;
				case RefType::Bebe:
					Pile.Ajouter(1);
					break;
				}
				break;					
			}
		case I_EMPILER2:
			{
				valeur v;
				Pile.Depiler(v);
				Pile2.Ajouter(v);
				break;
			}
/*		case I_EMPILERTOUT2:
			{
				valeur v;
				while (Pile.Pos)
				{
					Pile.Depiler(v);
					Pile2.Ajouter(v);
				}
				break;
			}*/
		case I_DEPILER2:
			{
				valeur v;
				Pile2.Depiler(v);
				Pile.Ajouter(v);
				break;
			}
/*		case I_DEPILERTOUT2:
			{
				valeur v;
				while (Pile2.Pos)
				{
					Pile2.Depiler(v);
					Pile.Ajouter(v);
				}
				break;
			}*/
		case I_VALEUR2:
			{
				Pile.Ajouter(Pile2.Dernier());
				break;
			}
	/*	case I_VALEURTOUT2:
			{
				for (int i=Pile.Pos-1; i>=0; i--)
				{
					Pile.Ajouter(Pile2.Pile[i]);
				}
				break;
			}*/
		case I_DECALG:
			{
				valeur combien;
				Pile.Depiler(combien);
				Pile.Dernier().EnEntier();
				Pile.Dernier().v.i<<=combien.Entier();
				break;
			}
		case I_DECALD:
			{
				valeur combien;
				Pile.Depiler(combien);
				Pile.Dernier().EnEntier();
				Pile.Dernier().v.i>>=combien.Entier();
				break;
			}
		case I_MODULO:
			{
				valeur combien;
				Pile.Depiler(combien);
				Pile.Dernier().EnEntier();
				Pile.Dernier().v.i%=combien.Entier();
				break;
			}
		case I_ETBIN:
			{
				valeur combien;
				Pile.Depiler(combien);
				Pile.Dernier().EnEntier();
				Pile.Dernier().v.i&=combien.Entier();
				break;
			}
		case I_OUBIN:
			{
				valeur combien;
				Pile.Depiler(combien);
				Pile.Dernier().EnEntier();
				Pile.Dernier().v.i|=combien.Entier();
				break;
			}
		case I_EXISTE:
			{
				valeur v;
				Pile.Depiler(v);
				Pile.Ajouter(v.Type!=TYPE_INCONNU);
				break;
			}
		case I_NPARAMS:
			{
				if (nAppels)
					Pile.Ajouter((int)Appel[nAppels-1].nParams);
				else
					throw TXT("impossible de conna�tre le nombre de param�tres quand on n'est pas dans une fonction");
				break;
			}
		case I_FONCTIONC:
			{
				Pile.AjouterType(TYPE_FONCTIONC,Ins.LireFonctionC());
				break;
			}
		case I_FONCTIONC_THIS:
			{
				Pile.AjouterType(TYPE_FONCTIONC_THIS,Ins.LireFonctionC());
				break;
			}
		case I_FONCTIONMODULE:
			{
				Pile.AjouterType(TYPE_FONCTIONMODULE,Ins.LireEntier());
				break;
			}
		case I_ETIQUETTE_THIS:
			Pile.AjouterType(TYPE_ETIQUETTE_THIS,Ins.LireEntier());
			break;
		/*case I_DEPLACERHAUTPILE:
			{
				int diff=Ins.LireEntier();
				ASSERT((int)Pile.Pos>=-diff);
				Pile.Pos+=diff;
				break;
			}*/
		case I_ALLOCATION:
			{
				Pile.Ajouter((IObjet*)GC_MALLOC(Ins.LireEntier()));
				break;
			}
		default:
			throw TXT("instruction inconnue");
		}
	}
	programme->mutex.Liberer();
}

void GotoPP::CInterpreteur::LireThis()
{
	valeur var;
	Pile.Depiler(var);

	if (var.Type!=TYPE_REFERENCE)
		throw TXT("r�f�rence attendue pour lancer une m�thode avec EXECTHIS");

	valeur *vp=Deref(var.v.p);
	if (vp->Type!=TYPE_TABLEAU)
		throw TXT("tableau attendu pour servir de this � EXECTHIS");
	This=vp->v.t;
}

void GotoPP::CInterpreteur::AppelerEtiquette(code* v, bool appelThis)
{
	if (nAppels==NMAXAPPELS)
		throw TXT("faudrait pas abuser de la r�cursion mon gars");
					
	Appel[nAppels].Instruction=Ins.i;
	if (appelThis)
	{
		if (This)
			Appel[nAppels].This=This;
		LireThis();
	}
	/*else
		Appel[nAppels].This=0;*/
	
	index_t param=Pile.Pos;
	while (param && Pile.Pile[param-1].Type!=TYPE_DEBUTPARAM)
		param--;
	//clog << "La pile contient "<<Pile.Pos<<" valeurs. Les params commencent par la valeur n�"<<param<<".\n";
	Appel[nAppels].lPile=param;
	Appel[nAppels].PosPileF=PileF.l;
	Appel[nAppels].nParams=Pile.Pos-param;
	DebutParam=PileF.l+param;
	DebutVarLoc=PileF.l+Pile.Pos;
	/*clog <<"Il y avait "<<PileF.l<<" variables locales.\n";
	clog << "DebutVarLoc = "<<DebutVarLoc<<"\n";*/
	if (Pile.Pos)
	{
		//On met la pile dans PileF
		//Oui, c'est pourri
		memcpy(PileF.Fin(),&Pile.Pile[0],sizeof(valeur)*Pile.Pos);
		for (index_t i=0; i<Pile.Pos; i++)
			Pile.Pile[i].Effacer();
		PileF.l+=Pile.Pos;
		Pile.Pos=0;
	}
	//clog << "Il y a "<<PileF.l<<" variables locales.\n";
	nAppels++;
	Appel[nAppels].EnErreur=NULL;

/*#ifdef _DEBUG
	clog << "Appel vers "<<v<<".\n";
#endif*/

	Ins.Goto(v);
}

void GotoPP::CInterpreteur::AppelerFonctionC(voidf fonc)
{
	try
	{
		fonc(this);
	}
	catch (const carac * car)
	{
		varGlob[0]=chaineval::Creer(car);
		Exception();
	}
}

void GotoPP::CInterpreteur::AppelerMethodeC(voidf fonc)
{
	//TODO: pourquoi une r�f�rence et pas la valeur ?
	valeur v;
	Pile.Depiler(v);
	if (v.Type!=TYPE_REFERENCE)
		throw TXT("appel de m�thode sur un truc qui n'est pas une r�f�rence sur un objet");
	valeur * obj=Deref(v.v.p);
	if (obj->Type!=TYPE_OBJETC)
		throw TXT("appel de m�thode sur un truc qui n'est pas un objet");
	
	ThisC=obj->v.o;
	try
	{
		fonc(this);
	}
	catch (const carac * car)
	{
		varGlob[0]=chaineval::Creer(car);
		Exception();
	}
}

void GotoPP::CInterpreteur::Retour()
{
	if (nAppels==0)
	      throw TXT("un REGOTO doit suivre un GOTOCHEPA ou un appel de fonction");

	Ins.Goto(Appel[--nAppels].Instruction);//On se remet � la bonne instruction
	/*#ifdef _DEBUG
	clog << "Retour vers "<<Ins.i-Ins.c<<".\n";
	#endif*/

	if (nAppels)//Si on n'est toujours pas en global
	{
		DebutParam=Appel[nAppels-1].PosPileF+Appel[nAppels-1].lPile;
		DebutVarLoc=DebutParam+Appel[nAppels-1].nParams;
	}
	else
	{
		DebutParam=0;
		DebutVarLoc=BasenParams;
	}
	if (Appel[nAppels].This)
	{
	      This=Appel[nAppels].This;
		  Appel[nAppels].This=0;
	}
	//Si il y a une valeur dans la pile on la met en valeur de retour
	if (!Pile.Vide())
	{
	      memcpy(&PileF[Appel[nAppels].PosPileF+Appel[nAppels].lPile-1],
		      &Pile.Dernier(),sizeof(valeur));
		  Pile.Dernier().Effacer();
	      Pile.Pos--;
	}

	Pile.Vider();
	//On supprime tout ce qui se trouve au del� de PosPileF+lPile
	//(c'est � dire la position avant l'appel de fonction +
	//ce qui �tait dans la pile et qu'on a mis l�)
	//En gros, c'est les variables locales
	index_t k=Appel[nAppels].PosPileF+Appel[nAppels].lPile;
	while (PileF.l>k)
		PileF.EffacerDernier();

	//On remet dans la pile les lPile valeurs � partir de PosPileF
	memcpy(&Pile.Pile[0],&PileF[Appel[nAppels].PosPileF],Appel[nAppels].lPile*sizeof(valeur));
	//On les supprime des variables locales
	memset(&PileF[Appel[nAppels].PosPileF],0,Appel[nAppels].lPile*sizeof(valeur));
	Pile.Pos=uint(Appel[nAppels].lPile);
	PileF.l=uint(Appel[nAppels].PosPileF);
}
namespace DefBib
{
	void RestaurerConsole();
}

void GotoPP::CInterpreteur::Exception()
{
	int a=nAppels;
	Pile.Vider();
	while (a && Appel[a].EnErreur==NULL)
	{
		Retour();
		a--;
	}
	if (Appel[a].EnErreur==NULL)
	{
		DefBib::RestaurerConsole();
		czerr << "Exception non g�r�e : ";
		varGlob[0].Ecrire(stderr);
		czerr <<"\n";
		Ins.Goto(NULL);
	}
	else
		Ins.Goto(Appel[a].EnErreur);
}

valeur * GotoPP::CInterpreteur::Deref(reference & r)
{
	if (r.Type==RefType::Global)
	{
		if (r.Decalage>=programme->varGlob.l) throw TXT("gourou global");
		return &varGlob[r.Decalage];
	}
	else if (r.Type==RefType::Local)
	{
		if (r.Decalage>=PileF.l) 
			throw TXT("gourou local");
		return &PileF[r.Decalage];
	}
	else if (r.Type==RefType::Hachage)
	{
		if (r.Decalage<r.BaseH->l)
		{
			//clog <<((pairehachage*)r.BaseH->v)[r.Decalage].Clef;
			return &((pairehachage*)r.BaseH->v)[r.Decalage].Val;
		}
		else
			throw TXT("gourou � hache");
	}
	else if (r.Type==RefType::Tableau)
	{
		if (r.Decalage<r.Base->l)
			return &((valeur*)r.Base->v)[r.Decalage];
		else
			throw TXT("gourou");
	}
	else if (r.Type==RefType::Bebe)
	{
		return (valeur*)r.BaseB;
	}
	else
		throw TXT("r�f�rence non intialis�e");
}		

void GotoPP::CInterpreteur::Goto(code *v)
{
	Pile.Vider();
	Ins.Goto(v);
}


void GotoPP::CInterpreteur::AfficherErreur(const carac *c)
{
	::AfficherErreur(c,AcFichierSource(),Ligne,true);
}

void GotoPP::CInterpreteur::AfficherErreur(GotoPP::CErreur&e)
{
	AfficherErreur(e.Texte());
}


//Ca vous a plu ? Envoyez-moi un message � gotopp@sidoine.net, vous recevrez
//un cadeau.
