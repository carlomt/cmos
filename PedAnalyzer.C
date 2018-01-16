#define PedAnalyzer_cxx
#include "PedAnalyzer.h"
#include <TH2.h>
#include <TStyle.h>
#include <iostream>
#include <TCanvas.h>

void PedAnalyzer::Loop()
{
	// ######################################################
	// ######################################################
	// Questa classe permette di estrarre i file txt di piedistallo e noise
	// Prende in input un file .root che e' il risultato di FileConverter
	// Per calcolare piedistalli e noise si usa una parte dei frame acquisiti (fPedToConsider)
	// Produce in output:
	// - due file di testo .txt, da passare poi ancora a FileConverter
	// - un file "XXX_PostPed.root" in cui sono salvati:
	//    - Un istogramma ("HistoXXXXXX") per pixel con il valore scelto come piedistallo per il dato pixel (Nframes entries)
	//    - Un istogramma ("DumpIstoTot") con tutti i valori rozzi letti in tutti i pixel (Npixel*Nframes entries)
	//    - Due istogrammi ("DumpIstoAllPed" e "DumpIstoAllNoise") con tutti i valori dei
	//      piedistalli e dei noise per ogni pixel (Npixel entries)
	//    - Due TVector per passare in maniera rapida i valori di piedistallo e noise alla Macro di controllo stabilità (MacroPerPedNoise)
	// ######################################################
	// tempo necessario per girare circa 10 minuti per 1000 files (1 minuto per 100)
	// #####################################################
	//	Percorso logico utilizzato:
	//	- Inizio ciclo sui frame: per ogni frame: (jentry->fPedToConsider)
	//	  - Inizio ciclo sui pixel: per ogni pixel: (kk->Size)
	//	    - Salvo il valore del k-esimo pixel al j-esimo frame nell’istogramma HistoKK;
	//	    - Salvo il valore del k-esimo pixel nell’istogramma totale DumpIstoTot;
	//	    - Aggiungo il valore del k-esimo pixel al vettoreVectVal di NPixel elementi che rappresenterà quindi la somma di tutte i conteggi di quel pixel in tutti i frame considerati
	//	  - Fine ciclo sui pixel
	//	- Fine ciclo sui frame
	//	- Secondo ciclo sui pixel: per ogni pixel: (kk->Size)
	//	  - Ciclo sui bin degli istogrammi: per ogni bin: (rr->100)
	//	    - Faccio media e devSTD considerando solo i bin con più di un entries, in modo da escludere quelli contenenti eventualmente un segnale che mi alzerebbero artificialmente la media del piedistallo. Se la devSTD è minore di 1 allora metto 1 per evitare i casi in cui sottostimo il noise (creando quindi falsi bad-pixel)
	//    - Fine ciclo sui bin
	//    - Salvo media e devSTD del k-esimo pixel in DumpIstoAllPed e DumpIstoAllNoise per avere istogrammi di TUTTI i piedistalli (cioè “NPixel” entries)
	//	  - Salvo media e devSTD del k-esimo pixel in due TVector per salvarli nel file di output in modo rapido da usare con la macro PedCompare
	//    - Scrivo sui due file .txt i valori di piedistallo e noise
	//  - Fine ciclo sui pixel
	
	
	//######### LAST MODIFIED ON 2018-01-15 by Collamaf
	
	//USAGE
	/*
	 .L PedAnalyzer.C
	 c=new PedAnalyzer("DatiProva/RootBuoni/prova100",100)
	 c->Loop()
	 */
	
	if (fChain == 0) return;
	
	Long64_t nentries = fChain->GetEntriesFast();
	//	int PedToConsider=100;
	if (fPedToConsider==-1) fPedToConsider=nentries;
	vector<double> VectVal;
	//	vector<double> VectValQuad;
	fChain->GetEntry(1); //carico una entry per poter usare fNRow e fNCol
	int Size=fNCol*fNRow;
	int kk=0;
	cout<<"Total number of pixels = "<<Size<<endl;
	
	TVectorD VectorPed(Size), VectorNoise(Size);
	TH1F* DumpIstoPed[Size];
	VectVal.resize(Size);
	//	VectValQuad.resize(Size);
	
	TH1F* DumpIstoTot=new TH1F("DumpIstoTot","Tutti i valori di tutti i pixel per tutti i frame",10000, 0,1000);
	TH1F* DumpIstoAllPed=new TH1F("DumpIstoAllPed","Valore del piedistallo di tutti i pixel",10000, 0,100);
	TH1F* DumpIstoAllNoise=new TH1F("DumpIstoAllNoise","Valore del noise di tutti i pixel",10000, 0,100);
	
	for (kk=0; kk<Size; kk++) { //inizializzo i vettori e gli istogrammi per ciascun pixel
		VectVal[kk]=0;
		//		VectValQuad.at(kk)=0;
		DumpIstoPed[kk]=new TH1F(Form("Histo%d",kk),Form("Histo%d",kk),100, 0, 100);
	}
	
	TString InputFileName=Form("%s.root",nomefile.Data());
	TString PedFileName=Form("%s_buio_%d.txt",nomefile.Data(),fPedToConsider);
	TString NoiseFileName=Form("%s_noise_%d.txt",nomefile.Data(),fPedToConsider);
	TString PostPedFileName=Form("%s_PostPed_%d.root",nomefile.Data(),fPedToConsider);
	out=new TFile(PostPedFileName,"RECREATE");
	ofstream PedFile(PedFileName);
	ofstream NoiseFile(NoiseFileName);
	
	cout<<"######################################## "<<endl<<"I will use the first "<<fPedToConsider<<" frames of file "<<endl<<InputFileName<<endl<<" to compute pedestals and noise. I will produce:"<<endl;
	cout<<PedFileName<<endl;
	cout<<NoiseFileName<<endl;
	cout<<PostPedFileName<<endl<<"######################################## "<<endl;
	
	Long64_t nbytes = 0, nb = 0;
	for (Long64_t jentry=0; jentry<fPedToConsider;jentry++) {    //ciclo su tutti i frames
		cout<<"Sto analizzando il file (frame) num= "<<jentry<<", di "<<nentries<<endl;
		Long64_t ientry = LoadTree(jentry);
		if (ientry < 0) break;
		nb = fChain->GetEntry(jentry);   nbytes += nb;
		for (kk=0; kk<VectVal.size(); kk++) { //giro su tutti i pixel per leggerne e salvarne il valore
			DumpIstoPed[kk]->Fill(fData[kk]);  //lo metto nell'istogramma del k-esimo pixel
			DumpIstoTot->Fill(fData[kk]);       //metto il valore letto nell'istogramma "calderone", contenente i valori di tutti i pixel in tutti i frame
			VectVal.at(kk)+=fData[kk];
		}
	} //chiude ciclo sui frames
	
	double STDDev=0;
	double PedEstimator=0;
	double MediaCut=0;
	double MediaCutSquare=0;
	int CounterMediaCut=0;
	int rr=0;
	
	for (kk=0; kk<VectVal.size(); kk++) {  //ri ciclo su tutti i pixel per scrivere la media
		MediaCut=0;
		MediaCutSquare=0;
		CounterMediaCut=0;
		
		for (rr=0; rr<100; rr++) {          //ciclo sui 100 bin degli istogrammi per fare la media dei soli bin con piu di una entries evitando quindi quelli (eventuali) di segnale
			double yval=DumpIstoPed[kk]->GetBinContent(rr);
			if (yval>1) {
				int xbin=DumpIstoPed[kk]->GetBinCenter(rr);
				MediaCut+= xbin*yval;
				MediaCutSquare+=xbin*xbin*yval;
				CounterMediaCut+=yval;
			}
		}
		PedEstimator= MediaCut/CounterMediaCut;
		MediaCutSquare/=CounterMediaCut;
		STDDev=sqrt(MediaCutSquare-(PedEstimator*PedEstimator));
		if (STDDev<1) STDDev=1;  //mettiamo il noise peggiore fra quello calcolato e 1

		VectorPed[kk]=PedEstimator;
		VectorNoise[kk]=STDDev;
		PedFile<<PedEstimator<<" ";
		NoiseFile<<STDDev<<" ";
		DumpIstoPed[kk]->Write();
		
		DumpIstoAllPed->Fill(PedEstimator);
		DumpIstoAllNoise->Fill(STDDev);
	}
	
	DumpIstoAllPed->Write();
	DumpIstoAllNoise->Write();
	DumpIstoTot->Write();
	VectorNoise.Write("VectorNoise");
	VectorPed.Write("VectorPed");
	
	PedFile.close();
	NoiseFile.close();
	
	
	cout<<"######################################## "<<endl<<"I used the first "<<fPedToConsider<<" frames of file "<<endl<<InputFileName<<endl<<" to compute pedestals and noise. I produced:"<<endl;
	cout<<PedFileName<<endl;
	cout<<NoiseFileName<<endl;
	cout<<PostPedFileName<<endl<<"######################################## "<<endl;
}
