#ifndef util_h
#define util_h
#include <cmath>
#include <string>		// definit la classe de variable "string"
#include <sstream>
using namespace std;


const double pi(4.0*atan2(1.0, 1.0));
const double dblpi(8.0*atan2(1.0, 1.0));
const string sep="    ";


template < class T > inline std::string to_string(const T & t) {
	stringstream ss;
	ss << t;
	return ss.str();
};


//string TimeToDate(double s) {
//	int m=int(s/60); s-=60*m;
//	int h=int(m/60); m-=60*h;
//	int d=int(h/24); h-=24*d;
//	stringstream ss;
//	if (d>0) { ss << d << "days "; }
//	if (h>0) { ss << h << "h "; }
//	if (m>0) { ss << m << "min "; }
//	ss << s << "s";
//	return ss.str();
//}

#endif
