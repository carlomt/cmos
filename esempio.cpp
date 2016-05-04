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
#include "Analisi.h"

//#include "getFileCreationTime.h"

using std::cout;
using std::endl;
using std::ifstream;
using std::string;
using std::atoi;
using std::atof;


int main(int argc, char *argv[])
{
  const string fname(argv[1]);
  TFile f(fname.c_str(),"READ");
  TTree *ReducedDataTree=(TTree*) f.Get("CMOSReducedData");
  SeedList seed_list;
  TBranch *bSeedList = ReducedDataTree->GetBranch("seed_list");
  bSeedList->SetAddress(&seed_list);

  Long64_t nentries = ReducedDataTree->GetEntries();

  cout<<"nentries: "<<nentries<<endl;

  Long64_t nbytes = 0, nb = 0;
  Analisi analisi;
  
   for (Long64_t jentry=0; jentry<nentries;jentry++)
     {
       nb = ReducedDataTree->GetEntry(jentry);   nbytes += nb;
       std::cout<<seed_list.GetIdFrame()<<std::endl;
       analisi.AnalisiData(seed_list);
     }
   analisi.WriteOnFile("provaAnalisi.root");
}
