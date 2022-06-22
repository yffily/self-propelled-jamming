#include "Box.h"


Box::Box(Parameters * & par) {
  this->par=par;
  boxFilename="box.dat";
  boundingBoxFilename="boundingBox.dat";
  ux=Geomvec(1.,0.); uy=Geomvec(0.,1.);
}

//Box & Box::operator=(const Box & b) { 
//  this->par=b.par;
//  this->boundary=b.boundary;
//  this->area=b.area;
//  this->perimeter=b.perimeter;
//  this->boundingSizeX=b.boundingSizeX;
//  this->boundingSizeY=b.boundingSizeY;
//  this->ux=b.ux;
//  this->uy=b.uy;
//  this->boxFilename=b.boxFilename;
//  this->boundingBoxFilename=b.boundingBoxFilename;
////cin.get();
//  return *this;
//}

void Box::makeBoundary(const double & dx) {
  boundary.clear();
  for (double x=0; x<=perimeter; x+=dx) { boundary.push_back(makeBoundaryPoint(x)); }
// makeBoundaryCorner ?? would be nice but then put them at the right place in vector boundary...
  printBoundary("boundary.dat");
}

void Box::addToBoundary(Geomvec v) {
  boundary.push_back(v);
}

Geomvec Box::makeBoundaryPoint(const double & l) {
  cout << "Sorry but this box shape doesn't handle glued boundaries." << endl;
  cin.get();
  return Geomvec();
}

Geomvec Box::bringBack(Geomvec & v) {
  cout << "Sorry but this box shape doesn't handle periodic boundaries." << endl;
  cin.get();
  return v;
}

void Box::rescale(double ax, double ay) {
  cout << "Sorry but this box shape doesn't handle anisotropic rescaling." << endl;
  cin.get();
}


//par->outdir+



void Box::printBoundingBox(const string & filename) {
  os.open(filename.c_str());
  Geomvec vx=boundingSizeX*ux;
  Geomvec vy=boundingSizeY*uy;
  os << (-vx-vy)/2 << endl << (vx-vy)/2 << endl << (vx+vy)/2 << endl << (-vx+vy)/2 << endl << (-vx-vy)/2 << endl;
  os.close();
}

void Box::printBoundary(const string & filename) {
  os.open(filename.c_str());
  vector<Geomvec>::iterator it;
  for (it=boundary.begin();it!=boundary.end();it++) { os << (*it) << endl; }
}



// Rectangle //

Box_Rectangle::Box_Rectangle(Parameters * par): Box(par) {
  boxSizeX=par->boxVec1.getX(0);
  boxSizeY=par->boxVec2.getX(1);
  updateMembers();
}

void Box_Rectangle::updateMembers() {
  vx=boxSizeX*ux;
  vy=boxSizeY*uy;
  halfBoxSizeX=0.5*boxSizeX;
  halfBoxSizeY=0.5*boxSizeY;
  area=boxSizeX*boxSizeY;
  perimeter=2*(boxSizeX+boxSizeY);
  boundingSizeX=boxSizeX;
  boundingSizeY=boxSizeY;
}

void Box_Rectangle::rescale(double a) { rescale(a,a); }

void Box_Rectangle::rescale(double ax, double ay) {
  boxSizeX*=ax; boxSizeY*=ay;
  updateMembers();
}

void Box_Rectangle::printBox(const string & filename) {
  os.open(filename.c_str());
  os << (-vx-vy)/2 << endl << (vx-vy)/2 << endl << (vx+vy)/2 << endl << (-vx+vy)/2 << endl << (-vx-vy)/2 << endl;
  os.close();
}

Geomvec Box_Rectangle::makeBoundaryPoint(const double & l) {
// l=length counted along the boundary from bottom right corner
  if (l<boxSizeX) { return (l-halfBoxSizeX)*ux-halfBoxSizeY*uy; }
  else if (l<boxSizeX+boxSizeY) { return halfBoxSizeX*ux+(l-boxSizeX-halfBoxSizeY)*uy; }
  else if (l<2*boxSizeX+boxSizeY) { return (1.5*boxSizeX+boxSizeY-l)*ux+halfBoxSizeY*uy; }
  else { return -halfBoxSizeX*ux+(2*boxSizeX+1.5*boxSizeY-l)*uy; }
}

bool Box_Rectangle::isIn(const Geomvec & v) {
  return ((v.getX(0)>-halfBoxSizeX)&&(v.getX(0)<=halfBoxSizeX)&&(v.getX(1)>-halfBoxSizeY)&&(v.getX(1)<=halfBoxSizeY));
}

Geomvec Box_Rectangle::getWallForce(Interaction * inter, Particle * p) {
  double dx1=p->getPos().getX(0)-halfBoxSizeX, dx2=dx1+boxSizeX;
  double dy1=p->getPos().getX(1)-halfBoxSizeY, dy2=dy1+boxSizeY;
  return inter->getForce(Geomvec(dx1,0.),-dx1,p->getRadius())+inter->getForce(Geomvec(0.,dy1),-dy1,p->getRadius())+inter->getForce(Geomvec(dx2,0.),dx2,p->getRadius())+inter->getForce(Geomvec(0.,dy2),dy2,p->getRadius());
}

