#ifndef Reader_H
#define Reader_H
#include <iostream>
#include <fstream>
#include <string>
//#include <vector>
//#include <stdlib.h>
//#include "Geomvec"
#include "Particle.h"
#include "Parameters.h"
#include "Box.h"



class Reader {

  protected:
    ifstream is;
    Parameters * par;
    vector<Particle> pList;
    double Lx,Ly,R;
    bool hasRadii, hasAngles;
    Box * box;
  
  public:
    Reader() {};
    Reader(Parameters * & par) { this->par=par; };
    ~Reader() { is.close(); };  //delete par; delete box; };

    void setParameters(Parameters * & par) { this->par=par; };
    Parameters * getParameters() { return par; };
    vector<Particle> & getPList() { return pList; };
    double getLx() { return Lx; };
    double getLy() { return Ly; };
    bool getHasRadii() { return hasRadii; };
    bool getHasAngles() { return hasAngles; };
    void open(const string & filename);

    void readPacking(const string & filename);
    void readPos(const string & filename);
    void readPosVel(const string & filename);
    void readPosinit(const string & filename);
    void readMeanPos(const string & filename);
    void glueBoundary();
    void glueBoundaryByAngle();
    
    void readBox(const string & filename);
};





#endif
