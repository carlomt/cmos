#include <iostream>
#include <fstream>

#include "Frame.h"
#include "Seed.h"
#include "SeedList.h"

#include "TTree.h"
#include "TFile.h"

//#include "getFileCreationTime.h"

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
  // std::string sourcename=execname.substr(0,execname.size()-1)+"C";
  // cout<<"from source file: "<<sourcename<<endl;
  // cout<<"edited last time at: "<<getFileCreationTime(sourcename)<<endl;

  double thres=2.0;
  size_t seedSide=7;
  size_t fiducialEdge=3;
  size_t localMaximumCheckSide=3;
  string outfname="reduced.root";
  string inputfname="";

  
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
  return  Riduzione( inputfname, thres,  fiducialEdge,  seedSide,  localMaximumCheckSide,  outfname);
}


int Riduzione(string fname,double thres, size_t fiducialSideDim,  const size_t seedSide, const size_t localMaximumCheckSide, string outfname)
{
  cout<<"reducing file: "<<fname<<" ."<<endl;
  
  TFile f(fname.c_str(),"READ");
  TTree *CMOSDataTree = (TTree*) f.Get("CMOSDataTree");
  //  f->GetObject("CMOSDataTree",CMOSDataTree);

  #ifdef DEBUG
  cout<<"Debug: f->GetObject(\"CMOSDataTree\",CMOSDataTree);"<<endl;
  #endif
  
  Frame *frame = NULL;
  CMOSDataTree->SetBranchAddress("frame",&frame);

  #ifdef DEBUG
  cout<<"Debug: CMOSDataTree->SetBranchAddress(\"frame\",&frame);"<<endl;
  #endif
  
  // TTree *ReducedDataTree=new TTree("CMOSReducedData","CMOS exp reduced data");
  SeedList *seed_list;
  // ReducedDataTree->Branch("seed_list",&seed_list);
  
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
       
       //       seed_list = frame->FindSeeds(thres,fiducialSideDim,seedSide,localMaximumCheckSide);
       // cout<<"seed list size: "<<seed_list->Size()<<endl;
       // // ReducedDataTree->Fill();
       // seed_list->Clear();

     }
   // TFile* outfile=new TFile(fname.c_str(),"RECREATE");
   // ReducedDataTree->Write();
   // outfile->Write();
   // outfile->Close();
   return(1);
}


void print_help(string fname)
{
  cout<<"Source: "<<__FILE__<<endl;
  cout<<endl;
  cout<<"Usage  : "<<fname<<" (option) inputfile"<<endl;
  cout<<"Option : -o  (set output filename, default: reduced.root)"<<endl;
  cout<<"Option : -t  (set threshold, default: 2.0)"<<endl;
  cout<<"Option : -seedSize  (set the seeds side dimensions, default: 7)"<<endl;
  cout<<"Option : -checkLocalMaximumSide  (set the submatrix used to check local max, default: 3)"<<endl;
  cout<<"Option : -edge  (set the fiducial edge, default: 3)"<<endl;
  cout<<"Option : -help     (show this help)"<<endl;
  //    printf("       : -log (Log filename)\n"); 
  cout<<endl;

  return;
}
