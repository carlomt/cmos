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

#define DEBUG

using std::cout;
using std::endl;
using std::ifstream;
using std::string;
using std::atoi;
using std::atof;

int main(int argc, char *argv[])
{

  // check to see if the event class is in the dictionary
  // if it is not load the definition in libEvent.so
  if (!TClassTable::GetDict("Event")) {
    gSystem->Load("libCMOS");
  }

  string fname="test/prova.root";

  cout<<"reducing file: "<<fname<<" ."<<endl;
  
TFile *f =new TFile(fname.c_str());
cout<<"File"<<endl;
  TTree *CMOSDataTree = (TTree*) f->Get("CMOSDataTree");
cout<<"f.Get"<<endl;

  #ifdef DEBUG
  cout<<"Debug: f->GetObject(\"CMOSDataTree\",CMOSDataTree);"<<endl;
  #endif
  
Frame *frame = new Frame(480,640);
TBranch *bFrame = CMOSDataTree->GetBranch("frame");
cout<<"CMOSDataTree->GetBranch(\"frame\");"<<endl;
// //CMOSDataTree->SetBranchAddress("frame",&frame);
bFrame->SetAddress(&frame);
cout<<"bFrame->SetAddress(&frame);"<<endl;


//   #ifdef DEBUG
//   cout<<"Debug: CMOSDataTree->SetBranchAddress(\"frame\",&frame);"<<endl;
//   #endif

  //  Long64_t nentries = CMOSDataTree->GetEntriesFast();
  Long64_t nentries = CMOSDataTree->GetEntries();

#ifdef DEBUG
  cout<<"Debug: CMOSDataTree->GetEntriesFast(): "<<nentries <<endl;
#endif
  
   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++)
     {
       // delete frame;
       // frame = NULL;
#ifdef DEBUG
       cout<<"Debug:  starting loop"<<endl;
       cout.flush();
#endif       
       //       Long64_t ientry = CMOSDataTree->LoadTree(jentry);
//  #ifdef DEBUG
//        cout<<"Debug:   CMOSDataTree->LoadTree(jentry): "<<ientry <<endl;
//        cout.flush();
// #endif
//       if (ientry < 0) break;
       nb = CMOSDataTree->GetEntry(jentry);   nbytes += nb;
#ifdef DEBUG
       cout<<"Debug:  CMOSDataTree->GetEntry(jentry): "<<nb <<endl;
       cout.flush();
#endif
  
       cout<<"frame id "<<frame->GetId()<<endl;
       cout<<"frame row "<<frame->GetNRow()<<endl;
       SeedList *prova= frame->FindSeeds(60);
       
       // seed_list = frame->FindSeeds(thres,fiducialSideDim,seedSide,localMaximumCheckSide);
       // cout<<"seed list size: "<<seed_list->Size()<<endl;
       // //       ReducedDataTree->Fill();
       // seed_list->Clear();

     }
//    // TFile* outfile=new TFile(fname.c_str(),"RECREATE");
//    // ReducedDataTree->Write();
//    // outfile->Write();
//    // outfile->Close();
   return(1);
}
