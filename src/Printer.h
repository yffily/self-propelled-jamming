#ifndef Printer_H
#define Printer_H
#include <iostream>		// entrees-sorties vers (depuis) l'ecran (le clavier)
#include <string>		// definit la classe de variable "string"
#include <fstream>		// entrees-sorties vers (depuis) des fichiers
#include "Container.h"
using namespace std;


class Printer {
	
	ofstream os;
	
	public:
//    Printer(ofstream & os) { this->os=os; };			// copying a stream is forbidden !!
		Printer(string filename) { os.open(filename.c_str()); };
		~Printer() { os.close(); };
		
		void close() { os.close(); };
		void printPos(const Container & C);
	void printPosRad(const Container & C);
		void printVel(const Container & C);
	void printPosVel(const Container & C);
		void printPosDyn(const Container & C);
	
};




#endif
