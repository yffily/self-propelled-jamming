#ifndef Container_H
#define Container_H
#include <vector>
#include <string>
#include "Particle.h"
#include "Parameters.h"
#include "Initializer.h"
#include "Dynamics.h"
#include "Interaction.h"
#include "NeighborList.h"
#include "Boundary.h"
#include "Box.h"
#include "Reader.h"
using namespace std;


class Container {
  
  Parameters * par, par_input;
  Box * box, * inibox;
  Initializer * ini;
  Dynamics * dyn;
  NeighborList * NL;
  Interaction * inter, * anginter;
  Boundary * BC;
  vector<Particle> pList;
  
  public:
    Container(Parameters * par);
    ~Container()
      { if (inibox!=box) { delete inibox; } delete box;
      delete ini; delete dyn; delete NL; delete BC;
      delete inter; delete anginter; 
};
    
    Dynamics * getDyn() const { return dyn; };
    double getTime() const { return dyn->getTime(); };  
    const vector<Particle> & getPList() const { return pList; };
    const Particle & getPList(int i) const { return pList[i]; };

    void update();
    void addParticle();
    void removeParticle();
};

void chooseBox(Box * & box, Parameters * par);
void chooseIniBox(Box * & inibox, Box * & box, Parameters * par);
void chooseInitializer(Initializer * & ini, Parameters * par);
void chooseBoundary(Boundary * & BC, Box * box, Parameters * par);
void chooseInteraction(Interaction * & inter, Parameters * par);
void chooseAngularInteraction(Interaction * & inter, Boundary * & BC, Parameters * par);
void chooseDynamics(Dynamics * & dyn, Boundary * BC, Interaction * inter, Interaction * anginter, Parameters * par);
void chooseNeighborList(NeighborList * & NL, Boundary * BC, Parameters * par);


#endif
