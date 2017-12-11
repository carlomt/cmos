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

#include "string_parser.h"
//#define DEBUG

//#include "getFileCreationTime.h"

using std::cout;
using std::endl;
using std::ifstream;
using std::string;
using std::atoi;
using std::atof;

void print_help(string fname="executable");
int Riduzione(string fname,double thres, string pedfname, string noisefname, size_t fiducialSideDim=3,  const size_t seedSide=7, const size_t localMaximumCheckSide=3, string outfname="reduced.root",  int FrameNCol=480, int FrameNRow=640);


int main(int argc, char *argv[])
{
    cout<<argv[0]<< " 1.0"<<endl;
    cout<<"Last edit:   Dec 7, 2017 by collamaf (unification)"<<endl;
    cout<<"Compiled at: "<< __DATE__ <<", "<< __TIME__<<"."<<endl;
    
    string execname=argv[0];
    // execname.pop_back();
    // string sourcename=execname+"C";
    // std::string sourcename=execname.substr(0,execname.size()-1)+"C";
    // cout<<"from source file: "<<sourcename<<endl;
    // cout<<"edited last time at: "<<getFileCreationTime(sourcename)<<endl;
    
    double thres=2;
    size_t seedSide=7;
    size_t fiducialEdge=3;
    size_t localMaximumCheckSide=3;
    string outfname="reduced.root";
    string inputfname="";
    string pedfname="";
    int FrameNCol = 480, FrameNRow = 640;
    string noisefname="";
    
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
                
                else if(option.compare("-noise")==0)
                {
                    noisefname=argv[++i];
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
                        std::cout<<"Error: the frame size has to have two numbers and has to be passed as 640x480."<<std::endl;
                        exit(-1);
                    }
                    FrameNRow = stoi(sizes[0]);
                    FrameNCol = stoi(sizes[1]);
                    std::cout<<"Setting the Frame size as: "<<FrameNRow<<"x"<<FrameNCol<<std::endl;
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
    
    return  Riduzione( inputfname, thres, pedfname, noisefname, fiducialEdge,  seedSide,  localMaximumCheckSide,  outfname, FrameNCol, FrameNRow);
    
    
}


int Riduzione(string fname,double thres, string pedfname, string noisefname, size_t fiducialSideDim,  const size_t seedSide, const size_t localMaximumCheckSide, string outfname, int FrameNCol, int FrameNRow)
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
    bool divideNoise=false;
    Frame *pedestal = NULL;
    Frame *noise = NULL;
    if(pedfname.compare("")==0)
    {
        std::cerr<<"WARNING: The pedestal file is mandatory."<<endl;
        //      exit(-1);
    }
    else
    {
        TFile pedf(pedfname.c_str(),"READ");
        TTree *CMOSDataTreePed = (TTree*) pedf.Get("CMOSDataTree");
        CMOSDataTreePed->SetName("CMOSDataTreePed");
        Long64_t nentriesPed = CMOSDataTreePed->GetEntries();
        if(nentriesPed>1)
        {
            std::cerr<<"WARNING: the pedestal file "<<pedfname<<" has more than one entry ("<<nentriesPed<<endl;
        }
        pedestal = new Frame(FrameNCol, FrameNRow);
        TBranch *bFramePed = CMOSDataTreePed->GetBranch("frame");
        bFramePed->SetAddress(&pedestal);
        CMOSDataTreePed->GetEntry(0);
        
        bFramePed=NULL;
        CMOSDataTreePed=NULL;
        pedf.Close();
        subtractPed=true;
    }
    
    if(noisefname.compare("")==0)
    {
        std::cerr<<"WARNING: The noise file is mandatory."<<endl;
        //      exit(-1);
    }
    else
    {
        TFile noisef( noisefname.c_str(),"READ");
        TTree *CMOSDataTreeNoise = (TTree*) noisef.Get("CMOSDataTree");
        CMOSDataTreeNoise->SetName("CMOSDataTreeNoise");
        Long64_t nentriesNoise = CMOSDataTreeNoise->GetEntries();
        if(nentriesNoise>1)
        {
            std::cerr<<"WARNING: the noise file "<<noisefname<<" has more than one entry ("<<nentriesNoise<<endl;
        }
        noise = new Frame(FrameNCol, FrameNRow);
        TBranch *bFrameNoise = CMOSDataTreeNoise->GetBranch("frame");
        bFrameNoise->SetAddress(&noise);
        CMOSDataTreeNoise->GetEntry(0);
        
        bFrameNoise=NULL;
        CMOSDataTreeNoise=NULL;
        noisef.Close();
        divideNoise=true;
    }
    
    cout<<"reducing file: "<<fname<<" ."<<endl;
    
    TFile f(fname.c_str(),"READ");
    TTree *CMOSDataTree = (TTree*) f.Get("CMOSDataTree");
    //  f->GetObject("CMOSDataTree",CMOSDataTree);
    
