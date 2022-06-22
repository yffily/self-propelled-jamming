#include "Reader.h"



void Reader::open(const string & filename) {
  is.open(filename.c_str());
  if (!is.is_open())
    { cout << "Unable to open '"+filename+"'" << endl; exit(100); }
}


void Reader::readPacking(const string & filename) {
  open(filename);
  double x,y,r;
  string line;
  getline(is,line);
  istringstream iss(line);
// first line is 0.5*Lx, 0.5*Ly and potential
  iss >> Lx >> Ly >> r;
  Lx*=2; Ly*=2;
  pList.clear();
  int i=0;
  while (getline(is,line))
    {
    istringstream iss(line);
// lines are x/Lx, y/Ly and 0.5*radius
    iss >> x >> y >> r;
    pList.push_back(Particle(i,Geomvec(x*Lx,y*Ly),par));
    pList.back().setRadius(2*r);
    i++;
    }
  is.close();
  hasRadii=true; hasAngles=false;
}


void Reader::readPos(const string & filename) {
  open(filename);
  string line;
  pList.clear();
  int i; double x,y,angle;
  while (getline(is,line))
    {
    istringstream iss(line);
    iss >> i >> x >> y >> angle;
    pList.push_back(Particle(i,Geomvec(x,y),angle*pi/180.,par));
    }
  is.close();
  hasRadii=false; hasAngles=true;
}


void Reader::readPosVel(const string & filename) {
  open(filename);
  string line;
  pList.clear();
  int i=0; double x,y,angle;
  while (getline(is,line))
    {
    istringstream iss(line);
    iss >> x >> y >> angle;
    pList.push_back(Particle(i,Geomvec(x,y),angle,par));
    i++;
    }
  is.close();
  hasRadii=false; hasAngles=true;
}


void Reader::readPosinit(const string & filename) {
  open(filename);
  string line;
  pList.clear();
  int i=0; double x,y,angle,r;
  while (getline(is,line))
    {
    istringstream iss(line);
    iss >> x >> y >> angle >> r;
    pList.push_back(Particle(i,Geomvec(x,y),angle,par));
    pList.back().setRadius(r);
    i++;
    }
  is.close();
  hasRadii=true; hasAngles=true;
}



void Reader::glueBoundary() {
  if (hasAngles && pList.front().getAngle()!=0) { glueBoundaryByAngle(); }
  else 
    { cout << "[Warning] Reader unable to recognize boundary." << endl; }
}


void Reader::glueBoundaryByAngle() {
  vector<Particle>::iterator it=pList.end();
  while (it->getAngle()==0) { it->setGlued(true); it--; }
}


// identify the type of box from 'box.dat'
// not tested!!
void Reader::readBox(const string & filename) {
  open(filename);
  string line;
  vector<Geomvec> boundary;
  double x,y;
  while (getline(is,line))
    {
    istringstream iss(line);
    iss >> x >> y;
    boundary.push_back(Geomvec(x,y));
    }
  is.close();
// if all particles are equidistant from the origin it's a circle
  bool circle=true; double r2=boundary.begin()->norm2();
//  cout << r2 << endl;
  vector<Geomvec>::iterator it=boundary.begin()+1;
  while(circle && it<boundary.end())
    { if (it->norm2()/r2-1>1e-3) { circle=false; }
    it++; }
  if (boundary.size()>5 && circle)
    { double radius=sqrt(r2);
    cout << "Box is a circle of radius " << radius << endl; }
  else if (boundary.size()==5 && boundary[0]==boundary[4])
    { Geomvec u1=boundary[1]+boundary[2];
    Geomvec u2=boundary[2]+boundary[3];
    par->boxtype="Skew";
    par->boxVec1=u1;
    par->boxVec2=u2;
    cout << "Box is skew with vectors u1=" << u1 << " and u2=" << u2 << endl; }
  else 
    { cout << "[Warning] readBox unable to recognize box." << endl; exit(100); }
}



