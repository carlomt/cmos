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

void print_help(string fname="executable");
int Riduzione(string fname,double thres, string pedfname, size_t fiducialSideDim=3,  const size_t seedSide=7, const size_t localMaximumCheckSide=3, string outfname="reduced.root");

int main(int argc, char *argv[])
{
  cout<<argv[0]<< " 1.0"<<endl;
  cout<<"Last edit:   Apr 27 2016."<<endl;
  cout<<"Compiled at: "<< __DATE__ <<", "<< __TIME__<<"."<<endl;
  
  string execname=argv[0];
  // execname.pop_back();
  // string sourcename=execname+"C";
  // std::string sourcename=execname.substr(0,execname.size()-1)+"C";
  // cout<<"from source file: "<<sourcename<<endl;
  // cout<<"edited last time at: "<<getFileCreationTime(sourcename)<<endl;

  double thres=2.0;
  size_t seedSide=7;
  size_t fiducialEdge=3;
  size_t localMaximumCheckSide=3;
  string outfname="reduced.root";
  string inputfname="";
  string pedfname="";
  
  if(argc==1)
    {
      print_help(argv[0]);
      exit(0); 
    }
  else
    {
      for(int i=1;i<argc;i++)
	if(argv[i][0] =='-')
	  {
	    string option(argv[i]);
	    cout<<"option: "<<i<<" "<<option<<endl;
	    if(option.compare("-help")==0)
	      {
		print_help(argv[0]);
		exit(0); 
	      }
	    else if(option.compare("-t")==0)
	      {
		thres=atof(argv[++i]);
#ifdef DEBUG
		cout<<"thres: "<<thres<<endl;
#endif
	      }
	    else if(option.compare("-seedSize")==0)
	      {
		seedSide=atoi(argv[++i]);
	      }
	     else if(option.compare("-checkLocalMaximumSide")==0)
	      {
		localMaximumCheckSide=atoi(argv[++i]);
	      }
	     else if(option.compare("-edge")==0)
	      {
		fiducialEdge=atoi(argv[++i]);
	      }
	     else if(option.compare("-ped")==0)
	      {
		pedfname=argv[++i];
	      }
	    else if(option.compare("-o")==0)
	      {
		outfname=argv[++i];
	      }
	    else
	      {
		cout<<"option not recognized: "<<argv[i];
		exit(-1);
	      }
	  }
	else
	  {
	    inputfname=string(argv[i]);
	    //cout<<"reducing file: "<<argv[i]<<" ."<<endl;
		    
	  }
    }
  return  Riduzione( inputfname, thres, pedfname, fiducialEdge,  seedSide,  localMaximumCheckSide,  outfname);
}


int Riduzione(string fname,double thres, string pedfname, size_t fiducialSideDim,  const size_t seedSide, const size_t localMaximumCheckSide, string outfname)
{

  // check to see if the event class is in the dictionary
  // if it is not load the definition in libEvent.so
  // if (!TClassTable::GetDict("Frame")) 
  //   {
  //     string library=string(CMOSCODEDIR)+"/libCMOS.so";
  //     cout<<"Loading library "<<library<<endl;
  //     gSystem->Load(library.c_str());
  //   }
  bool subtractPed=false;
  Frame *pedestal = NULL;
  if(pedfname.compare("")==0)
    {
      std::cerr<<"WARNING: The pedestal file is mandatory."<<endl;
      //      exit(-1);
    }
  else
    {
      TFile pedf( pedfname.c_str(),"READ");
      TTree *CMOSDataTreePed = (TTree*) pedf.Get("CMOSDataTree");
      CMOSDataTreePed->SetName("CMOSDataTreePed");
      Long64_t nentriesPed = CMOSDataTreePed->GetEntries();
      if(nentriesPed>1)
	{
	  std::cerr<<"WARNING: the pedestal file "<<pedfname<<" has more than one entry ("<<nentriesPed<<endl;
	}
      pedestal = new Frame(480,640);
      TBranch *bFramePed = CMOSDataTreePed->GetBranch("frame");
      bFramePed->SetAddress(&pedestal);
      CMOSDataTreePed->GetEntry(0);

      bFramePed=NULL;
      CMOSDataTreePed=NULL;
      pedf.Close();
      subtractPed=true;
    }
  cout<<"reducing file: "<<fname<<" ."<<endl;
  
  TFile f(fname.c_str(),"READ");
  TTree *CMOSDataTree = (TTree*) f.Get("CMOSDataTree");
  //  f->GetObject("CMOSDataTree",CMOSDataTree);

  #ifdef DEBUG
  cout<<"Debug: f->GetObject(\"CMOSDataTree\",CMOSDataTree);"<<endl;
  #endif
  
  Frame *frame = new Frame(480,640);
  //  CMOSDataTree->SetBranchAddress("frame",&frame);

  TBranch *bFrame = CMOSDataTree->GetBranch("frame");
  bFrame->SetAddress(&frame);

  #ifdef DEBUG
  cout<<"Debug: CMOSDataTree->SetBranchAddress(\"frame\",&frame);"<<endl;
  #endif
  
  TTree *ReducedDataTree=new TTree("CMOSReducedData","CMOS exp reduced data");
  SeedList seed_list;
  ReducedDataTree->Branch("seed_list",&seed_list);
  //  SeedList *seed_listP;

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
       cout<<"jentry: "<<jentry<<endl;
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
       cout<<"jentry: "<<jentry<<endl;
       cout.flush();
       cout<<"frame id "<<frame->GetId()<<endl;
       //       cout<<"frame row "<<frame->GetNRow()<<endl;
       //       SeedList prova= frame->FindSeeds(60);
       #endif
  
       if(subtractPed)
	 {
	   frame->Subtract(*pedestal);
	 }
       seed_list = frame->FindSeeds(thres,fiducialSideDim,seedSide,localMaximumCheckSide);
       //       seed_list = *seed_listP;
       cout<<"seed list size: "<<seed_list.Size()<<endl;
       // ReducedDataTree->Fill();
       seed_list.Clear();
       //       seed_listP = NULL;
     }
   TFile* outfile=new TFile(outfname.c_str(),"RECREATE");
   ReducedDataTree->Write();
   outfile->Write();
   outfile->Close();
   return(1);
}


void print_help(string fname)
{
  cout<<"Source: "<<__FILE__<<endl;
  cout<<endl;
  cout<<"Usage  : "<<fname<<" (option) inputfile"<<endl;
  cout<<"Option : -o  (set output filename, default: reduced.root)"<<endl;
  cout<<"Option : -t  (set threshold, default: 2.0)"<<endl;
  cout<<"Option : -ped  (set the pedestal file)"<<endl;
  cout<<"Option : -seedSize  (set the seeds side dimensions, default: 7)"<<endl;
  cout<<"Option : -checkLocalMaximumSide  (set the submatrix used to check local max, default: 3)"<<endl;
  cout<<"Option : -edge  (set the fiducial edge, default: 3)"<<endl;
  cout<<"Option : -help     (show this help)"<<endl;
  //    printf("       : -log (Log filename)\n"); 
  cout<<endl;

  return;
}
