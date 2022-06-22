#ifndef Params_H
#define Params_H
//#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>		// definit la classe de variable "string"
#include <cmath>		// fonctions mathematiques de base
#include <stdlib.h>
#include <algorithm>
#include "ConfigFile.h"
#include "GeomVec.h"
using namespace std;


class Parameters
  {
  public:				// to be made private !
  bool verbose;
  string outdir;			// output directory
  int np;
  double tsim;
  int nsuivi, nstepsuivi;
  double tprint, tprintini; int nprint, nprintini;

// particle
  double v0;
  double radius, polydis;		// radius of particles: average and half width
  unsigned int seed_iniRad;

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
  string initype, iniangletype, iniradtype, iniformat, fillmode;
  unsigned long int seed_iniPos, seed_iniAngle;
  string inifilename;
  double phi;				// packing fraction
  string iniboxtype;
  Geomvec iniboxVec1, iniboxVec2;
  double iniboxRadius;
  double offsetX, offsetY;

// dynamics
  string numtype;
  double dt; int nstep;
  double sigma_r, sigma_t;		// rotational and translational noise
  unsigned long int seed_rth,seed_tth;
  string filename_maxstep;

// interaction
  string inttype, anginttype;
  double eta;
  double k_rep;				// spring constant for contact repulsion
  double R_ad, F_ad;			// range and maximum force for adhesion
  double tau;
  double a_vs, a_vn, a_F;		// a_vs/a_vn=coef of own/neighbor velocity in angular alignment
  					// a_F=coef of force in angular alignment
  double poisson, F_bq;			// poisson ratio and boussinesq interaction prefactor
  double r_bq;				// cut off for boussinesq interaction

// dynamics: neighbor list
  string NLtype;
  string rListType;
  double rList, rListRatio;
  

//  public:
    Parameters();
    Parameters(char* filename);
    ~Parameters() {};
    
    void print(string filename);
};

#endif
