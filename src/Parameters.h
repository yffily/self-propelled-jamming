#ifndef Params_H
#define Params_H
//#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>		// definit la classe de variable "string"
#include <cmath>		// fonctions mathematiques de base
#include <stdlib.h>
#include "ConfigFile.h"
#include "GeomVec.h"
using namespace std;


class Parameters
  {
  public:  				// to be made private !
  string filename;
  
  bool verbose;
  string outdir;			// output directory
  int np;
  double tsim;
  int nsuivi, nstepsuivi;
  double tprint; int nprint;

// particle
  double v0;
  double tau_S;
  double radius, polydis;		// radius of particles: average and half width
  unsigned int seed_iniRad;
  bool selfAlign;

// box
  string boxtype, boxfile;
  Geomvec boxVec1, boxVec2;
  double boxRadius;

  double kBox;
  double rangeBox;

// boundary
  string BCtype;
  double BCradius, BCpolydis, BCoverlap;		// allows different radius for glued particles

// initializer
  string initype, iniangletype, iniformat, fillmode;
  string paramfile;
  unsigned long int seed_iniPos, seed_iniAngle;
  string inifilename;
  double phi;				// packing fraction
  double stripfrac;			// width of the strip as a fraction of the box size
  string iniboxtype;
  Geomvec iniboxVec1, iniboxVec2;
  double iniboxRadius;
  double offsetX, offsetY;

// dynamics
  string numtype;
  double dt; int nstep;
  double sigma;		// angular noise
  unsigned long int seed_th;
  string filename_maxstep;

// interaction
  string inttype, anginttype;
  double eta;
  double k_rep;				// spring constant for contact repulsion
  double R_ad, F_ad;			// range and maximum force for adhesion
  double tau_V;

// dynamics: neighbor list
  string NLtype;
  string rListType;
  double rList, rListRatio;
  

//  public:
    Parameters();
    Parameters(const char* filename);
//    Parameters(string filename) { Parameters(filename.c_str()); };
    ~Parameters() {};
    
    void print(string filename);
};

#endif
