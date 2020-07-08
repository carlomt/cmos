#define PedAnalyzer_cxx
#include "PedAnalyzer.h"
#include <TH2.h>
#include <TStyle.h>
#include <iostream>
#include <TCanvas.h>
#include "TStopwatch.h"

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
	//    - [NON PIU!!] Un istogramma ("HistoXXXXXX") per pixel con il valore scelto come piedistallo per il dato pixel (Nframes entries)
	//    - Un istogramma ("DumpIstoTot") con tutti i valori rozzi letti in tutti i pixel (Npixel*Nframes entries)
	//    - Due istogrammi ("DumpIstoAllPed" e "DumpIstoAllNoise") con tutti i valori dei
	//      piedistalli e dei noise per ogni pixel (Npixel entries)
	//    - Due TVector per passare in maniera rapida i valori di piedistallo e noise alla Macro di controllo stabilità (MacroPerPedNoise)
	// ######################################################
	// tempo necessario per girare circa 10 minuti per 1000 frames (1 minuto per 100)
	// #####################################################
	//	Percorso logico utilizzato:
	//	- Inizio ciclo sui frame: per ogni frame: (jentry->fPedToConsider)
	//	  - Inizio ciclo sui pixel: per ogni pixel: (kk->Size)
	//	    - Salvo il valore del k-esimo pixel al j-esimo frame nell’istogramma HistoKK;
	//	    - Salvo il valore del k-esimo pixel nell’istogramma totale DumpIstoTot;
	//	    - Aggiungo il valore del k-esimo pixel al vettore VectVal di NPixel elementi che rappresenterà quindi la somma di tutte i conteggi di quel pixel in tutti i frame considerati
	//	  - Fine ciclo sui pixel
	//	- Fine ciclo sui frame
	//	- Secondo ciclo sui pixel: per ogni pixel: (kk->Size)
	//	  - Ciclo sui bin degli istogrammi: per ogni bin: (rr->1024)
	//	    - Faccio media e devSTD considerando solo i bin con più di una entry, in modo da escludere quelli contenenti eventualmente un segnale che mi alzerebbero artificialmente la media del piedistallo. Se la devSTD è minore di 1 allora metto 1 per evitare i casi in cui sottostimo il noise (creando quindi falsi bad-pixel)
	//    - Fine ciclo sui bin
	//    - Salvo media e devSTD del k-esimo pixel in DumpIstoAllPed e DumpIstoAllNoise per avere istogrammi di TUTTI i piedistalli (cioè “NPixel” entries)
	//	  - Salvo media e devSTD del k-esimo pixel in due TVector per salvarli nel file di output in modo rapido da usare con la macro PedCompare
	//    - Scrivo sui due file .txt i valori di piedistallo e noise
	//  - Fine ciclo sui pixel
	
	
	//######### MODIFIED ON 2018-01-23 by Collamaf
	// now PedHisto have 1000 bins not 100
	//######### MODIFIED ON 2018-04-07 by amorusor
  // now we can choose the group of frames to consider (before: only from 0 to fPedToConsider)
	
	//######### LAST MODIFIED ON 2020-07-08 by collamaf
	// much more efficient way to compute each pixel's pedestal, no more huge array of useless TH1F, but std::vectors
	// new approach to (right) outlier management.. should work..
	
	//USAGE
	/*
	 .L PedAnalyzer.C
	 c=new PedAnalyzer("DatiProva/RootBuoni/prova100",100)
	 c->Loop()
	 */
	
	if (fChain == 0) return;
	

	cout<<"Time Check Point: Inizio Loop:  "<<timer.RealTime()<<endl;
	timer.Start();
	
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
	cout<<"DEBUG 0"<<endl;
	
	vector<vector<int> > allPixelsDump( Size , vector<int> (fPedToConsider, 0));
  
//    for (int i = 0; i < fNRow; i++) {
//        for (int j = 0; j < fNCol; j++){
//            cout<< vec[i][j]<< " ";
//        }
//        cout<< "\n";
//    }
	
	
	
