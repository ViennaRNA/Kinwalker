/*
  Last changed Time-stamp: <2006-02-14 00:39:18 xtof>
  $Id: Barrier.h,v 1.1.1.1 2006/02/15 10:15:53 xtof Exp $
*/
#ifndef _BARRIER_H_
#define _BARRIER_H_

#include <string>
#include <utility>

std::pair<double,std::string>
FindSaddle (std::string sequence,
	    std::string srcStructure,
	    std::string tgtStructure,
	    double E, double max, OptionS* OptS);

#endif

/* End of file */
