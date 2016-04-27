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

void print_help(string fname="executable");
int Riduzione(string fname,double thres, size_t fiducialSideDim=3,  const size_t seedSide=7, const size_t localMaximumCheckSide=3, string outfname="reduced.root");

int main(int argc, char *argv[])
{
  cout<<argv[0]<< " 1.0"<<endl;
  cout<<"Last edit:   Apr 27 2016."<<endl;
  cout<<"Compiled at: "<< __DATE__ <<", "<< __TIME__<<"."<<endl;
  
  string execname=argv[0];
  // execname.pop_back();
  // string sourcename=execname+"C";
  std::string sourcename=execname.substr(0,execname.size()-1)+"C";
  cout<<"from source file: "<<sourcename<<endl;
  cout<<"edited last time at: "<<getFileCreationTime(sourcename)<<endl;

  double thres=2.0;
  size_t seedSide=7;
  size_t fiducialEdge=3;
  size_t localMaximumCheckSide=3;
  string outfname="reduced.root";
  
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
	      }
	    else if(option.compare("-seedSize")==0)
	      {
		thres=atoi(argv[++i]);
	      }
	     else if(option.compare("-checkLocalMaximumSide")==0)
	      {
		localMaximumCheckSide=atoi(argv[++i]);
	      }
	     else if(option.compare("-edge")==0)
	      {
		fiducialEdge=atoi(argv[++i]);
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
	    filenames.push_back(argv[i]);
	    cout<<"adding file: "<<argv[i]<<" to the input file list."<<endl;
	  }
    }
  return  Riduzione( fname, thres,  fiducialSideDim,  seedSide,  localMaximumCheckSide,  outfname);
}


int Riduzione(string fname,double thres, size_t fiducialSideDim,  const size_t seedSide, const size_t localMaximumCheckSide, string outfname)
{
  TFile *f=new TFile(fname.c_str(),"READ");
  TTree *CMOSDataTree;
  f->GetObject("CMOSDataTree",CMOSDataTree);

  Frame *frame;
  CMOSDataTree->SetBranchAddress("frame",&frame);

  TTree *ReducedDataTree=new TTree("CMOSReducedData","CMOS exp reduced data");
  SeedList* seed_list=new SeedList();
  ReducedDataTree->Branch("seed_list",&seed_list);
  
  Long64_t nentries = CMOSDataTree->GetEntriesFast();

   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++)
     {
       seed_list->Clear();
       Long64_t ientry = CMOSDataTree->LoadTree(jentry);
       if (ientry < 0) break;
       nb = CMOSDataTree->GetEntry(jentry);   nbytes += nb;
       
       seed_list = frame->FindSeeds(thres,fiducialSideDim,seedSize,localMaximumCheckSide);
       
       cout<<"frame id "<<frame->GetId()<<endl;
       ReducedDataTree->Fill();
     }
   TFile* outfile=new TFile(fname.c_str(),"RECREATE");
   ReducedDataTree->Write();
   outfile->Write();
   outfile->Close();
   return(1);
}


void print_help(string fname)
{
  cout<<"Source: "<<__FILE__<<endl;
  cout<<endl;
  cout<<"Usage  : "<<fname<<" (option) <evtfile>"<<endl;
  cout<<"Option : -verbose  (show debug output)"<<endl;
  cout<<"Option : -o  (set output filename, by default is the name of the first input)"<<endl;
  cout<<"Option : -v  (version of the file 38: "<<endl
      <<"\t 1) for the old simulation;"<<endl
      <<"\t 2) for the output with mass and momentum of primaries;"<<endl
      <<"\t 3) reads also the primaries energy entering the probe; "<<endl
      <<"\t 4) with flag for the creation region)"<<endl;
  cout<<"Option : -help     (show this help)"<<endl;
  //    printf("       : -log (Log filename)\n"); 
  cout<<endl;

  return;
}
