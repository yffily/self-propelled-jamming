#include "Dynamics.h"


Dynamics::Dynamics(Parameters * par, Boundary * BC, Interaction * inter, Interaction * anginter) {
  this->par=par;
  this->BC=BC;
  this->inter=inter;
  this->anginter=anginter;
  RNG=RNG_taus(par->seed_th);
  t=0;
  dposmax=0.; dangmax=0.; stm_max.open((par->outdir+par->filename_maxstep).c_str());
}


void Dynamics::getNoise(vector<Particle> & pList) {
  for (int i=0;i<par->np;i++)
    {
    if (!(pList[i].getGlued()))
      { //pList[i].setNoise(RNG,par->sigma);
      pList[i].setAngNoise(RNG,par->sigma); }
    }
} 


void Dynamics::getVel(vector<Particle> & pList, NeighborList * NL) {
  for (int i=0;i<par->np;i++)
    {
    pList[i].setVel(Geomvec(0.,0.));
    pList[i].setActiveVel();
    bool g=!(pList[i].getGlued());
    list <Neighbor>::const_iterator it;
    for (it=NL->getHNL(i).begin();it!=NL->getHNL(i).end();it++)
      {
      if (g || !(it->getPart()->getGlued()))
        {
        Geomvec forceij=inter->getForce(pList[i],*it);
        pList[i].addVel(forceij);
        it->getPart()->addVel(-forceij);
        }
      }
    }
  for (int i=0;i<par->np;i++)
    {
    pList[i].setAngVel(0.);
    if (!(pList[i].getGlued()))
      {
      pList[i].setAlignAngVel();
      list <Neighbor>::const_iterator it;
      for (it=NL->getHNL(i).begin();it!=NL->getHNL(i).end();it++)
        {
        if (!(it->getPart()->getGlued()) && it->getContact())
          { double torqueij=anginter->getTorque(pList[i],*it);
          pList[i].addAngVel(torqueij/pList[i].getNc());
          it->getPart()->addAngVel(-torqueij/it->getPart()->getNc()); }
        }
      }
    }
} 


void Dynamics_Euler::move(vector<Particle> & pList, NeighborList * NL) {
  if (NL->checkForUpdate(pList)) { NL->makeList(pList); };
  NL->checkNeighbors(pList);
//  NL->printNeighborList("data/NL.dat");
  getVel(pList,NL);
  getNoise(pList);
  max=0;
  for (int i=0;i<par->np;i++)
    { 
    if (!(pList[i].getGlued()))		// only non-glued particles have dynamics
      { max=(pList[i].move(par->dt, dposmax, dangmax) || max); }
    }
  if (max) { stm_max << t << sep << dposmax << sep << dangmax << endl; }
  BC->Enforce(pList);
  t+=par->dt;
}



