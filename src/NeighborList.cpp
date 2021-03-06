#include "NeighborList.h"


NeighborList::NeighborList(Parameters * par, Boundary * BC) {
	this->par=par;
	this->BC=BC;
}

void NeighborList::checkNeighbors(vector<Particle> & pList, Interaction * inter, Interaction * anginter) {
	for (vector <Particle>::iterator it=pList.begin(); it<pList.end();it++)
		{ it->setNc(0); }
	for (unsigned int i=0;i<pList.size();i++)
		{
		for (list <Neighbor>::iterator j=HNL[i].begin();j!=HNL[i].end();j++)
			{ j->setDPos(BC->EnforceCentered(pList[i].getPos()-j->getPart()->getPos()));
			j->updateDist2();
			j->updateDist();
			if (j->getDist()<max(inter->getRInt(pList[i].getRadius()+j->getPart()->getRadius()),anginter->getRInt(pList[i].getRadius()+j->getPart()->getRadius())))
				{ j->setContact(true); j->getPart()->addNc(1); pList[i].addNc(1); }
			else { j->setContact(false); }
			}
		}
}

void NeighborList::printNeighborList(const string & filename) {
	ofstream os(filename.c_str());
	os << "# part ID ; neighbor ID ; dpos ; dist^2 ; dist"<< endl;
	for (int i=0;i<int(HNL.size());i++) {
		for (list <Neighbor>::iterator j=HNL[i].begin();j!=HNL[i].end();j++)
			{ os << i << sep << j->getPart()->getID() << sep << j->getDPos();
			os << j->getDist2() << sep << j->getDist() << sep << j->getContact() << endl; }
		}
	os.close();
}

void NeighborList::makeRInt(vector<Particle> & pList, Interaction * inter, Interaction * anginter) {
	rInt=max(inter->getRInt(inter->getRContactInt(pList)),anginter->getRInt(inter->getRContactInt(pList)));
	rInt2=rInt*rInt;
	checkRInt();
}


// Everyone //

void NeighborList_Everyone::makeList(vector<Particle> & pList) {
	HNL.clear();
	for (vector <Particle>::iterator i=pList.begin();i!=pList.end();i++)
		{ HNL.push_back(list <Neighbor>());
		for (vector <Particle>::iterator j=pList.begin();j!=i;j++)
			{ HNL.back().push_back(Neighbor(*j)); }
		}
}


// Square //

void NeighborList_Square::makeList(vector<Particle> & pList) {
	HNL.clear();
	for (vector <Particle>::iterator i=pList.begin();i!=pList.end();i++)
		{ HNL.push_back(list <Neighbor>());
		for (vector <Particle>::iterator j=pList.begin();j!=i;j++)
			{
			Geomvec2 dp=BC->EnforceCentered(i->getPos()-j->getPos());
			if (abs(dp.x)<=rInt && abs(dp.y)<=rInt) { HNL.back().push_back(Neighbor(*j)); }
			}
		}
}


// SquareVerlet //

NeighborList_SquareVerlet::NeighborList_SquareVerlet(Parameters * par, Boundary * BC): NeighborList(par,BC) {
	rList=par->rList; rList2=rList*rList;
}

void NeighborList_SquareVerlet::checkRInt() {
	if (par->rListType=="Ratio")
		{ rList= rInt*par->rListRatio;
		if (par->verbose) { cout << "rList adjusted to rInt*" << par->rListRatio << "=" << rList << endl; }
		}
	else if (rList<1.05*rInt)
		{  cout << "[Warning] The radius of the neighbor list is smaller than the range of the interaction: rList = " << par->rList << ", rInt = " << rInt << "!" << endl;
		cin.get(); }
	rUpdate2=rList-rInt; rUpdate2*=rUpdate2;
	par->rList=rList; rList2=rList*rList;
}

bool NeighborList_SquareVerlet::checkForUpdate(vector<Particle> & pList) {
	bool check=false;
	vector <Particle>::iterator i=pList.begin();
	vector <Geomvec2>::iterator j=pList_last.begin();
	while (check==false && i!=pList.end())
		{ if ((i->getPos()-*j).norm2()>rUpdate2) { check=true; }
		i++; j++; }
	return check;
}

void NeighborList_SquareVerlet::makeList(vector<Particle> & pList) {
	pList_last.clear();
	HNL.clear();
	vector <Particle>::iterator i,j;
	for (i=pList.begin();i!=pList.end();i++)
		{ pList_last.push_back(i->getPos()); }
	for (i=pList.begin();i!=pList.end();i++)
		{ HNL.push_back(list <Neighbor>());
		for (j=pList.begin();j!=i;j++)
			{
			Geomvec2 dp=BC->EnforceCentered(i->getPos()-j->getPos());
			if (abs(dp.x)<=rList && abs(dp.y)<=rList) { HNL.back().push_back(Neighbor(*j)); }
			}
		}
}


