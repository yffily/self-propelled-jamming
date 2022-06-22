#include "Initializer.h"


// base class

void Initializer::InitializeAngle(vector<Particle> & pList) {
  if (par->iniangletype=="Random")
    { InitializeAngle_Random(pList); }
  else if (par->iniangletype=="Zero")
    { InitializeAngle_Zero(pList); }
  else if (par->iniangletype=="Azimuthal")
    { InitializeAngle_Azimuthal(pList); }
  else if (par->iniangletype=="Radial")
    { InitializeAngle_Radial(pList); }
  else
    { cout << "[Error] "+par->iniangletype+" is not a valid type of angle initializer." << endl; exit(100); }
  if (par->verbose) { cout << "Type of angle initialization: "+par->iniangletype << endl; }
  for (vector<Particle>::iterator it=pList.begin(); it<pList.end(); it++)
    { if (it->getGlued()) { it->setAngle(0.); } }
}

void Initializer::InitializeAngle_Random(vector<Particle> & pList) {
  RNG_taus RNG(par->seed_iniAngle);
  vector <Particle>::iterator it;
  for (it=pList.begin();it!=pList.end();it++)
    { it->setAngle(2*pi*(RNG.get_double()-0.5)); }
}

void Initializer::InitializeAngle_Zero(vector<Particle> & pList) {
  vector <Particle>::iterator it;
  for (it=pList.begin();it!=pList.end();it++)
    { it->setAngle(0.); }
}

void Initializer::InitializeAngle_Azimuthal(vector<Particle> & pList) {
  vector <Particle>::iterator it;
  for (it=pList.begin();it!=pList.end();it++)
    { double angle=it->getPos().angle2D()+0.5*pi;
    if (angle>pi) { angle-=dblpi; }
    if (angle<=-pi) { angle+=dblpi; }
    it->setAngle(angle);
    }
}

void Initializer::InitializeAngle_Radial(vector<Particle> & pList) {
  vector <Particle>::iterator it;
  for (it=pList.begin();it!=pList.end();it++)
    { double angle=it->getPos().angle2D();
    if (angle>pi) { angle-=dblpi; }
    if (angle<=-pi) { angle+=dblpi; }
    it->setAngle(angle);
    }
}

void Initializer::rescale(double a, vector<Particle> & pList, Box * & inibox, Box * & box) {
  vector <Particle>::iterator it;
  for (it=pList.begin();it!=pList.end();it++)
    { it->setPos(a*it->getPos()); }
  box->rescale(a); box->printBox();
  if (inibox!=box) { inibox->rescale(a); inibox->printBox(); }
}

void Initializer::rescaleToPhi(double phi, vector<Particle> & pList, Box * & inibox, Box * & box) {
  double area=0.;
  vector <Particle>::iterator it;
  for (it=pList.begin();it!=pList.end();it++)
    { area+=pi*it->getRadius()*it->getRadius(); }
  double a=sqrt(area/(par->phi*inibox->getArea()));
  rescale(a,pList,inibox,box);
}


// File

