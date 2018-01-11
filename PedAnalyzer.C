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
	// applicato ad un un certo numero (insieme) di file raw usati come piedistallo
	// Produce in output:
	// - due file di testo .txt, da passare poi ancora a FileConverter
	// - un file "XXX_PostPed.root" in cui sono salvati:
	//    - Un istogramma ("HistoXXX") per pixel con il valore scelto come piedistallo per il dato pixel (Nframes entries)
	//    - Un istogramma ("DumpIstoTot") con tutti i valori rozzi letti in tutti i pixel (Npixel*Nframes entries)
	//    - Due istogrammi ("DumpIstoAllMean" e "DumpIstoAllNoise") con tutti i valori dei
	//      piedistalli e dei noise per ogni pixel (Npixel entries)
	//    - Due TVector per passare in maniera rapida i valori di piedistallo e noise alla Macro di controllo stabilità (MacroPerPedNoise)
	// ######################################################
	// tempo necessario per girare circa 10 minuti per 1000 files (1 minuto per 100)
	
	//USAGE
	/*
	 .L PedAnalyzer.C
	 c=new PedAnalyzer("DatiProva/RootBuoni/prova100")
	 c->Loop()
	 */
	
	if (fChain == 0) return;
	
	Long64_t nentries = fChain->GetEntriesFast();
	int PedToConsider=100;
	vector<double> VectVal;
	vector<double> VectValQuad;
	TVectorD VectorMean(316224), VectorNoise(316224);
	
	TH1F* DumpIstoMean[316224];
	
	TH1F* DumpIstoTot=new TH1F("DumpIstoTot","Tutti i valori di tutti i pixel per tutti i frame",10000, 0,1000);
	TH1F* DumpIstoAllMean=new TH1F("DumpIstoAllMean","Valore del piedistallo di tutti i pixel",10000, 0,100);
	TH1F* DumpIstoAllNoise=new TH1F("DumpIstoAllNoise","Valore del noise di tutti i pixel",10000, 0,100);
	//	cout<<"SizePre= "<<fNRow*fNCol<<endl;
	
	TString InputFileName=Form("%s.root",nomefile.Data());
	TString PedFileName=Form("%s_buio_%d.txt",nomefile.Data(),PedToConsider);
	TString NoiseFileName=Form("%s_noise_%d.txt",nomefile.Data(),PedToConsider);
	TString PostPedFileName=Form("%s_PostPed_%d.root",nomefile.Data(),PedToConsider);
	
	out=new TFile(PostPedFileName,"RECREATE");
	
	
	//	ofstream PedFile(Form("%s_buio_%d.txt",nomefile.Data(),PedToConsider));
	//	ofstream NoiseFile(Form("%s_noise_%d.txt",nomefile.Data(),PedToConsider));
	
	
	ofstream PedFile(PedFileName);
	ofstream NoiseFile(NoiseFileName);
	
	
	int kk=0;
	//	temp[488][648];
	Long64_t nbytes = 0, nb = 0;
	for (Long64_t jentry=0; jentry<PedToConsider;jentry++) {
		Long64_t ientry = LoadTree(jentry);
		if (ientry < 0) break;
		nb = fChain->GetEntry(jentry);   nbytes += nb;
		
		if (jentry==0) {  //Inizializzazioni varie fatte solo al primo frame
			int Size=fNRow*fNCol;
			cout<<"Size= "<<Size<<endl;
			VectVal.resize(Size);
			VectValQuad.resize(Size);
			//			TH1F* DumpIstoMean[Size];
			for (kk=0; kk<Size; kk++) { //inizializzo i vettori e gli istogrammi per ciascun pixel
				VectVal[kk]=0;
				VectValQuad.at(kk)=0;
				DumpIstoMean[kk]=new TH1F(Form("Histo%d",kk),Form("Histo%d",kk),100, 0, 100);
			}
		}                 //fine inizializzazione
		for (kk=0; kk<VectVal.size(); kk++) { //giro su tutti i pixel per leggerne e salvarne il valore
			DumpIstoMean[kk]->Fill(fData[kk]);  //lo metto nell'istogramma del k-esimo pixel
			DumpIstoTot->Fill(fData[kk]);       //metto il valore letto nell'istogramma "calderone"
			VectVal.at(kk)+=fData[kk];
			VectValQuad.at(kk)+=(fData[kk]*fData[kk]);
			if (kk==0) cout<<"File num= "<<jentry<<", pixel num= "<<kk<<", valore aggiunto= "<<fData[kk]<<", somma attuale= "<<VectVal.at(kk)<<", somma Quad attuale= "<<VectValQuad.at(kk)<<endl;
		}
	} //chiude ciclo su files
	
	double STDDev=0;
	double PedObsVal=0;
	double MediaCut=0;
	double MediaCutSquare=0;
	int CounterMediaCut=0;
	int rr=0;
	
	for (kk=0; kk<VectVal.size(); kk++) {  //ri ciclo su tutti i pixel per scrivere la media
		//		cout<<"Moda(-1)= "<<DumpIstoMean[kk]->GetMaximumBin()-1<<", RMS= "<<endl;
		MediaCut=0;
		MediaCutSquare=0;
		CounterMediaCut=0;
		
		for (rr=0; rr<100; rr++) {          //ciclo sui 100 bin degli istogrammi per fare la media dei soli bin con piu di una entries evitando quindi quelli di segnale
			if (DumpIstoMean[kk]->GetBinContent(rr)>1) {
				MediaCut+=(DumpIstoMean[kk]->GetBinCenter(rr)*DumpIstoMean[kk]->GetBinContent(rr));
				MediaCutSquare+=((DumpIstoMean[kk]->GetBinContent(rr)*DumpIstoMean[kk]->GetBinCenter(rr)*DumpIstoMean[kk]->GetBinCenter(rr)));
				
				CounterMediaCut+=DumpIstoMean[kk]->GetBinContent(rr);
				if (0&&fabs(kk-200)<10)	cout<<"kk= "<<kk<<", Valore Bin= "<< DumpIstoMean[kk]->GetBinCenter(rr)<<", Contenuto bin= "<<DumpIstoMean[kk]->GetBinContent(rr) <<", MediaCut= "<< MediaCut <<", CounterMediaCut= "<<CounterMediaCut<<", MediaCutSquare= "<<MediaCutSquare<<endl;
			}
		}
		PedObsVal= MediaCut/CounterMediaCut;
		MediaCutSquare/=CounterMediaCut;
		STDDev=MediaCutSquare-(PedObsVal*PedObsVal);
		//		if (kk==200)		cout<<"PedObsVal= "<< PedObsVal<<", MediaCutSquare= "<<MediaCutSquare<<", STDDev= "<<STDDev<<endl;
		//se voglio la media aritmetica
		/*
		 PedObsVal=VectVal.at(kk)/PedToConsider;
		 STDDev=sqrt(VectValQuad.at(kk)/PedToConsider-(PedObsVal*PedObsVal));
		 */
		
		//se voglio usare la moda al posto della media
		//		/*
		//		 PedObsVal=DumpIstoMean[kk]->GetMaximumBin()-1;
		//		 STDDev=DumpIstoMean[kk]->GetRMS();
		//		cout<<"MERDA"<<endl;
		//		 */
		
		if (kk==0) cout<<"#############  FINITO! ############### "<<endl<<"Somma finale= "<<VectVal.at(kk)<<", numero files= "<<PedToConsider<<", media (o mediana)= "<<PedObsVal<<", somma Quad finale= "<<VectValQuad.at(kk)<<", somma Quad finale/N = "<<VectValQuad.at(kk)/PedToConsider<<" ### DevSTD= "<<STDDev<<endl;
		
		if (0) {
			cout<<"kk= "<<kk<<", Moda= "<< PedObsVal <<", Media= "<<VectVal.at(kk)/PedToConsider<<endl;
		}
		VectorMean[kk]=PedObsVal;
		VectorNoise[kk]=STDDev;
		PedFile<<PedObsVal<<" ";
		NoiseFile<<STDDev<<" ";
		DumpIstoMean[kk]->Write();
		
		DumpIstoAllMean->Fill(PedObsVal);
		DumpIstoAllNoise->Fill(STDDev);
	}
	
	DumpIstoAllMean->Write();
	DumpIstoAllNoise->Write();
	DumpIstoTot->Write();
	VectorNoise.Write("VectorNoise");
	VectorMean.Write("VectorMean");
	
	PedFile.close();
	NoiseFile.close();
	
	
	cout<<"######################################## "<<endl<<"I use the first "<<PedToConsider<<" frames of file "<<endl<<InputFileName<<endl<<" to compute pedestals and noise. I will produce:"<<endl;
	cout<<PedFileName<<endl;
	cout<<NoiseFileName<<endl;
	cout<<PostPedFileName<<endl<<"######################################## "<<endl;
	
	
	
	/*
	 (TFile *) 0x7fe29eec9560
	 root [1] double x=0.5
	 (double) 0.500000
	 root [2] double y=0
	 (double) 0.000000
	 root [3] int nq=1
	 (int) 1
	 root [4] Histo2->GetQuantiles(
	 Int_t GetQuantiles(Int_t nprobSum, Double_t* q, const Double_t* probSum = 0)
	 root [4] Histo2->GetQuantiles(1, &y, &x)
	 (int) 1
	 root [5] cout<<y
	 11.4939(std::__1::basic_ostream &) @0x7fff99742660
	 root [6]
	 
	 
	 
	 */
	
	
	
}