#ifdef DEBUG
    cout<<"Debug: f->GetObject(\"CMOSDataTree\",CMOSDataTree);"<<endl;
#endif
    
    Frame *frame = new Frame(FrameNCol, FrameNRow);
    //  CMOSDataTree->SetBranchAddress("frame",&frame);
    
    TBranch *bFrame = CMOSDataTree->GetBranch("frame");
    bFrame->SetAddress(&frame);
    
    TBranch *nFrame = CMOSDataTree->GetBranch("frame");
    nFrame->SetAddress(&frame);
    
#ifdef DEBUG
    cout<<"Debug: CMOSDataTree->SetBranchAddress(\"frame\",&frame);"<<endl;
#endif
    
    TFile* outfile=new TFile(outfname.c_str(),"RECREATE");
    TTree *ReducedDataTree=new TTree("CMOSReducedData","CMOS exp reduced data");
    SeedList seed_list;
    ReducedDataTree->Branch("seed_list",&seed_list);
    //  SeedList *seed_listP;
    
    //  Long64_t nentries = CMOSDataTree->GetEntriesFast();
    Long64_t nentries = CMOSDataTree->GetEntries();
    
#ifdef DEBUG
    cout<<"Debug: CMOSDataTree->GetEntriesFast(): "<<nentries <<endl;
#endif
    
    double media=0;
    double pixel[FrameNCol][FrameNRow];
    int ww,qq;
    //Dynamic arrays cannot be initialized in declaration
    for ( ww=0; ww<FrameNCol; ww++ ) {
      for (qq=0; qq<FrameNRow;qq++) {
	pixel[ww][qq]=0;
      }
    }
    
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
        
        if(divideNoise)
        {
            frame->Divide(*noise);
            //cout<<"DEBUG"<<endl;
        }
        
        
        /////////////////////////////////////////Soglia///////////////////////////////////////////////////////
        /*
         double thresN[480][640]={};
         for(int i=0;i<480;i++){
         for(int j=0;j<640;j++){
         thresN[i][j] = 5;
         }
         }
         */
        seed_list = frame->FindSeeds(thres,fiducialSideDim,seedSide,localMaximumCheckSide);
        
        //////////////////////////////////////////////////////////////////////////////////////////////////////
        
        int Row_seed = 0;
        int Col_seed = 0;
        
        
        for(int i=0; i<seed_list.Size(); i++){
            Seed ts = seed_list.At(i);
            Row_seed = ts.GetRow();
            Col_seed = ts.GetCol();
            pixel[Row_seed][Col_seed] += 1;
//			cout<<"i= "<<i<<endl;
        }
