#include "Analisi.h"
#include <iostream>
#include <fstream>

//using namespace std;

using std::endl;
using std::cout;

ClassImp(Analisi);

Analisi::Analisi(const std::string OutFileName, const std::string BadFileName, int FrameNCol, int FrameNRow,  const Frame *FramePed, const Frame *FrameNoise, Double_t fcal)
{
	//********************GRAFICI***********************************
	HistoNSeeds = new TH1F("NSeeds","Number of seeds per frame;# of seeds;Counts",50,-1.5,48.5);
	
	map_seed = new TH2F("mapofseed","Seedmap;Col;Row",FrameNCol,0.,FrameNCol,FrameNRow,0.,FrameNRow);
	map_seedDist10 = new TH2F("mapofseedDist10","Seedmap with seeds at dist<10;Col;Row",FrameNCol,0.,FrameNCol,FrameNRow,0.,FrameNRow);
	
	map_seed_coarse = new TH2F("mapofseedcoarse8x8","Seedmapcoarse",8.,0.,FrameNCol,8.,0.,FrameNRow);           //mappa seed con bin larghi 8x8 pixel
	map_seed_coarse2 = new TH2F("mapofseedcoarse61x81","Seedmapcoarse61x81",61.,0.,FrameNCol,81.,0.,FrameNRow); //mappa seed con bin larghi 61x81 pixel
	map_seed1_4 = new TH2F("mapofseed_1di4", "mappa dei seed1",122.,0.,122.,FrameNRow,0.,FrameNRow);            //istogramma1 di 4: zoom su colonne da 0 a 122
	map_seed2_4 = new TH2F("mapofseed_2di4", "mappa dei seed2",122.,122.,244.,FrameNRow,0.,FrameNRow);
	map_seed3_4 = new TH2F("mapofseed_3di4", "mappa dei seed3",122.,244.,366.,FrameNRow,0.,FrameNRow);
	map_seed4_4 = new TH2F("mapofseed_4di4", "mappa dei seed4",122.,366.,FrameNRow,FrameNRow,0.,FrameNRow);
	map_Row_seed = new TH1F("mapofRow","posizione seed sulle righe",FrameNRow,0.,FrameNRow);                    //ATTENZIONE a regione fiduciale!! Con Riduzione.x si è tolto il bordo del sensore: 4<Righe<644 e 4<Colonne<484
	map_Col_seed = new TH1F("mapofCol","posizione seed sulle colonne",FrameNCol,0.,FrameNCol);
	
	HistoDistSeeds = new TH1F("Dist_Btw_Seeds","Relative distance between seeds;Distance of seeds;Counts", 150., -10.5, 889.5);
	HistoDistmin = new TH1F("DistMinSeed", "Minimum distance between seeds;Dist_min;Counts", 150., -10.5, 889.5);
	HistoDistmin_thr=new TH1F("DistMinSeed_thr", "Minimum distance between seeds over threshold(=10 pixels);Dist_min_thr;Counts", 150., -10.5, 889.5);
	
	HistoR_vs_Cluster = new TH1F("Dist_Cen_Cluster","Distance between sensor center and clusters;Distance;Counts",100,-50.5,549.5);    //distanza tra centro del sensore e la posizione di un seed
	
	///////////////////////////////////////////////////////
	//Nei grafici seguenti:
	//-> NpixClu : numero di pixel in un cluster
	//-> V=k*s+n : segnale del cluster
	// dove s=segnale prodotto dalla particella
	//      k= frazione del segnale raccolto dal pixel
	//      n=rumore del pixel
	
	HistoV_single = new TH1F("SinglePix_Signal","Single pixel signal: DV;Cluster Energy (ADC);Counts",200,-50.,1150.);//segnale netto che ho per ogni pixel: DV=(s-ped), calcolato per ogni frame e per ogni pixel
	
	HistoClusterAsy = new TH1F("Cluster_asymmetric","Cluster asymmetric signal;Cluster Energy (ADC);Counts",200, -50., 3950.);//energia del cluster asimmetrico
	HistoClusterAsy_keV = new TH1F("Cluster_asymmetric_keV","Cluster asymmetric signal;Cluster Energy (keV);Counts",200, -10., 90.);//energia del cluster asimmetrico
	HistoCluster3x3 = new TH1F("Cluster_3x3","Cluster signal 3x3;Cluster Energy (ADC);Counts",200,-50.,3950.);//energia del cluster 3x3 simmetrico
																																																						//  HistoCluster3x3keV = new TH1F("Cluster_3x3keV","Cluster signal 3x3 keV;Cluster Energy (keV);Counts",200,-50.,395.);
	HistoCluster5x5 = new TH1F("Cluster_5x5","Cluster signal 5x5;Cluster Energy (ADC);Counts",200,-50.,3950.);
	HistoCluster7x7 = new TH1F("Cluster_7x7","Cluster signal 7x7;Cluster Energy (ADC);Counts",200,-50.,3950.);
	HistoCluster9x9 = new TH1F("Cluster_9x9","Cluster signal 9x9;Cluster Energy (ADC);Counts",200,-50.,3950.);
	
	HistoClusterPrimEne = new TH1F("PrimEne","Energy of primary electron;E (keV);Counts",150,0,3000);
	
	
	HistoDelta_5x5_vs_3x3 = new TH1F("Delta_5x5vs3x3","",250,-50.,950.);                                  //"Delta"=verifica sull'isolamento topologico*
	HistoDelta_5x5_vs_3x3_norm = new TH1F("Delta_5x5vs3x3_norm","",100,-30.,70.);
	HistoDelta_7x7_vs_5x5 = new TH1F("Delta_7x7vs5x5","",250,-50.,950.);
	HistoDelta_7x7_vs_5x5_norm = new TH1F("Delta_7x7vs5x5_norm","",100,-30.,70.);
	HistoDelta_9x9_vs_7x7= new TH1F("Delta_9x9vs7x7","",250,-50.,950.);
	HistoDelta_9x9_vs_7x7_norm = new TH1F("Delta_9x9vs7x7_norm","",100,-30.,70.);
	
	
	V_vs_NpixClu = new TH2F("V_vs_NpixClu","Signal Vs Number of pixels in a cluster",10001.,-10.,10000.,50.,0.,50.);//grafico di correlazione
	NpixClu_vs_V = new TH2F("NpixClu_vs_V","Number of pixel in a cluster Vs Signal",50.,0.,50.,10001.,-10.,10000.); //grafico di correlazione
	HistoNpixClu= new TH1F("NpixClu","Number of pixel in a cluster",80,-1.,79.);                                    //numero di pixel appartenenti al cluster: *isolamento topologico
	HistoV_fract_NpixClu = new TH1F("E_vs_NpixClu","Signal normalized to the number of pixels",300, -10.,890.);
	HistoEPixMax = new TH1F("EpixelMax","Energy of max pixel",200,0.,1000.);
	V_vs_EPixMax = new TH2F("V_clu_vs_EPixMax","Cluster signal Vs Energy of max pixel",1000,0.,2000.,2500, 0.,2500.);//grafico di correlaz
	V_fract_NpixClu_vs_EPixMax = new TH2F("V_clu_fract_NpixClu_vs_EPixMax","Cluster signal normalized to N Vs Energy of max pixel",500,0.,1000.,2500, 0.,1500.);  //grafico di correlaz
	
	HistoRMSclu_Asy = new TH1F("RMSclu_Asy","RMS cluster asimmetrico",250,-10.,950.);                              //indice dispersione dell'energia del cluster
	RMS_vs_V = new TH2F("RMS_vs_V_clu","RMS Vs Signal of Cluster",1010,-10.,2000.,1500,0.,2500.);
	RMS_vs_V_fract_NpixClu = new TH2F("RMS_vs_V_clu_fract_NpixClu","RMS Vs Signal of Cluster Normalized to N",1010,-10.,1000.,1500,0.,2500.);
	NpixClu_vs_RMS = new TH2F("NpixClu_vs_RMS","Number of pixel in a cluster Vs RMS",500,-10.,1000.,1010,-10.,1000.);
	RMS_vs_EPixMax = new TH2F("RMS_vs_EPixMax","RMS Vs Energy of max pixel",1010,-10.,1000.,2500, 0.,3000.);
	HistoEmax_fract_E3x3 = new TH1F("Emax/E3x3","Rapporto energia pixel max ed energia matrice 3x3",250,0.,10.);
	HistoE_CluAsy_fract_NpixClu = new TH1F("E(asy)/NpixClu","Rapporto energia cluster asimmetrico e numero di pixel nel cluster",500,0.,1000.);
	
	E_CluAsy_fract_NpixClu_vs_3x3 = new TH2F("E(asy)/NpixClu_vs_3x3","E(asy)/N Vs Energia del cluster 3x3",500,0.,1000.,2500,0.,3000.);
	RMS_vs_3x3 = new TH2F ("RMS_vs_3x3","RMS Vs Segnale del cluster 3x3",1010,-10.,1000.,2500,0.,3000.);
	Emax_vs_3x3 = new TH2F ("Emax_vs_3x3","Emax Vs Energia del cluster 3x3",2500,0.,2500.,1000,0.,2500.);
	NpixClu_vs_3x3 = new TH2F ("NpixClu_vs_3x3","Numero di pixel nel cluster Vs Energia cluster 3x3",30,0.,30.,2500,0.,5000.);
	E_CluAsy_vs_3x3 = new TH2F ("E(asy)_vs_3x3","Energia cluster asimmetrico Vs Energia cluster 3x3",1500,0.,3000.,1000,0.,2000.);
	Emax_vs_NpixClu = new TH2F ("Emax_vs_NpixClu","Energia del pixel max Vs Numero di pixel nel cluster",2500,0.,100.,500,0.,1000.);
	E_CluAsy_fract_NpixClu_vs_E_CluAsy = new TH2F ("E(asy)/NpixClu_vs_E(asy)","E(asy)/N Vs Energia del cluster asimmetrico",1000,0.,2000.,1500,0.,3000.);
	E_CluAsy_fract_NpixClu_vs_NpixClu = new TH2F ("E(asy)/NpixClu_vs_NpixClu","E(asy)/N Vs Numero di pixel nel cluster",1000,0.,1000.,500,0.,100.);
	/////////////////////////////////////////////////////////////
	
	//********************FILE DI INPUT: badpixel.txt***********
	std::ifstream BadPixInFile(BadFileName.c_str(), std::ios::in);  //apro in lettura il file .txt dei bad pixels
	if (!BadPixInFile.is_open()) {
		cout<<"FILE NON TROVATO!"<< BadFileName.c_str() <<endl;
	}
	else{
		while(!BadPixInFile.eof()){
			int IDpixel=0, row=0, col=0, control=0, sigmas=0;
			BadPixInFile>>IDpixel>>col>>row>>control>>sigmas;
			//cout<< col<<" "<< row<<" "<<endl;
			ACPoint bad(col,row);
			Badpixels.push_back(bad);                                   //li metto nel vettore "Badpixels" (vedi Analisi.h)
		}
	}
	BadPixInFile.close();
	
	//********************FILE DI OUTPUT************************
	fOutFile= new TFile(OutFileName.c_str(),"Recreate");            //dichiarato già in Analisi.h; crea il file di output "NomeFileNNNN_Analized.root"
	
}

