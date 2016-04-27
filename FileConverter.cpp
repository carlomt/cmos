#include <iostream>
#include <fstream>
#include <map>
                
#include <stdlib.h>
#include <math.h>
#include <sstream>

#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>

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

using namespace std;

int FileConverter(string filename,int mesglevel,int version,string outfname);
int FileConverter(vector<string> filenames,int mesglevel,int version,string outfname);
void UnreadColumnsChecker(istringstream &iss, int counter);
void print_help(string fname="executable");
string getFileCreationTime(string path);
string CheckOutputExtension(const string fname);

int main(int argc, char *argv[])
{
  cout<<argv[0]<< " 1.0"<<endl;
  cout<<"Last edit:   Apr 26 2016."<<endl;
  cout<<"Compiled at: "<< __DATE__ <<", "<< __TIME__<<"."<<endl;
  
  string execname=argv[0];
  // execname.pop_back();
  // string sourcename=execname+"C";
  std::string sourcename=execname.substr(0,execname.size()-1)+"C";
  cout<<"from source file: "<<sourcename<<endl;
  cout<<"edited last time at: "<<getFileCreationTime(sourcename)<<endl;
  
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
	    else if(option.compare("-v")==0)
	      {
		version=atoi(argv[++i]);
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
  
  TFile *hfile;
  
  TTree *DataTree = new TTree("CMOSDataTree","CMOS exp data");
  Frame *frame = new Frame(480,640);
  DataTree->Branch("frame",&frame);  
  
  
  for(size_t iFile=0; iFile<filenames.size(); iFile++)
    {
      frame->Clear();
      
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
	  //	  std::cout << token << std::endl;
	  s.erase(0, pos + delimiter.length());
	}
      cout<<"s: "<<s<<endl;
      pos = s.find(".");

      string frameids = s.erase(pos,s.length());
      frame->SetId(myStoi(frameids));
      string filename_ext = filename.erase(0, filename.find("."));
      
      if(filename_ext.compare(".txt")!=0 )
	{
	  cout<<"WARNING: this program is made to read txt files"<<endl;
	}
      frame->ReadFile(originalfilename);
      if(iFile==0)
	{
	  hfile = new TFile(outfname.c_str(),"RECREATE","Example");
	}
      if(!hfile->IsOpen())
	{
	  cout<<"Unable to open output file "<<outfname<<endl;
	  return(-1);
	}

      DataTree->Fill();
    }//end loop on file names
  
  DataTree->Write();
  hfile->Write();
  hfile->Close();
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

string getFileCreationTime(string path) {
    struct stat attr;
    stat(path.c_str(), &attr);
    return ctime(&attr.st_mtime);
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