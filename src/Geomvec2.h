#ifndef Geomvec2_H
#define Geomvec2_H

#include <iostream>
#include <fstream>
//#include <sstream>
#include <cmath>
#include "util.h"
#include "RNG_taus.h"

//using namespace std;

class Geomvec2 {

public:
	double x,y;
	
	Geomvec2 ();
	Geomvec2 (const double & x_, const double & y_);
	Geomvec2 (const Geomvec2 & v);
//	Geomvec2 (const double & d);
	
	
	double X(int i);
	void set_X(int i, const double & d);
	
	Geomvec2 & operator = (const Geomvec2 & v);
	Geomvec2 & operator = (const double & d);
	
	Geomvec2 & operator += (const Geomvec2 & v);
	Geomvec2 & operator -= (const Geomvec2 & v);
	Geomvec2 & operator *= (const double & d);
	Geomvec2 & operator /= (const double & d);
	
	Geomvec2 operator + (const Geomvec2 & v) const;
	Geomvec2 operator - (const Geomvec2 & v) const;
	
	Geomvec2 operator * (const double & d) const;
	Geomvec2 operator / (const double & d) const;
	
	double operator * (const Geomvec2 & v) const;   // dot product
	double operator ^ (const Geomvec2 & v) const;   // 2D "cross product"
	
	Geomvec2 & invert_ew (void);                    // element wise inversion
	
	bool operator == (const Geomvec2 & v) const;
	bool operator != (const Geomvec2 & v) const;
	
	double norm (void) const;
	double norm2 (void) const;
	Geomvec2 & normalize (void);
	Geomvec2 unit (void) const;
	
	Geomvec2 perp(void) const;              // orthogonal vector
	double angle(void) const;             // angle with x axis

};


Geomvec2 operator - (const Geomvec2 & v);

Geomvec2 operator * (const double & f, const Geomvec2 & v);

Geomvec2 multiply_ew (const Geomvec2 & v, const Geomvec2 & w);
Geomvec2 divide_ew (const Geomvec2 & v, const Geomvec2 & w);

ostream & operator << (ostream & stm, const Geomvec2 & v);
istream & operator >> (istream & stm, Geomvec2 & v);

Geomvec2 unit_vector_from_angle(const double & angle);

//Geomvec2 random_vec(RNG_taus & rng); 
//Geomvec2 random_vec(RNG_taus & rng, double amplitude[]);
//Geomvec2 random_vec(RNG_taus & rng, Geomvec2 amplitude=Geomvec2(1.,1.));
//Geomvec2 random_normal_vec(RNG_taus & rng, Geomvec2 amplitude=Geomvec2(1.,1.));


#endif


