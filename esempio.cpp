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

void print_help(string fname="executable");


int main(int argc, char *argv[])
{
  cout<<argv[0]<< " 1.0"<<endl;
  //cout<<"Last edit:   May 4 2016."<<endl;
  //cout<<"Compiled at: "<< __DATE__ <<", "<< __TIME__<<"."<<endl;
  //cout<<"DEBUG1"<<endl;
  string execname=argv[0];
  //cout<<"DEBUG2"<<endl;
  string inputfname="";
  //cout<<"DEBUG3"<<endl;
  string outfname="analized.root";
  //cout<<"DEBUG4"<<endl;

  if(argc==1)
    { //cout<<"DEBUG5"<<endl;
      print_help(argv[0]);
      exit(0); 
    }
  else
    { //cout<<"DEBUG6"<<endl;
      for(int i=1;i<argc;i++)
	if(argv[i][0] =='-')
	  {
	    string option(argv[i]);
	    cout<<"option: "<<i<<" "<<option<<endl;
	    if(option.compare("-help")==0)
	      { //cout<<"DEBUG7"<<endl;
		print_help(argv[0]);
		exit(0); 
	      }
	    else if(option.compare("-o")==0)
	      { //cout<<"DEBUG8"<<endl;
		outfname=argv[++i];
	      }
	    else
	      { //cout<<"DEBUG9"<<endl;
		cout<<"option not recognized: "<<argv[i];
		exit(-1);
	      }
	  }
	else
	  { //cout<<"DEBUG10"<<endl;
	    inputfname=argv[i];
	    cout<<"reducing file: "<<argv[i]<<" ."<<endl;
	    
	  }
    }
  SeedList *seed_list=new SeedList();
  const string fname(inputfname);
  TFile f(fname.c_str(),"READ");
  TTree *ReducedDataTree=(TTree*) f.Get("CMOSReducedData");
  TBranch *bSeedList = ReducedDataTree->GetBranch("seed_list");
  bSeedList->SetAddress(&seed_list);

  Long64_t nentries = ReducedDataTree->GetEntries();

  cout<<"nentries: "<<nentries<<endl;

  Long64_t nbytes = 0, nb = 0;
  //cout<<"DEBUG11"<<endl;
  Analisi analisi(outfname);
  //cout<<"DEBUG12"<<endl;
   for (Long64_t jentry=0; jentry<nentries;jentry++)
     {
       //cout<<"GetEntry("<<jentry<<")"<<endl;
       // cout.flush();
       nb = ReducedDataTree->GetEntry(jentry);   nbytes += nb;
       // std::cout<<seed_list->GetIdFrame()<<std::endl;
       // cout.flush();
       analisi.AnalisiData(seed_list);
     }
   analisi.WriteOnFile();
}

void print_help(string fname)
{
  cout<<"Source: "<<__FILE__<<endl;
  cout<<endl;
  cout<<"Usage  : "<<fname<<" (option) inputfile"<<endl;
  cout<<"Option : -o  (set output filename, default: analized.root)"<<endl;
  cout<<"Option : -help     (show this help)"<<endl;
  cout<<endl;

  return;
}
