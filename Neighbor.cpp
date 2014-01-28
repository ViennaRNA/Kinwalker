/*
  Last changed Time-stamp: <2006-02-16 13:17:14 xtof>
  $Id: Neighbor.cpp,v 1.2 2006/02/16 12:27:07 xtof Exp $
*/
#include <iostream>
#include <string>
#include <vector>

#include "Energy.h"
#include "Utils.h"

std::vector<int>
MakeDeleteMove(int i, int j)
{
  std::vector<int> move;

  move.push_back(-i);
  move.push_back(-j);

  return (move);
}

std::vector<int>
MakeInsertMove(int i, int j)
{
  std::vector<int> move;

  move.push_back(i);
  move.push_back(j);

  return (move);
}

std::vector<int>
MakeShift_i_Move(int i, int j)
{
  std::vector<int> move;

  move.push_back(-i);
  move.push_back(j);

  return (move);
}

std::vector<int>
MakeShift_j_Move(int i, int j)
{
  std::vector<int> move;

  move.push_back(i);
  move.push_back(-j);

  return (move);
}

/**
 * Generates all base pair insertion/deletion neighbors for the input
 * structure structure
 */
std::vector<std::string>
MakeNeighborList(std::string sequence, std::string structure)
{
  int length = structure.size();
  std::vector<std::string> neighbors;
  std::vector<std::vector<int> > posl(length,std::vector<int>());
  std::vector<int> loop = MakeLoopIndex(structure);
  std::vector<int> ptbl = MakePairTable(structure);

  for (int p=0; p<length; p++) {

    if ( structure[p] != '.' ) continue;

    int l = loop[p];
    posl[l].push_back(p);
  }

  // find insert neighbors
  for (std::vector<std::vector<int> >::const_iterator pl = posl.begin();
       pl != posl.end(); pl++) {
    for (std::vector<int>::const_iterator i = (*pl).begin();
	 i != (*pl).end(); i++) {
      for (std::vector<int>::const_iterator j = (*pl).begin();
	   j != (*pl).end(); j++) {

	//PrettyPrint("%2d %2d %c %c 0\n", *i, *j, sequence[*i], sequence[*j]);
	// minimal loop constraint
	if ( *j < (*i + 4) ) continue;
	//PrettyPrint("%2d %2d %c %c 1\n", *i, *j, sequence[*i], sequence[*j]);
	// base pairing constraint
	if (! CanPair(sequence[*i], sequence[*j]) ) continue;
	//PrettyPrint("%2d %2d %c %c 2\n", *i, *j, sequence[*i], sequence[*j]);
	// are on the same tree level
	if (loop[*i] != loop[*j]) continue;
	//PrettyPrint("%2d %2d %c %c 3\n", *i, *j, sequence[*i], sequence[*j]);

	// valid insert neighbor
	std::string neighbor(structure);
	neighbor.replace(*i, 1, "(");
	neighbor.replace(*j, 1, ")");
	//PrettyPrint("%s\n", neighbor.c_str());
	neighbors.push_back(neighbor);
      }
    }
  }

  // find delete neighbors
  for (int p=0; p<length; p++) {

    if ( structure[p] != '(' ) continue;

    // valid delete neighbor
    std::string neighbor(structure);
    neighbor.replace(p, 1, ".");
    neighbor.replace(ptbl[p], 1, ".");
    neighbors.push_back(neighbor); 
  }

  return (neighbors);  
}

// End of file
