#include "Geomvec2.h"


Geomvec2::Geomvec2() {}

Geomvec2::Geomvec2(const double & x_, const double & y_) : x(x_), y(y_) {}

Geomvec2::Geomvec2(const Geomvec2 & v) : x(v.x), y(v.y) {}

//Geomvec2::Geomvec2(const double & d) : x(d), y(d) {}


double Geomvec2::X(int i)
	{
	if (i==0)      return x;
	else if (i==1) return y;
	else { cerr << "vector has no component #" << i << endl; exit(1); }
	}

void Geomvec2::set_X(int i, const double & d)
	{
	if (i==0)      x=d;
	else if (i==1) y=d;
	else { cerr << "vector has no component #" << i << endl; exit(1); }
	}


Geomvec2 & Geomvec2::operator = (const Geomvec2 & v)
	{
	x = v.x; y = v.y;
	return *this; 
	}

Geomvec2 & Geomvec2::operator = (const double & d)
	{
	x = d; y = d;
	return *this; 
	}

Geomvec2 & Geomvec2::operator += (const Geomvec2 & v)
	{
	x += v.x; y += v.y;
	return *this; 
	}

Geomvec2 & Geomvec2::operator -= (const Geomvec2 & v)
	{
	x -= v.x; y -= v.y; 
	return *this; 
	}

Geomvec2 & Geomvec2::operator *= (const double & d)
	{ 
	x *= d; y *= d;
	return *this; 
	}

Geomvec2 & Geomvec2::operator /= (const double & d)
	{ 
	x /= d; y /= d;
	return *this; 
	}


Geomvec2 Geomvec2::operator + (const Geomvec2 & v) const
		{ 
		return Geomvec2(x+v.x,y+v.y);
		};

Geomvec2 Geomvec2::operator - (const Geomvec2 & v) const
	{ 
	return Geomvec2(x-v.x,y-v.y);
	};


Geomvec2 Geomvec2::operator * (const double & d) const
	{ 
	return Geomvec2(x*d,y*d); 
	}

Geomvec2 Geomvec2::operator / (const double & d) const
	{ 
	return Geomvec2(x/d,y/d); 
	}


double Geomvec2::operator * (const Geomvec2 & v) const
	{ 
	return x*v.x+y*v.y; 
	}

double Geomvec2::operator ^ (const Geomvec2 & v) const
	{
	return x*v.y-y*v.x;
	}

Geomvec2 & Geomvec2::invert_ew (void)   // element inversion
	{
	x=1./x; y=1./y;
	return *this;
	}


bool Geomvec2::operator == (const Geomvec2 & v) const
	{
	return ( x==v.x && y==v.y );
	}

bool Geomvec2::operator != (const Geomvec2 & v) const
	{
	return ( x!=v.x || y!=v.y );
	}


double Geomvec2::norm(void) const
	{ 
	return sqrt(x*x+y*y);
	};

double Geomvec2::norm2(void) const
	{ 
	return x*x+y*y;
	};

Geomvec2 & Geomvec2::normalize(void)
	{ 
	double n=sqrt(x*x+y*y);
	x/=n; y/=n;
	return *this;
	};

Geomvec2 Geomvec2::unit(void) const
	{ 
	double n=sqrt(x*x+y*y);
	return Geomvec2(x/n,y/n);
	};

Geomvec2 Geomvec2::perp(void) const              // orthogonal vector
	{ return Geomvec2(-y,x); }

double Geomvec2::angle(void) const               // angle with x axis
	{
	return atan2(y,x);
	}


//-----------------------------------------------------------------


Geomvec2 operator - (const Geomvec2 & v)
	{
	return Geomvec2(-v.x,-v.y);
	}

Geomvec2 operator * (const double & f, const Geomvec2 & v)
	{
	return Geomvec2(f*v.x,f*v.y);
	}


Geomvec2 multiply_ew (const Geomvec2 & v, const Geomvec2 & w) // element wise multiplication
	{
	return Geomvec2(v.x*w.x,v.y*w.y);
	}

Geomvec2 divide_ew (const Geomvec2 & v, const Geomvec2 & w)   // element wise division
	{
	return Geomvec2(v.x/w.x,v.y/w.y);
	}


ostream & operator << (ostream& stm, const Geomvec2 & v)		// stm << v
	{ 
	stm << v.x << sep << v.y << sep;
	return stm; 
	};
	
// reading: first character and delimiters have to be exactly one (non number or dot) character.
istream & operator >> (istream & stm, Geomvec2 & v)
	{ 
	char a;
	stm >> a >> v.x >> a >> v.y;
	return stm; 
	}


Geomvec2 unit_vector_from_angle(const double & angle)
	{ 
	double c = cos(angle);
	double s = sin(angle); // replace with sqrt(1-c*c) + sign condition (only work it -pi<angle<pi though)
	return Geomvec2(c,s);
	};



//Geomvec2 random_vec(RNG_taus & rng) 
//	{ 
//	return Geomvec2(rng.get_double(),rng.get_double(),rng.get_double()); 
//	}

//Geomvec2 random_vec(RNG_taus & rng, double amplitude[]) 
//	{
//	Geomvec2 v(amplitude[0],amplitude[1],amplitude[2]);
//	v.x*=rng.get_double();
//	v.y*=rng.get_double();
//	v.z*=rng.get_double();
//	return v;
//	}

//Geomvec2 random_vec(RNG_taus & rng, Geomvec2 amplitude) 
//	{
//	amplitude.x*=rng.get_double();
//	amplitude.y*=rng.get_double();
//	amplitude.z*=rng.get_double();
//	return amplitude;
//	}

//Geomvec2 random_normal_vec(RNG_taus & rng, Geomvec2 amplitude) 
//	{
//	amplitude.x*=random_normal(rng);
//	amplitude.y*=random_normal(rng);
//	amplitude.z*=random_normal(rng);
//	return amplitude;
//	}



