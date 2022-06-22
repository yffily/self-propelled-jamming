#ifndef Initializer_H
#define Initializer_H
#include <iomanip>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <vector>
#include <cmath>
#include "Reader.h"
#include "RNG_taus.h"
//#include "Boundary.h"
#include "GeomVec.h"
#include "Particle.h"
#include "Box.h"
// NB: in order to get cross references between Container and Initializer to work:
// - "Container.h" shouldn't be included here, but a forward declaration should be used
// - "Container.h" should include "Initializer.h" but souldn't forward declare it
#include "Parameters.h"
#include "Constants.h"
using namespace std;


class Initializer {
  
  protected:
    Parameters * par;
    Reader R;
  
  public:
    Initializer() {};
    Initializer(Parameters * par) { this->par=par; };

    Parameters * getParameters() { return par; };
    virtual void Initialize(vector<Particle> & pList, Box * & inibox, Box * & box) =0;
    void InitializeAngle(vector<Particle> & pList);
    void InitializeAngle_Random(vector<Particle> & pList);
    void InitializeAngle_Zero(vector<Particle> & pList);
    void InitializeAngle_Azimuthal(vector<Particle> & pList);
    void rescale(double a, vector<Particle> & pList, Box * & inibox, Box * & box);
    void rescaleToPhi(double phi, vector<Particle> & pList, Box * & inibox, Box * & box);
};


class Initializer_fromFile: public Initializer {
  
  public:
    Initializer_fromFile(Parameters * par): Initializer(par) {};

    void Initialize(vector<Particle> & pList, Box * & inibox, Box * & box);
};


class Initializer_Random: public Initializer {
  
  RNG_taus RNG_pos, RNG_rad;

  public:
    Initializer_Random(Parameters * par): Initializer(par) {};
    void Initialize(vector<Particle> & pList, Box * & inibox, Box * & box);
};


class Initializer_Hex: public Initializer {		// hexagonal lattice
  
  RNG_taus RNG_rad;
  
  public:
    Initializer_Hex(Parameters * par): Initializer(par) {};
    void Initialize(vector<Particle> & pList, Box * & inibox, Box * & box);
};


#endif
