/*
  Last changed Time-stamp: <2006-02-10 17:11:03 xtof>
  $Id: Neighbor.h,v 1.1.1.1 2006/02/15 10:15:53 xtof Exp $
*/
#ifndef _NEIGHBOR_H_
#define _NEIGHBOR_H_

#include <vector>

std::vector<int>
MakeDeleteMove(int i, int j);
std::vector<int>
MakeInsertMove(int i, int j);
std::vector<int>
MakeShift_i_Move(int i, int j);
std::vector<int>
MakeShift_j_Move(int i, int j);
std::vector<std::string>
MakeNeighborList(std::string sequence, std::string structure);

#endif

/* End of file */