Analisi::~Analisi()
{
	cout<<"Numero Totale Seed: "<<map_seed->GetEntries()<<endl;
	cout<<"Numero seed a dist<10: "<<map_seedDist10->GetEntries()<<endl;
	
	delete HistoNSeeds;
	delete map_seed;
	delete map_seedDist10;
	delete map_seed_coarse;
	delete map_seed_coarse2;
	delete map_seed1_4;
	delete map_seed2_4;
	delete map_seed3_4;
	delete map_seed4_4;
	delete map_Row_seed;
	delete map_Col_seed;
	delete HistoDistSeeds;
	delete HistoDistmin;
	delete HistoDistmin_thr;
	delete HistoR_vs_Cluster;
	delete HistoV_single;
	delete HistoClusterAsy;
	delete HistoClusterAsy_keV;
	delete HistoCluster3x3;
	//  delete HistoCluster3x3keV;
	delete HistoCluster5x5;
	delete HistoCluster7x7;
	delete HistoCluster9x9;
	delete HistoDelta_5x5_vs_3x3;
	delete HistoDelta_5x5_vs_3x3_norm;
	delete HistoDelta_7x7_vs_5x5;
	delete HistoDelta_7x7_vs_5x5_norm;
	delete HistoDelta_9x9_vs_7x7;
	delete HistoDelta_9x9_vs_7x7_norm;
	delete V_vs_NpixClu;
	delete NpixClu_vs_V;
	delete HistoNpixClu;
	delete HistoV_fract_NpixClu;
	delete HistoEPixMax;
	delete V_vs_EPixMax;
	delete V_fract_NpixClu_vs_EPixMax;
	delete HistoRMSclu_Asy;
	delete RMS_vs_3x3;
	delete RMS_vs_V;
	delete RMS_vs_V_fract_NpixClu;
	delete NpixClu_vs_RMS;
	delete RMS_vs_EPixMax;
	delete HistoEmax_fract_E3x3;          //Energia pixel max : energia cluster3x3
	delete HistoE_CluAsy_fract_NpixClu;   //Energia cluster asimmetrico : numero di pixel nel cluster
	delete E_CluAsy_fract_NpixClu_vs_3x3;
	delete Emax_vs_3x3;
	delete NpixClu_vs_3x3;
	delete E_CluAsy_vs_3x3;
	delete Emax_vs_NpixClu;
	delete E_CluAsy_fract_NpixClu_vs_E_CluAsy;
	delete E_CluAsy_fract_NpixClu_vs_NpixClu;
}

