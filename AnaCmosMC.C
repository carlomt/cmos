
//###############################################
//################## Macro to analyze .root files
//################## produced by GEANT4
//################## simulation of CMOS
//################## version 2.0 - 2018-01-18 for Y90 campaign
//################## Collamaf
//###############################################
//##### Scopo della macro:
// L'output della simulazione GEANT4 è scritto evento per evento, mentre i dati sono scritti frame per frame. Conoscendo l'attività totale della sorgente è possibile ricostruire una struttura di frame anche per il MC
//### Produce in output:
// - un file XXX_Frame_NNN.root con la struttura di frame, da mandare alla stessa procedura di analisi dei dati
// - un file XXX_Image.root, che contiene un TH2F per ogni frame con il segnale di quel frame, piu un TH2F globale con la sovrapposizone di tutti i segnali di tutti i frame
// USE:
/*
.L AnaCmosMC.C
c=new AnaCmosMC("/Users/francesco/MonteCarlo/Sonda/SimCMOS/build/CMOSmcX0_Z2_NOCuD_Fil0_TBR10_DOTA_115")
c->Loop()
*/

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
#include <iostream>

void AnaCmosMC::Loop()
{
	
	if (fChain == 0) return;
	
	Long64_t nentries = fChain->GetEntriesFast();
	
	int SensorChoice=2; //1 is MT9V011, 2 is MT9V115
	fChain->GetEntry(0); //carico una entries per poter prendere i valori dal file MC

	int NPixX=480, NPixY=640;
	if (SensorChoice==2) {
		NPixX=488;
		NPixY=648;
	}
	int NPixTot=NPixX*NPixY;
	//	int NPrimMC=1188000;
	int NPrimMC=Nev;
	double AttSorg=26.33e3; //Total activity of source [Bq] (was 5.94e3 maybe for Sr PG source)
	double DTacq=200e-3; //Data Acquisition Time [s]
	int NFramesMC=0, NFramesMCLow=0;
	int FrameCounter=0;
	int RedFactor=1, kk=0;
	bool LowFlag=kFALSE;
	bool debug=kFALSE;
	
	Long64_t nbytes = 0, nb = 0;
	
	FrameFile->cd();
	TTree *MCTree = new TTree("CMOSDataTree","CMOS Monte Carlo sim");
	Frame *frame = new Frame(NPixX,NPixY); //~480x640
	MCTree->Branch("frame",&frame);
	
	cout<<"Created output file: "<<FrameFile->GetName()<<endl;
	cout<<"Created output file: "<<ImageFile->GetName()<<endl;
	
	NFramesMC=NPrimMC/AttSorg/DTacq;  //is the number of "frame-equivalents" in the MC run
	cout<<"#####################################"<<endl;
	cout<<std::scientific<<"Number of Primaries Generated in MC: "<<(double)NPrimMC<<std::fixed<<endl;
	cout<<"Source Activity [Bq]= "<<AttSorg<<endl;
	cout<<"Entries = "<<nentries<<endl; //is the number of events in which I have a signal
	cout<<"It's like I'have simulated "<<NPrimMC/AttSorg<<" [s] of data taking, equivalent to about "<<NFramesMC <<" frames of 200 ms"<<endl;
	cout<<"I will create a frame every "<<(int) nentries/NFramesMC<<" events, and append in case some empty ones" <<endl;
	
	if (nentries/NFramesMC == 0) {
		LowFlag=kTRUE;
		NFramesMCLow=(NPrimMC/AttSorg*NFramesMC/nentries/DTacq);
		cout<<"ATTENTION! NUMBER OF EVENT TOO LOW FOR FRAME DIVISION, assuming 1 per frame and adding extra empty frames!"<<endl;
	}
	
	ImageFile->cd();
	TH2F* cluster =new TH2F("cluster","cluster", NPixX/RedFactor, 0, NPixX, NPixY/RedFactor, 0, NPixY);
//	double temp=0;

	int ContaPixel[NFramesMC];

	TH2F* ClusterVec[NFramesMC];
	for (kk=0; kk<NFramesMC; kk++){
	ClusterVec[kk]=new TH2F(Form("ClusterVec%d",kk),Form("Cluster for frame n%d", kk), NPixX/RedFactor, 0, NPixX, NPixY/RedFactor, 0, NPixY);
		ContaPixel[kk]=0;
	}
	//	ClusterVec[0]->Dump();
	kk=0;
	//############################################
	//########## LOOP SU TUTTI GLI EVENTI
	//#######
	for (Long64_t jentry=0; jentry<nentries&&1;jentry++) { //Ciclo su tutti gli eventi MC (cioè Isotopo priario per Isotopo primario )
		Long64_t ientry = LoadTree(jentry);
		if (ientry < 0) break;
		nb = fChain->GetEntry(jentry);   nbytes += nb;
		if (debug) cout<<endl;
//		temp+=EDepInPixel->size();
		ContaPixel[kk]+=PixelID->size(); //conto di quanti pixel accesi è fatto ciascun frame
		//		cout<<"MERDA "<<EDepInPixel->size()<<" Somma= "<<temp<<endl;
		for (int ii=0; ii<PixelID->size(); ii++){  //loop su tutti i pixel accesi durante quell'evento

			//calcolo gli indici I e J per il pixel considerato: in questo modo vanno da 0 a NCol/NRow -1 come vuole Frame
			int Ipix=((int)(PixelID->at(ii))-1)%NPixX;
			int Jpix=((PixelID->at(ii)-1)/NPixX);
			
			//			frame->SetAdd(Ipix, Jpix, (EDepInPixel->at(ii))*1000);
			frame->SetAdd(Ipix, Jpix, (int)(EDepInPixel->at(ii)*1000*ConvFactor));
			
			if (debug) cout<<"evento: "<<jentry<<", px: "<<PixelID->at(ii)<<", colonna= "<<Ipix<<", x= "<<PixXPos->at(ii)<<", riga= "<<Jpix<<", y= "<<PixYPos->at(ii)<<", ene= "<<EDepInPixel->at(ii)<<", eneConv= "<<(int)(EDepInPixel->at(ii)*1000*ConvFactor)<<endl;
			//			cout<<"evento: "<<jentry<<", ene= "<<EDepInPixel->at(ii)*1000<<" [keV], = "<<(int)(EDepInPixel->at(ii)*1000*ConvFactor)<<" [ADC]"<<endl;
			cluster->Fill(Ipix, Jpix);
			//			cout<<"Ipix= "<<Ipix;
			//			cout<<", Jpix= "<<Jpix<<endl;
			
			ClusterVec[FrameCounter]->Fill(Ipix, Jpix);
			//			ClusterVec[FrameCounter]->SetBinContent(Ipix, Jpix, 1);
		}
		//			cout<<"jentries: "<<jentry<<", nentries: "<<nentries<<", NFramesMC= "<<NFramesMC<<", nentries/NFramesMC= "<<nentries/NFramesMC<<", jentry%(nentries/NFramesMC)= "<<jentry%(nentries/NFramesMC)<<endl;
		if (LowFlag || jentry%(nentries/NFramesMC+1)==0 ) {
			if (FrameCounter%100==0) cout<<"Saving Frame Number: "<<FrameCounter<<endl;
			frame->SetId(FrameCounter);
			ImageFile->cd();
			ClusterVec[FrameCounter]->Write();
			//			FrameFile->cd();
			FrameCounter++;
//			ClusterVec[FrameCounter]=new TH2F(Form("ClusterVec%d",FrameCounter),"ClusterVec", NPixX/RedFactor, 0, NPixX, NPixY/RedFactor, 0, NPixY);
			MCTree->Fill();
			frame->Clear();
			kk++; //contatore per il contatore dei pixel nel frame
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
	
	
	for (kk=0; kk<NFramesMC; kk++){
		if (1) cout<<"k= "<<kk<<", Counter "<<ContaPixel[kk]<<endl;;
	}
	FrameFile->Write();
	ImageFile->cd();
	cluster->Write();
	FrameFile->Close();
	ImageFile->Close();
}
