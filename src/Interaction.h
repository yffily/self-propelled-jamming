#ifndef Interaction_H
#define Interaction_H
#include <iostream>
#include <stdlib.h>
#include "Particle.h"
#include "GeomVec.h"
#include "NeighborList.h"
#include "Boundary.h"
using namespace std;

class Neighbor;
class Boundary;

// NB: the active force is in the class'particle'

class Interaction {

  protected:
    Parameters * par;
    Boundary * BC;

  public:
    Interaction() {};
    Interaction(Parameters * par, Boundary * BC) { this->par=par; this->BC=BC; };

    virtual Geomvec getForce(const Geomvec & dpos, const double & dist, const double & cutoff)
      { return Geomvec(0.,0.); };
    virtual Geomvec getForce(const Particle & p, const Neighbor & n) { return Geomvec(0.,0.); };
    virtual double getTorque(const Particle & p, const Neighbor & n) { return 0.; };
};


class Interaction_Elastic: public Interaction {

  double k_rep;			// elastic constant
// this way this class can be used with different k's (eg for a cell-cell and cell-wall interaction)

  public:
    Interaction_Elastic(const double k_rep) { this->k_rep=k_rep; };
    Geomvec getForce(const Geomvec & dpos, const double & dist, const double & cutoff);
    Geomvec getForce(const Particle & p, const Neighbor & n);
}; 


class Interaction_ElasticWithAdhesion: public Interaction {

  double k_rep, F_ad, k_ad, R_ad;	// k_ad=prefactor for adhesion, F_ad=maximum adhesion force

  public:
    Interaction_ElasticWithAdhesion(const double k_rep, const double F_ad, const double R_ad);
    Geomvec getForce(const Geomvec & dpos, const double & dist, const double & cutoff);
    Geomvec getForce(const Particle & p, const Neighbor & n);
}; 


class Interaction_Vicsek: public Interaction {

  double tau_V;			// coupling time between neighbors

  public:
    Interaction_Vicsek(Boundary * BC, const double tau_V) { this->BC=BC; this->tau_V=tau_V; };
    double getTorque(const Particle & p, const Neighbor & n);
}; 


#endif
