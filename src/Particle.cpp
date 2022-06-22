#include "Particle.h"
using namespace std;



    
Particle::Particle(int i) {
  index=i;
  glued=false;
}
    
Particle::Particle(int i, Geomvec pos) {
  index=i;
  (this->pos)=pos;
  glued=false;
}
    
Particle::Particle(int i, Geomvec pos, Parameters * par) {
  index=i;
  this->pos=pos;
  this->radius=par->radius;
  this->v0=par->v0;
  this->tau_S=par->tau_S;
  noise=Geomvec(pos.getDimension(),0.);
  angnoise=0.;
  glued=false;
}

Particle::Particle(int i, Geomvec pos, double angle) {
  index=i;
  this->pos=pos;
  this->angle=angle;
  glued=false;
}

Particle::Particle(int i, Geomvec pos, double angle, Parameters * par) {
  index=i;
  this->pos=pos;
  this->angle=angle;
  this->radius=par->radius;
  this->v0=par->v0;
  this->tau_S=par->tau_S;
  noise=Geomvec(pos.getDimension(),0.);
  angnoise=0.;
  glued=false;
}

Particle::Particle(int i, Geomvec pos, double radius, double angle, Parameters * par) {
  index=i;
  this->pos=pos;
  this->angle=angle;
  this->radius=radius;
  this->v0=par->v0;
  this->tau_S=par->tau_S;
  noise=Geomvec(pos.getDimension(),0.);
  angnoise=0.;
  glued=false;
}

Particle::Particle(int i, Geomvec pos, double radius, bool glued, Parameters * par) {
  index=i;
  this->pos=pos;
  this->angle=0.0;
  this->radius=radius;
  this->v0=0.0;
  this->tau_S=par->tau_S;
  noise=Geomvec(pos.getDimension(),0.);
  angnoise=0.;
  this->vel=Geomvec(0.0,0.0);
  this->angvel=0.0;
  this->glued=glued;
}
    
void Particle::setNoise(RNG_taus & RNG, const double sigma) {
  noise=Geomvec(sigma*random_normal(RNG),sigma*random_normal(RNG));
}

void Particle::setAngNoise(RNG_taus & RNG, const double sigma) {
  angnoise=sigma*random_normal(RNG);
}

void Particle::setAlignAngVel() {
  if (selfAlign)
    {
    double theta=vel.angle2D()-angle;
    if (theta>pi) {theta-=dblpi;}
    if (theta<=-pi) {theta+=dblpi;}
    angvel+=theta/tau_S;
    }
}


void Particle::move(const double & dt) {
  pos+=dt*vel;			//+noise;
  angle+=dt*angvel+angnoise;
}

// this version of 'move' keeps track of max displacements (for time step adjustments)
bool Particle::move(const double & dt, double & dposmax, double & dangmax) {
  bool max=0;
  double dpos=(dt*vel).norm2();	//+noise).norm2();
  double dang=dt*angvel+angnoise;
  pos+=dt*vel;			//+noise;
  angle+=dang;
  if (dpos>dposmax) { dposmax=dpos; max=1;}
  if (dang>dangmax) { dangmax=dang; max=1;}
  return max;
}


void Particle::printPos(ofstream & stm) const {
  stm << index << sep << pos << angle << endl;
}

void Particle::printPosRad(ofstream & stm) const {
  stm << pos << angle << sep << radius << endl;
}

void Particle::printVel(ofstream & stm) const {
  stm << index << sep << vel << angvel << endl;
}

void Particle::printPosVel(ofstream & stm) const {
  stm << pos << angle << sep << vel << angvel << sep << radius << sep << nc << endl;
}


