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
int Riduzione(string fname,double thres, string pedfname, string noisefname, size_t fiducialSideDim=3,  const size_t seedSide=7, const size_t localMaximumCheckSide=3, string outfname="reduced.root",  int FrameNCol=488, int FrameNRow=648);


int main(int argc, char *argv[])
{
	cout<<argv[0]<< " 2.0"<<endl;
	cout<<"Last edit:   Jan 1, 2018 by collamaf+amorusor+alunni (during Y campaign)"<<endl;
	cout<<"Compiled at: "<< __DATE__ <<", "<< __TIME__<<"."<<endl;
	
	string execname=argv[1];  //Salvo il nome del file che voglio ridurre
	std::string sourcename=execname.substr(0,execname.size()-5);  //tolgo ".root" alla fine per usare il nome file in seguito
	
	//	cout<<"from source file: "<<execname<<endl;
	//	cout<<"edited last time at: "<<getFileCreationTime(execname)<<endl;
	
	double thres=2;
	size_t seedSide=7;
	size_t fiducialEdge=3;
	size_t localMaximumCheckSide=3;
	string outfname=Form("%s_Reduced.root",sourcename.c_str());
	string inputfname="";
	string pedfname="";
	int FrameNCol = 488, FrameNRow = 648;
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
						std::cout<<"Error: the frame size has to have two numbers and has to be passed as 648x488."<<std::endl;
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
	std::string badfname=fname.substr(0,fname.size()-5);
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
	{ //Carica il Frame di piedistallo
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
//		std::cerr<<"WARNING: The noise file is mandatory."<<endl;
		std::cout<<"WARNING: You are running without noise file!! All noises will be assumed to be 1!!!"<<endl;

		//      exit(-1);
	}
	else
	{ //Carica il Frame di noise
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
	
	//Creo l'oggetto frame con i dati
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
	
	
	double NClusterTot=0;  //numero totale di cluster ottenuti in tutti i frame
	double pixel[FrameNCol][FrameNRow];  //matrice che contiene le volte che ogni pixel ha superato la soglia
	int ww,qq;
	//Dynamic arrays cannot be initialized in declaration, so we do it explicitly
	for ( ww=0; ww<FrameNCol; ww++ ) {
		for (qq=0; qq<FrameNRow;qq++) {
			pixel[ww][qq]=0;
		}
	}
	
	
	Long64_t nbytes = 0, nb = 0;
	for (Long64_t jentry=0; jentry<nentries;jentry++)       // CICLO PRINCIPALE SUI FRAME
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
		
		// Frame per frame sottraggo il piedistallo e dividio per noise
		/*
		double signal=frame->At(170,384);
		double noiseVal= noise->At(170,384);
		double pedVal= pedestal->At(170,384);
		 */
		if(subtractPed)
		{
			
			frame->Subtract(*pedestal);

		}
		
		if(divideNoise)
		{
			frame->Divide(*noise);
			//cout<<"DEBUG"<<endl;
		}
		//double post=frame->At(170,384);
		//if (post>7) cout<<"########## ped= "<<pedVal<< ", noise= "<<noiseVal<<", signal= "<<signal<<", post= "<<post<<endl;
		
		// Alla fine di questa procedura la mia matrice non e' piu espressa in ADC ma in numero di sigma di differenza rispetto al piedistallo
		
		
		/////////////////////////////////////////Soglia///////////////////////////////////////////////////////
		/*
		 double thresN[488][648]={};
		 for(int i=0;i<488;i++){
		 for(int j=0;j<648;j++){
		 thresN[i][j] = 5;
		 }
		 }
		 */
		seed_list = frame->FindSeeds(thres,fiducialSideDim,seedSide,localMaximumCheckSide);
		
		
		// Ho creato la lista dei seeds sopra soglia
		
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		
		int Row_seed = 0;
		int Col_seed = 0;
		
		for(int i=0; i<seed_list.Size(); i++){  //Creo una mappa delle volte che ciascun pixel si è acceso, girando su tutti i seed della lista
			Seed ts = seed_list.At(i);
			Row_seed = ts.GetRow();
			Col_seed = ts.GetCol();
//			cout<<"############ col" <<Col_seed<<" "<<Row_seed<<endl;
			pixel[Col_seed][Row_seed] += 1;
			//			cout<<"i= "<<i<<endl;
		}
		NClusterTot += seed_list.Size();              //Conto i cluster totali in tutta l'acquisizione (sono nel ciclo sui vari frame)
		
		//       seed_list = *seed_listP;
		//cout<<seed_list.Size()<<endl;
		//frame->GetId()<<" "<<"seed list size: "<<seed_list.Size()<<endl;
		ReducedDataTree->Fill();
		seed_list.Clear();
		//       seed_listP = NULL;
		
	}                                                      // FINE CICLO PRINCIPALE SUI FRAME
	cout<<"Prima di controllare i badpixel ho trovato Ncluster= " <<NClusterTot<<endl;

	//	double Ncluster = mediaClusterPerFrame;
	double MediaPixelTotAcq=NClusterTot/(FrameNCol*FrameNRow);          //numero medio di volte che un pixel si è acceso durante l'intera acquisizione
	//	mediaClusterPerFrame = mediaClusterPerFrame/(FrameNCol*FrameNRow);  //media di cluster ottenuti per frame
	
	int somma=0;
	//	std::ofstream BadPixelFile("badpixel.txt",std::ios::out);
	std::ofstream BadPixelFile(Form("%s_badpixel.txt",badfname.c_str()),std::ios::out);
	
	double MeanEvalMethodThr=3;
	double GaussStatThr=7;  //to be checked
	///////////////////////////////////Selezione bad pixels con la gaussiana/////////////////////////////
	//scorro la matrice di occupancy che mi ero creato per confrontare il numero di volte che ciascun pixel si è acceso con la media
	if(MediaPixelTotAcq >= MeanEvalMethodThr){ //Luisa had "2", Stefano 3
		cout<<"I am using Gauss"<<endl;
		for(int j=0;j<FrameNCol;j++){
			for(int k=0;k<FrameNRow;k++){
				if(pixel[j][k] > MediaPixelTotAcq+GaussStatThr*sqrt(MediaPixelTotAcq)){   //se il pixel ha suonato molto piu della media lo scrivo nel file e aggiorno il contatore di badpixel e riduco il contatore di cluster del numero di volte che il badpixel aveva suonato
					// if(pixel[j][k] >= 5){
					BadPixelFile<<j<<" "<<k<<", val "<<pixel[j][k]<<endl;
					somma += 1;
					NClusterTot -= pixel[j][k];
				}
			}
		}
	}
	
	///////////////////////////////////Selezione bad pixels con Poisson//////////////////////////////
	//scorro la matrice di occupancy che mi ero creato per confrontare il numero di volte che ciascun pixel si è acceso con la media
	if(MediaPixelTotAcq < MeanEvalMethodThr){ //Luisa had "2", Stefano 3
		cout<<"I am using Poisson"<<endl;

		long double epsilon=2.559625088e-12; //Luisa had "0.0001", Stefano "0.0000001" - 1e-7 corrisponde a 6 sigma, ora mettiamo questo valore che corrispodne a 7 sigma: cout<<std::setprecision(10)<<TMath::Prob(49,1)<<endl;
		int n=0;
		double prob=0,fattoriale;
		
		for(int j=0;j<FrameNCol;j++){
			for(int k=0;k<FrameNRow;k++){
				for(n=0;n<pixel[j][k]/*&&pixel[j][k]<3*MediaPixelTotAcq*/;n++){
					fattoriale = 1;
					for(int i=n;i>1;i--){
						fattoriale *= i;
					}
					prob += pow(MediaPixelTotAcq,n)*exp(-MediaPixelTotAcq)/fattoriale;
//					if (j==154 && k==299) cout<<"OOOOOOOOO prob= "<<std::setprecision(10)<<std::setw(10)<<prob<<endl;
				}
				if ((1-prob) < epsilon){
					BadPixelFile<<j<<" "<<k<<" "<<prob<<" "<<pixel[j][k]<<endl;
//					if (j==154 && k==299) cout<<"OOOOOOOOO pixel val = "<<pixel[j][k]<<endl;
					somma += 1;
					NClusterTot -= pixel[j][k];
				}
				prob=0;
			}
		}
	}
	
	
	BadPixelFile.close();
	cout<<"Media: "<<MediaPixelTotAcq<<endl<<"# bad-pixels: "<<somma<<endl<<"# cluster: "<<NClusterTot<<endl;
	
	ReducedDataTree->Write();
	outfile->Write();
	outfile->Close();
	return(NClusterTot);
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
	cout<<"Option : -frameSize (set the frame sizes, default: 648x488)"<<endl;
	cout<<"Option : -help     (show this help)"<<endl;
	//    printf("       : -log (Log filename)\n");
	cout<<endl;
	
	return;
}
