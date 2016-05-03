#include <iostream>
#include <fstream>
#include <cstdlib>

#include "Frame.h"
#include "Seed.h"
#include "SeedList.h"

#include "TROOT.h"
#include "TSystem.h"
#include "TTree.h"
#include "TFile.h"
#include "TClassTable.h"

//#include "getFileCreationTime.h"

using std::cout;
using std::endl;
using std::ifstream;
using std::string;
using std::atoi;
using std::atof;


int main()
{
  TFile f(fname.c_str(),"READ");
  TTree *ReducedDataTree=(TTree*) f.Get("CMOSReducedData");
  SeedList seed_list;
  TBranch *bSeedList = CMOSDataTree->GetBranch("seed_list");
  bSeedList->SetAddress(&seed_list);

  Long64_t nentries = ReducedDataTree->GetEntries();

  Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++)
     {
       nb = ReducedDataTree->GetEntry(jentry);   nbytes += nb;
       std::cout<<seed_list.GetIdFrame()<<std::endl;
     }
}
