#ifndef _COMPILATEUR_H
#define _COMPILATEUR_H

namespace GotoPP
{
	class BoutCode;
	class CErreur;

	enum AccesPropriete
	{
		PROP_PUBLIQUE,//La propri�t� est toujours accessible
		PROP_PROTEGEE,//La propri�t� n'est accessible que depuis les m�thodes
		PROP_PRIVEE, //La propri�t� devient cach�e quand on d�rive la classe
		PROP_CACHEE //La propri�t� est inaccessible
	};

	class Compilateur;
	class Programme;

	class ICompilateur
	{
	public:
		virtual BoutCode* Charger(const carac * fichier)=0;//Charge et compile ce fichier
		virtual BoutCode* Compiler(const carac *CodeSource)=0;//Compile ce code source
		
		virtual void AfficherErreur(CErreur & e)=0;
		virtual void AfficherErreur(const carac *c)=0;

		static ICompilateur * Factory(Programme * programme);
	};
}

#endif
