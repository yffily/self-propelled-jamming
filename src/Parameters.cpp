#include "Parameters.h"

Parameters::Parameters(){}

Parameters::Parameters(const char* filepath) {
  
  string filepathstr(filepath);
  filename=filepathstr.substr(filepathstr.find_last_of("/")+1);

  ConfigFile conf(filepath);
  conf.readInto(verbose, "verbose");
  conf.readInto(outdir, "outdir");
  conf.readInto(np, "np");
  conf.readInto(tsim, "tsim");
  conf.readInto(nsuivi, "nsuivi");
  conf.readInto(tprint, "tprint");
  conf.readInto(seed_iniRad, "seed_iniRad");

// particle
  conf.readInto(v0, "v0");
  conf.readInto(tau_S, "tau_S");
  conf.readInto(radius, "radius");
  conf.readInto(polydis,"polydis");
  conf.readInto(selfAlign,"selfAlign");

// box
  conf.readInto(boxtype, "boxtype");
  conf.readInto(boxfile, "boxfile");
  conf.readInto(boxVec1, "boxVec1");
  conf.readInto(boxVec2, "boxVec2");
  conf.readInto(boxRadius, "boxRadius");

// boundary
  conf.readInto(BCtype, "BCtype");
  conf.readInto(BCradius, "BCradius");
  conf.readInto(BCpolydis, "BCpolydis");
  conf.readInto(BCoverlap, "BCoverlap");

// initializer
  conf.readInto(initype, "initype");
  conf.readInto(paramfile, "paramfile");
  conf.readInto(iniangletype, "iniangletype");
  conf.readInto(iniformat, "iniformat");
  conf.readInto(fillmode,"fillmode");
  conf.readInto(seed_iniPos, "seed_iniPos");
  conf.readInto(seed_iniAngle, "seed_iniAngle");

  conf.readInto(inifilename, "inifilename");
  conf.readInto(phi, "phi");
  conf.readInto(stripfrac, "stripfrac");

  conf.readInto(iniboxtype, "iniboxtype");
  conf.readInto(iniboxVec1, "iniboxVec1");
  conf.readInto(iniboxVec2, "iniboxVec2");
  conf.readInto(iniboxRadius, "iniboxRadius");
  conf.readInto(offsetX, "offsetX");
  conf.readInto(offsetY, "offsetY");

// dynamics
  conf.readInto(numtype, "numtype");
  conf.readInto(dt, "dt");
  conf.readInto(sigma, "sigma");
  conf.readInto(seed_th, "seed_th");
  conf.readInto(filename_maxstep, "filename_maxstep");

// interaction
  conf.readInto(inttype, "inttype");
//  conf.readInto(eta, "eta");
  conf.readInto(k_rep, "k_rep");
  conf.readInto(R_ad, "R_ad");
  conf.readInto(F_ad, "F_ad");
  conf.readInto(anginttype, "anginttype");
  conf.readInto(tau_V, "tau_V");

// neighbor list
  conf.readInto(NLtype, "NLtype");
  conf.readInto(rListType, "rListType");
  conf.readInto(rList, "rList");
  conf.readInto(rListRatio, "rListRatio");


// make sure there's a "/" at the end of the folder names
  outdir=outdir.substr(0,outdir.find_last_not_of("/")+1)+"/";

  nstep=int(tsim/dt);
  nprint=int(tprint/dt);
  nstepsuivi=nstep/nsuivi;
  sigma*=sqrt(dt);
}

//parameters::~parameters(){}


