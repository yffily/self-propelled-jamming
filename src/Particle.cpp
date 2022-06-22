#include "Particle.h"
using namespace std;



		
Particle::Particle(int i) {
	index=i;
	glued=false;
}
		
Particle::Particle(int i, Geomvec2 pos) {
	index=i;
	(this->pos)=pos;
	glued=false;
}
		
Particle::Particle(int i, Geomvec2 pos, Parameters * par) {
	index=i;
	this->pos=pos;
	this->radius=par->radius;
	this->v0=par->v0;
	noise=Geomvec2(0.,0.);
	angnoise=0.;
	glued=false;
}

Particle::Particle(int i, Geomvec2 pos, double angle) {
	index=i;
	this->pos=pos;
	this->angle=angle;
	glued=false;
}

Particle::Particle(int i, Geomvec2 pos, double angle, Parameters * par) {
	index=i;
	this->pos=pos;
	this->angle=angle;
	this->radius=par->radius;
	this->v0=par->v0;
	noise=Geomvec2(0.,0.);
	angnoise=0.;
	glued=false;
}


void Particle::setNoise(RNG_taus & RNG, const double sigma) {
	noise=Geomvec2(sigma*random_normal(RNG),sigma*random_normal(RNG));
}

void Particle::setAngNoise(RNG_taus & RNG, const double sigma) {
	angnoise=sigma*random_normal(RNG);
}

void Particle::move(const double & dt) {
	pos+=dt*vel+noise;
	angle+=dt*angvel+angnoise;
}

// this version of 'move' keeps track of max displacements (for time step adjustments)
bool Particle::move(const double & dt, double & dposmax, double & dangmax) {
	bool max=0;
	Geomvec2 dpos=dt*(vel+aVel)+noise;
	double dr=dpos.norm2();
	double dang=dt*angvel+angnoise;
	pos+=dpos;
	angle+=dang;
	if (dr>dposmax) { dposmax=dr; max=1;}
	if (dang>dangmax) { dangmax=dang; max=1;}
	return max;
}

// motion at constant velocity (norm)
bool Particle::move_cstVel(const double & dt, double & dposmax, double & dangmax) {
	bool max=0;
	Geomvec2 dpos=dt*aVel;
	double dr=dpos.norm2();
	double dang=dt*angvel+angnoise;
	pos+=dpos;
	angle+=dang;
	if (dr>dposmax) { dposmax=dr; max=1;}
	if (dang>dangmax) { dangmax=dang; max=1;}
	return max;
}

void Particle::printPos(ostream & stm) const {
	stm << index << sep << pos << angle << endl;
}

void Particle::printPosRad(ostream & stm) const {
	stm << setprecision(15);
	stm << pos << angle << sep << radius << endl;
}

void Particle::printVel(ostream & stm) const {
	stm << index << sep << vel+aVel << angvel << endl;
}

void Particle::printPosVel(ostream & stm) const {
	stm << setprecision(15) << pos << setprecision(8) << angle << sep << vel+aVel << angvel << sep << nc << endl;
}


