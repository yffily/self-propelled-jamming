#include "Container.h"


Container::Container(Parameters * par) {
  cout << fixed << setprecision(2);
  this->par=par;					// load the parameter file
  chooseBox(box,par);					// choose a type of box
  box->printBox();
  chooseIniBox(inibox,box,par);				// choose a type of box for initialization
  chooseInitializer(ini,par);				// choose a type of initializer
  ini->Initialize(pList,inibox,box);			// create the list of particles
  chooseBoundary(BC,box,par);				// choose a type of boundary conditions
  BC->makeBoundary(pList);
  chooseInteraction(inter,par);				// choose a type of interaction
  chooseAngularInteraction(anginter,BC,par);		// choose a type of angular interaction
  for (int i=0;i<par->np;i++) { pList[i].setSelfAlign(par->selfAlign); }	// sets self alignment
  chooseDynamics(dyn,BC,inter,anginter,par);			// choose a type of integrator
  cout << resetiosflags(ios::floatfield) << setprecision(3);
  chooseNeighborList(NL,BC,par);			// choose a type of neighbor list
  NL->makeRInt(pList);
  NL->makeList(pList);
  cout << resetiosflags(ios::floatfield) << setprecision(10);
  par->print((par->outdir+"Parameters_final.dat"));
}


void Container::update() {
  dyn->move(pList,NL);
}


void chooseBox(Box * & box, Parameters * par) {
  if ((par->boxtype)=="None") { box=new Box_None(par); }
  else if ((par->boxtype)=="Rectangle")
    { box=new Box_Rectangle(par);
// only the x component of boxVec1 and the y component of boxVec2 are read
    cout << "[Warning] Rectangle: Geomvec boxVec deprecated to double boxSize" << endl; }
  else if ((par->boxtype)=="Skew")
    {
    if (par->boxVec1.getX(1)==0 && par->boxVec2.getX(0)==0)
      { box=new Box_Rectangle(par); par->boxtype="Rectangle"; }
    else { box=new Box_Skew(par); }
    }
  else if ((par->boxtype)=="Circle") { box=new Box_Circle(par); }
  else if ((par->boxtype)=="File")
    {
    Reader R; R.readBox(par->boxfile);
//    box=new Box_Circle(par); 
    }
  else
    { cout << "[Error] "+par->boxtype+" is not a known type of box." << endl; exit(100); }
  if (par->verbose) { cout << "Type of box: " << par->boxtype << endl; }
}


void chooseIniBox(Box * & inibox, Box * & box, Parameters * par) {
  Parameters inipar=*par;
  if (inipar.iniboxtype=="Full") { inibox=box; }
  else
    {
    if (inipar.iniboxtype=="Strip")
      { if (par->boxtype!="Rectangle")
        { cout << "Sorry but 'Strip' mode only works with 'Rectangle' box." << endl; exit(100); }
      if (par->fillmode=="L")
        { cout << "Sorry but 'Strip' mode only doesn't work with 'L' fillmode." << endl; exit(100); }
      inipar.boxVec1*=par->stripfrac; }
    else
      { inipar.boxtype=par->iniboxtype;
      inipar.boxVec1=par->iniboxVec1;
      inipar.boxVec2=par->iniboxVec2;
      inipar.boxRadius=par->iniboxRadius; }
    chooseBox(inibox,&inipar);			// use inipar to build inibox
    inibox->setBoxFilename("inibox.dat");
    inibox->printBox();
    inibox->setParameters(par);			// give the original par to inibox
    }
}


void chooseInitializer(Initializer * & ini, Parameters * par) {
  if (par->initype=="File"||par->initype=="File_rescaled")
    { ini=new Initializer_fromFile(par); }
  else if (par->initype=="Random") { ini=new Initializer_Random(par); }
  else if (par->initype=="Hex") { ini=new Initializer_Hex(par); }
  else
    { cout << "[Error] "+par->initype+" is not a known type of initializer." << endl; exit(100); }
  if (par->verbose) { cout << "Type of initialization: "+par->initype; }
}


void chooseBoundary(Boundary * & BC, Box * box, Parameters * par) {
  if ((par->BCtype)=="Free") { BC=new Boundary_Free(); }
  else if ((par->BCtype)=="Periodic") { BC=new Boundary_Periodic(par,box); }
  else if ((par->BCtype)=="Glued") { BC=new Boundary_Glued(par,box); }
  else
    { cout << "[Error] "+par->BCtype+" is not a known type of boundary." << endl; exit(100); }
  if (par->verbose) { cout << "Type of boundary: "+par->BCtype << endl; }
}


void chooseInteraction(Interaction * & inter, Parameters * par) {
  if (par->inttype=="Elastic") { inter=new Interaction_Elastic(par->k_rep); }
  else if (par->inttype=="Adhesion")
    { inter=new Interaction_ElasticWithAdhesion(par->k_rep,par->F_ad,par->R_ad); }
  else if (par->inttype=="None") { inter=new Interaction(); }
  else
    { cout << "[Error] " << par->inttype << " is not a known type of interaction." << endl; exit(100); }
  if (par->verbose) { cout << "Type of interaction: "+par->inttype << endl; }
}


void chooseAngularInteraction(Interaction * & inter, Boundary * & BC, Parameters * par) {
  if (par->anginttype=="Vicsek") { inter=new Interaction_Vicsek(BC,par->tau_V); }
  else if (par->anginttype=="None") { inter=new Interaction(); }
  else
    { cout << "[Error] " << par->anginttype << " is not a known type of angular interaction." << endl; exit(100); }
  if (par->verbose) { cout << "Type of interaction: "+par->anginttype << endl; }
}


void chooseDynamics(Dynamics * & dyn, Boundary * BC, Interaction * inter, Interaction * anginter, Parameters * par) {
  if ((par->numtype)=="Euler") { dyn=new Dynamics_Euler(par,BC,inter,anginter); }
  else if ((par->numtype)=="RK2") { dyn=new Dynamics_RK2(par,BC,inter,anginter); }
  else
    { cout << "[Error] " << par->numtype << " is not a known type of integrator." << endl; exit(100); }
  if (par->verbose) { cout << "Type of integrator: "+par->numtype << endl; }
}


void chooseNeighborList(NeighborList * & NL, Boundary * BC, Parameters * par) {
  if ((par->NLtype)=="Everyone") { NL=new NeighborList_Everyone(par,BC); }
  else if ((par->NLtype)=="Verlet") { NL=new NeighborList_Verlet(par,BC); }
  else
    { cout << "[Error] " << par->NLtype << " is not a known type of neighbor list." << endl; exit(100); }
  if (par->verbose) { cout << "Type of neighbor list: "+par->NLtype << endl; }
}



