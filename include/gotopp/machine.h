#ifndef _GPP_MV_H
#define _GPP_MV_H
#include "pile.h"
#include "instruction.h"
#include "gppmodule.h"

namespace GotoPP
{
	class CVariable;
	class CErreur;
	class Programme;
	class CSwitch;
	struct SLabel;
	class CTache;

	class CAppel
	{
	public:
		code * Instruction;
		index_t lPile;//Nombre de valeurs qu'il y avait dans la pile avant l'appel
				//y compris le param�tre de retour
		index_t PosPileF;//Combien il y avait de variables locales avant l'appel
		index_t nParams;//Nombre de param�tres pass�s � la fonction
		tableauval * This;
		code * EnErreur;
	};

	const int NMAXAPPELS=128;

	class PileFonction
	{
		static const index_t TailleMax=4096;
		valeur val[TailleMax];
	public:
		index_t l;
		valeur & operator[](index_t i)
		{
			ASSERT(i<l);
			return val[i];
		}
		void RedimMin(index_t taille)
		{
			ASSERT(taille<TailleMax);
			if (l<taille)
				l=taille;
		}

		void Redim(index_t taille)
		{
			ASSERT(taille<TailleMax);
			l=taille;
		}
		valeur * Fin()
		{
			return val+l;
		}
		void EffacerDernier()
		{
			ASSERT(l>0);
			val[--l].Effacer();
		}
		PileFonction():l(0){}
	};

	class CInterpreteur:public gc, public IInterpreteur
	{
		CPile Pile2;
		index_t BasenParams;
		tableauval * This;
		uint Ligne;
		carac* AcFichierSource();
		code *DebutLigne;
		CAppel Appel[NMAXAPPELS];
		CInstruction Ins;
		PileFonction PileF;
		index_t posPileF;
		index_t DebutParam;
		index_t DebutVarLoc;
		CSwitch ** Switch;
		
	public:
		valeur * varGlob;
		CPile Pile;
		IObjet * ThisC;
		regexp * DerExpReg;
		valeur DerVar;
		int nAppels;

	public:
		SLabel ** Label;
		Programme * programme;
		void UtiliserProgramme(Programme * programme);
		CTache * NouvelleTache(code * Depart);
		CInterpreteur(Programme * programme);
		//Divers
		valeur * Deref(reference & r);
		void Executer(code * i);
		void Executer();
		void ExecuterCatch(code * i);
		void ExecuterCatch();
		void ContenuPile();
		void Sauver(FILE *F);
		void Charger(FILE *F,int Taille);
		void Reinit();
		void *Compilateur;
		void Retour();
		void AppelerEtiquette(code * v, bool appelThis=false);
		void AppelerFonctionC(voidf f);
		void AppelerMethodeC(voidf f);
		void Goto(code *v);
		void Exception();
		void LireThis();
		void AfficherErreur(CErreur & e);
		void AfficherErreur(const carac *c);

	public: //Impl�mentation de IInterpreteur
		IObjet * GetThis();
		virtual const chaineval * LireChaine();
		virtual int LireEntier();
		virtual float LireFlottant();
		virtual void * LirePointeur();
		virtual void * LireVariable(int v);
		virtual IChaineA * LireChaineA();
		virtual void FinParametres();
		virtual IObjet * LireObjet();
		virtual ITableauValeurs * LireTableauValeurs();
		virtual regexp * LireRegExp();
		virtual IValeur *LireValeur();
		virtual entier64 LireEntier64();
		virtual int GetNombreParametres();
		
		virtual void RetournerChaine(carac * c);
		virtual void RetournerChaineA(char * c);
		virtual void RetournerEntier(int i);
		virtual void RetournerFlottant(float f);
		virtual void RetournerIndefini();
		virtual void RetournerChaine(const IChaine * chaine);
		virtual void RetournerCarac(carac c);
		virtual void RetournerTableauValeurs(ITableauValeurs * tableau);
		virtual void RetournerEntier64(entier64 i);
		virtual void RetournerObjet(IObjet * o);

		virtual void Modifier(void * p, IObjet * o);
		virtual void Modifier(void * p, int i);
		virtual void Modifier(void * p, carac * c);
		virtual void Modifier(void * p, carac * c, int l);
#ifdef _UNICODE
		virtual void Modifier(void * p, char * c, int l);
#endif

		virtual void Ajouter(void *p, const chaineval * clef, const chaineval * val, size_t lval);
		
		virtual void Exception(carac *Texte);
		virtual void DebutParametres();
		virtual void AppelerFonction(code* f);
	};

	typedef void (CInterpreteur::*FonctionGPP)();
}


#endif