//	Size=1000000;
//	TH1F* DumpIstoPed[Size];
	cout<<"DEBUG 1"<<endl;

	VectVal.resize(Size);
	cout<<"DEBUG 2"<<endl;
	//	VectValQuad.resize(Size);
	
	TH1F* DumpIstoTot=new TH1F("DumpIstoTot","Tutti i valori di tutti i pixel per tutti i frame",10240, 0,1024);
	TH1F* DumpIstoAllPed=new TH1F("DumpIstoAllPed","Valore del piedistallo di tutti i pixel",10240, 0,1024);
	TH1F* DumpIstoAllNoise=new TH1F("DumpIstoAllNoise","Valore del noise di tutti i pixel",10000, 0,100);
	
	for (kk=0; kk<Size; kk++) { //inizializzo i vettori e gli istogrammi per ciascun pixel
		VectVal[kk]=0;
		//		VectValQuad.at(kk)=0;
//		DumpIstoPed[kk]=new TH1F(Form("Histo%d",kk),Form("Histo%d",kk),1024, 0,1024);
	}
	cout<<"Time Check Point: Inizializzati vettori e istogrammi: "<<timer.RealTime()<<endl;
	timer.Start();

	TString InputFileName=Form("%s.root",nomefile.Data());
	TString PedFileName=Form("%s_buio_%d_%d.txt",nomefile.Data(),fPedToConsider, fStartPed);
	TString NoiseFileName=Form("%s_noise_%d_%d.txt",nomefile.Data(),fPedToConsider, fStartPed);
	TString PostPedFileName=Form("%s_PostPed_%d_%d.root",nomefile.Data(),fPedToConsider, fStartPed);
	out=new TFile(PostPedFileName,"RECREATE");
	ofstream PedFile(PedFileName);
	ofstream NoiseFile(NoiseFileName);
	
	cout<<"######################################## "<<endl<<"I will use the frames from number "<<fStartPed<<" to number "<<(fPedToConsider+fStartPed-1)<<" of file "<<endl<<InputFileName<<endl<<" to compute pedestals and noise. I will produce:"<<endl;
	cout<<PedFileName<<endl;
	cout<<NoiseFileName<<endl;
	cout<<PostPedFileName<<endl<<"######################################## "<<endl;
	
	cout<<"Time Check Point: Inizio ciclo sui frame: "<<timer.RealTime()<<endl;
	timer.Start();

	Long64_t nbytes = 0, nb = 0;
	for (Long64_t jentry=fStartPed; jentry<fPedToConsider+fStartPed;jentry++) {    //ciclo su tutti i frames
		cout<<"Sto analizzando il file (frame) num= "<<jentry<<", di "<<nentries<<endl;
		Long64_t ientry = LoadTree(jentry);
		if (ientry < 0) break;
		nb = fChain->GetEntry(jentry);   nbytes += nb;
		for (kk=0; kk<VectVal.size(); kk++) {      //giro su tutti i pixel per leggerne e salvarne il valore
//			DumpIstoPed[kk]->Fill(fData[kk]);  //lo metto nell'istogramma del k-esimo pixel
			allPixelsDump[kk][jentry]=fData[kk];
			DumpIstoTot->Fill(fData[kk]);      //metto il valore letto nell'istogramma "calderone", contenente i valori di tutti i pixel in tutti i frame
			VectVal.at(kk)+=fData[kk];
		}
	} //chiude ciclo sui frames
	cout<<"Time Check Point: Fine ciclo sui frame: "<<timer.RealTime()<<endl;
	timer.Start();

	double STDDev=0;
	double PedEstimator=0;
	double MediaCut=0;
	double MediaCutSquare=0;
	int CounterMediaCut=0;
	int rr=0;
	
	cout<<"Time Check Point: Ri ciclo su tutti i pixel: "<<timer.RealTime()<<endl;
	timer.Start();

	for (kk=0; kk<VectVal.size(); kk++) {  //ri ciclo su tutti i pixel per scrivere la media
		MediaCut=0;
		MediaCutSquare=0;
		CounterMediaCut=0;
		std::sort(allPixelsDump[kk].begin(), allPixelsDump[kk].end()); //Ordino il vettore in modo che l'elemento più grande (eventuale outiler) sia l'ultimo
		
		int cutLastElement=0; //di default non effettuo nessun taglio nel calcolo della media
		
		double tempMeanForCut=1.0*std::accumulate( allPixelsDump[kk].begin(), allPixelsDump[kk].end()-1, 0 )/(allPixelsDump[kk].size()-1);
		if (allPixelsDump[kk].back()>tempMeanForCut*5) { //se l'elemento piu grande del vettore è piu di 5 volte piu grande della media di tutti gli altri è probabilmente un outiler
			cutLastElement=1;
			cout<<"###########\nPresente outlier, taglio il valore piu alto dalla media\n###########"<<endl;
		}
		
		double MediaUnCut=1.0*std::accumulate( allPixelsDump[kk].begin(), allPixelsDump[kk].end()-cutLastElement, 0 )/(allPixelsDump[kk].size()-cutLastElement); //per calcolare la media escludo (in caso) l'elemento piu grande, dove si troverebbe l'eventuale bin dovuto al segnale della particella
		MediaCut=MediaUnCut;
		
		//Calcolo della deviazione standard
		std::vector<double> diff(allPixelsDump[kk].size()-cutLastElement);
		std::transform(allPixelsDump[kk].begin(), allPixelsDump[kk].end()-cutLastElement, diff.begin(), [MediaCut](double x) { return x - MediaCut; });
		double sq_sum = std::inner_product(diff.begin(), diff.end()-cutLastElement, diff.begin(), 0.0);
		STDDev = std::sqrt(sq_sum / allPixelsDump[kk].size());
		

		
//		for (rr=1; rr<=1024; rr++) {          //ciclo sui 1024 bin degli istogrammi per fare la media dei soli bin con piu di una entry evitando quindi quelli (eventuali) di segnale
////			double yval=DumpIstoPed[kk]->GetBinContent(rr);
//			double yval=17;
//			if (yval>1) {
////				int xbin=DumpIstoPed[kk]->GetBinCenter(rr);
//				int xbin=13;
//				MediaCut+= xbin*yval;
//				MediaCutSquare+=xbin*xbin*yval;
//				CounterMediaCut+=yval;
//			}
//		}
//		PedEstimator= MediaCut/CounterMediaCut;
		PedEstimator=MediaCut;
//		if (CounterMediaCut==0) cout<<"ATTENZIONE DIVIDO PER 0!!! pixel num "<<kk<<endl;
//		MediaCutSquare/=CounterMediaCut;
//		MediaCutSquare=1;
//		STDDev=sqrt(MediaCutSquare-(PedEstimator*PedEstimator));
//		STDDev=stdev;
		if (STDDev<1) STDDev=1;  //mettiamo il noise peggiore fra quello calcolato e 1

		VectorPed[kk]=PedEstimator;
		VectorNoise[kk]=STDDev;
		PedFile<<PedEstimator<<" ";
		NoiseFile<<STDDev<<" ";
//		DumpIstoPed[kk]->Write();
		
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
	
	
	cout<<"######################################## "<<endl<<"I used the frames from number "<<fStartPed<<" to number "<<(fPedToConsider+fStartPed-1)<<" of file "<<endl<<InputFileName<<endl<<" to compute pedestals and noise. I produced:"<<endl;
	cout<<PedFileName<<endl;
	cout<<NoiseFileName<<endl;
	cout<<PostPedFileName<<endl<<"######################################## "<<endl;
	
	timer.Print();
	cout<<"Time Check Point: fine: "<<timer.RealTime()<<endl;


}
