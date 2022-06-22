#ifndef Particle_H
#define Particle_H
#include <vector>
#include <list>
#include "Constants.h"
#include "GeomVec.h"
#include "Parameters.h"
#include "RNG_taus.h"
using namespace std;


class Geomvec;

class Particle {
  
  double radius;
  Geomvec pos;
  Geomvec vel;
  double angle;
  double angvel;
  Geomvec aVel;			// active velocity=v0*(cos(angle),sin(angle))
  Geomvec noise;
  double angnoise;
  double v0;
  
  int index;
  bool glued;
  int nc;			// number of particles in contact/in interaction
    
  public:
    
    Particle() {};
    Particle(int i);
    Particle(int i, Geomvec v);
    Particle(int i, Geomvec v, Parameters * par);
    Particle(int i, Geomvec v, double angle);
    Particle(int i, Geomvec v, double angle, Parameters * par);
    Particle(int i, Geomvec v, double radius, double angle, Parameters * par);
    Particle(int i, Geomvec pos, double radius, bool glued, Parameters * par);
//    Particle(Parameters par);
//    ~Particle();
    
    void setID(int i) { index=i; };
    void setPos(const Geomvec & pos) { this->pos=pos; };
    void setVel(const Geomvec & vel) { this->vel=vel; };
    void setAngle(double angle) { this->angle=angle; };
    void setAngVel(double angvel) { this->angvel=angvel; };
    void setRadius(double radius) { this->radius=radius; };
    void setGlued(bool glued) { this->glued=glued; };
    void setNoise(RNG_taus & RNG, const double sigma);
    void setAngNoise(RNG_taus & RNG, const double sigma);
    void setV0(double v0) { this->v0=v0; };
    void setNc(int nc) { this->nc=nc; };

    int getID() const { return index; };
    Geomvec getPos() const { return pos; };
    Geomvec getVel() const { return vel; };
    double getAngle() const { return angle; };
    double getAngVel() const { return angvel; };
    Geomvec getAVel() const { return aVel; };
    double getRadius() const { return radius; };
    bool getGlued() const { return glued; };
    Geomvec getNoise() { return noise; };
    double getAngNoise() { return angnoise; };
    double getV0() const { return v0; };
    int getNc() const { return nc; };

    void addPos(const Geomvec & pos) { this->pos+=pos; };
    void addAngle(const double & angle) { this->angle+=angle; };
    void updateAVel() { aVel=v0*Geomvec(angle); };
    void addVel(const Geomvec & vel) { this->vel+=vel; };
    void addAngVel(const double & angvel) { this->angvel+=angvel; };
    void addNc(const int i) { this->nc+=i; };
    void setActiveVel() { vel+=v0*Geomvec(angle); };
    void setAlignAngVel();
    void move(const double & dt);
    bool move(const double & dt, double & dposmax, double & dangmax);
    bool move_cstVel(const double & dt, double & dposmax, double & dangmax);
    void printPos(ofstream & stm) const;
	void printPosRad(ofstream & stm) const;
    void printVel(ofstream & stm) const;
	void printPosVel(ofstream & stm) const;
       
};


#endif