void Analisi::WriteOnFile()
{
	HistoNSeeds->Write();
	map_seed->Write();
	map_seedDist10->Write();
	map_seed_coarse->Write();
	map_seed_coarse2->Write();
	map_seed1_4->Write();
	map_seed2_4->Write();
	map_seed3_4->Write();
	map_seed4_4->Write();
	map_Col_seed->Write();
	map_Row_seed->Write();
	HistoDistSeeds->Write();
	HistoDistmin->Write();
	HistoDistmin_thr->Write();
	HistoR_vs_Cluster->Write();
	HistoV_single->Write();
	HistoClusterAsy->Write();
	HistoClusterAsy_keV->Write();
	HistoCluster3x3->Write();
	//  HistoCluster3x3keV->Write();
	HistoCluster5x5->Write();
	HistoCluster7x7->Write();
	HistoCluster9x9->Write();
	HistoClusterPrimEne->Write();
	HistoDelta_5x5_vs_3x3->Write();
	HistoDelta_5x5_vs_3x3_norm->Write();
	HistoDelta_7x7_vs_5x5->Write();
	HistoDelta_7x7_vs_5x5_norm->Write();
	HistoDelta_9x9_vs_7x7->Write();
	HistoDelta_9x9_vs_7x7_norm->Write();
	V_vs_NpixClu->Write();
	NpixClu_vs_V->Write();
	HistoNpixClu->Write();
	HistoV_fract_NpixClu->Write();
	HistoEPixMax->Write();
	V_vs_EPixMax->Write();
	V_fract_NpixClu_vs_EPixMax->Write();
	HistoRMSclu_Asy->Write();
	RMS_vs_3x3->Write();
	RMS_vs_V->Write();
	RMS_vs_V_fract_NpixClu->Write();
	NpixClu_vs_RMS->Write();
	RMS_vs_EPixMax->Write();
	HistoEmax_fract_E3x3->Write();
	HistoE_CluAsy_fract_NpixClu->Write();
	E_CluAsy_fract_NpixClu_vs_3x3->Write();
	Emax_vs_3x3->Write();
	NpixClu_vs_3x3->Write();
	E_CluAsy_vs_3x3->Write();
	Emax_vs_NpixClu->Write();
	E_CluAsy_fract_NpixClu_vs_E_CluAsy->Write();
	E_CluAsy_fract_NpixClu_vs_NpixClu->Write();
	fOutFile->Write();
	fOutFile->Close();
}

