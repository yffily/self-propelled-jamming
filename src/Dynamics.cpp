#include "Dynamics.h"


Dynamics::Dynamics(Parameters * par, Boundary * BC, Interaction * inter, Interaction * anginter) {
  this->par=par;
  this->BC=BC;
  this->inter=inter;
  this->anginter=anginter;
  RNG_r=RNG_taus(par->seed_rth);
  RNG_t=RNG_taus(par->seed_tth);
  t=0;
  dposmax=0.; dangmax=0.; stm_max.open((par->outdir+par->filename_maxstep).c_str());
  targetAngle.resize(par->np);
}


void Dynamics::getNoise(vector<Particle> & pList) {
  if (par->sigma_t!=0)
    { for (vector <Particle>::iterator it=pList.begin(); it<pList.end();it++)
      { if (!it->getGlued()) { it->setNoise(RNG_t,par->sigma_t); } }
    }
  if (par->sigma_r!=0)
    { for (vector <Particle>::iterator it=pList.begin(); it<pList.end();it++)
      { if (!it->getGlued()) { it->setAngNoise(RNG_r,par->sigma_r); } }
    }
} 


void Dynamics::getAngVel(vector<Particle> & pList, NeighborList * NL) {
  for (vector <Particle>::iterator it=pList.begin(); it<pList.end();it++)
    { it->setAngVel(0.); }
  if (par->anginttype=="Target") { getTargetAngle(pList,NL); getAngVel_target(pList); }
  else if (par->anginttype!="None") { getAngVel_angint(pList,NL); }
}

// angVel_angint computes an angular velocity based on a pairwise torque
void Dynamics::getAngVel_angint(vector<Particle> & pList, NeighborList * NL) {
  list <Neighbor>::const_iterator j;
  for (unsigned int i=0;i<pList.size();i++)
    { pList[i].setAngVel(0.);
    if (!(pList[i].getGlued()))
      {
      for (j=NL->getHNL(i).begin();j!=NL->getHNL(i).end();j++)
        {
        double torqueij=anginter->getTorque(pList[i],*j);
        pList[i].addAngVel(torqueij);
        j->getPart()->addAngVel(-torqueij);
        }
      }
    }
}

// angVel_target computes an angular velocity based on a target angle and lag time tau_S
void Dynamics::getAngVel_target(vector<Particle> & pList) {
  for (unsigned int i=0;i<pList.size();i++)
    {
    pList[i].setAngVel(0.);
    if (!pList[i].getGlued())
      {
      double dangle=targetAngle[i]-pList[i].getAngle();
      while (dangle>pi) {dangle-=dblpi;}
      while (dangle<=-pi) {dangle+=dblpi;}
      pList[i].setAngVel(dangle/par->tau);
      }
    }
}

void Dynamics::getTargetAngle(vector<Particle> & pList, NeighborList * NL) {
//  targetAngle.resize(pList.size());		// only necessary if the number of particles changes
  Geomvec v[pList.size()];
  for (unsigned int i=0;i<pList.size();i++) { v[i]=0.; }
  for (unsigned int i=0;i<pList.size();i++)
    {
    if (!pList[i].getGlued())
      {
      list <Neighbor>::const_iterator it;
      for (it=NL->getHNL(i).begin();it!=NL->getHNL(i).end();it++)
        {
        if (!(it->getPart()->getGlued()) && it->getContact())
          { v[i]+=it->getPart()->getAVel();
          v[it->getPart()->getID()]+=it->getPart()->getAVel(); }
        }
      }
    }
  for (unsigned int i=0;i<pList.size();i++)
    {
    if (!pList[i].getGlued())
      { if (pList[i].getNc()!=0) { v[i]/=pList[i].getNc(); }
      targetAngle[i]=(par->a_vs*pList[i].getAVel()+par->a_vn*v[i]+par->a_F*pList[i].getVel()).angle2D(); }
    }
}

void Dynamics::getVel(vector<Particle> & pList, NeighborList * NL) {
  for (unsigned int i=0;i<pList.size();i++)
    {
    pList[i].updateAVel();
    pList[i].setVel(Geomvec(0.,0.));
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
} 

void Dynamics_Euler::move(vector<Particle> & pList, NeighborList * NL) {
  if (NL->checkForUpdate(pList)) { NL->makeList(pList); };
  NL->checkNeighbors(pList,inter,anginter);
  getAngVel(pList,NL);
  getVel(pList,NL);
  getNoise(pList);
  max=0;
  for (unsigned int i=0;i<pList.size();i++)
    { 
    if (!(pList[i].getGlued()))		// only non-glued particles have dynamics
      { max=(pList[i].move(par->dt, dposmax, dangmax) || max); }
    }
  if (max) { stm_max << t << sep << sqrt(dposmax) << sep << dangmax << endl; }
  BC->Enforce(pList);
  t+=par->dt;
}


// Euler for positions and instantaneous update for angles

void Dynamics_Euler_InstAng::move(vector<Particle> & pList, NeighborList * NL) {
  if (NL->checkForUpdate(pList)) { NL->makeList(pList); };
  NL->checkNeighbors(pList,inter,anginter);
  getTargetAngle(pList,NL);
  getNoise(pList);
  getVel(pList,NL);
  setAngle(pList,NL);
  max=0;
  for (unsigned int i=0;i<pList.size();i++)
    { 
    if (!(pList[i].getGlued()))		// only non-glued particles have dynamics
      { max=(pList[i].move(par->dt, dposmax, dangmax) || max); }
    }
  if (max) { stm_max << t << sep << dposmax << sep << dangmax << endl; }
  BC->Enforce(pList);
  t+=par->dt;
}


void Dynamics_Euler_InstAng::setAngle(vector<Particle> & pList, NeighborList * NL) {
  for (unsigned int i=0;i<pList.size();i++)
    {
    pList[i].setAngVel(0.);
    if (!pList[i].getGlued()) { pList[i].setAngle(targetAngle[i]); }
    }
}



