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
#include "TChain.h"
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
    cout<<"Last edit:   May 4 2016."<<endl;
    cout<<"Compiled at: "<< __DATE__ <<", "<< __TIME__<<"."<<endl;
    
    string execname=argv[0];
    std::vector<std::string> filenames;
    string outfname="analized.root";
    Double_t fcal = 1;
    int Ncluster = 0;
    
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
                else if(option.compare("-c")==0)
                {
                    fcal=atof(argv[++i]);
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
                //cout<<"reducing file: "<<argv[i]<<" ."<<endl;
                
            }
    }
    SeedList *seed_list=new SeedList();
    //  const string fname(inputfname);
    // TFile f(fname.c_str(),"READ");
    TChain* ReducedDataTree=new TChain("CMOSReducedData");
    for(size_t i =0;i<filenames.size();i++)
    {
        int check_add=0;
        std::string filename=filenames[i];
        check_add = ReducedDataTree->AddFile(filename.c_str());
        cout<<"adding "<<filename<<" to analysis "<<check_add<<" chain size: "<<ReducedDataTree->GetNtrees()<<endl;
    }
    // TTree *ReducedDataTree=(TTree*) f.Get("CMOSReducedData");
    
    TBranch *bSeedList = ReducedDataTree->GetBranch("seed_list");
    bSeedList->SetAddress(&seed_list);
    
    Long64_t nentries = ReducedDataTree->GetEntries();
    
    cout<<"nentries: "<<nentries<<endl;
    
    Long64_t nbytes = 0, nb = 0;
    Analisi analisi(outfname,fcal);
    
    for (Long64_t jentry=0; jentry<nentries;jentry++)
    {
        //cout<<"GetEntry("<<jentry<<")"<<endl;
        // cout.flush();
        nb = ReducedDataTree->GetEntry(jentry);   nbytes += nb;
        //std::cout<<"Frame_"<<seed_list->GetIdFrame()<<std::endl;
        // cout.flush();
        //Ncluster += analisi.AnalisiData(seed_list, fcal);
        //cout<<Ncluster<<endl;
        Ncluster += analisi.AnalisiData(seed_list, fcal);
    }
    analisi.WriteOnFile();
    //cout<<Ncluster<<endl;
    
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
