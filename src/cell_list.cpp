#include "cell_list.h"


//______________________________________________________________________________
//
// Cell class
//______________________________________________________________________________

double Cell::cell_size[];


//______________________________________________________________________________
//
// Cell_list class
//______________________________________________________________________________


Cell_list::Cell_list(const double & target_cell_size, double box_size[]) { //Box * box_) {
	initialize(target_cell_size,box_size);
}


void Cell_list::initialize(const double & target_cell_size, double box_size_ []) { 
	box_size[0]=box_size_[0];
	box_size[1]=box_size_[1];
	
	// find the correct cell size (box size needs to be a mulitple)
	for (int j=0; j<2; j++)
		{ 
		n_cells[j]=max((int) floor(box_size[j]/target_cell_size),1);
		cell_size[j]=box_size[j]/n_cells[j]; 
		Cell::cell_size[j]=cell_size[j]; 
		}
	n_cells_tot=n_cells[0]*n_cells[1];
	cells.resize(n_cells_tot);
	
	// identify neighboring cells
	int j;
	for (int i=0; i<n_cells_tot; i++)
		{
		// convert cell ID to ID triplet
		cells[i].idt[0]=i%n_cells[0];
		cells[i].idt[1]=(i/n_cells[0])%n_cells[1];
		cells[i].origin=Geomvec2(cells[i].idt[0]*cell_size[0]-0.5*box_size[0],cells[i].idt[1]*cell_size[1]-0.5*box_size[1]);
		}
	for (int i=0; i<n_cells_tot; i++)
		{
		// find the 9 neighboring cells (including self).
		// cell ID douplet is wrapped around to account for periodic boundary.
		for (int k0=cells[i].idt[0]-1; k0<=cells[i].idt[0]+1; k0++)
			{
			int l0=k0-(int) floor(k0/double(n_cells[0]))*n_cells[0];
			for (int k1=cells[i].idt[1]-1; k1<=cells[i].idt[1]+1; k1++)
				{
				int l1=k1-(int) floor(k1/double(n_cells[1]))*n_cells[1];
				j=idt_to_id(l0,l1);
				if (find(cells[i].neighboring_cells.begin(),cells[i].neighboring_cells.end(),&(cells[j]))==cells[i].neighboring_cells.end()) cells[i].neighboring_cells.push_back(&(cells[j]));
				}
			}
		}
}


void Cell_list::sort_particle(Particle & p) {
	int i[2];
	Geomvec2 v=p.getPos();
//	box->bringBack(v);
	for (int j=0; j<2; j++) { i[j]=(int) floor((v.X(j)+0.5*box_size[j])/cell_size[j])%n_cells[j]; }
	int k=idt_to_id(i);
	cells[k].particles.push_back(&p);
	particle_to_cell[p.getID()]=&cells[k];
}

////______________________________________________________________________________


void Cell_list::print_cells(string p) {
	ofstream os(p.c_str());
	for (int i=0; i<=n_cells[0]; i++)
		{
		os << -0.5*box_size[0]+i*cell_size[0] << sep << -0.5*box_size[1] << endl;
		os << -0.5*box_size[0]+i*cell_size[0] << sep << 0.5*box_size[1] << "\n\n";
	 	}
	for (int i=0; i<=n_cells[1]; i++)
		{
		os << -0.5*box_size[0] << sep << -0.5*box_size[1]+i*cell_size[1] << endl;
		os << 0.5*box_size[0] << sep << -0.5*box_size[1]+i*cell_size[1] << "\n\n";
 		}
}

