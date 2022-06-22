#ifndef Boundary_H
#define Boundary_H
//#include <iostream>
#include <vector>
#include <list>
#include <stdlib.h>
#include "Constants.h"
#include "Particle.h"
#include "Box.h"
#include "GeomVec.h"
using namespace std;

class Box;
//class Geomvec;

class Boundary {
  
  protected:
    Parameters * par;
    Box * box;

  public:
    Boundary() {};
    Boundary(Parameters * par, Box * box) {this->par=par; this->box=box; };

    virtual void makeBoundary(vector<Particle> & pList) {};
    void Enforce(vector<Particle> & pList);
    virtual Geomvec EnforceCentered(Geomvec v) { return v; };	// can't be Geomvec & v since pList[i] can't be modified directly
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
    Geomvec EnforceCentered(Geomvec v);
};
 

class Boundary_Glued: public Boundary {
	
  public:
    Boundary_Glued(Parameters * par, Box * box): Boundary(par,box) {};
    void makeBoundary(vector<Particle> & pList);
    Geomvec EnforceCentered(Geomvec v) {return v;};
};


#endif
