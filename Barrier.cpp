/*
  Last changed Time-stamp: <2006-02-14 00:21:10 xtof>
  $Id: Barrier.cpp,v 1.2 2007/10/21 21:01:35 Kinwalker Exp $
*/
#include <iostream>
#include <limits>
#include <string>
#include <utility>
#include <vector>

#include "Energy.h"
#include "Neighbor.h"
#include "Utils.h"
#include "options.h"
#define INF 1000000


/**
 * Find best path connecting source and target structure
 */
std::pair<double,std::string>
FindSaddleOnce(std::string seq,
	       std::string src,
	       std::string tgt,
	       double saddlE, OptionS* OptS)
{

  float srcE = EvalEnergy(seq, src);
  float tgtE = EvalEnergy(seq, tgt);
  
  if (OptS->verbose == 1) PrintESpair(make_pair(srcE, src));
  if (OptS->verbose == 1) PrintESpair(make_pair(tgtE, tgt));

  std::vector<int> srcTbl = MakePairTable(src);
  std::vector<int> tgtTbl = MakePairTable(tgt);

  std::vector<std::vector<int> > indelMoves;
  std::vector<std::vector<int> > shiftMoves;

  for(std::vector<int>::const_iterator i = srcTbl.begin();
      i != srcTbl.end();
      i++) {

    if (srcTbl[*i] != tgtTbl[*i]) {
    
      // insert move
      if ( *i < srcTbl[*i] )
	indelMoves.push_back(MakeDeleteMove(*i, srcTbl[*i]));

      // delete move
      if ( *i < tgtTbl[*i] )
	indelMoves.push_back(MakeInsertMove(*i, tgtTbl[*i]));    

#if 0
      // shift move if poth are paired
      if ( (srcTbl[*i] >= 0) && (tgtTbl[*i] >= 0) )
	shiftMoves.push_back(MakeShift_i_Move(srcTbl[*i], tgtTbl[*i]));  
#endif

      // greedy walk from source to target structure
      
    }
  }

  

  double a;
  std::string b;

  return (make_pair(a, b));
}

std::pair<double,std::string>
FindSaddle (std::string sequence,
	    std::string srcStructure,
	    std::string tgtStructure,
	    double E, double max, OptionS* OptS)
{
  double saddleE = INF;//std::numeric_limits<double>::max();
  double maxKeep = 0.5;
  std::string saddle;
  // std::map<std::string,sEntry> shash;


  while (maxKeep < max) {
    maxKeep *= 2;
    if (max < maxKeep) maxKeep = max;

    // find a path from source structure to target structure
    std::pair<double,std::string> result
      = FindSaddleOnce(sequence,
		       srcStructure,
		       tgtStructure,
		       saddleE, OptS);

    if (result.first < saddleE) {
      saddleE = result.first;
      saddle  = result.second;
    }

    std::string tmp = srcStructure;
    srcStructure = tgtStructure;
    tgtStructure = tmp;
  }

  return (make_pair(saddleE, saddle));
}

// End of file
