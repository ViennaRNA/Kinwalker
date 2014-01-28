s/*
  Last changed Time-stamp: <2006-02-15 17:07:30 xtof>
  $Id: Utils.cpp,v 1.6 2007/10/01 08:29:47 Kinwalker Exp $
*/
#include "Utils.h"

void
Fatal(char *fmt, ...)
{
  va_list args;

  va_start(args, fmt);
  fprintf(stderr, "FATAL ERROR: ");
  if ( errno != 0 ) fprintf(stderr, "%s: ", strerror(errno));
  vfprintf(stderr, fmt, args);
  fprintf(stderr, "\n");
  fflush(stderr);
  va_end(args);

  exit(EXIT_FAILURE);
}

void
PrettyPrint(char *fmt, ...)
{
  va_list args;

  va_start(args, fmt);
  vfprintf(stdout, fmt, args);
  fflush(stdout);
  va_end(args);  
}

std::string PrintBasePair(std::pair<int,int> bp){
  return "("+Str(bp.first)+","+Str(bp.second)+")";
}


std::string PrintBasePairList(std::vector<std::pair<int,int> > list){
  std::string sText="";
  for(size_t i=0;i<list.size();i++) sText+=PrintBasePair(list[i]);
  return sText;
}


/**
 * Create a Random RNA string of specified length.
 */
void
GetRNAString(char* sequence, int length)
{ 
  char bases[4] = {'A','U','C','G'};
  for (int j=0; j<length; j++) sequence[j] = bases[lrand48() % 4];
  sequence[length] = '\0';
}


std::string
PairTableToStructure(std::vector<int> pTbl)
{
  std::string structure(pTbl.size(), '.');
  
  int i=0;
  for (std::vector<int>::const_iterator j = pTbl.begin();
       j != pTbl.end();
       j++, i++) {

    // we processed already the i < *j case
    if (i > *j) continue;

    // unpaired position
    if (*j == -1) {
      structure[*j] = '.';
      continue;
    }
    
    structure[i]  = '(';
    structure[*j] = ')';
  }

  return (structure);
}

std::string
BasePairListToStructure(int length, std::vector<std::pair<int,int> > pl)
{
  std::string structure(length, '.');

  for (std::vector<std::pair<int,int> >::const_iterator p = pl.begin();
       p != pl.end();
       p++) {

    structure[(*p).first] = '(';
    structure[(*p).second] = ')';
  }

  return (structure);
}

std::string
BasePairListToStructure1(int length, std::vector<std::pair<int,int> > pl)
{
  std::string structure(length, '.');

  for (std::vector<std::pair<int,int> >::const_iterator p = pl.begin();
       p != pl.end();
       p++) {

    structure[(*p).first-1] = '(';
    structure[(*p).second-1] = ')';
  }

  return (structure);
}

void
StringToTextFile(std::string sText, std::string filePath)
{
  std::ofstream outfile(filePath.c_str());
  outfile << sText;
}



 bool IntroducesPseudoKnot(const std::vector<std::pair<int,int> >& node, const std::pair<int,int>& p1){
       for(size_t i=0;i<node.size();i++){
          std::pair<int,int> p2=node[i];
          if(p1.first==p2.first || p1.first==p2.second || p1.second==p2.first || p1.second==p2.second) return true;
          if(p1.first<=p2.first && p1.second>= p2.first && p1.second<=p2.second) return true;
          if(p1.first>=p2.first && p1.first<=p2.second && p1.second>=p2.second)  return true;    
       }
       return false;
 }

bool ConformationHasPair(const std::vector<std::pair<int,int> >& node,std::pair<int,int> p){
  for(size_t i=0;i<node.size();i++){
    if(node[i].first==p.first && node[i].second==p.second) {
       return true;
    }
  }
  return false;
}

   bool Conflict(const std::vector<std::pair<int,int> >& node, const std::pair<int,int>& p1){
      if(ConformationHasPair(node,p1)) return true;
      if(IntroducesPseudoKnot(node,p1)) return true;
      return false;
   }      

 bool Conflict(const std::vector<std::pair<int,int> >& node1, const std::vector<std::pair<int,int> >& node2){
     for(size_t i=0;i<node2.size();i++){
       if(Conflict(node1,node2[i])) return true;
     }
     return false;

   }

std::vector<std::vector<std::pair<int,int> > > ConformationToStacks(std::vector<std::pair<int,int> > node,int stacksize){
  std::vector<std::vector<std::pair<int,int> > > stacks= std::vector<std::vector<std::pair<int,int> > >();
  //std::vector<std::pair<int,int> > contacts=Contacts(node);
  sort(node.begin(),node.end());
  //std::pair<int,int> prev=node.front();
  std::vector<std::pair<int,int> > stack(node.begin(),node.begin()+1);//=std::vector<std::pair<int,int> >();
  //  stack.push_back(prev);
  //bool added=false;
  if(node.size()==1 && stacksize==1) stacks.push_back(stack);
  for(size_t i=1;i<node.size();i++){
    bool added=false;
    if(node[i].first==stack.back().first+1 && node[i].second==stack.back().second-1) {
      added=true;
      stack.push_back(std::pair<int,int>(stack.back().first+1,stack.back().second-1));
    }
    else if(node[i].first==stack.back().first+2 && node[i].second==stack.back().second-1) {
      added=true;
      stack.push_back(std::pair<int,int>(stack.back().first+2,stack.back().second-1));
    }
    else if(node[i].first==stack.back().first+1 && node[i].second==stack.back().second-2) {
      added=true;
      stack.push_back(std::pair<int,int>(stack.back().first+1,stack.back().second-2));
    }
    if(stack.size()>=(size_t)stacksize && (!added || i==node.size()-1) ) {
      stacks.push_back(stack);
      stack.clear();
      stack.push_back(node[i]);
    }
    if(!added) {
      stack.clear();
      stack.push_back(node[i]);
    }
  }
  return stacks;
}



// End of file
