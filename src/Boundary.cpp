#include "Boundary.h"
#include "RNG_taus.h"

// base class //

void Boundary::Enforce(vector<Particle> & pList) {
  vector <Particle>::iterator it;
  for (it=pList.begin();it!=pList.end();it++)
    { it->setPos(EnforceCentered(it->getPos()));
    it->setAngle(EnforceAngle(it->getAngle())); }
}

double Boundary::EnforceAngle(double angle) {
  if (angle>pi) { angle-=dblpi; }
  if (angle<=-pi) { angle+=dblpi; }
  return angle;
}


// Periodic //

Geomvec Boundary_Periodic::EnforceCentered(Geomvec v) {
  return box->bringBack(v);
}


// Glued //

void Boundary_Glued::makeBoundary(vector<Particle> & pList) {
  RNG_taus RNG(par->seed_iniRad);		// make a different seed ?
  int i=par->np;
  double a=(1+1.5*par->BCradius/sqrt(box->getArea()));	// rescaling factor to conserve box effective area
  double l=0, rad=par->BCradius+par->BCpolydis;
  while (l<=box->getPerimeter())
    {
    pList.push_back(Particle(i,a*box->makeBoundaryPoint(l),par));
    pList.back().setRadius(rad);
    pList.back().setGlued(true);
    l+=rad*(1-par->BCoverlap)/a;
    rad=par->BCradius+par->BCpolydis*(2*RNG.get_double()-1);
    l+=rad*(1-par->BCoverlap)/a;
    i++;
    }
  if (par->verbose)
    { cout << i-par->np << " glued particles added on the boundary" << endl; }
  par->np=i;
}
		