int Analisi::AnalisiData (SeedList *sl, int FrameNCol, int FrameNRow, const Frame *FramePed, const Frame *FrameNoise, Double_t fcal, SeedList *slPrimEn, SeedList *slPrimN)
{
	int Row_seed=0;
	int Col_seed=0;
	double distSeed=0;
	double sogliaMinSeed=10;                         //distanza minima che devono avere i seed per appartenere allo stesso evento
	int SeedOK=0;
	bool debug = false;
	int PreviousPrimary = -1; // added on 16-10-2018 by collamaf+amorusor to avoid double counting primaries and having 120% eff
	
	for(size_t i=0; i<sl->Size(); i++)               //INIZIO 1° CICLO SULLA LISTA SEED
	{
		//cout<<"SEED_tot = "<<sl->Size()<<endl;                                      //check: numero dei seed per ogni frame
		Seed ts = sl->At(i);                                                        //accedo agli elementi appartenenti alla lista (vedi SeedList.h)
		
		double PrimEneVal=0;
		if (slPrimEn!=NULL)
			PrimEneVal=slPrimEn->At(i).GetVal(0);

		double PrimNVal=0;
		if (slPrimN!=NULL)
			PrimNVal=slPrimN->At(i).GetVal(0);
		
//		if(debug) cout<<"At(i="<<i<<") "<<ts<<endl;
		Row_seed = ts.GetRow();
		Col_seed = ts.GetCol();
//		if(debug) cout<<"Seed_i #"<<i<<": "<<"Col_i "<<Col_seed<<" Row_i "<<Row_seed<<endl;
		
		
		//////////////////////////////////////ELIMINO BAD PIXEL//////////////////////////////////
		
		int flagBadPixA=0;
		for(size_t i=0; i<Badpixels.size();i++)                                      //ciclo sui bad pixels
		{
			if(Badpixels.at(i).x == Col_seed && Badpixels.at(i).y == Row_seed)       //se la colonna e la riga del bad pixel coincidono con quelle del seed..
			{
				flagBadPixA=1;
				//cout<<".. è BAD!"<<endl;
				break;                                                               //..esco dal ciclo saltando il bad pixel
			}
		}
		if(flagBadPixA==1)
		{
			continue;                                                                //se flagBadPix è 1 ripeto il ciclo sui bad pixels per cercarne altri, altrimenti proseguo
		}
		
		////////////////////////////////////////////////////////////////////////////////////////
		
		
		/////////////////////////////////ELIMINAZIONE SEED VICINI///////////////////////////////
		
		int flagDistB=0;
		int Row_seed2 = 0;
		int Col_seed2 = 0;
		double distmin=1E12, distmin_thr=1E12;
		for(size_t j=0; j<sl->Size(); j++)                           //INIZIO 2° CILCO SULLA LISTA DEI SEED
		{
			Seed ts2 = sl->At(j);
			Row_seed2 = ts2.GetRow();
			Col_seed2 = ts2.GetCol();
//			if(debug) cout<<"Seed_j #"<<j<<": "<<"Col_j "<<Col_seed2<<" Row_j "<<Row_seed2<<endl;
			
			if((Row_seed == Row_seed2) && (Col_seed == Col_seed2))
			{
//				if(debug) cout<<"..è lo STESSO PIXEL!"<<endl;
				continue;
			}
			
			int flagBadPixB=0;
			for(size_t i=0; i<Badpixels.size();i++)                  //quando riscorro la lista per la 2a volta devo ritogliere i bad pixels
			{
				if(Badpixels.at(i).x == Col_seed2 && Badpixels.at(i).y == Row_seed2)
				{
					flagBadPixB=1;
					//cout<<".. è BAD!"<<endl;
					break;
				}
			}
			if(flagBadPixB==1)
			{
				continue;
			}
			
			int distX = Col_seed - Col_seed2;
			int distY = Row_seed - Row_seed2;
			distSeed = sqrt( pow(distX,2) + pow(distY,2) );
//			if(debug) cout<<"Distanza = "<<distSeed<<" Segnale_i = "<<ts(0,0)<<" Segnale_j = "<<ts2(0,0)<<endl;
			
			if(ts(0,0) < ts2(0,0))                                     //se il valore registrato dal seed 1 è minore di quello del seed 2..//questa condizione serve per non contare due volte la distanza tra due stessi seed
			{
				if(distSeed < distmin) distmin=distSeed;
				if((distSeed < distmin_thr) && (distSeed > sogliaMinSeed)) distmin_thr=distSeed;
				HistoDistSeeds->Fill(distSeed);
				//cout<<"->DistSeed: "<<distSeed<<endl;
			}
			
			if(distSeed <= sogliaMinSeed && (ts(0,0)<ts2(0,0)) )        //1a condizione utile per identificare il double counting: ho lo stesso evento se mi trovo entro 10 pixel; 2a condizione idem sopra
			{
				flagDistB=1;
				//cout<<"..è < 10!  "<<NSeedDouble<<endl;
			}
		}                                                            //FINE 2° CICLO SULLA LISTA DEI SEED
		
		if(flagDistB==1)
		{
			map_seedDist10->Fill((Col_seed),(Row_seed));
			continue;                                                  //tra i seed a dist<10 proseguono solo quelli più energetici
		}
		
		SeedOK +=1;                                                    //conto i seed buoni che ho avuto in questo frame
		
		HistoDistmin->Fill(distmin);
		HistoDistmin_thr->Fill(distmin_thr);
		
		////////////////////////////////////////////////////////////////////////////////////////
		
		
		///////////////////////////////Riempimento istogrammi mappe dei seed////////////////////////////////////////
		
		map_Row_seed->Fill(Row_seed);
		map_Col_seed->Fill(Col_seed);
		map_seed->Fill((Col_seed),(Row_seed));
		map_seed_coarse->Fill((Col_seed),(Row_seed));
		map_seed_coarse2->Fill((Col_seed),(Row_seed));
		map_seed1_4->Fill((Col_seed),(Row_seed));
		map_seed2_4->Fill((Col_seed),(Row_seed));
		map_seed3_4->Fill((Col_seed),(Row_seed));
		map_seed4_4->Fill((Col_seed),(Row_seed));
		
		////////////////////////////////////////////////////////////////////////////////////////
		
		
		////////////////////////Calcolo la distanza tra il centro del sensore e i seed////////////////////////
		
		double R_center_vs_cluster;
		R_center_vs_cluster = sqrt( pow( ( (FrameNCol/2) - Col_seed), 2) + pow( ( (FrameNRow/2)-Row_seed), 2) );
		HistoR_vs_Cluster->Fill(R_center_vs_cluster);
		
		////////////////////////////////////////////////////////////////////////////////////////
		
		
		
		//////////////////////////////Costruzione dei Cluster Asimmetrici//////////////////////////////////
		
		const int TOLERANCE_PIXELS = 2;                               //pixel di tolleranza nel calcolo del cluster asimmetrico: vengono annessi al cluster anche i pixel che superano la soglia secondaria (V_adja) e che distano fino a 2 pixel dal primo vicino
		double sigma;
		double V_adja=2.;                                             //soglia secondaria //fSecondaryThr prima era 4.0//prima era 2.6
		
		std::vector<ACPoint> preCluster;                              //conterrà le coordinate dei pixel con valore > V_adja (della matrice centrata nel seed salvata da Riduzione.x)
		std::vector<ACPoint> cluster;
		cluster.push_back(ACPoint(0,0));                              //inizializzo vettore che conterrà tutte le coord. del cluster asim.
		int N = 1;
		double EPixMax=0.;                                            //massimo valore registrato da un pixel all'interno della matrice: energia del seed
		int pixOverVadja=0;
		
		
		//INIZIO ciclo sulla matrice centrata nel seed (salvata da Riduzione.x) a partire dal pixel in alto a sx
		for (int k = ts.GetPixelMin(); k < ts.GetPixelMax(); k++)     //vedi Seed.h/Seed.C->p.e. per la matrice 7x7 sto scorrendo le RIGHE a partire dal pixel -3 fino al pixel +3 (rispetto al centro)
		{
			for (int l = ts.GetPixelMin(); l < ts.GetPixelMax(); l++) //COLONNE
			{
				float tsValue = ts(l,k);                              //valore registrato dal pixel alla posizione (l,k)
				if(tsValue>EPixMax)                                   //se il valore è maggiore di..
				{
					EPixMax=tsValue;                                  //..assegna a quest'ultimo l'etichetta di "pixel con massimo valore all'interno della matrice"
				}
				
				if (k == 0 && l == 0)                                  //se k=l=0 sono nel centro della matrice, quindi sono il seed..
				{
					continue;                                          //..quindi vado a cercare altri possibili candidati
				}
				
				sigma=FrameNoise->At(Col_seed+l,Row_seed+k);
				bool isOverThreshold = tsValue > V_adja*sigma;
				//cout<<"tsValue: "<<tsValue<<" V_adja: "<<V_adja<<" sigma: "<<sigma<<" V_adja*sigma: "<<V_adja*sigma<<endl;
				
				if (isOverThreshold)                                   //se sono sopra soglia..
				{
					pixOverVadja++;
					//cout<<"E' sopra soglia!!"<<" pixOverVadja="<<pixOverVadja<<endl;
					ACPoint currentPoint = ACPoint(l,k);
					preCluster.push_back(currentPoint);                //..riempio il vettore con le coord dei pixel possibili candidati
				}
			}
		}//FINE ciclo sulla matrice centrata nel seed
		
		
		bool found;                                                   //found=esiste almeno un pixel nel "preCluster" adiacente ad uno di quelli nel "cluster"?
		
		do {
			found = false;
			for (size_t i = 0; i < preCluster.size(); i++)              //giro sui possibili candidati che potrebbero essere inclusi nel cluster
			{
				ACPoint preClusterPoint = preCluster.at(i);             //i-esima posizione del vettore preCluster
				for (size_t j = 0; j < cluster.size(); j++)             //giro su tutti i pixel della matrice
				{
					ACPoint clusterPoint = cluster.at(j);
					if (std::abs(preClusterPoint.x-clusterPoint.x) <= TOLERANCE_PIXELS && std::abs(preClusterPoint.y-clusterPoint.y) <= TOLERANCE_PIXELS)                                                            //se la distanza è minore di 2 sia su x che su y..
					{
						N++;                                            //..aggiungo il pixel al cluster
						found = true;
						cluster.push_back(preClusterPoint);             //riempio il vettore con i pixel "vincitori"
						preCluster.erase(preCluster.begin()+i);         //rimuovo gli elementi del vettore dall'inizio fino all'i-esima posizione, in modo che quando arrivo alla fine il vettore sia completamente vuoto
						break;   //esco dal ciclo j (tutti i pixel)
					}
				}
				if (found)
				{
					break;       //esco dal ciclo i (candidati)
				}
			}
		} while (found);
		
		////////////////////////////////////////////////////////////////////////////////////////
		
		
		///////////////////////Calcolo Segnale Cluster Asimmetrico/////////////////////////
		
		double V_clu_Asy = 0;
		double V_clu_Asy_medio = 0.;
		for (size_t i = 0; i < cluster.size(); i++)                 //scorro la lista dei pixel appartenenti al cluster
		{
			ACPoint currentPoint = cluster.at(i);                   //accedo al pixel i-esimo del cluster
			V_clu_Asy += ts(currentPoint.x, currentPoint.y);        //sommo i segnali dei pixel appartenenti al cluster
		}
		V_clu_Asy_medio = (V_clu_Asy)/cluster.size();               //calcolo segnale medio del cluster
		
		////////////////////////////////////////////////////////////////////////////////////////
		
		
		/////////////////////////Calcolo RMS Cluster Asimmetrico////////////////////////
		
		double RMS_2 = 0.;
		double RMS_2_1 = 0.;
		double_t RMSclu_Asy = 0.;
		for (size_t i = 0; i < cluster.size(); i++)
		{
			ACPoint currentPoint = cluster.at(i);
			float tsValue = ts(currentPoint.x,currentPoint.y);      //valore del segnale del singolo pixel appartenente al cluster asimmetrico
			RMS_2_1 += (pow((tsValue - V_clu_Asy_medio),2));        //calcolo RMS^2
		}
		RMS_2 = cluster.size()>1 ? RMS_2_1/(cluster.size()-1) : 0 ; // -> espressione da testare ? azione_true : azione_false //in questo caso l'espressione da testare è cluster.size()>1 e se è vera RMS_2 vale "azione_true", altrimenti vale 0 ("azione_false")
		RMSclu_Asy = sqrt(RMS_2);
		
		////////////////////////////////////////////////////////////////////////////////////////
		
		
		////////////////////////////Calcolo Cluster Simmetrici///////////////////////////
		
		double V_clu_3x3 = 0.;
		double V_clu_5x5 = 0.;
		double V_clu_7x7 = 0.;
		double V_clu_9x9 = 0.;
		
		//ciclo sulla matrice centrata nel seed (salvata da Riduzione.x) a partire dal pixel in alto a sx
		for(int j=ts.GetPixelMin(); j<ts.GetPixelMax(); j++)        //RIGHE
		{
			for(int i=ts.GetPixelMin(); i<ts.GetPixelMax(); i++)    //COLONNE
			{
				HistoV_single->Fill(ts(i,j));                       //in questo istogramma si avranno, per ogni seed trovato, tutti i valori registrati dai pixel che appartengono alla matrice 3x3 centrata nel seed
				if(j>=-1 && j<=1 && i>=-1 && i<=1)
				{
					V_clu_3x3 += ts(i,j);
				}
				
				if(j>=-2 && j<=2 && i>=-2 && i<=2)
				{
					V_clu_5x5 += ts(i,j);
				}
				
				if(j>=-3 && j<=3 && i>=-3 && i<=3)
				{
					V_clu_7x7 += ts(i,j);
				}
				
				if(j>=-4 && j<=4 && i>=-4 && i<=4)
				{
					V_clu_9x9 += ts(i,j);
				}
			}
		}
		
		////////////////////////////////////////////////////////////////////////////////////////
		
		///////////////////////////Calcolo del Delta//////////////////////////
		
		double E_CluAsy_fract_NpixClu;
		double Emax_fract_E3x3 =0.;
		double Delta_5x5_vs_3x3 = 0.;
		double Delta_5x5_vs_3x3_norm = 0.;
		double Delta_7x7_vs_5x5 = 0.;
		double Delta_7x7_vs_5x5_norm = 0.;
		double Delta_9x9_vs_7x7 = 0.;
		double Delta_9x9_vs_7x7_norm = 0.;
		Delta_5x5_vs_3x3 = V_clu_5x5 - V_clu_3x3;
		Delta_5x5_vs_3x3_norm = (V_clu_5x5 - V_clu_3x3)/16;     //normalizzata al numero dei pixel contenuti nella cornice esterna: 25-9=16
		Delta_7x7_vs_5x5 = V_clu_7x7 -V_clu_5x5;
		Delta_7x7_vs_5x5_norm = (V_clu_7x7 - V_clu_5x5)/24;     //idem sopra
		Delta_9x9_vs_7x7 = V_clu_9x9 -V_clu_7x7;
		Delta_9x9_vs_7x7_norm = (V_clu_9x9 - V_clu_7x7)/32;
		
		////////////////////////////////////////////////////////////////////////////////////////
		
		
		///////////////////////////Altri calcoli/////////////////////////////
		
		Emax_fract_E3x3 =(float(EPixMax) / V_clu_3x3);
		E_CluAsy_fract_NpixClu = (V_clu_Asy /float(N));
		
		///////////////////////////Conversione in keV//////////////////////////
		//      HistoClusterAsy_keV
		double V_clu_3x3_keV = 0.;
		V_clu_3x3_keV = V_clu_3x3/fcal;                         //fcal: fattore di calibrazione ADC/keV
																														//      HistoCluster3x3keV->Fill(V_clu_3x3_keV);
																														////////////////////////////////////////////////////////////////////////////////////////
		
		///////////////////////RIEMPIMENTO ISTOGRAMMI/////////////////////////
		
		HistoClusterAsy->Fill(V_clu_Asy);
		HistoClusterAsy_keV->Fill(V_clu_Asy/fcal);
		HistoCluster3x3->Fill(V_clu_3x3);
		HistoCluster5x5->Fill(V_clu_5x5);
		HistoCluster7x7->Fill(V_clu_7x7);
		HistoCluster9x9->Fill(V_clu_9x9);
		
		if(debug) cout<<"i = "<<i<<" PreviousPrimary= "<<PreviousPrimary<<endl;
		if (PrimNVal!=PreviousPrimary) {
			if(debug) cout<<"Primario nuovo!!"<<endl;
			HistoClusterPrimEne->Fill(PrimEneVal); //se è un nuovo primario riempio l'istogramma (numeratore efficienza)
			PreviousPrimary=PrimNVal; //Mi segno che ho gia considerato le tracce derivanti da questo primario
		}
		else {
			if(debug) cout<<"Primario già considerato!"<<endl;
			
		}
		
		
		HistoDelta_5x5_vs_3x3->Fill(Delta_5x5_vs_3x3);
		HistoDelta_5x5_vs_3x3_norm->Fill(Delta_5x5_vs_3x3_norm);
		HistoDelta_7x7_vs_5x5->Fill(Delta_7x7_vs_5x5);
		HistoDelta_7x7_vs_5x5_norm->Fill(Delta_7x7_vs_5x5_norm);
		HistoDelta_9x9_vs_7x7->Fill(Delta_9x9_vs_7x7);
		HistoDelta_9x9_vs_7x7_norm->Fill(Delta_9x9_vs_7x7_norm);
		HistoEmax_fract_E3x3->Fill(Emax_fract_E3x3);
		HistoE_CluAsy_fract_NpixClu->Fill(E_CluAsy_fract_NpixClu);
		
		V_vs_NpixClu->Fill(float(V_clu_Asy),float(N));          //Nota: N è il numero di pixel nel cluster asimmetrico!
		NpixClu_vs_V->Fill(float(N),float(V_clu_Asy));
		HistoV_fract_NpixClu->Fill(float(V_clu_Asy_medio));
		HistoNpixClu->Fill(float(N));
		HistoEPixMax->Fill(float(EPixMax));
		HistoRMSclu_Asy->Fill(float(RMSclu_Asy));
		V_vs_EPixMax->Fill(float(EPixMax),float(V_clu_Asy));
		V_fract_NpixClu_vs_EPixMax->Fill(float(EPixMax),float(V_clu_Asy_medio));
		RMS_vs_V->Fill(float(V_clu_Asy),float(RMSclu_Asy));
		RMS_vs_V_fract_NpixClu->Fill(float(V_clu_Asy_medio),float(RMSclu_Asy));
		//*************************
		RMS_vs_3x3->Fill(float(RMSclu_Asy),V_clu_3x3);
		//**************************
		NpixClu_vs_RMS->Fill(float(RMSclu_Asy),float(N));
		RMS_vs_EPixMax->Fill(float(EPixMax),float(RMSclu_Asy));
		
		E_CluAsy_fract_NpixClu_vs_3x3->Fill(E_CluAsy_fract_NpixClu,V_clu_3x3);
		Emax_vs_3x3->Fill(float(EPixMax),V_clu_3x3);
		NpixClu_vs_3x3->Fill(float(N),V_clu_3x3);
		E_CluAsy_vs_3x3->Fill(V_clu_Asy,V_clu_3x3);
		Emax_vs_NpixClu->Fill(float(N),float(EPixMax));
		E_CluAsy_fract_NpixClu_vs_E_CluAsy->Fill(E_CluAsy_fract_NpixClu,float(V_clu_Asy));
		E_CluAsy_fract_NpixClu_vs_NpixClu->Fill(E_CluAsy_fract_NpixClu,float(N));
		
	}                                            //FINE 1° CICLO SULLA LISTA DEI SEED
//	cout<<endl;
	HistoNSeeds->Fill(SeedOK);
	
	return(0);
	
}
