
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
// - un file XXX_Frame_NNN.root con la struttura di frame, da mandare alla stessa procedura di analisi dei dati (NNN è 10x fattore di conversione ADC-KeV)
// - un file XXX_Image.root, che contiene un TH2F per ogni frame con il segnale di quel frame, piu un TH2F globale con la sovrapposizone di tutti i segnali di tutti i frame, da usare come "Event Display"

//#### Latest update (2018-01-25): Now adding Noise to MC!
// The program requires a second filename for the PostPed file containing the vector with all noises for each pixel. Use the one corresponding to the experimetnal run to compare the simulation with!
// Worflow:
// First of all, the program reads the number of events generated in the MC. Knowing the emissivity of the source (Bq) at the time of the experiment it computes the time equivalent of MC and thus the number of frames it has to fill. Than it divides the available entries using a Poissonian generation of the number of events per frame.
// For each frame, the program loops on each active pixel and computes the total energy deposition in it. When reached the amount of events to be stored in that frame it converts the total energy depisiton in the pixel from keV to ADC, and adds a gaussian smearing with mean 0 and sigma the noise for that pixel. At the end the program adds some empty frame to reach the exact frame number for that time of acquisition.
// If no Noise file is provided no subtraction is done
// USE:
/*
 .L AnaCmosMC.C
 c=new AnaCmosMC("/Users/francesco/MonteCarlo/Sonda/SimCMOS/build/CMOSmcX0_Z2_NOCuD_Fil0_TBR10_DOTA_115", "XXX_PostPed", int verbose=0)
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
#include <TStopwatch.h>

void AnaCmosMC::Loop()
{
	
	if (fChain == 0) return;
	
	Long64_t nentries = fChain->GetEntriesFast();
	
	int SensorChoice=2; //1 is MT9V011, 2 is MT9V115
	fChain->GetEntry(0); //carico una entries per poter prendere i valori dal file MC
	
	int NPixX=640, NPixY=480;
	if (SensorChoice==2) {
		NPixX=648;
		NPixY=488;
	}
	int NPixTot=NPixX*NPixY;
	int NPrimMC=Nev;
	double AttSorg=2.54e3; //Total emissivity of source [Bq] (was 5.94e3 maybe for Sr PG source) 26.33e3 (??) - 2.53e3 Sr Roma
	double DTacq=200e-3; //Data Acquisition Time [s]
	int NFramesMC=0, NFramesMCLow=0;
	int FrameCounter=0;
	int RedFactor=1, kk=0, jj=0;
	bool LowFlag=kFALSE;
	bool debug=kTRUE;
	int verbose=fVerbose;
	double NumEvtPerFrame=0;
	vector<double> VectTot(NPixTot,0.); //Vettore in cui appoggiare i valori di energia depositata nel dato pixel
	vector<double> VectTotConv(NPixTot,0.); //Vettore in cui appoggiare i valori di energia depositata nel dato pixel convertita in ADC
	vector<double> VectTotPrimEn(NPixTot,-10.); //Vettore in cui appoggiare i valori dell'energia del primario che ha lasciato segnale nel dato pixel
	
	double SmearingFactor=0;
	TRandom *rand= new TRandom();
	TRandom *randPoiss= new TRandom();
	TStopwatch sw;
	double NoiseFactor=0;
	
	TH1F* CheckPoisson=new TH1F("CheckPoisson","CheckPoisson", 100, 0, 100);
	
	Long64_t nbytes = 0, nb = 0;
	
	FrameFile->cd();
	TTree *MCTree = new TTree("CMOSDataTree","CMOS Monte Carlo sim");
	Frame *frameData = new Frame(NPixX,NPixY); //~480x640
	MCTree->Branch("frame",&frameData);
	
	Frame *framePrimEn = new Frame(NPixX,NPixY); //~480x640
	MCTree->Branch("framePrimEn",&framePrimEn);
	
	
	cout<<"Created output file: "<<FrameFile->GetName()<<endl;
	cout<<"Created output file: "<<ImageFile->GetName()<<endl;
	
	NFramesMC=NPrimMC/AttSorg/DTacq;  //is the number of "frame-equivalents" in the MC run
	NumEvtPerFrame=((double)nentries)/NFramesMC;
	cout<<"#####################################"<<endl;
	cout<<std::scientific<<"Number of Primaries Generated in MC: "<<(double)NPrimMC<<std::fixed<<endl;
	cout<<"Source Activity [Bq]= "<<AttSorg<<endl;
	cout<<"Entries = "<<nentries<<endl; //is the number of events in which I have a signal
	cout<<"It's like I'have simulated "<<NPrimMC/AttSorg<<" [s] of data taking, equivalent to about "<<NFramesMC <<" frames of 200 ms"<<endl;
	cout<<"I will create a frame every "<<(int)round((double)nentries/NFramesMC)<<" events, and append in case some empty ones" <<endl;
	cout<<"OR I will try to use Poisson, and create a frame every about "<<NumEvtPerFrame<<" events, and append in case some empty ones" <<endl;
	
	if (nentries/NFramesMC == 0) {
		LowFlag=kTRUE;
		NFramesMCLow=(NPrimMC/AttSorg*NFramesMC/nentries/DTacq);
		cout<<"WARNING! NUMBER OF EVENT TOO LOW FOR FRAME DIVISION, assuming 1 per frame and adding extra empty frames!"<<endl;
	}
	
	ImageFile->cd();
	TH2F* cluster =new TH2F("cluster","cluster", NPixX/RedFactor, 0, NPixX, NPixY/RedFactor, 0, NPixY);
	//	double temp=0;
	
	int ContaPixel[NFramesMC]; //Contatore del numero di pixel presenti in ogni frame
	
	// Creo un Cluster grafico per frame
	TH2F* ClusterVec[NFramesMC];
	for (kk=0; kk<NFramesMC; kk++){
		ClusterVec[kk]=new TH2F(Form("ClusterVec%d",kk),Form("Cluster for frame n%d", kk), NPixX/RedFactor, 0, NPixX, NPixY/RedFactor, 0, NPixY);
		ContaPixel[kk]=0;
	}
	kk=0; //azzero il contatore dei frame
	
	
	sw.Start();
	
	
	
	//############################################
	//########## LOOP SU TUTTI GLI EVENTI
	//#######
	int PoissonVal=randPoiss->Poisson(NumEvtPerFrame);
	int PoissonIncrement=0;
	if (verbose>=1) cout<<"First Poisson trial: n= "<<PoissonVal<<endl;
	
	for (Long64_t jentry=0; jentry<nentries&&1;jentry++) { //Ciclo su tutte le entries MC (cioè Isotopo priario per Isotopo primario che ha rilasciato un segnale!)
		Long64_t ientry = LoadTree(jentry);
		if (ientry < 0) break;
		nb = fChain->GetEntry(jentry);   nbytes += nb;
		if (verbose>=2) cout<<endl;
		ContaPixel[kk]+=PixelID->size(); //conto di quanti pixel accesi è fatto ciascun frame
		
		
		
		for (int ii=0; ii<PixelID->size(); ii++){  //loop su tutti i pixel accesi durante quell'evento
			
			//calcolo gli indici I e J per il pixel considerato: in questo modo vanno da 0 a NCol/NRow -1 come vuole Frame (PixelID va da 1 in su)
			int Ipix=((int)(PixelID->at(ii))-1)%NPixX;
			int Jpix=((PixelID->at(ii)-1)/NPixX);
			
			// Incremento l'elemento del vettore corrispondente a quel pixel aggiungendo l'energia depositata in [keV]
			//			VectTot[(int)(PixelID->at(ii))]+=(int)(InCmosEn->at(ii)*1000*ConvFactor);
			
			// ###################################################
			// ############ LA CICCIA ############################
			// ###################################################
			VectTot[(int)(PixelID->at(ii))]+=(InCmosEn->at(ii));
			VectTotPrimEn[(int)(PixelID->at(ii))]=InCmosEnPrim->at(ii);
			// ###################################################
			// ###################################################

			
			//			cout<< (int)(PixelID->at(ii)) <<" "<< (InCmosEn->at(ii)) <<" "<<VectTot[(int)(PixelID->at(ii))] <<endl;
			// ##############################
			
			
			if (verbose>=2) cout<<"evento: "<<jentry<<", px: "<<(int)PixelID->at(ii)<<", colonna= "<<Ipix<<", x= "<<PixXPos->at(ii)<<", riga= "<<Jpix<<", y= "<<PixYPos->at(ii)<<", ene= "<<InCmosEn->at(ii)<<", eneConv= "<<(int)(InCmosEn->at(ii)*ConvFactor)<<endl;
			
			//Accendo il pixel nei relativi istogrammi
			cluster->Fill(Ipix, Jpix);
			ClusterVec[FrameCounter]->Fill(Ipix, Jpix);
		}  //fine loop su tutti i pixel accesi in quell'evento
			 // Ora dentro ClusterVec ho salvato per ciascun pixel il rilascio di energia (ancora in keV!!) dovuto agli eventi appartenenti a questo frame!
		
		
		// ########################################
		// È il momento di chiudere il frame
		if (LowFlag || ( jentry!=nentries&& ((jentry+1)%(PoissonVal)==0)) || jentry==nentries ) { //POISSON VERSION - se sono in regime di "ogni entry va in un frame" o se sono al cambio di frame salvo il frame e ne inizio un altro
			
			//			if (LowFlag || ((jentry+1)%((int)round((double)nentries/NFramesMC))==0) ) { //CONSTANT NUMBER VERSION - se sono in regime di "ogni entry va in un frame" o se sono al cambio di frame salvo il frame e ne inizio un altro
			for (jj=0; jj<NPixTot; jj++) {
				//prima di salvare il frame riciclo su tutti i pixel per aggiumgere l'eventuale rumore MC
				if (verbose>=3&&VectTot[jj]>=0) cout<<endl<<"Fino a evento n= "<<jentry<<", Frame n= "<<FrameCounter<<", Px n= "<<jj<<", Before sm and conv was= " << VectTot[jj];
				//				SmearingFactor=(rand->Gaus(0,1));
				if(NoiseFlag) {
					NoiseFactor=fVectorNoise[0][jj];
					SmearingFactor=(rand->Gaus(0,NoiseFactor));
				}
				//				VectTotConv[jj]=(int)(ConvFactor*VectTot[jj]); //converte keV -> ADC
				VectTotConv[jj]=round(ConvFactor*VectTot[jj]+SmearingFactor); //converte keV -> ADC
																																			//				if (verbose>=3&&VectTot[jj]>=0) cout<<", Before smearing after conv was= " << VectTotConv[jj]<<", noise was= "<<NoiseFactor<<", adding smearing of= "<<(int)SmearingFactor;
																																			//				VectTotConv[jj]+=(int)SmearingFactor;
				if (verbose>=3&&VectTot[jj]>=0) cout<<", after smearing= "<<VectTotConv[jj]<<endl;
				
				int IpixBis=(jj)%NPixX;
				int JpixBis=((jj)/NPixX);
				frameData->Set(IpixBis, JpixBis, VectTotConv[jj]); //aggiungo al frame il valore di quel pixel
				framePrimEn->Set(IpixBis, JpixBis, VectTotPrimEn[jj]); //aggiungo al frame il valore di quel pixel

			}
			//			some_vector.assign(some_vector.size(), 0);
			VectTot.assign(VectTot.size(),0);
			VectTotConv.assign(VectTotConv.size(),0);
			VectTotPrimEn.assign(VectTotPrimEn.size(),-10);
			//			VectTot.clear(); //svuoto il vettore di appoggio di tutti i pixel per prepararmi al nuovo frame
			//			VectTotConv.clear();
			
			if (verbose==0) {
				if (FrameCounter%100==0) cout<<endl<<"Saving Frame Number: "<<FrameCounter<<endl;
			}
			else if (verbose>0) {
				if (FrameCounter%1==0) cout<<endl<<"Saving Frame Number: "<<FrameCounter<<" jentry= "<<jentry<<endl;
			}
			frameData->SetId(FrameCounter);
			framePrimEn->SetId(FrameCounter);
			ImageFile->cd();
			ClusterVec[FrameCounter]->Write();
			FrameCounter++;
			MCTree->Fill();
			frameData->Clear();
			framePrimEn->Clear();
			kk++; //contatore dei frame
			
			if (verbose>=1) cout<<"New Poisson trial: was= "<<PoissonVal;
			PoissonIncrement= randPoiss->Poisson(NumEvtPerFrame);
			CheckPoisson->Fill(PoissonIncrement);
			PoissonVal+=PoissonIncrement;
			if (verbose>=1)cout<<" now is: n= "<<PoissonVal<<", increment= "<<PoissonIncrement<<", media= "<<(double)PoissonVal/FrameCounter<<endl;
			
			while(PoissonIncrement==0) {
				if (verbose>=1)cout<<"It happened poisson=0!!! Adding an empty frame and going on!"<<endl;
				frameData->SetId(FrameCounter);
				framePrimEn->SetId(FrameCounter);
				ImageFile->cd();
				ClusterVec[FrameCounter]->Write();
				FrameCounter++;
				MCTree->Fill();
				frameData->Clear();
				framePrimEn->Clear();
				kk++; //contatore dei frame
				PoissonIncrement= randPoiss->Poisson(NumEvtPerFrame);
				CheckPoisson->Fill(PoissonIncrement);
				PoissonVal+=PoissonIncrement;
				if (verbose>=1)cout<<"new Poisson val = "<<PoissonIncrement<<", media= "<<(double)PoissonVal/FrameCounter<<endl;
			}
			
			
		} //fine ciclo scrittura frame
			// ########################################
		
		
		
	} //fine ciclo sulle entries MC
	
	//#######
	//########## FINE LOOP SU TUTTI GLI EVENTI
	//############################################
	
	//	for (kk=0; kk<NPixTot;kk++) cout<<"pixel num "<<kk<<", valore= "<<VectTot[kk]<<endl;
	
	
	// ########################################
	// Aggiungo frame vuoti se devo far tornare il numero totale
#if 1
	if (FrameCounter<NFramesMC && !LowFlag) {
		cout<<"Now I have to add some empty frames since NFramesMC= "<<NFramesMC<<" while  FrameCounter= "<<FrameCounter<<endl;
		for (int jj=FrameCounter; jj<NFramesMC; jj++){
			frameData->SetId(FrameCounter);
			framePrimEn->SetId(FrameCounter);
			FrameCounter++;
			MCTree->Fill();
			frameData->Clear();
			framePrimEn->Clear();
			if (0) cout<<"jj: "<<jj<<", FrameCounter= "<<FrameCounter<<endl;
		}
	}
	
	if (LowFlag) {
		for (int jj=nentries; jj<NFramesMC; jj++){
			frameData->SetId(FrameCounter);
			framePrimEn->SetId(FrameCounter);
			FrameCounter++;
			MCTree->Fill();
			frameData->Clear();
			framePrimEn->Clear();
			if (0&&nentries==NFramesMC-1) cout<<"jj: "<<jj<<", FrameCounter= "<<FrameCounter<<endl;
		}
	}
#endif
	
	for (kk=0; kk<NFramesMC; kk++){
		if (0) cout<<"k= "<<kk<<", Counter "<<ContaPixel[kk]<<endl;;
	}
	
	FrameFile->Write();
	ImageFile->cd();
	cluster->Write();
	CheckPoisson->Write();
	
	FrameFile->Close();
	ImageFile->Close();
}
