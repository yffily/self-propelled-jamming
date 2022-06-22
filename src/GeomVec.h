#ifndef geomvec_H
#define geomvec_H

#include <iostream>
#include <sstream>
#include <cmath>
#include <fstream>
#include "Constants.h"
using namespace std;

class Geomvec {
//  protected:
  static int nd;
  double* x;

  public:
    Geomvec(int n);
    Geomvec(void);
    Geomvec(int n, double f);
    Geomvec(const Geomvec &v2);
    Geomvec(const double &x1, const double &x2);
    Geomvec(const double &angle);
    ~Geomvec(void);
    void setDimension(int n);
    void setX(int i, double f);
    inline int getDimension() const;
    inline double getX(int i) const;
    Geomvec& operator=(const Geomvec &v);
    Geomvec& operator=(const double &v);
    Geomvec& operator+=(const Geomvec &v);
    Geomvec& operator+=(const double &v);
    Geomvec& operator-=(const Geomvec &v);
    Geomvec& operator-=(const double &v);
    Geomvec& operator*=(const double &v);
    Geomvec& operator/=(const double &v);

    Geomvec operator+(Geomvec v) const;
    Geomvec operator-(Geomvec v) const;
    double operator*(const Geomvec& v) const;

    bool operator == (const Geomvec& v);
    bool operator != (const Geomvec& v);

    inline double norm(void) const;
    inline double norm2(void) const;
    Geomvec unit(void) const;
    Geomvec perp2D(void) const;
    double angle2D(void) const;

    friend Geomvec operator-(Geomvec v);
    friend double crossprod2D(const Geomvec& v1, const Geomvec& v2);
    friend ostream& operator<<(ostream& stm, const Geomvec &v);
    friend istream& operator>>(istream& stm, Geomvec &v);
    friend istringstream& operator>>(istringstream& stm, Geomvec &v);
    friend ostringstream& operator<<(ostringstream& stm, Geomvec &v);
};


  inline int Geomvec::getDimension() const { return nd; }
  inline double Geomvec::getX(int i) const { return x[i]; }

// Note on operations involving an object which is not a vector:
// When the vector is on the right of the operator the function can't be a member !
// ex: -vector, scalar+vector, scalar*vector, stream << vector
// for clarity, the same structure is used when the vector is first by the second term is not a vector
  Geomvec operator+(Geomvec v, const double& f);
  Geomvec operator+(const double& f, Geomvec v);
  Geomvec operator-(Geomvec v, const double& f);
  Geomvec operator-(const double& f, Geomvec v);
  Geomvec operator*(Geomvec v, const double &f);
  Geomvec operator*(const double& f, Geomvec v);
  Geomvec operator/(Geomvec v, const double &f);

  inline double Geomvec::norm(void) const	// norme du vecteur
    { return sqrt(norm2()); }
  inline double Geomvec::norm2(void) const		// norme au carre
    { double l2=0;
    for (int k=0;k<nd;k++) {l2+=x[k]*x[k];}
    return l2; }

#endif

// pass by value or by reference ?
// 1. +=, -=, *=,...
// the argument isn't modified in the function
// -> pass by const reference
// 2. -(Geomvec v1)
// returns modified version of v1, but the actual v1 shouldn't be modified
// -> a copy of v1 needed to be made anyway -> pass by value


