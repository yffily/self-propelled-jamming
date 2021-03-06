#ifndef Box_H
#define Box_H
//#include <stdlib.h>
#include <vector>
#include <string>
#include <iostream>
#include "util.h"
#include "Particle.h"
#include "Geomvec2.h"
#include "Interaction.h"
#include "Parameters.h"
using namespace std;

class Interaction;
class Parameters;

class Box {
	
	protected:
		Parameters * par;
		vector<Geomvec2> boundary;
		double area, perimeter;
		double boundingSizeX, boundingSizeY;
		Geomvec2 ux, uy;				// unit vectors along x and y
		ofstream os;
		string boxFilename, boundingBoxFilename;

	public:
		Box() {};
		Box(Parameters * & par);
		virtual ~Box() {};

		void setParameters(Parameters * & par) { this->par=par; };
		void setBoxFilename(string s) { boxFilename=s; };
		void setBoundingBoxFilename(string s) { boundingBoxFilename=s; };

		Parameters * getParameters() const { return par; };
		vector<Geomvec2> getBoundary() const { return boundary; };
		double getArea() const { return area; };
		double getPerimeter() const { return perimeter; };
		double getBoundingSizeX() const { return boundingSizeX; };
		double getBoundingSizeY() const { return boundingSizeY; };
		virtual void updateMembers() {};
//    virtual Box & operator=(const Box & b);

		void makeBoundary(const double & dx);
		void addToBoundary(Geomvec2 v);
		virtual Geomvec2 makeBoundaryPoint(const double & l);
		virtual bool isIn(const Geomvec2 & pos) =0;
		virtual Geomvec2 getWallForce(Interaction * inter, Particle * p) =0;
		virtual Geomvec2 bringBack(Geomvec2 & v);
		virtual void rescale(double a) =0;
		virtual void rescale(double ax, double ay);
		void printBox() { printBox(par->outdir+boxFilename); };
		virtual void printBox(const string & filename) =0;
		void printBoundingBox() { printBoundingBox(par->outdir+boundingBoxFilename); };
		virtual void printBoundingBox(const string & filename);
		void printBoundary(const string & filename);
		virtual void updatePar(Parameters * & par) {};
		virtual void updateIniPar(Parameters * & par) {};
};


class Box_None: public Box {

	public:
		Box_None(Parameters * par): Box(par) {};
		void rescale(double a) {};
		void rescale(double ax, double ay) {};
		void makeBoundary(const double & dx) {};
		bool isIn(const Geomvec2 & pos) { return true; };
		Geomvec2 getWallForce(Interaction * inter, Particle * p) { return Geomvec2(0.,0.); };
		Geomvec2 bringBack(Geomvec2 & v) { return v; };
		void printBox(const string & filename) {};
		void printBoundingBox(const string & filename) {};
};
 

class Box_Rectangle: public Box {

	double boxSizeX, boxSizeY;
	double halfBoxSizeX, halfBoxSizeY;
	Geomvec2 vx, vy;

	public:
		Box_Rectangle() {};
		Box_Rectangle(Parameters * par);

		double getBoxSizeX() { return boxSizeX; };
		double getBoxSizeY() { return boxSizeX; };

		void updateMembers();
		void rescale(double a);
		void rescale(double ax, double ay);
		Geomvec2 makeBoundaryPoint(const double & l);
		bool isIn(const Geomvec2 & pos);
		Geomvec2 getWallForce(Interaction * inter, Particle * p);
		Geomvec2 bringBack(Geomvec2 & v);
		void printBox(const string & filename);
		void updatePar(Parameters * & par);
		void updateIniPar(Parameters * & par);
};
 

class Box_Skew: public Box {

	double boxSide1, boxSide2;
// v1 and v2 are the two vectors generating the box
// e1 and e2 are such that v=(v.e1)*v1+(v.e2)*v2 where . is the usual dot product
// u1 and u2 are unit vectors along v1 and v2
	Geomvec2 e1, e2, u1, u2, v1, v2;

	public:
		Box_Skew(Parameters * par);
		void updateMembers();
		void rescale(double a);
		void rescale(double ax, double ay);
		Geomvec2 makeBoundaryPoint(const double & l);
		bool isIn(const Geomvec2 & pos);
		Geomvec2 getWallForce(Interaction * inter, Particle * p);
		Geomvec2 bringBack(Geomvec2 & v);
		void printBox(const string & filename);
		void updatePar(Parameters * & par);
		void updateIniPar(Parameters * & par);
};


class Box_Circle: public Box {

	double radius, radius2;

	public:
		Box_Circle(Parameters * par);

		void updateMembers();
		void rescale(double a);
		Geomvec2 makeBoundaryPoint(const double & l);
		bool isIn(const Geomvec2 & pos);
		Geomvec2 getWallForce(Interaction * inter, Particle * p);
		Geomvec2 bringBack(Geomvec2 & v);
		void printBox(const string & filename);
		void updatePar(Parameters * & par);
		void updateIniPar(Parameters * & par);
};


#endif
