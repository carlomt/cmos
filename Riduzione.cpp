#include <iostream>
#include <fstream>

#include "Frame.h"
#include "Seed.h"
#include "SeedList.h"

#include "TTree.h"
#include "TFile.h"



using std::cout;
using std::endl;
using std::ifstream;
using std::string;


int main()
{

  TFile *f=new TFile("prova.root","READ");
  TTree *CMOSDataTree;
  f->GetObject("CMOSDataTree",CMOSDataTree);

  Frame *frame;
  CMOSDataTree->SetBranchAddress("frame",&frame);

  
  Long64_t nentries = CMOSDataTree->GetEntriesFast();

   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = CMOSDataTree->LoadTree(jentry);
      if (ientry < 0) break;
      nb = CMOSDataTree->GetEntry(jentry);   nbytes += nb;
      // if (Cut(ientry) < 0) continue;

      cout<<"frame id "<<frame->GetId()<<endl;
   }
}
