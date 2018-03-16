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
  cout<<"Last edit:   Mar 15 2018, by amorusor+collamaf"<<endl;
  cout<<"Compiled at: "<< __DATE__ <<", "<< __TIME__<<"."<<endl;
  
  string execname=argv[1];                                       //Salvo il nome del file che voglio analizzare
  std::string sourcename=execname.substr(0,execname.size()-13);  //tolgo "_Reduced.root" alla fine per usare il nome file in seguito
  
  cout<<"from source file: "<<execname<<endl;
  
  string inputfname="";
  string outfname=Form("%s_Analized.root", sourcename.c_str());
  string badfname=Form("%s_badpixel.txt", sourcename.c_str());
  
  int FrameNCol = 640, FrameNRow = 480;
  
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
  	
  SeedList *seed_list=new SeedList();                            //lista dei seed originale
  SeedList *mySeedList= new SeedList();                          //lista dei seed modificata(per ogni pix ho: valore*sigma)

  const string fname(inputfname);
  TFile f(fname.c_str(),"READ");
  TTree *ReducedDataTree=(TTree*) f.Get("CMOSReducedData");
  TBranch *bSeedList = ReducedDataTree->GetBranch("seed_list");
  bSeedList->SetAddress(&seed_list);

  //Frame *FramePed = (Frame*) f.Get("PedestalFrame");
  Frame *FrameNoise = (Frame*) f.Get("NoiseFrame");
  //  for (int qq=0; qq<100; qq++)
  //  cout<<" AOOOOOO "<<FramePed->At(100+qq,200-qq)<<endl;

  /*for(int i=0; i<FrameNRow; i++)
    {
      for(int j=0; j<FrameNCol; j++)
	{
	  double val=FrameNoise->At(j,i);
	  cout<<"Val: "<<val<<" Col "<<j<<" Row "<<i<<endl;
	}
	}*/
  
  Long64_t nentries = ReducedDataTree->GetEntries();
  cout<<"nentries: "<<nentries<<endl;

  Seed temp, temp2;
  double valPix=0, valPixNEW=0, sigma=0;
  int index=0;
  Long64_t nbytes = 0, nb = 0;
  Analisi analisi(outfname, badfname, FrameNCol, FrameNRow);
  for (Long64_t jentry=0; jentry<nentries; jentry++)                //INIZIO ciclo sui FRAME
    {
      nb = ReducedDataTree->GetEntry(jentry);   nbytes += nb;
      cout<<"######################\nFrame #"<<seed_list->GetIdFrame()<<endl;   //check
      //cout.flush();
      mySeedList->Clear();
      for(size_t i=0; i<seed_list->Size(); i++)                            //ciclo sui SEED della lista originale
	{
	  temp=seed_list->At(i);                                           //accedo agli elementi contenuti nella lista originale
	  cout<<"FRAME "<<seed_list->GetIdFrame()<<" Seed N "<<i<<"\n ######################\nLISTA originale:\n"<<temp<<endl;
	  int Row_seed=temp.GetRow();
	  int Col_seed=temp.GetCol();
	  //cout<<"\ncol "<<Col_seed<<" row "<<Row_seed<<endl;
	  sigma=0;
	  valPix=0;
	  valPixNEW=0;
	  index=0;
	  for (int k = temp.GetPixelMin(); k < temp.GetPixelMax(); k++)    //scorro i PIXEL associati al seed (ex. matrix 7x7=49 pix) in oriz. partendo da quello in alto a sx
	    {
	      for (int l = temp.GetPixelMin(); l < temp.GetPixelMax(); l++)
		{
		  sigma=0;
		  valPix=0;
		  valPixNEW=0;
		  index=0;
		  
		  index=temp.SetIndex(l,k);
		  valPix=temp(l,k);
		  cout<<"Pixel #"<<index<<" Val prima: "<<valPix;
		  //		  sigma=2.1;
		  const size_t colonna=Col_seed+l;
		  const size_t riga=Row_seed+k;
		  //sigma+=0.001;
		  //sigma=(FrameNoise->At(Col_seed+l,Row_seed+k));             //è +!!! perchè l e k sono anche negativi
		  sigma=FrameNoise->At(colonna, riga);             //è +!!! perchè l e k sono anche negativi
		  //		  cout<<"Pixel #"<<index<<" col "<<Col_seed+l<<" row "<<Row_seed+k<<" sigma "<<sigma<<endl;
		  cout<<", sigma: "<<sigma;
		  //		  cout<<", sigma: "<<(FrameNoise->At(Col_seed+l,Row_seed+k));
		  valPixNEW=valPix*sigma;                                  //modifico i valori registrati dai pixel
		  //		  valPixNEW=valPix*2;
		  //if (valPix==0) valPixNEW=17;
		  temp.SetVal(index, valPixNEW);
		  cout<<", dopo: "<<valPixNEW<<", getval "<<temp.GetVal(index)<<endl;
		}
	    }                                                              //Fine ciclo sui PIXEL
	  
	  cout<<" getval FUORi "<<temp.GetVal(index)<<endl;
	  

	  /*for(size_t j=0; j<temp.GetSize(); j++)                    
	    {
	    valPix=temp.GetVal(j);
	    cout<<"Pixel #"<<j<<" Val prima: "<<valPix;
	    valPixNEW=valPix*2;                                   
	    temp.SetVal(j, valPixNEW);                                  
	    cout<<", dopo: "<<valPixNEW<<endl;
	    }*/


			      
	  mySeedList->Add(temp);                                          //nuova lista di seed
	  temp2=mySeedList->At(i);
	  //cout<<"\nLISTA modificata t2:\n"<< VAAAL <<endl;
	  /*
	  for (int c=0; c<49; c++)
	    {
	      double VAAAL=temp2.GetVal(c);
	      cout<<"\nLISTA modificata t2:\n"<< VAAAL <<endl;
	    }
	  */
	  cout<<"\nLISTA modificata:\n"<<temp2<<endl;
	  if(temp.GetVal(48)!=temp2.GetVal(48))
	    {
	      cout<<"Frame #"<<seed_list->GetIdFrame()<<" Seed #"<<i<<"\n***ERRORE!!!!***\n"<<endl;
	    }
	}                                                                 //FINE ciclo sui SEED
      
      analisi.AnalisiData(mySeedList, FrameNCol, FrameNRow);
    }                                                                     //FINE ciclo sui FRAME
  cout<<"output file name: "<<outfname<<endl;
  analisi.WriteOnFile();
}

void print_help(string fname)
{
  cout<<"Source: "<<__FILE__<<endl;
  cout<<endl;
  cout<<"Usage  : "<<fname<<" (option) inputfile"<<endl;
  cout<<"Option : -o  (set output filename)"<<endl;		     
  cout<<"Option : -frameSize (set the frame sizes, default: 480x640)"<<endl;
  cout<<"Option : -help     (show this help)"<<endl;
  cout<<endl;
  
  return;
}