void Initializer_fromFile::Initialize(vector<Particle> & pList, Box * & inibox, Box * & box) {
  RNG_rad=RNG_taus(par->seed_iniRad);
  pList.clear();
  R.setParameters(par);
  if (par->verbose) { cout << " (loading " << par->inifilename << ")" << endl; }
  if (par->iniformat=="Packing")
    { R.readPacking(par->inifilename); }
  else if (par->iniformat=="Pos")
    { R.readPos(par->inifilename); }
  else if (par->iniformat=="PosVel")
    { R.readPosVel(par->inifilename); }
  else if (par->iniformat=="Posinit")
    { R.readPosinit(par->inifilename); }
  else
    { cout << "[Error] "+par->iniformat+" is not a known file format." << endl; exit(100); }
  for ( vector<Particle>::iterator it=(R.getPList()).begin() ; it<(R.getPList()).end(); it++ )
    { if (inibox->isIn(it->getPos())) { pList.push_back(Particle(*it)); } }
// numbers the particles correctly
  for (int i=0; i<int(pList.size()); i++ ) { pList[i].setID(i); }
  if (int(pList.size())!=par->np && par->verbose)
    { cout << "[Warning] " << pList.size() << " particles read from file, np changed accordingly." << endl; }
  par->np=pList.size();
  if (par->iniangletype=="File")
    {
    if (!R.getHasAngles())
      { cout << "[Warning] File doesn't contains angles. Using random angles instead." << endl;
      par->iniangletype=="Random"; }
    }
  else { InitializeAngle(pList); }
  if (par->iniradtype=="File" and !R.getHasRadii())
    { cout << "[Warning] File doesn't contains radii. Using random radii instead." << endl;
    par->iniradtype=="Random"; }
  if (par->iniradtype=="Random")
    {
    vector <Particle>::iterator it;
    for (it=pList.begin();it!=pList.end();it++)
      { it->setRadius(par->radius+2.0*par->polydis*(RNG_rad.get_double()-0.5)); }
    }
  else
    { cout << "[Error] "+par->iniradtype+" is not a valid type of radii initializer." << endl; exit(100); }
  if (par->verbose) { cout << "Type of radius initialization: "+par->iniradtype << endl; }

  
// !! no !! the box has to be adjusted to the file read !
  if (par->initype=="File_rescaled") { rescaleToPhi(par->phi,pList,inibox,box); }
}



// Random

void Initializer_Random::Initialize(vector<Particle> & pList, Box * & inibox, Box * & box) {
  if (par->verbose) { cout << endl; }
  RNG_pos=RNG_taus(par->seed_iniPos);
  RNG_rad=RNG_taus(par->seed_iniRad);
  bool validMode=false;
// phi~np/L^2 where L=box size. 2 of these are fixed, the 3rd (=fillmode) is a function of them.
// In 'N' mode, the phi is not exactly the one required because you can't add a fraction of a particle !
  if ((par->fillmode)=="N")
    { validMode=true; par->np=round(par->phi*inibox->getArea()/(pi*par->radius*par->radius));
    if (par->verbose) { cout << "[N mode] Number of particles changed to " << par->np; } }
// To get the exact desired phi in 'L' mode, the radii need to be determined now.
// It has to be done once np is known for good, so after the 'N' mode condition.
  double area=0., radius[par->np];
  for (int i=0;i<par->np;i++)
    { validMode=true;
    radius[i]=par->radius+2.0*par->polydis*(RNG_rad.get_double()-0.5);
    area+=pi*radius[i]*radius[i]; }
// Now we can adapt th box size accurately in 'phi' mode.
  if ((par->fillmode)=="phi") { validMode=true; }
  if ((par->fillmode)=="L")
    { validMode=true;
    double a=sqrt(area/(par->phi*inibox->getArea()));
// here I rescale the simulation box when rescaling the initialization box
// it could work the other way as well (could be made an option)
    box->rescale(a); box->printBox();
    if (inibox!=box) { inibox->rescale(a); inibox->printBox(); }
    if (par->verbose) { cout << "[L mode] Box dimensions multiplied by " << a; } }
  if (!validMode)
    { cout << "[Error] "+par->fillmode+" is not a known fillmode." << endl; exit(100); }
  int i=0;
  double x,y;
  while (i<par->np)
    {
    x=inibox->getBoundingSizeX()*(RNG_pos.get_double()-0.5);
    y=inibox->getBoundingSizeY()*(RNG_pos.get_double()-0.5);
    Geomvec pos(x,y);
    if (inibox->isIn(pos))
      {
      pos+=par->offsetX*Geomvec(1.,0.)+par->offsetY*Geomvec(0.,1.);
      pList.push_back(Particle(i,pos,par));
      pList[i].setRadius(radius[i]);
      i++;
      }
    }
  par->phi = area/inibox->getArea();		// (re)calculate the packing fraction
  if (par->verbose)
    { cout << ", Final packing fraction " << par->phi << endl; }
  InitializeAngle(pList);
}



