#ifndef Interaction_H
#define Interaction_H
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <algorithm>
#include "Particle.h"
#include "Geomvec2.h"
#include "NeighborList.h"
#include "Boundary.h"
using namespace std;

class Neighbor;
class Boundary;

// NB: the active force is in the class 'particle'

class Interaction {

	protected:
		Parameters * par;
		Boundary * BC;
		bool sym;			// whether or not there is action reaction
		double * nonsymTorque;

	public:
		Interaction() {};
		Interaction(Parameters * par, Boundary * BC) { 
		this->par=par; this->BC=BC; sym=true; };
		virtual ~Interaction() {};
		
		bool getSym() { return sym; };

		double getRContactInt(vector <Particle> & pList);
		virtual double getRInt(double d) { return d; };
		virtual Geomvec2 getForce(const Geomvec2 & dpos, const double & dist, const double & cutoff)
			{ return Geomvec2(0.,0.); };
		virtual Geomvec2 getForce(const Particle & p, const Neighbor & n) { return Geomvec2(0.,0.); };
		virtual double getTorque(const Particle & p, const Neighbor & n) { return 0.; };
		virtual double * getTorqueNS(const Particle & p, const Neighbor & n)
			{ nonsymTorque[0]=0.; nonsymTorque[1]=0.; return nonsymTorque; };
		virtual Geomvec2 getPol(const Particle & p, const Neighbor & n) { return Geomvec2(0.,0.); };
};


class Interaction_Elastic: public Interaction {

	double k_rep;			// elastic constant
// this way this class can be used with different k's (eg for a cell-cell and cell-wall interaction)

	public:
		Interaction_Elastic(const double k_rep) { this->k_rep=k_rep; };
		Geomvec2 getForce(const Geomvec2 & dpos, const double & dist, const double & cutoff);
		Geomvec2 getForce(const Particle & p, const Neighbor & n);
}; 


class Interaction_ElasticWithAdhesion: public Interaction {

	double k_rep, F_ad, k_ad, R_ad;	// k_ad=prefactor for adhesion, F_ad=maximum adhesion force

	public:
		double getRInt(double d) { return d+R_ad; };
		Interaction_ElasticWithAdhesion(const double k_rep, const double F_ad, const double R_ad);
		Geomvec2 getForce(const Geomvec2 & dpos, const double & dist, const double & cutoff);
		Geomvec2 getForce(const Particle & p, const Neighbor & n);
}; 


class Interaction_Vicsek: public Interaction {

	double tau;			// coupling time between neighbors

	public:
		Interaction_Vicsek(Boundary * BC, const double tau) { this->BC=BC; this->tau=tau; };
		double getTorque(const Particle & p, const Neighbor & n);
}; 


class Interaction_Boussinesq: public Interaction {

	double k_rep;			// elastic constant for soft repulsion
	double poisson, F_bq;		// poisson ratio and magnitude of boussinesq coupling
	double r_bq;			// cut off radius for boussinesq
	double r3,theta;		// distance cube and angle of dpos (used by both force et torque)
	double pref_force_cut;
	double pref_torque_cut;

	public:
		Interaction_Boussinesq(const double k_rep, const double poisson, const double F_bq, const double r_bq)
			{ this->k_rep=k_rep; this->poisson=poisson; this->F_bq=F_bq; this->r_bq=r_bq;
			sym=false; nonsymTorque=new double[2];
			pref_force_cut=0.25*(poisson+1)*F_bq/pow(r_bq,3);
			pref_torque_cut=0.375*F_bq*(poisson+1)/pow(r_bq,5);
			};
		~Interaction_Boussinesq() { delete [] nonsymTorque; };
		
		double getRInt(double d) { return max(d,r_bq); };
		Geomvec2 getForce(const Particle & p, const Neighbor & n);
		Geomvec2 getForce_bq(const Geomvec2 & dpos, const double & dist, const double & t1, const double & t2);
		Geomvec2 getForce_repulsion(const Geomvec2 & dpos, const double & dist, const double & cutoff);
//		double getTorque(const Particle & p, const Neighbor & n);
		double * getTorqueNS(const Particle & p, const Neighbor & n);
}; 


#endif
