#include "Interaction.h"



// Elastic

Geomvec Interaction_Elastic::getForce(const Geomvec & dpos, const double & dist, const double & cutoff) {
  if (dist>cutoff)
    { return Geomvec(0.,0.); }		// dpos.getDimension(),0.); }
  else
    { return k_rep*(cutoff/dist-1)*dpos; }
}

Geomvec Interaction_Elastic::getForce(const Particle & p, const Neighbor & n) {
  return getForce(n.getDPos(),n.getDist(),p.getRadius()+n.getPart()->getRadius());
}



// Elastic with adhesion

Interaction_ElasticWithAdhesion::Interaction_ElasticWithAdhesion(const double k_rep, const double F_ad, const double R_ad) {
  this->k_rep=k_rep; this->F_ad=F_ad; this->R_ad=R_ad;
  k_ad=4*F_ad/(R_ad*R_ad);
}


Geomvec Interaction_ElasticWithAdhesion::getForce(const Geomvec & dpos, const double & dist, const double & cutoff) {
  if (dist>cutoff+R_ad)
    { return Geomvec(0.,0.); }
  else if (dist>cutoff)
    { return k_ad*(cutoff/dist-1)*(cutoff+R_ad-dist)*dpos; }
  else
    { return k_rep*(cutoff/dist-1)*dpos; }
}

Geomvec Interaction_ElasticWithAdhesion::getForce(const Particle & p, const Neighbor & n) {
  return getForce(n.getDPos(),n.getDist(),p.getRadius()+n.getPart()->getRadius());
}



// Vicsek

double Interaction_Vicsek::getTorque(const Particle & p, const Neighbor & n) {
//  if (n.getContact()) {
  return BC->EnforceAngle(n.getPart()->getAngle()-p.getAngle())/tau_V; // }
//  else
//    { return 0.; }
}
