
#include <iostream>		// entrees-sorties vers (depuis) l'ecran (le clavier)
#include <fstream>		// entrees-sorties vers (depuis) des fichiers
#include <cmath>		// fonctions mathematiques de base
#include <stdlib.h>
#include <time.h>
#include <string>		
#include "GeomVec.h"
#include "Container.h"
#include "Particle.h"
#include "Parameters.h"
#include "Printer.h"
#include "Constants.h"
using namespace std;

string TimeToDate(double t);

int main(int argc,char* argv[])
  {
  Geomvec(2);					// initialize number of dimensions
  Parameters par(argv[1]);			// load parameter file
  system(("mkdir "+par.outdir).c_str());
  system(("cp "+string(argv[1])+" "+par.outdir+par.filename).c_str());
// initialize variables for measuring computing time
  clock_t start, end;
  time_t tstart, tend;
  start = clock(); time(&tstart);
  ofstream stmtime((par.outdir+"time.dat").c_str());
  stmtime << "start: " << asctime(localtime(&tstart));

  Container C(&par);				// create sample
  
  Printer posinit(par.outdir+"posinit.dat");
  posinit.printPosRad(C); 
 // Printer posdyn((par.outdir+"posdyn.dat").c_str());
 // posdyn.printPosDyn(C);
  system(("mkdir "+par.outdir+"PosVel").c_str());
  for (int it=0;it<par.nstep;it++)
    {
// print progress status
    if (it%par.nstepsuivi==0)
      { time(&tend);
      cout << it/par.nstepsuivi << "/" << par.nsuivi << "\t" << asctime(localtime(&tend)); }
// updates the particle list (ie forward 1 time step)
    C.update();
// print outputs
    if (it%par.nprint==0)
      {
      stringstream ss;
      ss << par.outdir << "PosVel/PosVel" << it/par.nprint << ".dat";
      Printer posvel(ss.str());
      posvel.printPosVel(C);
//       pos.printPos(C);
//       vel.printVel(C);
     //  posdyn.printPosDyn(C);
      }
    }

// print start and end dates and simulation duration
  time(&tend);
  stmtime << "end:   " << asctime(localtime(&tend)) << endl;
  double ttime=difftime(tend,tstart);
  stmtime << "duration (c_time):  ";
  stmtime  << TimeToDate(ttime) << endl;
  end = clock();
  double dur=((double) (end - start)) / CLOCKS_PER_SEC;
  stmtime << "duration (c_clock): " << dur << "s" << endl;
  return 0;
  }



string TimeToDate(double s) {
  int m=int(s/60); s-=60*m;
  int h=int(m/60); m-=60*h;
  int d=int(h/24); h-=24*d;
  stringstream ss;
  if (d>0) { ss << d << "days "; }
  if (h>0) { ss << h << "h "; }
  if (m>0) { ss << m << "min "; }
  ss << s << "s";
  return ss.str();
}

