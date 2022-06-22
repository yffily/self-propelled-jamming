#ifndef Boundary_H
#define Boundary_H
//#include <iostream>
#include <vector>
#include <list>
#include <stdlib.h>
#include "util.h"
#include "Particle.h"
#include "Box.h"
#include "Geomvec2.h"
using namespace std;

class Box;
//class Geomvec2;

class Boundary {
	
	protected:
		Parameters * par;

	public:
		Box * box;
		
		Boundary() {};
		Boundary(Parameters * par, Box * box) {this->par=par; this->box=box; };
		virtual ~Boundary() {};

		virtual void makeBoundary(vector<Particle> & pList) {};
		void Enforce(vector<Particle> & pList);
		virtual Geomvec2 EnforceCentered(Geomvec2 v) { return v; };	// can't be Geomvec2 & v since pList[i] can't be modified directly
		double EnforceAngle(double angle);
};


class Boundary_Free: public Boundary {

	public:
//    Boundary_Free(Parameters * par, Box * box): Boundary(par,box) {};
		void Enforce(vector<Particle> & pList) {};
};
 

class Boundary_Periodic: public Boundary {

	public:
		Boundary_Periodic(Parameters * par, Box * box): Boundary(par,box) {};
		Geomvec2 EnforceCentered(Geomvec2 v);
};
 

class Boundary_Glued: public Boundary {
	
	public:
		Boundary_Glued(Parameters * par, Box * box): Boundary(par,box) {};
		void makeBoundary(vector<Particle> & pList);
		Geomvec2 EnforceCentered(Geomvec2 v) {return v;};
};


#endif
