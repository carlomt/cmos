#include <iostream>
#include <fstream>
#include <map>
                
#include <stdlib.h>
#include <math.h>
#include <sstream>

#include "TROOT.h"
#include <TApplication.h>
#include "TSystem.h"
#include "TStyle.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TH1D.h"
#include "TCanvas.h"
#include "TTree.h"
#include "TNtuple.h"
#include "TLegend.h"
#include "TFile.h"
#include "TCut.h"
#include "TCutG.h"
#include "TMath.h"

#include "Frame.h"
#include "Seed.h"
#include "SeedList.h"

#include "myStoi.h"
//#include "getFileCreationTime.h"

using namespace std;

int FileConverter(string filename,int mesglevel,int version,string outfname);
int FileConverter(vector<string> filenames,int mesglevel,int version,string outfname);
void UnreadColumnsChecker(istringstream &iss, int counter);
void print_help(string fname="executable");
string CheckOutputExtension(const string fname);

int main(int argc, char *argv[])
{
  cout<<argv[0]<< " 1.1"<<endl;
  cout<<"Last edit:   Jun 15 2017."<<endl;
  cout<<"Compiled at: "<< __DATE__ <<", "<< __TIME__<<"."<<endl;
  
  string execname=argv[0];
  // execname.pop_back();
  // string sourcename=execname+"C";
  std::string sourcename=execname.substr(0,execname.size()-1)+"C";
  cout<<"from source file: "<<sourcename<<endl;
  //cout<<"edited last time at: "<<getFileCreationTime(sourcename)<<endl;
  
  int success=-1;
  int result=1;
  int mesglevel=0;
  int version=1;
  string outfname="";
  vector<string> filenames;

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
	    else if(option.compare("-verbose")==0)
	      {
		mesglevel=1; 
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

  
  success= FileConverter(filenames,mesglevel,version,outfname);
  if(success<0)
    {
      result=-1;
    }
  return result;
}

int FileConverter(string filename, int mesglevel,int version,string outfname)
{
  vector<string> filenames;
  filenames.push_back(filename);
  return FileConverter(filenames,mesglevel,version,outfname);
}

int FileConverter(vector<string> filenames, int mesglevel,int version,string outfnameI)
{
  if(outfnameI.compare("")==0)
    {
      outfnameI=filenames[0];
    }
  const string outfname=CheckOutputExtension(outfnameI);
  cout<<"output file name: "<<outfname<<endl;
  
  TFile *hfile = new TFile(outfname.c_str(),"RECREATE","Example");
  
  TTree *DataTree = new TTree("CMOSDataTree","CMOS exp data");
  Frame *frame = new Frame(480,640);
  DataTree->Branch("frame",&frame);  
  
  
  for(size_t iFile=0; iFile<filenames.size(); iFile++)
    {
      const string originalfilename=filenames[iFile];
      cout<<"Converting file: "<<originalfilename<<endl;
      string filename=filenames[iFile];
      size_t pos;
      string s = filename;
      string token;
      const string delimiter="_";
      while ((pos = s.find(delimiter)) != std::string::npos) 
	{
	  token = s.substr(0, pos);
	  #ifdef DEBUG
	  std::cout << token << std::endl;
	  #endif
	  s.erase(0, pos + delimiter.length());
	}
      cout<<"s: "<<s<<endl;
      pos = s.find(".");

      string frameids = s.erase(pos,s.length());
      frame->SetId(myStoi(frameids));
      string filename_ext = filename.erase(0, filename.find("."));
      
      if(filename_ext.compare(".txt")!=0 && filename_ext.compare(".raw")!=0)
	{
	  cout<<"WARNING: this program is made to read txt or binary raw files."<<endl;
	}
#ifdef DEBUG
      cout<<"Debug: prima di frame->ReadFile(originalfilename);"<<endl;
      cout.flush();
#endif
      int counter=0;
      if(filename_ext.compare(".txt")==0)
	{
	  counter = frame->ReadFile(originalfilename);
	}
      else 
	{
	  counter = frame->ReadFile(originalfilename, true);
	}
      cout<<counter<<" values read"<<endl;
#ifdef DEBUG
      cout<<"Debug: dopo di frame->ReadFile(originalfilename);"<<endl;
      cout.flush();
#endif
      // if(iFile==0)
      // 	{
      // 	  hfile = new TFile(outfname.c_str(),"RECREATE","Example");
      // 	}
      // if(!hfile->IsOpen())
      // 	{
      // 	  cout<<"Unable to open output file "<<outfname<<endl;
      // 	  return(-1);
      // 	}
#ifdef DEBUG
      cout<<"Filling"<<endl;
      for(size_t j=0; j<480; j++)
	{
	  for(size_t i=0; i<640; i++)
	    {
	      if(frame->At(i,j)>=157)
		{
		  cout<<"frame->At("<<i<<","<<j<<") "<<frame->At(i,j)<<endl;
		}
	    }
	}
#endif

      DataTree->Fill();

#ifdef DEBUG
      cout<<"Clearing"<<endl;
#endif
      frame->Clear();
      
    }//end loop on file names
#ifdef DEBUG
  cout<<"Printing DataTree"<<endl;
  cout.flush();
#endif
  DataTree->Print();
#ifdef DEBUG
  cout<<"Writing DataTree"<<endl;
  cout.flush();
#endif
  DataTree->Write();
#ifdef DEBUG
  cout<<"Writing hfile"<<endl;
  cout.flush();
#endif
  hfile->Write();
#ifdef DEBUG
  cout<<"Closing hfile"<<endl;
  cout.flush();
#endif
  hfile->Close();

  //#ifdef DEBUG

  //#endif

  return(1);
  
}



void print_help(string fname)
{
  cout<<"Source: "<<__FILE__<<endl;
  cout<<endl;
  cout<<"Usage  : "<<fname<<" (option) <evtfile>"<<endl;
  cout<<"Option : -verbose  (show debug output)"<<endl;
  cout<<"Option : -o  (set output filename, by default is the name of the first input)"<<endl;
  cout<<"Option : -help     (show this help)"<<endl;
  //    printf("       : -log (Log filename)\n"); 
  cout<<endl;

  return;
}

void UnreadColumnsChecker(istringstream &iss, int counter)
{
  int unread_columns=0;
  int last_unread;
  while(iss.good())
    {
      iss>>last_unread;
      unread_columns++;
    }
  unread_columns--;
  if(unread_columns>0)
    {
      cout<<"Warning: in the line "<<counter<<" there are "<<unread_columns<<" more columns that the program didn't read. Check the code."<<endl;
      cout<<"The last line number in this line is: "<<last_unread<<endl;
    }
}



string CheckOutputExtension(const string fname)
{
  string OutFile;
  string s = fname;
  size_t pos = 0;
  string token;
  bool atleastone=false;
  const string delimiter=".";
  while ((pos = s.find(delimiter)) != std::string::npos) 
    {
      token = s.substr(0, pos);
      //      std::cout << token << std::endl;
      s.erase(0, pos + delimiter.length());
      atleastone=true;
    }
  if(s.compare("root")==0)
    {
      OutFile=fname;
    }
  else
    {
      //      std::cout<<"Warning: you passed a file with extension "<<s<<", I will remove it and use .root "<<std::endl;
      if(atleastone)
	{
	  OutFile=token+".root";
	}
      else
	{
	  OutFile=fname+".root";
	}
    }
  return OutFile;
}
