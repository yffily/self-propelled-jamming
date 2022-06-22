#include "GeomVec.h"

// notations:
// v1=*this is the vector currently in use (for member functions only)
// v2 is another vector
// v is used when there's only one vector and the function is not a member
// f is a float (double)

  int Geomvec::nd;

// constructors et destructor  
  Geomvec::Geomvec(int n)				// to be used once to initialize
    { nd=n; x=new double[nd]; }				// static integer nd
  Geomvec::Geomvec(void)				// default vector equals 0
    { x=new double[nd]; }
//    for (int i=0;i<nd;i++) {x[i] = 0.0;} }
  Geomvec::Geomvec(int n, double f)			// default vector equals 0
    { nd=n; x=new double[nd];
    for (int i=0;i<nd;i++) {x[i] = f;} }
  Geomvec::Geomvec(const Geomvec &v2)			// define a vector equal to another
    { // nd=v2.getDimension();
    x=new double[nd];
    for (int i=0;i<nd;i++) {x[i] = v2.getX(i);} }
  Geomvec::Geomvec(const double &x1, const double &x2)	// [2D] defines a vector by its components
    { x=new double[nd]; x[0]=x1; x[1]=x2; }
  Geomvec::Geomvec(const double &angle)			// [2D] make a unit vector from angle
    { x=new double[nd]; x[0]=cos(angle); x[1]=sin(angle); }
  Geomvec::~Geomvec(void) {delete [] x;}

// setting private members
  void Geomvec::setDimension(int n) { nd=n; }		// sets the dimension nd
  void Geomvec::setX(int i, double f) { x[i]=f; }	// set the coordinate i

// assignment operators
  Geomvec& Geomvec::operator=(const Geomvec &v2)	// v1 = v2;
    { //nd=v2.getDimension();
    for (int i=0;i<nd;i++) {x[i] = v2.getX(i);}
    return *this; }
  Geomvec& Geomvec::operator=(const double &f)		// v1 = f;
    { for (int i=0;i<nd;i++) {x[i] = f;}
    return *this; }
  Geomvec& Geomvec::operator+=(const Geomvec &v2)	// v1 += v2;
    { for (int i=0;i<nd;i++) {x[i] += v2.getX(i);}
    return *this; }
  Geomvec& Geomvec::operator+=(const double &f)		// v1 += f;
    { for (int i=0;i<nd;i++) {x[i] += f;}
    return *this; }
  Geomvec& Geomvec::operator-=(const Geomvec &v2)	// v1 -= v2;
    { for (int i=0;i<nd;i++) {x[i] -= v2.getX(i);}
    return *this; }
  Geomvec& Geomvec::operator-=(const double &f)		// v1 -= f;
    { for (int i=0;i<nd;i++) {x[i] -= f;}
    return *this; }
  Geomvec& Geomvec::operator*=(const double &f)		// v1 *= f
    { for (int i=0;i<nd;i++) {x[i] *= f;}
    return *this; }
  Geomvec& Geomvec::operator/=(const double &f)		// v1 /= f
    { for (int i=0;i<nd;i++) {x[i] /= f;}
    return *this; }
  Geomvec operator-(Geomvec v1)				// -v1
    { for (int i=0;i<v1.getDimension();i++) {v1.setX(i,-v1.getX(i));}
    return v1; }

// binary operators & arithmetic operators
  Geomvec Geomvec::operator+(Geomvec v2) const		// v1+v2
    { return v2+=*this; };
  Geomvec operator+(Geomvec v, const double& f)		// v+f
    { return v+=f; }
  Geomvec operator+(const double& f, Geomvec v)		// f+v
    { return v+=f; }
  Geomvec Geomvec::operator-(Geomvec v2) const		// v1-v2
    { return -v2+=*this; };
  Geomvec operator-(Geomvec v, const double& f)		// v-f
    { return v-=f; }
  Geomvec operator-(const double& f, Geomvec v)		// f-v
    { return -v+=f; }
  double Geomvec::operator*(const Geomvec& v2) const	// v1.v2 (inner product)
    { double ps=x[0]*v2.x[0];
    for (int i=1;i<nd;i++) {ps+=x[i]*v2.getX(i);}
    return ps; };
  Geomvec operator*(Geomvec v, const double &f)		// v*f
    { return v*=f; }
  Geomvec operator*(const double &f, Geomvec v)		// f*v
    { return v*=f; }
  Geomvec operator/(Geomvec v, const double &f)		// v/f
    { return v/=f; }
  double crossprod2D(const Geomvec& v1, const Geomvec& v2)	// [2D] cross product
    { return v1.getX(0)*v2.getX(1)-v1.getX(1)*v2.getX(0); }
// boolean operators
  bool Geomvec::operator == (const Geomvec& v2)		// v1==v2
    { bool b=true;
    for (int i=0;i<nd && b;i++) {b = (b && x[i]==v2.getX(i)); }
    return b; }
  bool Geomvec::operator != (const Geomvec& v2)		// v1!=v2
    { return !(*this==v2); }

// misc
  Geomvec Geomvec::unit(void) const			// unit vector
    { return Geomvec(*this)/=norm(); }
  Geomvec Geomvec::perp2D(void) const			// [2D] orthogonal vector
    { return Geomvec(-x[1],x[0]); }
  double Geomvec::angle2D(void) const			// [2D] angle with x axis
    {
    return atan2(x[1],x[0]);
//    if (x[0]>0) { return atan(x[1]/x[0]); }
//    else if (x[0]<0 && x[1]>=0) { return atan(x[1]/x[0])+pi; }
//    else if (x[0]<0 && x[1]<0) { return atan(x[1]/x[0])-pi; }
//    else if (x[0]==0 && x[1]>0) { return pi/2; }
//    else if (x[0]==0 && x[1]<0) { return -pi/2; }
//    else { return 0; }
    }


// printing and reading
  ostream& operator<<(ostream& stm, const Geomvec& v)		// stm << v
    { for (int i=0;i<v.getDimension();i++) {stm << v.getX(i) << sep;} return stm; }
  ostringstream& operator<<(ostringstream& stm, Geomvec& v)	// stgstm << v
    { for (int i=0;i<v.getDimension();i++) {stm << v.getX(i) << sep;} return stm; }
// reading: first character and delimiters have to be exactly one (non number or dot) character.
// the dimension nd is assumed to be known already and additional coordinates are ignored.
  istream& operator>>(istream& stm, Geomvec& v)	// stm << v
    { double x; char a;
    stm >> a;
    for (int i=0;i<v.getDimension();i++) {stm >> x >> a; v.setX(i,x);}
    return stm; }
  istringstream& operator>>(istringstream& stm, Geomvec& v)	// stgstm >> v
    { double x; char a;
    stm >> a;
    for (int i=0;i<v.getDimension();i++) {stm >> x >> a; v.setX(i,x);}
    return stm; }

