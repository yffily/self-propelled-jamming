#include "Printer.h"



void Printer::printPos(const Container & C) {
  for ( vector<Particle>::const_iterator it=(C.getPList()).begin() ; it<(C.getPList()).end(); it++ )
    { it->printPos(os); }
}

void Printer::printPosRad(const Container & C) {
  for ( vector<Particle>::const_iterator it=(C.getPList()).begin() ; it<(C.getPList()).end(); it++ )
    { it->printPosRad(os); }	
}

void Printer::printVel(const Container & C) {
  for ( vector<Particle>::const_iterator it=(C.getPList()).begin() ; it<(C.getPList()).end(); it++ )
    { it->printVel(os); }
}

void Printer::printPosDyn(const Container & C) {
  for ( vector<Particle>::const_iterator it=(C.getPList()).begin() ; it<(C.getPList()).end(); it++ )
    { os << C.getTime() << sep; it->printPos(os); }	
}

void Printer::printPosVel(const Container & C) {
  for ( vector<Particle>::const_iterator it=(C.getPList()).begin() ; it<(C.getPList()).end(); it++ )
    { it->printPosVel(os); }
}