void Parameters::print(string outputname) {
  ofstream os(outputname.c_str());
  int w=20;
  os << setw(w) << "verbose"  << setw(w) << verbose << endl;
  os << setw(w) << "outdir" << setw(w) << outdir << endl;
  os << setw(w) << "np" << setw(w) << np << endl;
  os << setw(w) << "tsim" << setw(w) << tsim << endl;
  os << setw(w) << "nsuivi" << setw(w) << nsuivi << endl;
  os << setw(w) << "tprint" << setw(w) << tprint << endl;
  os << setw(w) << "seed_iniRad" << setw(w) << seed_iniRad << endl;

// particle
  os << setw(w) << "v0" << setw(w) << v0 << endl;
  os << setw(w) << "tau_S" << setw(w) << tau_S << endl;
  os << setw(w) << "radius" << setw(w) << radius << endl;
  os << setw(w) << "polydis" << setw(w) << polydis << endl;
  os << setw(w) << "selfAlign" << setw(w) << selfAlign << endl;

// box
  os << setw(w) << "boxtype" << setw(w) << boxtype << endl;
  os << setw(w) << "boxfile" << setw(w) << boxfile << endl;
  os << setw(w) << "boxVec1x" << setw(w) << boxVec1.getX(0) << endl;
  os << setw(w) << "boxVec1y" << setw(w) << boxVec1.getX(1) << endl;
  os << setw(w) << "boxVec2x" << setw(w) << boxVec2.getX(0) << endl;
  os << setw(w) << "boxVec2y" << setw(w) << boxVec2.getX(1) << endl;
  os << setw(w) << "boxRadius" << setw(w) << boxRadius << endl;

// boundary
  os << setw(w) << "BCtype" << setw(w) << BCtype << endl;
  os << setw(w) << "BCradius" << setw(w) << BCradius << endl;
  os << setw(w) << "BCpolydis" << setw(w) << BCpolydis << endl;
  os << setw(w) << "BCoverlap" << setw(w) << BCoverlap << endl;

// initializer
  os << setw(w) << "initype" << setw(w) << initype << endl;
  os << setw(w) << "iniangletype" << setw(w) << iniangletype << endl;
  os << setw(w) << "iniformat" << setw(w) << iniformat << endl;
  os << setw(w) << "fillmode" << setw(w) << fillmode << endl;
  os << setw(w) << "seed_iniPos" << setw(w) << seed_iniPos << setw(w) << endl;
  os << setw(w) << "seed_iniAngle" << setw(w) << seed_iniAngle << endl;

  os << setw(w) << "inifilename" << setw(w) << inifilename << endl;
  os << setw(w) << "phi" << setw(w) << phi << endl;
  os << setw(w) << "stripfrac" << setw(w) << stripfrac << endl;

  os << setw(w) << "iniboxtype" << setw(w) << iniboxtype << endl;
  os << setw(w) << "iniboxVec1x" << setw(w) << iniboxVec1.getX(0) << endl;
  os << setw(w) << "iniboxVec1y" << setw(w) << iniboxVec1.getX(1) << endl;
  os << setw(w) << "iniboxVec2x" << setw(w) << iniboxVec2.getX(0) << endl;
  os << setw(w) << "iniboxVec2y" << setw(w) << iniboxVec2.getX(1) << endl;
  os << setw(w) << "iniboxRadius" << setw(w) << iniboxRadius << endl;
  os << setw(w) << "offsetX" << setw(w) << offsetX << endl;
  os << setw(w) << "offsetY" << setw(w) << offsetY << endl;

// dynamics
  os << setw(w) << "numtype" << setw(w) << numtype << endl;
  os << setw(w) << "dt" << setw(w) << dt << endl;
  os << setw(w) << "sigma" << setw(w) << sigma << endl;
  os << setw(w) << "seed_th" << setw(w) << seed_th << endl;
  os << setw(w) << "filename_maxstep" << setw(w) << filename_maxstep << endl;

// interaction
  os << setw(w) << "inttype" << setw(w) << inttype << endl;
//  os << setw(w) << "eta" << setw(w) << eta << endl;
  os << setw(w) << "k_rep" << setw(w) << k_rep << endl;
  os << setw(w) << "R_ad" << setw(w) << R_ad << endl;
  os << setw(w) << "F_ad" << setw(w) << F_ad << endl;
  os << setw(w) << "anginttype" << setw(w) << anginttype << endl;
  os << setw(w) << "tau_V" << setw(w) << tau_V << endl;

// neighbor list
  os << setw(w) << "NLtype" << setw(w) << NLtype << endl;
  os << setw(w) << "rListType" << setw(w) << rListType << endl;
  os << setw(w) << "rList" << setw(w) << rList << endl;
  os << setw(w) << "rListRatio" << setw(w) << rListRatio << endl;

  os << endl;
  os << setw(w) << "nstep" << setw(w) << nstep << endl;
  os << setw(w) << "nsuivi" << setw(w) << nsuivi << endl;
  os << setw(w) << "nprint" << setw(w) << nprint << endl;
}


