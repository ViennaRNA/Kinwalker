/*
  Last changed Time-stamp: <2006-02-15 17:03:24 xtof>
  $Id: Utils.h,v 1.4 2007/04/02 16:43:17 xtof Exp $
*/
#ifndef _UTILS_H_
#define _UTILS_H_

#include <string>
#include <vector>
#include <algorithm>
#include <cerrno>
#include <cstdarg>
#include <fstream>
#include <iostream>
#include <iterator>
#include <iomanip>
#include <sstream>

/*
void
Warn(char *fmt, ...);
void
Fatal(char* fmt, ...);
void
PrettyPrint(char *fmt, ...);
void 
GetRNAString(char* sequence, int length);
void
StringToTextFile(std::string sText, std::string filePath);
template <class T>
std::string Str(T x);
void
PrintESpair(std::pair<float,std::string> p);
void
PrintESpairNL(std::pair<float, std::string> p);
void
PrintPairTable(std::vector<int> pTbl);
*/

std::string
PairTableToStructure(std::vector<int> pTbl);
//void
//PrintBasePairList(std::vector<std::pair<int,int> > bpl);
std::string
BasePairListToStructure(int lenght, std::vector<std::pair<int,int> > pl);
std::string
BasePairListToStructure1(int length, std::vector<std::pair<int,int> > pl);
std::string PrintBasePair(std::pair<int,int> bp);
std::string PrintBasePairList(std::vector<std::pair<int,int> > list);


bool IntroducesPseudoKnot(const std::vector<std::pair<int,int> >& node, const std::pair<int,int>& p1);

bool ConformationHasPair(const std::vector<std::pair<int,int> >& node,std::pair<int,int> p);

bool Conflict(const std::vector<std::pair<int,int> >& node, const std::pair<int,int>& p1);

bool Conflict(const std::vector<std::pair<int,int> >& node1, const std::vector<std::pair<int,int> >& node2);

//struct BasePairConflict :public std::binary_function<std::pair<int,int>,std::pair<int,int>,bool>;

std::vector<std::vector<std::pair<int,int> > > ConformationToStacks(std::vector<std::pair<int,int> > node,int stacksize);
#endif

/* End of file */
