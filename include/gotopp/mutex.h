#ifndef _MUTEX_H
#define _MUTEX_H

namespace GotoPP
{
	class Mutex
	{
		char _data[8];//TODO: remplacer par un truc dans config.h
		//Comme �a on cache compl�tement l'impl�mentation
		//histoire de ne pas avoir � inclure 500000 fichiers
	public:
		Mutex();
		~Mutex();
		void Liberer();
		void Prendre();
	};
}

#endif
