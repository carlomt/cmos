#include <iostream>
#include <fstream>
#include <cstdlib>
#include <iomanip>

#include "Frame.h"
#include "Seed.h"
#include "SeedList.h"

#include "TROOT.h"
#include "TSystem.h"
#include "TTree.h"
#include "TFile.h"
#include "TClassTable.h"
#include "Analisi.h"

#include "string_parser.h"

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
  cout<<argv[0]<< " 2.0"<<endl;
  cout<<"Last edit:   Feb 25 2018, by amorusor"<<endl;
  cout<<"Compiled at: "<< __DATE__ <<", "<< __TIME__<<"."<<endl;
  
  string execname=argv[1];                                       //Salvo il nome del file che voglio analizzare
  std::string sourcename=execname.substr(0,execname.size()-13);  //tolgo "_Reduced.root" alla fine per usare il nome file in seguito
  
  cout<<"from source file: "<<execname<<endl;
  
  string inputfname="";
  string outfname=Form("%s_Analized.root", sourcename.c_str());
  string badfname=Form("%s_badpixel.txt", sourcename.c_str());
  
  int FrameNCol = 480, FrameNRow = 640;
  
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
	    else if(option.compare("-o")==0)
	      {
		outfname=argv[++i];
	      }
	    else if(option.compare("-frameSize")==0)
	      {
		std::vector<std::string> sizes = string_parser(argv[++i],"x");
		if(sizes.size()!=2)
		  {
		    std::cout<<"Error: the frame size has to have two numbers and has to be passed as 648x488."<<std::endl;
		    exit(-1);
		  }
		FrameNRow = stoi(sizes[0]);
		FrameNCol = stoi(sizes[1]);
		std::cout<<"setting the Frame size as: "<<FrameNRow<<"x"<<FrameNCol<<std::endl;
	      }
	    else
	      {
		cout<<"option not recognized: "<<argv[i];
		exit(-1);
	      }
	  }
	else
	  {
	    inputfname=argv[i];
	    cout<<"analyzing file: "<<argv[i]<<" ."<<endl;
	    
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
  Analisi analisi(outfname, badfname, FrameNCol, FrameNRow);

  for (Long64_t jentry=0; jentry<nentries; jentry++)              //Ciclo sui FRAME
    {
      //std::cout<<"Frame #"<<seed_list->GetIdFrame()<<std::endl;   //check
      //cout.flush();
      nb = ReducedDataTree->GetEntry(jentry);   nbytes += nb;
      analisi.AnalisiData(seed_list, FrameNCol, FrameNRow);       //qui avrò ciclo sui seed (vedi Analisi.C)
    }
  cout<<"output file name: "<<outfname<<endl;
  analisi.WriteOnFile();
}

void print_help(string fname)
{
  cout<<"Source: "<<__FILE__<<endl;
  cout<<endl;
  cout<<"Usage  : "<<fname<<" (option) inputfile"<<endl;
  cout<<"Option : -o  (set output filename)"<<endl;		     
  cout<<"Option : -frameSize (set the frame sizes, default: 640x480)"<<endl;
  cout<<"Option : -help     (show this help)"<<endl;
  cout<<endl;
  
  return;
}
