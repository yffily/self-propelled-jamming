#ifndef NeighborList_H
#define NeighborList_H
#include <iostream>
#include <vector>
#include <list>
#include <stdlib.h>
#include <cmath>
#include <algorithm>
#include "Particle.h"
#include "GeomVec.h"
#include "Boundary.h"
#include "Interaction.h"
using namespace std;

class Boundary;
class Box;
class Particle;
class Parameters;
class Interaction;

class Neighbor {

  protected:
//    Particle * p0;		// pointer to particle to which p is a neighbor
    Particle * p;		// pointer to particle which is a neighbor to p0
    Geomvec dpos;
    double dist, dist2;
    bool contact;		// true if the neighbor within the interaction range
// for some types of interactions, only the square of the distance might be needed
// it might be a good idea to make two derived classes to deal with that

  public:
    Neighbor() {};
    Neighbor(const Neighbor & n) { *this=n; };
    Neighbor(Particle & p) { this->p=&p; };
    Neighbor(Particle & p, Geomvec dpos) { this->p=&p; this->dpos=dpos; };
    Neighbor(Particle & p, Geomvec dpos, double dist2) 
      { this->p=&p; this->dpos=dpos; this->dist2=dist2; };

//    Particle * getPart0() const { return p0; };
    Particle * getPart() const { return p; };
    Geomvec getDPos() const { return dpos; };
    double getDist() const { return dist; };
    double getDist2() const { return dist2; };
    bool getContact() const { return contact; };
    void setDPos(Geomvec dpos) { this->dpos=dpos; };
    void setDist(double dist) { this->dist=dist; };
    void setDist2(double dist2) { this->dist=dist2; };
    void setContact(bool contact) { this->contact=contact; };
    void updateDist() { dist=sqrt(dist2); };	// dist2 needs to have been updated already !
    void updateDist2() { dist2=dpos.norm2(); };

};


class NeighborList {
  
  protected:
    Parameters * par;
    Boundary * BC;
    vector <list <Neighbor> > HNL;		// half neighbor list (no double counting)
    vector <int> nN;				// number of neighbors
    double rInt, rInt2;
    
  public:
    NeighborList() {};
    NeighborList(Parameters * par, Boundary * BC);

    vector <list <Neighbor> > & getHNL() { return HNL; };
    list <Neighbor> & getHNL(int i) { return HNL[i]; };
    void makeRInt(vector<Particle> & pList, Interaction * inter, Interaction * anginter);
    virtual void checkRInt() {};

    void checkNeighbors(vector<Particle> & pList, Interaction * inter, Interaction * anginter);
    virtual bool checkForUpdate(vector<Particle> & pList) { return false; };
    virtual void makeList(vector<Particle> & pList) =0;
    void printNeighborList(const string & filename);

};



class NeighborList_Everyone: public NeighborList {
  public:
    NeighborList_Everyone(Parameters * par, Boundary * BC): NeighborList(par,BC) {};
    void makeList(vector<Particle> & pList);
};
 

class NeighborList_Square: public NeighborList {

  public:
    NeighborList_Square(Parameters * par, Boundary * BC): NeighborList(par,BC) {};
    virtual bool checkForUpdate(vector<Particle> & pList) { return true; };
    void makeList(vector<Particle> & pList);
};
 

class NeighborList_SquareVerlet: public NeighborList {

  protected:
    double rList, rList2, rUpdate2;
    vector<Geomvec>  pList_last;		// list of positions at last NL update

  public:
    NeighborList_SquareVerlet(Parameters * par, Boundary * BC);
    void checkRInt();
    bool checkForUpdate(vector<Particle> & pList);
    void makeList(vector<Particle> & pList);
};
 

class NeighborList_Verlet: public NeighborList {

  protected:
    double rList, rList2, rUpdate2;
    vector<Geomvec>  pList_last;		// list of positions at last NL update

  public:
    NeighborList_Verlet(Parameters * par, Boundary * BC);
    void checkRInt();
    bool checkForUpdate(vector<Particle> & pList);
    void makeList(vector<Particle> & pList);
};
 


#endif
