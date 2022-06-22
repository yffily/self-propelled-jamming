#ifndef cell_list_h
#define cell_list_h
#include <stdlib.h>
#include <string>
#include <vector>
#include <list>
#include "Geomvec2.h"
//#include "Box.h"
#include "RNG_taus.h"
#include "Parameters.h"
#include "Particle.h"
using namespace std;

class Particle;
//class Box;

class Cell
{
public:
	static double cell_size[2];           // cell size in each direction
	int idt[2];                           // cell id triplet (one integer per direction)
	list <Particle *> particles;
	list <Cell *> neighboring_cells;      // all the neighboring cells
	Geomvec2 origin;
};


class Cell_list
{
public:
	int n_cells[2];          // number of cells in each direction
	int n_cells_tot;         // total number of cells
	double cell_size[2];
	vector <Cell> cells;
//	Box * box;
	double box_size[2];
	vector <Cell *> particle_to_cell;
	
	Cell_list() {};
	Cell_list(const double & target_cell_size, double box_size[]); //Box * box);
	
	void initialize(const double & target_cell_size, double box_size[]);
	int idt_to_id(const int & i0, const int & i1) { return i0+n_cells[0]*i1; };
	int idt_to_id(const int i[]) { return i[0]+n_cells[0]*i[1]; };
	
	void sort_particle(Particle & p);
	
	void clear()
		{
		particle_to_cell.clear();
		for (int i=0; i<n_cells_tot; i++) { cells[i].particles.clear(); }
		};
	
	void print_cells(string p);
};


#endif
