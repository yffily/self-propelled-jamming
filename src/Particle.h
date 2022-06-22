#ifndef Particle_H
#define Particle_H
#include <vector>
#include <list>
#include "util.h"
#include "Geomvec2.h"
#include "Parameters.h"
#include "RNG_taus.h"
using namespace std;


class Geomvec2;

class Particle {
	
	public:
	double radius;
	Geomvec2 pos;
	Geomvec2 vel;
	double angle;
	double angvel;
	Geomvec2 aVel;			// active velocity=v0*(cos(angle),sin(angle))
	Geomvec2 noise;
	double angnoise;
	double v0;
	
	int index;
	bool glued;
	int nc;			// number of particles in contact/in interaction
		
//	public:
		
		Particle() {};
		Particle(int i);
		Particle(int i, Geomvec2 v);
		Particle(int i, Geomvec2 v, Parameters * par);
		Particle(int i, Geomvec2 v, double angle);
		Particle(int i, Geomvec2 v, double angle, Parameters * par);
		Particle(int i, Geomvec2 v, double radius, double angle, Parameters * par);
		Particle(int i, Geomvec2 pos, double radius, bool glued, Parameters * par);
//    Particle(Parameters par);
//    ~Particle();
		
		void setID(int i) { index=i; };
		void setPos(const Geomvec2 & pos) { this->pos=pos; };
		void setVel(const Geomvec2 & vel) { this->vel=vel; };
		void setAngle(double angle) { this->angle=angle; };
		void setAngVel(double angvel) { this->angvel=angvel; };
		void setRadius(double radius) { this->radius=radius; };
		void setGlued(bool glued) { this->glued=glued; };
		void setNoise(RNG_taus & RNG, const double sigma);
		void setAngNoise(RNG_taus & RNG, const double sigma);
		void setV0(double v0) { this->v0=v0; };
		void setNc(int nc) { this->nc=nc; };

		int getID() const { return index; };
		Geomvec2 getPos() const { return pos; };
		Geomvec2 getVel() const { return vel; };
		double getAngle() const { return angle; };
		double getAngVel() const { return angvel; };
		Geomvec2 getAVel() const { return aVel; };
		double getRadius() const { return radius; };
		bool getGlued() const { return glued; };
		Geomvec2 getNoise() { return noise; };
		double getAngNoise() { return angnoise; };
		double getV0() const { return v0; };
		int getNc() const { return nc; };

		void addPos(const Geomvec2 & pos) { this->pos+=pos; };
		void addAngle(const double & angle) { this->angle+=angle; };
		void updateAVel() { aVel=v0*unit_vector_from_angle(angle); };
		void addVel(const Geomvec2 & vel) { this->vel+=vel; };
		void addAngVel(const double & angvel) { this->angvel+=angvel; };
		void addNc(const int i) { this->nc+=i; };
		void setActiveVel() { vel+=v0*unit_vector_from_angle(angle); };
		void setAlignAngVel();
		void move(const double & dt);
		bool move(const double & dt, double & dposmax, double & dangmax);
		bool move_cstVel(const double & dt, double & dposmax, double & dangmax);
		void printPos(ostream & stm) const;
	void printPosRad(ostream & stm) const;
		void printVel(ostream & stm) const;
	void printPosVel(ostream & stm) const;
			 
};


#endif
