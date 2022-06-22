#include "Interaction.h"


double Interaction::getRContactInt(vector<Particle> & pList) {
	double rInt=0.;
	vector<Particle>::iterator i;
	for (i=pList.begin();i!=pList.end();i++)
		{ if (2*i->getRadius()>rInt) { rInt=2*i->getRadius(); } }
	return rInt;
}


// Elastic

Geomvec2 Interaction_Elastic::getForce(const Geomvec2 & dpos, const double & dist, const double & cutoff) {
	if (dist>cutoff)
		{ return Geomvec2(0.,0.); }
	else
		{ return k_rep*(cutoff/dist-1)*dpos; }
}

Geomvec2 Interaction_Elastic::getForce(const Particle & p, const Neighbor & n) {
	return getForce(n.getDPos(),n.getDist(),p.getRadius()+n.getPart()->getRadius());
}



// Elastic with adhesion

Interaction_ElasticWithAdhesion::Interaction_ElasticWithAdhesion(const double k_rep, const double F_ad, const double R_ad) {
	this->k_rep=k_rep; this->F_ad=F_ad; this->R_ad=R_ad;
	k_ad=4*F_ad/(R_ad*R_ad);
}

Geomvec2 Interaction_ElasticWithAdhesion::getForce(const Geomvec2 & dpos, const double & dist, const double & cutoff) {
	if (dist>cutoff+R_ad)
		{ return Geomvec2(0.,0.); }
	else if (dist>cutoff)
		{ return k_ad*(cutoff/dist-1)*(cutoff+R_ad-dist)*dpos; }
	else
		{ return k_rep*(cutoff/dist-1)*dpos; }
}

Geomvec2 Interaction_ElasticWithAdhesion::getForce(const Particle & p, const Neighbor & n) {
	return getForce(n.getDPos(),n.getDist(),p.getRadius()+n.getPart()->getRadius());
}


// Vicsek

double Interaction_Vicsek::getTorque(const Particle & p, const Neighbor & n) {
//  if (n.getContact()) {
	return BC->EnforceAngle(n.getPart()->getAngle()-p.getAngle())/tau; // }
//  else
//    { return 0.; }
}



// Boussinesq

Geomvec2 Interaction_Boussinesq::getForce(const Particle & p, const Neighbor & n) {
	if (n.getDist()>r_bq)
		{ return Geomvec2(0.,0.); }
	else
		{
		theta=n.getDPos().angle();
		double t1=p.getAngle()-theta;
		double t2=n.getPart()->getAngle()-theta;
		double r5;
		// prevent boussinesq force from diverging at small r
		double d=p.getRadius()+n.getPart()->getRadius();
		if (n.getDist()<d) { r5=d*d*d*d*d; }
		else { r5=n.getDist()*n.getDist2()*n.getDist2(); }
		return getForce_repulsion(n.getDPos(),n.getDist(),d) \
							+ getForce_bq(n.getDPos(),r5,t1,t2);
		}
}

Geomvec2 Interaction_Boussinesq::getForce_bq(const Geomvec2 & dpos, const double & r5, const double & t1, const double & t2) {
	double pref=0.375*F_bq*(poisson+1)/r5-pref_force_cut;
	double f_rad= ( 6*(poisson-1)*(cos(2*t1)+cos(2*t2)) +2*(poisson-1) \
				 +(poisson-2)*cos(2*t2-2*t1)-15*poisson*cos(2*t1+2*t2) );
	double f_az=(20*sin(2*t2+2*t1)-4*sin(2*t2)-4*sin(2*t1))*poisson+4*sin(2*t2)+4*sin(2*t1);
	return pref*(f_rad*dpos+f_az*dpos.perp());
}

Geomvec2 Interaction_Boussinesq::getForce_repulsion(const Geomvec2 & dpos, const double & r, const double & cutoff) {
	if (r>cutoff)
		{ return Geomvec2(0.,0.); }
	else
		{ return k_rep*(cutoff/r-1)*dpos; }
}

//double Interaction_Boussinesq::getTorque(const Particle & p, const Neighbor & n) {
//	theta=n.getDPos().angle();
//	double t1=p.getAngle()-theta;
//	double t2=n.getPart()->getAngle()-theta;
//	double r3=n.getDist()*n.getDist2();
//	return 0.25*(poisson+1)*F_bq/r3*( 6*(poisson-1)*sin(2*t1)-15*poisson*sin(2*(t2+t1))+(2-poisson)*sin(2*(t2-t1)) );
//}

double * Interaction_Boussinesq::getTorqueNS(const Particle & p, const Neighbor & n) {
	if (n.getDist()>r_bq)
		{ nonsymTorque[0]=0; nonsymTorque[1]=0; }
	else
		{
		theta=n.getDPos().angle();
		double t1=p.getAngle()-theta;
		double t2=n.getPart()->getAngle()-theta;
		double r3=n.getDist()*n.getDist2();
		double pref=0.25*(poisson+1)*F_bq/r3-pref_torque_cut;
		double sym=-15*poisson*sin(2*(t2+t1));
		double asym=(2-poisson)*sin(2*(t2-t1));
		nonsymTorque[0]=pref*(sym+asym+6*(poisson-1)*sin(2*t1));
		nonsymTorque[1]=pref*(sym-asym+6*(poisson-1)*sin(2*t2));
		}
	return nonsymTorque;
}



