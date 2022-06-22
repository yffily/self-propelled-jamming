#include "Box.h"


Box::Box(Parameters * & par) {
	this->par=par;
	boxFilename="box.dat";
	boundingBoxFilename="boundingBox.dat";
	ux=Geomvec2(1.,0.); uy=Geomvec2(0.,1.);
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

void Box::addToBoundary(Geomvec2 v) {
	boundary.push_back(v);
}

Geomvec2 Box::makeBoundaryPoint(const double & l) {
	cout << "Sorry but this box shape doesn't handle glued boundaries." << endl;
	cin.get();
	return Geomvec2();
}

Geomvec2 Box::bringBack(Geomvec2 & v) {
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
	Geomvec2 vx=boundingSizeX*ux;
	Geomvec2 vy=boundingSizeY*uy;
	os << (-vx-vy)/2 << endl << (vx-vy)/2 << endl << (vx+vy)/2 << endl << (-vx+vy)/2 << endl << (-vx-vy)/2 << endl;
	os.close();
}

void Box::printBoundary(const string & filename) {
	os.open(filename.c_str());
	vector<Geomvec2>::iterator it;
	for (it=boundary.begin();it!=boundary.end();it++) { os << (*it) << endl; }
}



// Rectangle //

Box_Rectangle::Box_Rectangle(Parameters * par): Box(par) {
	boxSizeX=par->boxVec1.x;
	boxSizeY=par->boxVec2.y;
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

Geomvec2 Box_Rectangle::makeBoundaryPoint(const double & l) {
// l=length counted along the boundary from bottom right corner
	if (l<boxSizeX) { return (l-halfBoxSizeX)*ux-halfBoxSizeY*uy; }
	else if (l<boxSizeX+boxSizeY) { return halfBoxSizeX*ux+(l-boxSizeX-halfBoxSizeY)*uy; }
	else if (l<2*boxSizeX+boxSizeY) { return (1.5*boxSizeX+boxSizeY-l)*ux+halfBoxSizeY*uy; }
	else { return -halfBoxSizeX*ux+(2*boxSizeX+1.5*boxSizeY-l)*uy; }
}

bool Box_Rectangle::isIn(const Geomvec2 & v) {
	return ((v.x>-halfBoxSizeX)&&(v.x<=halfBoxSizeX)&&(v.y>-halfBoxSizeY)&&(v.y<=halfBoxSizeY));
}

Geomvec2 Box_Rectangle::getWallForce(Interaction * inter, Particle * p) {
	double dx1=p->getPos().x-halfBoxSizeX, dx2=dx1+boxSizeX;
	double dy1=p->getPos().y-halfBoxSizeY, dy2=dy1+boxSizeY;
	return inter->getForce(Geomvec2(dx1,0.),-dx1,p->getRadius())+inter->getForce(Geomvec2(0.,dy1),-dy1,p->getRadius())+inter->getForce(Geomvec2(dx2,0.),dx2,p->getRadius())+inter->getForce(Geomvec2(0.,dy2),dy2,p->getRadius());
}

Geomvec2 Box_Rectangle::bringBack(Geomvec2 & v) {
	while (v.x>halfBoxSizeX) { v-=vx; }
	while (v.x<=-halfBoxSizeX) { v+=vx; }
	while (v.y>halfBoxSizeY) { v-=vy; }
	while (v.y<=-halfBoxSizeY) { v+=vy; }
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
	area=v1^v2;
	boxSide1=v1.norm(); boxSide2=v2.norm();
	perimeter=2*(boxSide1+boxSide2);
	u1=v1.unit(), u2=v2.unit();
	e1=-v2.perp()/area;
	e2=v1.perp()/area;
	boundingSizeX=abs((v1+v2).x);
	boundingSizeY=abs((v1+v2).y);
}

void Box_Skew::rescale(double a) { rescale(a,a); }

void Box_Skew::rescale(double ax, double ay) {
	v1=v1.x*Geomvec2(ax,0.)+v1.y*Geomvec2(0.,ay);
	v2=v2.x*Geomvec2(ax,0.)+v2.y*Geomvec2(0.,ay);
	updateMembers();
}

Geomvec2 Box_Skew::makeBoundaryPoint(const double & l) {
// l=length counted along the boundary from bottom right corner
	if (l<boxSide1) { return (l-0.5*boxSide1)*u1-0.5*boxSide2*u2; }
	else if (l<boxSide1+boxSide2) { return 0.5*boxSide1*u1+(l-boxSide1-0.5*boxSide2)*u2; }
	else if (l<2*boxSide1+boxSide2) { return (1.5*boxSide1+boxSide2-l)*u1+0.5*boxSide2*u2; }
	else { return -0.5*boxSide1*u1+(2*boxSide1+1.5*boxSide2-l)*u2; }
}

bool Box_Skew::isIn(const Geomvec2 & v) {
	return ((v*e1>-0.5)&&(v*e1<=0.5)&&(v*e2>-0.5)&&(v*e2<=0.5));
}

Geomvec2 Box_Skew::getWallForce(Interaction * inter, Particle * p) {
	return Geomvec2(0.,0.);
}

Geomvec2 Box_Skew::bringBack(Geomvec2 & v) {
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

Geomvec2 Box_Circle::makeBoundaryPoint(const double & l) {
// l=length counted along the boundary from "trigonometric origin"
	return radius*unit_vector_from_angle(l/radius);
}

bool Box_Circle::isIn(const Geomvec2 & v) {
	return (v.norm2()<=radius2);
}

Geomvec2 Box_Circle::getWallForce(Interaction * inter, Particle * p) {
	return Geomvec2(0.,0.);
}

Geomvec2 Box_Circle::bringBack(Geomvec2 & v) {
	if (!isIn(v)) { v-=2*radius*v.unit(); }
	return v;
}

void Box_Circle::printBox(const string & filename) {
	string s=filename;
	os.open(s.c_str());
	double dangle=dblpi/100;
	for (double angle=0; angle<=dblpi; angle+=dangle)
	os << radius*unit_vector_from_angle(angle) << endl;
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


