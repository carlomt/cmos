
//###############################################
//################## Macro to analyze .root files
//################## produced by GEANT4
//################## simulation of CMOS
//################## version 2.0 - 2018-01-18 for Y90 campaign
//################## Collamaf
//###############################################
//##### Scopo della macro:
// L'output della simulazione GEANT4 è scritto evento per evento, mentre i dati sono scritti frame per frame. Conoscendo l'attività totale della sorgente è possibile ricostruire una struttura di frame anche per il MC



#define AnaCmosMC_cxx
#include "AnaCmosMC.h"
#include <TH2.h>
#include <TH1.h>
#include <TStyle.h>
#include <TCanvas.h>
#include "Frame.h"
#include "Frame.C"
#include "Seed.C"
#include "SeedList.C"


void AnaCmosMC::Loop()
{
	
	if (fChain == 0) return;
	
	Long64_t nentries = fChain->GetEntriesFast();
	
	int SensorChoice=2; //1 is MT9V011, 2 is MT9V115
	
	int NPixX=480, NPixY=640;
	if (SensorChoice==2) {
		NPixX=488;
		NPixY=648;
	}
	int NPixTot=NPixX*NPixY;
//	int NPrimMC=1188000;
	int NPrimMC=3600000;
	double AttSorg=5.94e3; //Bq
	double DTacq=200e-3; //Data Acquisition Time [s]
	int NFramesMC=0, NFramesMCLow=0;
	int FrameCounter=0;
	int RedFactor=1;
	bool LowFlag=kFALSE;
	
	Long64_t nbytes = 0, nb = 0;
	
	FrameFile->cd();
	TTree *MCTree = new TTree("CMOSDataTree","CMOS Monte Carlo sim");
	Frame *frame = new Frame(NPixX,NPixY); //~480x640
	MCTree->Branch("frame",&frame);
	
	cout<<"Created output file: "<<FrameFile->GetName()<<endl;
	cout<<"Created output file: "<<ImageFile->GetName()<<endl;
	
	NFramesMC=NPrimMC/AttSorg/0.2;  //is the number of "frame-equivalents" in the MC run
	cout<<"#####################################"<<endl;
	cout<<"Number of Primaries Generated in MC: "<<NPrimMC<<endl;
	cout<<"Source Activity [Bq]= "<<AttSorg<<endl;
	cout<<"Entries = "<<nentries<<endl;
	cout<<"It's like I'have simulated "<<NPrimMC/AttSorg<<" [s] of data taking, equivalent to about "<<NFramesMC <<" frames of 200 ms"<<endl;
	
	if (nentries/NFramesMC == 0) {
		
		LowFlag=kTRUE;
		NFramesMCLow=(NPrimMC/AttSorg*NFramesMC/nentries/0.2);
		cout<<"ATTENTION! NUMBER OF EVENT TOO LOW FOR FRAME DIVISION, assuming 1 per frame and adding extra empty frames!"<<endl;
		
	}
	
	ImageFile->cd();
	cluster=new TH2F("cluster","cluster", NPixX/RedFactor, 0, NPixX, NPixY/RedFactor, 0, NPixY);
	double temp=0;
	
	TH2F* ClusterVec[NFramesMC];
	ClusterVec[0]=new TH2F(Form("ClusterVec%d",0),"ClusterVec", NPixX/RedFactor, 0, NPixX, NPixY/RedFactor, 0, NPixY);


	
	//############################################
	//########## LOOP SU TUTTI GLI EVENTI
	//#######
	for (Long64_t jentry=0; jentry<nentries;jentry++) {
		Long64_t ientry = LoadTree(jentry);
		if (ientry < 0) break;
		nb = fChain->GetEntry(jentry);   nbytes += nb;
		
		//	if (jentry==0) NPrimMC=Nev;
		
		temp+=EDepInPixel->size();
//		cout<<"MERDA "<<EDepInPixel->size()<<" Somma= "<<temp<<endl;
		
		for (int ii=0; ii<PixelID->size(); ii++){  //loop su tutti i pixel
			
			int xpix=((int)(PixelID->at(ii)))%640;
			int ypix=(PixelID->at(ii)/640);
			//			frame->SetAdd(xpix, ypix, (EDepInPixel->at(ii))*1000);
			frame->SetAdd(xpix, ypix, (int)(EDepInPixel->at(ii)*1000*ConvFactor));
			//			cout<<"evento: "<<jentry<<"px: "<<PixelID->at(ii)<<", colonna= "<<xpix%640<<", x= "<<PixXPos->at(ii)<<", riga= "<<ypix<<", y= "<<PixYPos->at(ii)<<", ene= "<<EDepInPixel->at(ii)<<endl;
			//			cout<<"evento: "<<jentry<<", ene= "<<EDepInPixel->at(ii)*1000<<" [keV], = "<<(int)(EDepInPixel->at(ii)*1000*ConvFactor)<<" [ADC]"<<endl;
			cluster->Fill(xpix, ypix);
//			cout<<"Xpix= "<<xpix;
//			cout<<", Ypix= "<<ypix<<endl;
			
			ClusterVec[FrameCounter]->Fill(xpix, ypix);
//			ClusterVec[FrameCounter]->SetBinContent(xpix, ypix, 1);
		}
		//			cout<<"jentries: "<<jentry<<", nentries: "<<nentries<<", NFramesMC= "<<NFramesMC<<", nentries/NFramesMC= "<<nentries/NFramesMC<<", jentry%(nentries/NFramesMC)= "<<jentry%(nentries/NFramesMC)<<endl;
		if (LowFlag || jentry%(nentries/NFramesMC+1)==0 ) {
			frame->SetId(FrameCounter);
			ImageFile->cd();
			ClusterVec[FrameCounter]->Write();
//			FrameFile->cd();
			FrameCounter++;
			ClusterVec[FrameCounter]=new TH2F(Form("ClusterVec%d",FrameCounter),"ClusterVec", NPixX/RedFactor, 0, NPixX, NPixY/RedFactor, 0, NPixY);
			MCTree->Fill();
			frame->Clear();
		}
	}
	
	//#######
	//########## FINE LOOP SU TUTTI GLI EVENTI
	//############################################
	
	if (FrameCounter<NFramesMC && !LowFlag) {
		cout<<"Now I have to add some empty frames since NFramesMC= "<<NFramesMC<<" while  FrameCounter= "<<FrameCounter<<endl;
		for (int jj=FrameCounter; jj<NFramesMC; jj++){
			frame->SetId(FrameCounter);
			FrameCounter++;
			MCTree->Fill();
			frame->Clear();
			if (0) cout<<"jj: "<<jj<<", FrameCounter= "<<FrameCounter<<endl;
		}
	}
	
	if (LowFlag) {
		for (int jj=nentries; jj<NFramesMC; jj++){
			frame->SetId(FrameCounter);
			FrameCounter++;
			MCTree->Fill();
			frame->Clear();
			if (nentries==NFramesMC-1) cout<<"jj: "<<jj<<", FrameCounter= "<<FrameCounter<<endl;
		}
	}
	
	FrameFile->Write();
	ImageFile->cd();
	cluster->Write();
	FrameFile->Close();
	ImageFile->Close();
}
