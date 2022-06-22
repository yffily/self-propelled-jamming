#ifndef Dynamics_H
#define Dynamics_H
#include <stdlib.h>
#include "Particle.h"
#include "GeomVec.h"
#include "Interaction.h"
#include "Boundary.h"
#include "NeighborList.h"
#include "RNG_taus.h"
using namespace std;


class Dynamics {
  
  protected:
    Parameters * par;
    Interaction * inter, * anginter;
    NeighborList * NL;
    Boundary * BC;
    RNG_taus RNG;
    double dposmax, dangmax;
    bool max;
    ofstream stm_max;
    double t;
  
  public:
    Dynamics() {};
    Dynamics(Parameters * par, Boundary * BC, Interaction * inter, Interaction * anginter);

    double getTime() const { return t; };  
//    void setParameters(Parameters * par) { this->par=par; };

    void getNoise(vector<Particle> & pList);  
    void getVel(vector<Particle> & pList, NeighborList * NL);  
    virtual void move(vector<Particle> & pList, NeighborList * NL) =0;

};



class Dynamics_Euler: public Dynamics {
  
  public:
    Dynamics_Euler(Parameters * par, Boundary * BC, Interaction * inter, Interaction * anginter): Dynamics(par,BC,inter,anginter) {};
    
    void move(vector<Particle> & pList, NeighborList * NL);

};
 


class Dynamics_RK2: public Dynamics {
  
  public:
    Dynamics_RK2(Parameters * par, Boundary * BC, Interaction * inter, Interaction * anginter): Dynamics(par,BC,inter,anginter) {};
    
    void move(vector<Particle> & pList, NeighborList * NL) 
      { cout << "Sorry the RK2 integrator hasn't been written yet" << endl; exit(100); };

};
 

#endif
