#ifndef Dynamics_H
#define Dynamics_H
#include <stdlib.h>
#include <vector>
//#include <iostream>
//#include <iomanip>
#include "Particle.h"
#include "Geomvec2.h"
#include "Interaction.h"
#include "Boundary.h"
#include "NeighborList.h"
#include "RNG_taus.h"
using namespace std;


class Dynamics {
	
	protected:
		Parameters * par;
		Interaction * inter, * anginter;
//    NeighborList * NL;
		Boundary * BC;
		RNG_taus RNG_r;	// random numbers for rotational noise
		RNG_taus RNG_t;	// random numbers for translational noise
		vector<double> targetAngle;
		double dposmax, dangmax;
		bool max;
		ofstream stm_max;
		double t;
	
	public:
		Dynamics() {};
		Dynamics(Parameters * par, Boundary * BC, Interaction * inter, Interaction * anginter);
		virtual ~Dynamics() {};

		double getTime() const { return t; };  
//    void setParameters(Parameters * par) { this->par=par; };
		void getNoise(vector<Particle> & pList);  
		void getTargetAngle(vector<Particle> & pList, NeighborList * NL);  
		void getAngVel(vector<Particle> & pList, NeighborList * NL);
		void getAngVel_angint(vector<Particle> & pList, NeighborList * NL);
		void getAngVel_target(vector<Particle> & pList);  
		void getVel(vector<Particle> & pList, NeighborList * NL);
		void resetDrMax() { dposmax=0; dangmax=0; };

		virtual void move(vector<Particle> & pList, NeighborList * NL) =0;

};



class Dynamics_Euler: public Dynamics {
	
	public:
		Dynamics_Euler(Parameters * par, Boundary * BC, Interaction * inter, Interaction * anginter): Dynamics(par,BC,inter,anginter) {};
		
		void move(vector<Particle> & pList, NeighborList * NL);

};
 


class Dynamics_Euler_InstAng: public Dynamics {
	
	public:
		Dynamics_Euler_InstAng(Parameters * par, Boundary * BC, Interaction * inter, Interaction * anginter): Dynamics(par,BC,inter,anginter) {};
		
		void move(vector<Particle> & pList, NeighborList * NL);
		void setAngle(vector<Particle> & pList, NeighborList * NL);

};
 


class Dynamics_RK2: public Dynamics {
	
	public:
		Dynamics_RK2(Parameters * par, Boundary * BC, Interaction * inter, Interaction * anginter): Dynamics(par,BC,inter,anginter) {};
		
		void move(vector<Particle> & pList, NeighborList * NL) 
			{ cout << "Sorry the RK2 integrator hasn't been written yet" << endl; exit(100); };

};
 

#endif