Geomvec Box_Rectangle::bringBack(Geomvec & v) {
  while (v.getX(0)>halfBoxSizeX) { v-=vx; }
  while (v.getX(0)<=-halfBoxSizeX) { v+=vx; }
  while (v.getX(1)>halfBoxSizeY) { v-=vy; }
  while (v.getX(1)<=-halfBoxSizeY) { v+=vy; }
  return v;
}

void Box_Rectangle::updatePar(Parameters * & par) {
  par->boxtype="Rectangle";
  par->boxVec1=vx;
  par->boxVec2=vy;
}

void Box_Rectangle::updateIniPar(Parameters * & par) {
// if iniboxtype was "Full" that information should be kept!
  if (par->iniboxtype!="Full") { par->iniboxtype="Rectangle"; }
  par->iniboxVec1=vx;
  par->iniboxVec2=vy;
}


// Skew rectangle //

Box_Skew::Box_Skew(Parameters * par): Box(par) {
  v1=par->boxVec1;
  v2=par->boxVec2;
  updateMembers();
}

void Box_Skew::updateMembers() {
  area=crossprod2D(v1,v2);
  boxSide1=v1.norm(); boxSide2=v2.norm();
  perimeter=2*(boxSide1+boxSide2);
  u1=v1.unit(), u2=v2.unit();
  e1=-v2.perp2D()/area;
  e2=v1.perp2D()/area;
  boundingSizeX=abs((v1+v2).getX(0));
  boundingSizeY=abs((v1+v2).getX(1));
}

void Box_Skew::rescale(double a) { rescale(a,a); }

void Box_Skew::rescale(double ax, double ay) {
  v1=v1.getX(0)*Geomvec(ax,0.)+v1.getX(1)*Geomvec(0.,ay);
  v2=v2.getX(0)*Geomvec(ax,0.)+v2.getX(1)*Geomvec(0.,ay);
  updateMembers();
}

Geomvec Box_Skew::makeBoundaryPoint(const double & l) {
// l=length counted along the boundary from bottom right corner
  if (l<boxSide1) { return (l-0.5*boxSide1)*u1-0.5*boxSide2*u2; }
  else if (l<boxSide1+boxSide2) { return 0.5*boxSide1*u1+(l-boxSide1-0.5*boxSide2)*u2; }
  else if (l<2*boxSide1+boxSide2) { return (1.5*boxSide1+boxSide2-l)*u1+0.5*boxSide2*u2; }
  else { return -0.5*boxSide1*u1+(2*boxSide1+1.5*boxSide2-l)*u2; }
}

bool Box_Skew::isIn(const Geomvec & v) {
  return ((v*e1>-0.5)&&(v*e1<=0.5)&&(v*e2>-0.5)&&(v*e2<=0.5));
}

Geomvec Box_Skew::getWallForce(Interaction * inter, Particle * p) {
  return Geomvec(0.,0.);
}

Geomvec Box_Skew::bringBack(Geomvec & v) {
  if (v*e1>0.5) { v-=v1; }
  if (v*e1<=-0.5) { v+=v1; }
  if (v*e2>0.5) { v-=v2; }
  if (v*e2<=-0.5) { v+=v2; }
  return v;
}

void Box_Skew::printBox(const string & filename) {
  os.open(filename.c_str());
  os << (-v1-v2)/2 << endl << (v1-v2)/2 << endl << (v1+v2)/2 << endl << (-v1+v2)/2 << endl << (-v1-v2)/2 << endl;
  os.close();
}

void Box_Skew::updatePar(Parameters * & par) {
  par->boxtype="Skew";
  par->boxVec1=v1;
  par->boxVec2=v2;
}

void Box_Skew::updateIniPar(Parameters * & par) {
  par->iniboxtype="Skew";
  par->iniboxVec1=v1;
  par->iniboxVec2=v2;
}


// Circle box //
Box_Circle::Box_Circle(Parameters * par): Box(par) {
  radius=par->boxRadius;
  updateMembers();
}

void Box_Circle::updateMembers() {
  radius2=radius*radius;
  area=pi*radius2;
  perimeter=dblpi*radius;
  boundingSizeX=2*radius;
  boundingSizeY=2*radius;
  printBoundingBox();
}

void Box_Circle::rescale(double a) {
  radius*=a;
  updateMembers();
}

Geomvec Box_Circle::makeBoundaryPoint(const double & l) {
// l=length counted along the boundary from "trigonometric origin"
  return radius*Geomvec(l/radius);
}

bool Box_Circle::isIn(const Geomvec & v) {
  return (v.norm2()<=radius2);
}

Geomvec Box_Circle::getWallForce(Interaction * inter, Particle * p) {
  return Geomvec(0.,0.);
}

Geomvec Box_Circle::bringBack(Geomvec & v) {
  if (!isIn(v)) { v-=2*radius*v.unit(); }
  return v;
}

void Box_Circle::printBox(const string & filename) {
  string s=filename;
  os.open(s.c_str());
  double dangle=dblpi/100;
  for (double angle=0; angle<=dblpi; angle+=dangle)
  os << radius*Geomvec(angle) << endl;
  os.close();
}

void Box_Circle::updatePar(Parameters * & par) {
  par->boxtype="Circle";
  par->boxRadius=radius;
}

void Box_Circle::updateIniPar(Parameters * & par) {
  par->iniboxtype="Circle";
  par->iniboxRadius=radius;
}