// Hexagonal lattice

void Initializer_Hex::Initialize(vector<Particle> & pList, Box * & inibox, Box * & box) {
  if (par->verbose) { cout << endl; }
  par->BCtype="Free";
  RNG_rad=RNG_taus(par->seed_iniRad);
// phi~np/L^2 where L=box size. 2 of these are fixed, the 3rd (=fillmode) is a function of them.
  if ((par->fillmode)=="N")
    { par->np=round(par->phi*inibox->getArea()/(pi*par->radius*par->radius));
    if (par->verbose) { cout << "[N mode] Number of particles changed to " << par->np << endl; } }
  else if ((par->fillmode)=="L")
    { double a=par->radius*sqrt(par->np*pi/(par->phi*inibox->getArea()));
// here I rescale the simulation box when rescaling the initialization box
// it could work the other way as well (could be made an option)
    box->rescale(a); box->printBox();
    inibox->rescale(a); inibox->printBox();
    if (par->verbose) { cout << "[L mode] Box dimensions multiplied by " << a << endl; } }
  else if ((par->fillmode)!="phi")
    { cout << "[Error] "+par->fillmode+" is not a known fillmode." << endl; exit(100); }
  cout << "[Warning] 'Hex' initialization: N or phi is only matched approximately" << endl;
  
  
  double spacing=sqrt(2*inibox->getArea()/(sqrt(3.)*par->np));
  int nx=int(inibox->getBoundingSizeX()/(2*spacing))+1;
  int ny=int(inibox->getBoundingSizeY()/(spacing*sqrt(3.)))+1;
  double x,y;
  cout << "[Warning] 'Hex' taking over boundary type (switched to 'Glued')" << endl;
  int k=0;
  int kEdge[2*nx+3][2*ny+3];		// index of the particle on the (i,j) site (=-1 if no particle)
  for (int i=-nx-1;i<=nx+1;i++)
    {
    for (int j=-ny-1;j<=ny+1;j++)
      {
      kEdge[i+nx+1][j+ny+1]=-1;
      x=spacing*(i+0.5*(j%2));
      y=spacing*j*sqrt(3.)/2.;
      Geomvec pos(x,y);
      if (inibox->isIn(pos))
        {
        pList.push_back(Particle(k,pos,par));
        pList.back().setRadius(par->radius+2.0*par->polydis*(RNG_rad.get_double()-0.5));
        kEdge[i+nx+1][j+ny+1]=k;
        k++;
        }
      }
    }
  if (par->BCtype=="glued")
    {
// sweep through particles vertically then horizontally to find boundaries
		for (int i=0;i<2*nx+3;i++)
		  { int j=0;
		  while(j<2*ny+3 && kEdge[i][j]==-1) { j++; }
		  if (j<2*ny+2) { pList[kEdge[i][j]].setGlued(true); }
		  j=2*ny+2;
		  while(j>0 && kEdge[i][j]==-1) { j--; }
		  if (j>0) { pList[kEdge[i][j]].setGlued(true); }
		  }
		for (int j=0;j<2*ny+3;j++)
		  { int i=0;
		  while(i<2*nx+3 && kEdge[i][j]==-1) { i++; }
		  if (i<2*nx+2) { pList[kEdge[i][j]].setGlued(true); }
		  i=2*nx+2;
		  while(i>0 && kEdge[i][j]==-1) { i--; }
		  if (i>0) { pList[kEdge[i][j]].setGlued(true); }
		  }
		}
// (re)calculate the packing fraction
  double area=0.;
  for (vector<Particle>::iterator it=pList.begin(); it<pList.end(); it++)
    { area+=pi*it->getRadius()*it->getRadius(); }
  par->phi = area/inibox->getArea();
  par->np=pList.size();
  if (par->verbose)
    { cout << "Final packing fraction = " << par->phi << ", Final number of particles = " << par->np << endl; }
// initialize angles
  InitializeAngle(pList);
}




