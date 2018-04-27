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
	int debug=0;
	cout<<argv[0]<< " 2.0"<<endl;
	cout<<"Last edit:   Apr 27 2018, by collamaf"<<endl;
	cout<<"Compiled at: "<< __DATE__ <<", "<< __TIME__<<"."<<endl;
	
	string execname=argv[1];                                       //Salvo il nome del file che voglio analizzare
	std::string sourcename=execname.substr(0,execname.size()-13);  //tolgo "_Reduced.root" alla fine per usare il nome file in seguito
	
	cout<<"from source file: "<<execname<<endl;
	
	string inputfname="";
	string outfname=Form("%s_Analized.root", sourcename.c_str());
	string badfname=Form("%s_badpixel.txt", sourcename.c_str());
	
	int FrameNCol = 640, FrameNRow = 480;
	Double_t fcal = 150;                                             //fattore di calibrazione ADC-keV
	bool MCflag=kFALSE;
	
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
				else if(option.compare("-mc")==0)
				{
					MCflag=kTRUE;
				}		else if(option.compare("-frameSize")==0)
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
	
	std::ofstream FileAppoggio("DebugDataAna.txt",std::ios::out);
	
	
	SeedList *seed_list=new SeedList();                            //lista dei seed originale
	SeedList *mySeedList= new SeedList();                          //lista dei seed modificata(per ogni pix ho: valore*sigma)
	if (debug) FileAppoggio<<"Creo la mia SeedList, ora ne contiene: "<<mySeedList->Size()<<endl;
	
	SeedList *seed_listPrimEn=new SeedList();    //seed list for MC Primary particle energy

	
	
	const string fname(inputfname);
	TFile f(fname.c_str(),"READ");
	TTree *ReducedDataTree=(TTree*) f.Get("CMOSReducedData");
	TBranch *bSeedList = ReducedDataTree->GetBranch("seed_list");
	bSeedList->SetAddress(&seed_list);
	
	Frame *FramePed = (Frame*) f.Get("PedestalFrame");
	Frame *FrameNoise = (Frame*) f.Get("NoiseFrame");
	
	if (MCflag) {
		TBranch *bSeedListPrimEn = ReducedDataTree->GetBranch("seed_listPrimEn");
		bSeedListPrimEn->SetAddress(&seed_listPrimEn);
	}
	Long64_t nentries = ReducedDataTree->GetEntries();
	cout<<"nentries: "<<nentries<<endl;
	
	Seed  temp, temp2;
	double valPix=0, valPixNEW=0, sigma=0;
	int index=0;
	Long64_t nbytes = 0, nb = 0;
	int ErrorCounter=0;
	
	//Inizializzo la classe (Histo...)
	Analisi analisi(outfname, badfname, FrameNCol, FrameNRow, FramePed, FrameNoise, fcal);


	//Rimoltiplico per la sigma ogni pixel (la divisione era stata fatta in Riduzione), cosi ho di nuovo ADC!
	for (Long64_t jentry=0; jentry<nentries; jentry++)                //INIZIO ciclo sui FRAME
	{
		nb = ReducedDataTree->GetEntry(jentry);   nbytes += nb;
		if (debug) FileAppoggio<<endl<<"############################################\n######################\nFrame #"<<seed_list->GetIdFrame()<<" Numero seed= "<<seed_list->Size()<<"\n######################\n############################################\n"<<endl;   //check
																																																																																																																																			//cout.flush();
		mySeedList->Clear();
		if (debug) FileAppoggio<<"Svuoto la mia SeedList, ora ne contiene: "<<mySeedList->Size()<<endl;
		
		for(size_t SeedIndex=0; SeedIndex<seed_list->Size(); SeedIndex++)            //Dato un Frame, ciclo sui SEED della lista originale e mi copio il primo seed in un seed temporaneo che modificherò e inserirò poi nella mia nuova lista
		{
			temp=seed_list->At(SeedIndex);                                           //accedo agli elementi contenuti nella lista originale
			if (debug) FileAppoggio<<"#################################################################################### FRAME "<<seed_list->GetIdFrame()<<" Seed N "<<SeedIndex<<" \nLISTA originale:\n"<<temp<<endl;
			
			if (debug) FileAppoggio<<"Mi copio in temp il seed num "<<SeedIndex<<" ha n elementi: "<<temp.GetSeedSize()<<endl;
			
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
					
					index=temp.GetIndex(l,k);
					valPix=temp(l,k);
					if (debug) FileAppoggio<<"Pixel #"<<index<<" Val prima: "<<valPix;
					sigma=(FrameNoise->At(Col_seed+l,Row_seed+k));
					//sigma=2; //for test
					//cout<<"Pixel #"<<index<<" col "<<Col_seed+l<<" row "<<Row_seed+k<<" sigma "<<sigma<<endl;
					valPixNEW=valPix*sigma;                                  //modifico i valori registrati dai pixel
					temp.SetVal(index, valPixNEW);
					if (debug) {
						FileAppoggio<<",\tsigma: "<<sigma;
						FileAppoggio<<", dopo: "<<temp.GetVal(index)<<endl;
					}
				}
			} //Fine ciclo sui PIXEL
			if (debug) {
				FileAppoggio<<endl;
				FileAppoggio<<"Bene, ho creato il seed temp: : "<<endl<<temp<<endl;
				FileAppoggio<<"E lo aggiungo alla mia SeedList, prima ne conteneva: "<<mySeedList->Size();
			}
			
			mySeedList->Add(temp);                                          //nuova lista di seed
			if (debug) FileAppoggio<<", ora ne contiene: "<<mySeedList->Size()<<endl;
			temp2=mySeedList->At(mySeedList->Size()-1);
			
			if(debug&& temp.GetVal(48)!=temp2.GetVal(48))
			{
				ErrorCounter++;
				if (debug) FileAppoggio<<"Frame #"<<seed_list->GetIdFrame()<<" Seed #"<<SeedIndex<<"\n***ERRORE!!!!***\n"<<endl;
			}
		}//FINE ciclo sui SEED
		
		//Ho finito di rimoltiplicare per sigma: ora ho una nuova seedlist (mySeedList) che contiene i valori in ADC, e la posso passare ad AnalisiData per l'analisi vera e propria!
		
		//cout<<"################\nSTART\n################\n"<<"Frame #"<<seed_list->GetIdFrame()<<endl;
		if (MCflag)
		analisi.AnalisiData(mySeedList, FrameNCol, FrameNRow, FramePed, FrameNoise, fcal, seed_listPrimEn);
		else analisi.AnalisiData(mySeedList, FrameNCol, FrameNRow, FramePed, FrameNoise, fcal);
	}//FINE ciclo sui FRAME
	
	cout<<"output file name: "<<outfname<<endl;
	analisi.WriteOnFile();
	cout<<"Numero di errori= "<<ErrorCounter<<endl;
}

void print_help(string fname)
{
	cout<<"Source: "<<__FILE__<<endl;
	cout<<endl;
	cout<<"Usage  : "<<fname<<" (option) inputfile"<<endl;
	cout<<"Option : -o  (set output filename)"<<endl;
	cout<<"Option : -mc  (set the MC flag to carry on primary particle energy info)"<<endl;
	cout<<"Option : -frameSize (set the frame sizes, default: 480x640)"<<endl;
	cout<<"Option : -help     (show this help)"<<endl;
	cout<<endl;
	
	return;
}