//        	cout<<"MERDA !"<<endl;
        media += seed_list.Size();
        
        //       seed_list = *seed_listP;
        //cout<<seed_list.Size()<<endl;
        //frame->GetId()<<" "<<"seed list size: "<<seed_list.Size()<<endl;
        ReducedDataTree->Fill();
        seed_list.Clear();
        //       seed_listP = NULL;

    }
    
    double Ncluster = media;
    media = media/(FrameNCol*FrameNRow);
    
    int somma=0;
    std::ofstream out1("badpixel.txt",std::ios::out);

    ///////////////////////////////////Selezione bad pixels con la media/////////////////////////////
    
    if(media >= 3){ //Luisa had "2"
        for(int j=0;j<FrameNCol;j++){
            for(int k=0;k<FrameNRow;k++){
                if(pixel[j][k] > media+7*sqrt(media)){
                    // if(pixel[j][k] >= 5){
                    out1<<j<<" "<<k<<endl;
                    somma += 1;
                    Ncluster -= pixel[j][k];
                }
            }
		}
	}
	
	///////////////////////////////////Selezione bad pixels con Poisson//////////////////////////////
	
	if(media < 3){ //Luisa had "2"
		long double epsilon=0.0000001; //Luisa had "0.0001", Stefano "0.0000001"
		int n=0;
		double prob=0,fattoriale;
		
		
		
		for(int j=0;j<FrameNCol;j++){
			for(int k=0;k<FrameNRow;k++){
				for(n=0;n<pixel[j][k]/*&&pixel[j][k]<3*media*/;n++){
					fattoriale = 1;
					for(int i=n;i>1;i--){
						fattoriale *= i;
					}
					prob += pow(media,n)*exp(-media)/fattoriale;
				}
				if ((1-prob) < epsilon){
					out1<<j<<" "<<k<<" "<<prob<<" "<<pixel[j][k]<<endl;
					somma += 1;
					Ncluster -= pixel[j][k];
				}
				prob=0;
			}
		}
	}
	

		
		//########################
		
#if 0
		for(int j=0;j<FrameNCol;j++){
			for(int k=0;k<FrameNRow;k++){
				prob=1;
				if(pixel[j][k]<3*media) { //added with Ric
					for(n=0;n<pixel[j][k];n++){
						fattoriale = 1;
						for(int i=n;i>1;i--){
							fattoriale *= i;
							//							cout<<"n="<< n<<", i="<< i<<" fattoriale= "<<fattoriale<<endl;
						}
						prob += pow(media,n)*exp(-media)/fattoriale;
						//					cout<<"media= "<<media<<endl;
						
					}
				}
				if ((1-prob) < epsilon){
					out1<<j<<" "<<k<<" "<<prob<<" "<<pixel[j][k]<<endl;
					somma += 1;
					Ncluster -= pixel[j][k];
					//					cout<<"MERDA dentro!"<<endl;
				}
				prob=0;
			}
		}
	}
#endif
//	cout<<"MERDA fuori!"<<endl;

    out1.close();
    cout<<"Media: "<<media<<endl<<"# bad-pixels: "<<somma<<endl<<"# cluster: "<<Ncluster<<endl;
    
    ReducedDataTree->Write();
    outfile->Write();
    outfile->Close();
    return(Ncluster);
}

void print_help(string fname)
{
    cout<<"Source: "<<__FILE__<<endl;
    cout<<endl;
    cout<<"Usage  : "<<fname<<" (option) inputfile"<<endl;
    cout<<"Option : -o  (set output filename, default: reduced.root)"<<endl;
    cout<<"Option : -t  (set threshold, default: 2.0)"<<endl;
    cout<<"Option : -ped  (set the pedestal file)"<<endl;
    cout<<"Option : -noise  (set the noise file)"<<endl;
    cout<<"Option : -seedSize  (set the seeds side dimensions, default: 7)"<<endl;
    cout<<"Option : -checkLocalMaximumSide  (set the submatrix used to check local max, default: 3)"<<endl;
    cout<<"Option : -edge  (set the fiducial edge, default: 3)"<<endl;
    cout<<"Option : -frameSize (set the frame sizes, default: 640x480)"<<endl;
    cout<<"Option : -help     (show this help)"<<endl;
    //    printf("       : -log (Log filename)\n");
    cout<<endl;
    
    return;
}