// Verlet //

NeighborList_Verlet::NeighborList_Verlet(Parameters * par, Boundary * BC): NeighborList(par,BC) {
	rList=par->rList; rList2=rList*rList;
}

void NeighborList_Verlet::checkRInt() {
	if (par->rListType=="Ratio")
		{ rList= rInt*par->rListRatio;
		if (par->verbose) { cout << "rList adjusted to rInt*" << par->rListRatio << "=" << rList << endl; }
		}
	else if (rList<1.05*rInt)
		{  cout << "[Warning] The radius of the neighbor list is smaller than the range of the interaction: rList = " << par->rList << ", rInt = " << rInt << "!" << endl;
		cin.get(); }
	rUpdate2=rList-rInt; rUpdate2*=rUpdate2;
	par->rList=rList; rList2=rList*rList;
}

bool NeighborList_Verlet::checkForUpdate(vector<Particle> & pList) {
	bool check=false;
	vector <Particle>::iterator i=pList.begin();
	vector <Geomvec2>::iterator j=pList_last.begin();
	while (check==false && i!=pList.end())
		{ if ((i->getPos()-*j).norm2()>rUpdate2) { check=true; }
		i++; j++; }
	return check;
}

void NeighborList_Verlet::makeList(vector<Particle> & pList) {
	pList_last.clear();
	vector<Particle>::iterator i,j;
	HNL.clear();
	double d2;
	Neighbor n;
	for (i=pList.begin();i!=pList.end();i++)
		{ pList_last.push_back(i->getPos()); }
	for (i=pList.begin();i!=pList.end();i++)
		{
		HNL.push_back(list <Neighbor>());
		for (j=pList.begin();j!=i;j++)
			{
			Geomvec2 dpos=BC->EnforceCentered(i->getPos()-j->getPos());
			d2=dpos.norm2();
			if (d2<rList2)
				{ n=Neighbor(*j,dpos,d2);
				n.updateDist();
				HNL.back().push_back(n); }
			}
		}
}


// Verlet_cell_list //

NeighborList_Verlet_cell_list::NeighborList_Verlet_cell_list(Parameters * par, Boundary * BC): NeighborList(par,BC) {
	rList=par->rList; rList2=rList*rList;
}

void NeighborList_Verlet_cell_list::checkRInt() {
	if (par->rListType=="Ratio")
		{ rList= rInt*par->rListRatio;
		if (par->verbose) { cout << "rList adjusted to rInt*" << par->rListRatio << "=" << rList << endl; }
		}
	else if (rList<1.05*rInt)
		{  cout << "[Warning] The radius of the neighbor list is smaller than the range of the interaction: rList = " << par->rList << ", rInt = " << rInt << "!" << endl;
		cin.get(); }
	rUpdate2=rList-rInt; rUpdate2*=rUpdate2;
	par->rList=rList; rList2=rList*rList;
	
	double box_size[2];
	box_size[0]=BC->box->getBoundingSizeX();
	box_size[1]=BC->box->getBoundingSizeY();
	cell_list.initialize(rList,box_size);
	cell_list.print_cells((par->outdir+"cells.dat").c_str());
	cell_list.particle_to_cell.resize(par->np);
}

bool NeighborList_Verlet_cell_list::checkForUpdate(vector<Particle> & pList) {
	bool check=false;
	vector <Particle>::iterator i=pList.begin();
	vector <Geomvec2>::iterator j=pList_last.begin();
	while (check==false && i!=pList.end())
		{ if ((i->getPos()-*j).norm2()>rUpdate2) { check=true; }
		i++; j++; }
	return check;
}

void NeighborList_Verlet_cell_list::makeList(vector<Particle> & pList) {
//	clog << "renewing neighbor list" << endl;
	pList_last.clear();
	vector <Particle>::iterator i,j;
	list <Particle *>::iterator J;
	list <Cell *>::iterator K;
	HNL.clear();
	double d2;
	Neighbor n;
	cell_list.clear();
	for (i=pList.begin();i!=pList.end();i++)
		{ 
		pList_last.push_back(i->getPos()); 
		cell_list.sort_particle(*i);
		}
	for (i=pList.begin();i!=pList.end();i++)
		{
		HNL.push_back(list <Neighbor>());
		list <Cell *> NC=cell_list.particle_to_cell[i->getID()]->neighboring_cells;
		for (K=NC.begin(); K!=NC.end(); K++)
			{
			for (J=(*K)->particles.begin(); J!=(*K)->particles.end(); J++)
				{
				if ((*J)->getID()<i->getID())
					{
					Geomvec2 dpos=BC->EnforceCentered(i->getPos()-(*J)->getPos());
					d2=dpos.norm2();
					if (d2<rList2)
						{ 
						n=Neighbor(**J,dpos,d2);
						n.updateDist();
						HNL.back().push_back(n); 
						}
					}
				}
			}
		}
}
