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
int Analisi (SeedList* sl, string outfname);
SeedList* Riduzione(string fname,string fnamebkg,double thres, size_t fiducialSideDim=3,  const size_t seedSide=7, const size_t localMaximumCheckSide=3);

int main(int argc, char *argv[]){
  
  cout<<argv[0]<< " 1.0"<<endl;
  cout<<"Last edit:   Apr 28 2016."<<endl;
  cout<<"Compiled at: "<< __DATE__ <<", "<< __TIME__<<"."<<endl;
  
  string execname=argv[0];
  
  double thres=2.0;
  size_t seedSide=7;
  size_t fiducialEdge=3;
  size_t localMaximumCheckSide=3;
  // string outfname="reduced.root";
  string inputfname="";
  string inputfnamebkg="";

    
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
	    /*else if(option.compare("-o")==0)
	      {
		outfname=argv[++i];
		}*/
	    else if(option.compare("-b")==0)
	      {
		inputfnamebkg=argv[++i];
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
  
  SeedList* sl=  Riduzione( inputfname, inputfnamebkg, thres,  fiducialEdge,  seedSide,  localMaximumCheckSide);
  
} 

SeedList* Riduzione(string fname,string fnamebkg,double thres, size_t fiducialSideDim,  const size_t seedSide, const size_t localMaximumCheckSide) {
  
  
  Frame fr(480,640);
  fr.ReadFile(fname);
  Frame bkg(480,640);
  bkg.ReadFile(fnamebkg);
    
  fr.Subtract(bkg);
  
  //  SeedList sl=fr.FindSeeds(3.36,3,3);
  SeedList *sl=fr.FindSeeds(thres);
  cout<<"seeds.size(): "<<sl->Size()<<endl;
  
  for(size_t i=0; i<sl->Size(); i++)
    {
      Seed ts=sl->At(i);
      //  cout<<" evento 0 scritto il seed "<<i<<" row "<< ts.GetRow() <<" col "<<ts.GetCol();
      // cout << std::fixed << std::setprecision(2) << std::setfill('0');
      // cout<<" "<<ts(0,0)<<endl;
      
    }
  return(sl);
}

int Analisi (SeedList* sl , string outfname)
{
  //  TH2F *map_seed = new TH2F ("mapofseed","seedmap",nbinx,xbinmin,xbinmax,nbiny,ybinmin,ybinmax );
  
  for(size_t i=0; i<sl->Size(); i++)
    {
      Seed ts = sl->At(i);
      int Row_seed = ts.GetRow();
      int Col_seed = ts.GetCol();
      //map_seed->Fill(Row_seed,Col_seed);

      double adccountsCenter=ts(0,0);
      
      for(int j=ts.GetPixelMin(); j<ts.GetPixelMax(); j++)
	{
	  for(int i=ts.GetPixelMin(); i<ts.GetPixelMax(); i++)
	    {
	      cout<<i<<" "<<j<<" "<< ts(i,j) << " ";
	    }
	  cout << std::endl;
	}
    }
  
  return(1);     
  
}

void print_help(string fname)
{
  cout<<"Source: "<<__FILE__<<endl;
  cout<<endl;
  cout<<"Usage  : "<<fname<<" (option) inputfile"<<endl;
  cout<<"Option  : -b (set input background file)"<<endl;
  //  cout<<"Option : -o  (set output filename, default: reduced.root)"<<endl;
  cout<<"Option : -t  (set threshold, default: 2.0)"<<endl;
  cout<<"Option : -seedSize  (set the seeds side dimensions, default: 7)"<<endl;
  cout<<"Option : -checkLocalMaximumSide  (set the submatrix used to check local max, default: 3)"<<endl;
  cout<<"Option : -edge  (set the fiducial edge, default: 3)"<<endl;
  cout<<"Option : -help     (show this help)"<<endl;
  //    printf("       : -log (Log filename)\n"); 
  cout<<endl;
  
  return;
}
