#include "Analisi.h"
#include <iostream>
#include <fstream>

//using namespace std;

using std::endl;
using std::cout;

ClassImp(Analisi);


Analisi::Analisi(const std::string OutFileName, const std::string BadFileName, int FrameNCol, int FrameNRow)
{
  //********************GRAFICI***********************************
  HistoNCluster = new TH1F("NCluster","Numero di cluster nei frame",200,0.,200.);
  HistoNCluster->GetXaxis()->SetTitle("Numero di cluster");
  
  map_seed = new TH2F("mapofseed","Seedmap",FrameNCol,0.,FrameNCol,FrameNRow,0.,FrameNRow);
  map_seed_coarse = new TH2F("mapofseedcoarse8x8","Seedmapcoarse",8.,0.,FrameNCol,8.,0.,FrameNRow);           //mappa seed con bin larghi 8x8 pixel
  map_seed_coarse2 = new TH2F("mapofseedcoarse61x81","Seedmapcoarse61x81",61.,0.,FrameNCol,81.,0.,FrameNRow); //mappa seed con bin larghi 61x81 pixel
  map_seed1_4 = new TH2F("mapofseed 1di4", "mappa dei seed1",122.,0.,122.,FrameNRow,0.,FrameNRow);            //istogramma1 di 4: zoom su colonne da 0 a 122
  map_seed2_4 = new TH2F("mapofseed 2di4", "mappa dei seed2",122.,122.,244.,FrameNRow,0.,FrameNRow);          //istogramma2 di 4
  map_seed3_4 = new TH2F("mapofseed 3di4", "mappa dei seed3",122.,244.,366.,FrameNRow,0.,FrameNRow);          //istogramma3 di 4
  map_seed4_4 = new TH2F("mapofseed 4di4", "mappa dei seed4",122.,366.,488.,FrameNRow,0.,FrameNRow);          //istogramma4 di 4
  map_Row_seed = new TH1F("mapofRow","posizione seed sulle righe",FrameNRow,0.,FrameNRow);
  map_Col_seed = new TH1F("mapofCol","posizione seed sulle colonne",FrameNCol,0.,FrameNCol);

  HistoDistSeed = new TH1F("Distanza seed","Distanza relativa tra i seed",500.,0.,1000.);

  HistoDist_min_cluster = new TH1F("Distanza minima cluster","Distanza minima cluster",500.,0.,1000.);
  HistoDist_min_cluster->Draw();
  gPad->Update();
  TPaveStats *st2 = (TPaveStats*)HistoDist_min_cluster->FindObject("stats");
  st2->SetOptStat(1111111);   //Stampa l'integrale
  
  HistoR_vs_Cluster = new TH1F("Dist_Cen_Cluster","Distance between center and cluster",100,-50.,500.);        //distanza tra centro del sensore e la posizione di un seed

  ///////////////////////////////////////////////////////
  //Nei grafici seguenti:
  //-> NpixClu : numero di pixel in un cluster
  //-> V=k*s+n : segnale del cluster
  // dove s=segnale prodotto dalla particella
  //      k= frazione del segnale raccolto dal pixel
  //      n=rumore del pixel
  HistoV_single = new TH1F("Single pixel signal","Single pixel signal: DV",47750,-50.,1000.);//0.2 per bin     //segnale netto che ho per ogni pixel: DV=(s-ped), calcolato per ogni frame e per ogni pixel
  
  HistoClusterAsy = new TH1F("Cluster_asymmetric","Cluster asymmetric signal",1000, -50., 3000.);              //energia del cluster asimmetrico
  HistoClusterAsy->GetXaxis()->SetTitle("Energia Cluster (ADC)");
  HistoClusterAsy->GetYaxis()->SetTitle("Conteggi");
  
  HistoCluster3x3 = new TH1F("Cluster_3x3","Cluster signal 3x3",1000,-50.,2500.);                               //energia del cluster 3x3 simmetrico
  HistoCluster3x3->GetXaxis()->SetTitle("Energia Cluster (ADC)");
  HistoCluster3x3->GetYaxis()->SetTitle("Conteggi");
  
  HistoCluster5x5 = new TH1F("Cluster_5x5","Cluster signal 5x5",1000,-50.,2500.);
  HistoCluster5x5->GetXaxis()->SetTitle("Energia Cluster (ADC)");
  HistoCluster5x5->GetYaxis()->SetTitle("Conteggi");
  
  HistoCluster7x7 = new TH1F("Cluster_7x7","Cluster signal 7x7",1000,-50.,2500.);
  HistoCluster7x7->GetXaxis()->SetTitle("Energia Cluster (ADC)");
  HistoCluster7x7->GetYaxis()->SetTitle("Conteggi");
  
  HistoDelta_5x5_vs_3x3 = new TH1F("Delta 5x5 vs 3x3","",200,-200.,200.);                                       //"Delta"=verifica sull'isolamento topologico* 
  HistoDelta_5x5_vs_3x3_norm = new TH1F("Delta 5x5 vs 3x3 norm","",155,-200.,200.);
  HistoDelta_7x7_vs_5x5 = new TH1F("Delta 7x7 vs 5x5","",155,-200.,200.);
  HistoDelta_7x7_vs_5x5_norm = new TH1F("Delta 7x7 vs 5x5 norm","",155,-200.,200.);
  
  V_vs_NpixClu = new TH2F("V_vs_NpixClu","Signal Vs Number of pixels in a cluster",10001.,-10.,10000.,50.,0.,50.);//grafico di correlazione
  NpixClu_vs_V = new TH2F("NpixClu_vs_V","Number of pixel in a cluster Vs Signal",50.,0.,50.,10001.,-10.,10000.); //grafico di correlazione
  HistoNpixClu= new TH1F("NpixClu","Number of pixel in a cluster",60,0,60);                                       //numero di pixel appartenenti al cluster: *isolamento topologico
  HistoV_fract_NpixClu = new TH1F("E_vs_NpixClu","Signal normalized to the number of pixels",4000,-50.,2500.);
  HistoEPixMax = new TH1F("EpixelMax","Energy of max pixel",500,0.,1000.); 
  HistoContNeigh = new TH1F("ContNeigh","Frequency of neighbors pixels",100,-10.,200.);
  V_vs_EPixMax = new TH2F("V_clu_vs_EPixMax","Cluster signal Vs Energy of max pixel",1000,0.,2000.,2500, 0.,2500.);//grafico di correlaz
  V_fract_NpixClu_vs_EPixMax = new TH2F("V_clu_fract_NpixClu_vs_EPixMax","Cluster signal normalized to N Vs Energy of max pixel",500,0.,1000.,2500, 0.,1500.);  //grafico di correlaz
  
  HistoRMSclu_Asy = new TH1F("RMSclu_Asy","RMS cluster asimmetrico",1010,-10.,1000.);                                                                //indice dispersione dell'energia del cluster
  RMS_vs_V = new TH2F("RMS_vs_V_clu","RMS Vs Signal of Cluster",1010,-10.,2000.,1500,0.,2500.);
  RMS_vs_V_fract_NpixClu = new TH2F("RMS_vs_V_clu_fract_NpixClu","RMS Vs Signal of Cluster Normalized to N",1010,-10.,1000.,1500,0.,2500.);
  NpixClu_vs_RMS = new TH2F("NpixClu_vs_RMS","Number of pixel in a cluster Vs RMS",500,-10.,1000.,1010,-10.,1000.);
  RMS_vs_EPixMax = new TH2F("RMS_vs_EPixMax","RMS Vs Energy of max pixel",1010,-10.,1000.,2500, 0.,3000.);
  HistoEmax_fract_E3x3 = new TH1F("Emax/E3x3","Rapporto energia pixel max ed energia matrice 3x3",500,0.,10.);
  HistoE_CluAsy_fract_NpixClu = new TH1F("E(asy)/NpixClu","Rapporto energia cluster asimmetrico e numero di pixel nel cluster",1500,0.,1500.);
 
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
  cout<<"Numero Totale Cluster: "<<map_seed->GetEntries()<<endl;
  
  delete HistoNCluster;
  delete map_seed;
  delete map_seed_coarse;
  delete map_seed_coarse2;
  delete map_seed1_4;
  delete map_seed2_4;
  delete map_seed3_4;
  delete map_seed4_4;
  delete map_Row_seed;
  delete map_Col_seed;
  delete HistoDistSeed;
  delete HistoDist_min_cluster;
  delete HistoR_vs_Cluster;
  delete HistoV_single;
  delete HistoClusterAsy;
  delete HistoCluster3x3;
  delete HistoCluster5x5;
  delete HistoCluster7x7;
  delete HistoDelta_5x5_vs_3x3;
  delete HistoDelta_5x5_vs_3x3_norm;
  delete HistoDelta_7x7_vs_5x5;
  delete HistoDelta_7x7_vs_5x5_norm;
  delete V_vs_NpixClu;
  delete NpixClu_vs_V;
  delete HistoNpixClu;
  delete HistoV_fract_NpixClu;
  delete HistoEPixMax;
  delete HistoContNeigh;
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
  HistoNCluster->Write();
  map_seed->Write();
  map_seed_coarse->Write();
  map_seed_coarse2->Write();
  map_seed1_4->Write();
  map_seed2_4->Write();
  map_seed3_4->Write();
  map_seed4_4->Write();
  map_Col_seed->Write();
  map_Row_seed->Write();
  HistoDistSeed->Write();
  HistoDist_min_cluster->Write();
  HistoR_vs_Cluster->Write();
  HistoV_single->Write();
  HistoClusterAsy->Write();
  HistoCluster3x3->Write();
  HistoCluster5x5->Write();
  HistoCluster7x7->Write();
  HistoDelta_5x5_vs_3x3->Write();
  HistoDelta_5x5_vs_3x3_norm->Write();
  HistoDelta_7x7_vs_5x5->Write();
  HistoDelta_7x7_vs_5x5_norm->Write();
  V_vs_NpixClu->Write();
  NpixClu_vs_V->Write();
  HistoNpixClu->Write();
  HistoV_fract_NpixClu->Write();
  HistoEPixMax->Write();
  HistoContNeigh->Write();
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

int Analisi::AnalisiData (SeedList *sl, int FrameNCol, int FrameNRow)
{
  const int TOLERANCE_PIXELS = 2;                  //pixel di tolleranza nel calcolo del cluster asimmetrico: vengono annessi al cluster anche i pixel che superano la soglia secondaria e che distano fino a 2 pixel dal primo vicino
  
  int Row_seed = 0;                                //riga del seed
  int Col_seed = 0;                                //colonna del seed
  int Row_seed2 = 0;
  int Col_seed2 = 0;
  double distMinCluster = 10000, distSeed = 10000; //(??)
  int flagDist1=0, flagDist2=0;

  double sogliaMinSeed = 10;                       //distanza minima che devono avere i seed
  double V_adja = 2.;                              //soglia secondaria//fSecondaryThr prima era 4.0//prima era 2.6
  int cont = 0;
  int Ncluster = 0;
  
  for(size_t i=0; i<sl->Size(); i++)              //INIZIO 1° CICLO SULLA LISTA DEI SEED 
    {
      Seed ts = sl->At(i);                        //accedo agli elementi appartenenti alla lista dei seed
      Row_seed = ts.GetRow();                     //ad ogni seed (ts) vi associo la riga...
      Col_seed = ts.GetCol();                     //...e la colonna
      
      
      //////////////////////////////////////ELIMINO BAD PIXEL//////////////////////////////////
      
      int flagBadPix=0;
      for(size_t i=0; i<Badpixels.size();i++){
	if(Badpixels.at(i).x == Col_seed && Badpixels.at(i).y == Row_seed){    //se la colonna e la riga dei bad pixels coincidono con quelle relative ai seed...
	  flagBadPix=1;                                                        //...setto la flag uguale a 1..
	  break;                                                               //..e esco dal ciclo scartando il bad pixel
	}
      }
      //cout<<"flag: "<<flagBadPixel<<endl;
      if(flagBadPix==1) continue;                                              //se la flag è uguale a 1 ripeto il ciclo for per cercare altri bad pixel, altrimenti proseguo

      
      /////////////////////////////////ELIMINAZIONE SEED VICINI///////////////////////////////

      //cout<<"1 "<<Row_seed<<" "<<Col_seed<<endl;
      for(size_t j=0; j<sl->Size(); j++){            //INIZIO 2° CILCO SULLA LISTA DEI SEED
            
	Seed ts2 = sl->At(j);                        //rigiro sui seed e ridefinisco il seed come "ts2"
	Row_seed2 = ts2.GetRow();                    //il seed ts2 si troverà alla riga.. 
	Col_seed2 = ts2.GetCol();                    //..e alla colonna
	if((Row_seed == Row_seed2) && (Col_seed == Col_seed2)) continue;//se sto vedendo lo stesso seed nei due cicli, non lo considero
            
	int flagBadPix=0;
	for(size_t i=0; i<Badpixels.size();i++){     //quando riscorro la lista dei seed per la 2a volta devo ritogliere i bad pixel
	  if(Badpixels.at(i).x == Row_seed2 && Badpixels.at(i).y == Col_seed2){
	    flagBadPix=1;
	    break;
	  }
	}
	if(flagBadPix==1) continue;
            
	//cout<<"2 "<<Row_seed2<<" "<<Col_seed2<<endl;
            
	int distX = Col_seed - Col_seed2;
	int distY = Row_seed - Row_seed2;
            
	distSeed = sqrt(distX*distX+distY*distY);
	HistoDistSeed->Fill(distSeed); 
	flagDist1=0;
	//cout<<"Distanza "<<distSeed<<" Energia 1 "<<ts(0,0)<<" Energia 2 "<<ts2(0,0)<<endl;
            
	if(distSeed <= sogliaMinSeed && (ts(0,0) < ts2(0,0)) ){  //se la distanza tra i seed è minore di 10 e il valore del segnale registrato dal seed 1 è minore di quello del seed 2
	  flagDist1=1;
	  break;                                                 //...esco dal 2°ciclo sui seed e continuo la ricerca*: questi due seed quindi verranno considerati come un unico evento appartenenete allo stesso cluster
	}
      
	if(distSeed<=sogliaMinSeed)flagDist2=1;                  //se la distanza tra i seed è minore di 10 (?)

	if(distSeed<distMinCluster){
	  distMinCluster = distSeed;
	}
            
	if(distX <= 3 && distX >= -3 && distY <= 3 && distY >= -3 && (distX!=0 || distY!=0)){  //se mi trovo entro una matrice 7x7
	  cont++;
	}
      }                                              //FINE 2° CICLO SULLA LISTA DEI SEED

      if(flagDist1==1) continue;                     //*(vedi primo if)
        
      //if(distMinSeed != 10000){
      //cout<<endl<<"Dist min "<<distMinSeed<<endl;
      if(flagDist2==0)HistoDist_min_cluster->Fill(distMinCluster);
      //}
      distMinCluster = 10000;
      Ncluster += 1;
      
      
      ///////////////////////////////Riempimento istogrammi mappe dei seed////////////////////////////////////////
      
      map_Row_seed->Fill(Row_seed);
      map_Col_seed->Fill(Col_seed);
      map_seed->Fill((Col_seed),(Row_seed));         //perche cast?
      map_seed_coarse->Fill((Col_seed),(Row_seed));  //perche cast?
      map_seed_coarse2->Fill((Col_seed),(Row_seed));
      map_seed1_4->Fill((Col_seed),(Row_seed));
      map_seed2_4->Fill((Col_seed),(Row_seed));
      map_seed3_4->Fill((Col_seed),(Row_seed));
      map_seed4_4->Fill((Col_seed),(Row_seed));

      ////////////////////////Calcolo la distanza tra il centro del sensore e i seed////////////////////////
      
      double R_center_vs_cluster;
      R_center_vs_cluster = sqrt( pow( ( (FrameNCol/2) - Col_seed), 2) + pow( ( (FrameNRow/2)-Row_seed), 2) ); 
      HistoR_vs_Cluster->Fill(R_center_vs_cluster);                                                             
      
      //////////////////////////////Costruzione dei Cluster Asimmetrici//////////////////////////////////     
      
      //RF definire
      // char logStr[10000];
      // char logStr3[10000];
      // sprintf(logStr, ""); //?!?!
		
      std::vector<ACPoint> preCluster; // = vector<ACPoint>{};
      std::vector<ACPoint> cluster;    // = vector<ACPoint>{};
      cluster.push_back(ACPoint(0,0));
      int N = 1;
      double EPixMax=0.;                                            //pixel che ha il massimo valore di segnale all'interno del cluster: energia del seed
      
      for (int k = ts.GetPixelMin(); k < ts.GetPixelMax(); k++)
	{
	  for (int l = ts.GetPixelMin(); l < ts.GetPixelMax(); l++)
	    {
	      float tsValue = ts(l,k);                              //valore registrato dal pixel
	      if(tsValue>EPixMax)
		{
		  EPixMax=tsValue;
		}
	      
	      bool isOverThreshold = tsValue > V_adja;              //variabile che mi permette di valutare i pixel maggiori della soglia secondaria
	      
	      // if (isOverThreshold)
	      // 	{
	      // 	  sprintf(logStr, "%s[[%+04.0f]] ", logStr, tsValue);
	      // 	}
	      // else
	      // 	{
	      // 	  sprintf(logStr, "%s (%+04.0f)  ", logStr, tsValue);
	      // 	}
	      
	      
	      if (k == 0 && l == 0)
		{
		  continue;
		}
	      
	      if (isOverThreshold)
		{
		  ACPoint currentPoint = ACPoint(l,k);    //creo il punto corrispondente al k e l attuali nel ciclo
		  preCluster.push_back(currentPoint);     //scrivo il pixel che potrebbe appartenente al cluster
		  
		}
	    }
	  //sprintf(logStr, "%s\n", logStr);
	}
      
      //sprintf(logStr, "%s\n\n", logStr);
      
      bool found;                                         //found=esiste almeno un pixel nei precluster adiacente ad uno di quelli nel cluster?
      
      do {
	found = false;
	
	for (size_t i = 0; i < preCluster.size(); i++)
	  {
	    
	    ACPoint preClusterPoint = preCluster.at(i);    //dammi il punto all'i-sima posizione della lista di precluster
	    for (size_t j = 0; j < cluster.size(); j++)
	      {
		ACPoint clusterPoint = cluster.at(j);
		if (std::abs(preClusterPoint.x-clusterPoint.x) <= TOLERANCE_PIXELS && std::abs(preClusterPoint.y-clusterPoint.y) <= TOLERANCE_PIXELS)
		  {
		    
		    //sprintf(logStr, "%s\n(%d, %d) is near (%d, %d)", logStr, preClusterPoint.x, preClusterPoint.y, clusterPoint.x, clusterPoint.y);
		    N++;                                   //aumento il numero dei pixel nel cluster
		    found = true;
		    cluster.push_back(preClusterPoint);
		    preCluster.erase(preCluster.begin()+i);
		    break;
		  }
	      }
	    
	    if (found)
	      {
		break;
	      }
	  }
	
      } while (found);
      //sprintf(logStr, "%s\n\n", logStr);
      
      
      //cout << logStr;
      //char logStr2[10000];
      //sprintf(logStr2, "Result: ");
      
      
      ///////////////////////Calcolo Segnale Cluster Asimmetrico/////////////////////////
      
      double V_clu_Asy = 0;
      double V_clu_Asy_medio = 0.;
      //if(N==4)cout<<" cluster a 4 pixels"<< endl; //debug 4pixels
      //cout<< cluster.size()<< " "<< N << endl;
      for (size_t i = 0; i < cluster.size(); i++)                 //scorro la lista dei pixel appartenenti al cluster
	{
	  ACPoint currentPoint = cluster.at(i);                   //accedo al pixel i-esimo del cluster
	  //	  sprintf(logStr2, "%s (%d, %d) -", logStr2, currentPoint.x, currentPoint.y);
	  V_clu_Asy += ts(currentPoint.x, currentPoint.y);        //sommo i segnali dei pixel appartenenti al cluster
	  //  if(N==4)
	  // cout<<currentPoint.x<<" " <<currentPoint.y<<" "<<ts(currentPoint.x,currentPoint.y)<<endl;//debug 4pixels
	}
    V_clu_Asy_medio = (V_clu_Asy)/cluster.size();                 //calcolo segnale medio del cluster
      //sprintf(logStr2, "%s\n\n\n\n\n\n", logStr2);
      //cout << logStr2;
       
       /////////////////////////Calcolo RMS Cluster Asimmetrico////////////////////////
      
      double RMS_2 = 0.;
      double RMS_2_1 = 0.;
      double_t RMSclu_Asy = 0.;
      for (size_t i = 0; i < cluster.size(); i++)
	{
	  ACPoint currentPoint = cluster.at(i);
	  float tsValue = ts(currentPoint.x,currentPoint.y);      //valore del segnale del singolo pixel
	  //sprintf(logStr3, "%s[[%f]]\n ",logStr3,tsValue);
	  RMS_2_1 += (pow((tsValue - V_clu_Asy_medio),2));
	}
      RMS_2=cluster.size()>1 ? RMS_2_1/(cluster.size()-1) : 0 ;     // -> espressione da testare ? azione_true : azione_false
      RMSclu_Asy = sqrt(RMS_2);
      
      //////////////////////////////DEBUG RMS//////////////////////////////////////
      
      /* if (RMS_2 <1  && cluster.size()==15)
	 {
	 //cout << logStr;
	 //cout << logStr2;
	 //	  cout<<logStr3;
	 cout<<"V_clu ="<<V_clu_Asy<<endl;
	 cout<<"Npix ="<<cluster.size()<<endl;
	 cout<<"V_clu_fractN ="<<V_clu_fract_n<<endl;
	 cout<<"RMS2_1 ="<<RMS_2_1<<endl;
	 cout<<"RMS2 ="<<RMS_2<<endl;
	 cout<<"RMS =" << RMS_<<endl;
	 }*/
      
      ////////////////////////////Calcolo Cluster Simmetrici///////////////////////////

      double V_clu_3x3 = 0.;
      double V_clu_5x5 = 0.;
      double V_clu_7x7 = 0.;
      for(int j=ts.GetPixelMin(); j<ts.GetPixelMax(); j++)
	{
	  for(int i=ts.GetPixelMin(); i<ts.GetPixelMax(); i++)
	    {
	      HistoV_single->Fill(ts(i,j));
	      
	      if(j>=-1 && j<=1 && i>=-1 && i<=1)
		{
		  V_clu_3x3+=ts(i,j);
		}
	      
	      if(j>=-2 && j<=2 && i>=-2 && i<=2)
		{
		  V_clu_5x5+=ts(i,j);
		}
	      
	      if(j>=-3 && j<=3 && i>=-3 && i<=3)
		{
		  V_clu_7x7+=ts(i,j);
		}
	    }
	}
      
      ///////////////////////////Calcolo del Delta//////////////////////////

      double E_CluAsy_fract_NpixClu;
      double Emax_fract_E3x3 =0.;
      double Delta_5x5_vs_3x3 = 0.;
      double Delta_5x5_vs_3x3_norm = 0.;
      double Delta_7x7_vs_5x5 = 0.;
      double Delta_7x7_vs_5x5_norm = 0.;
      Delta_5x5_vs_3x3 = V_clu_5x5 - V_clu_3x3;
      Delta_5x5_vs_3x3_norm = (V_clu_5x5 - V_clu_3x3)/16;     //normalizzata al numero dei pixel contenuti nella cornice esterna: 25-9=16
      Delta_7x7_vs_5x5 = V_clu_7x7 -V_clu_5x5;
      Delta_7x7_vs_5x5_norm = (V_clu_7x7 - V_clu_5x5)/24;     //idem sopra

      ///////////////////////////Altri calcoli/////////////////////////////
      
      Emax_fract_E3x3 =(float(EPixMax) / V_clu_3x3);
      E_CluAsy_fract_NpixClu = (V_clu_Asy /float(N));         
      //RF qui calcoli
      
      ///////////////////////RIEMPIMENTO ISTOGRAMMI/////////////////////////
      
      V_vs_NpixClu->Fill(float(V_clu_Asy),float(N));
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
      
      HistoClusterAsy->Fill(V_clu_Asy);
      HistoCluster3x3->Fill(V_clu_3x3);
      HistoCluster5x5->Fill(V_clu_5x5);
      HistoCluster7x7->Fill(V_clu_7x7);
      
      HistoDelta_5x5_vs_3x3->Fill(Delta_5x5_vs_3x3);
      HistoDelta_5x5_vs_3x3_norm->Fill(Delta_5x5_vs_3x3_norm);
      HistoDelta_7x7_vs_5x5->Fill(Delta_7x7_vs_5x5);
      HistoDelta_7x7_vs_5x5_norm->Fill(Delta_7x7_vs_5x5_norm);
      HistoEmax_fract_E3x3->Fill(Emax_fract_E3x3); // qui calcoli
      HistoE_CluAsy_fract_NpixClu->Fill(E_CluAsy_fract_NpixClu);
      
      E_CluAsy_fract_NpixClu_vs_3x3->Fill(E_CluAsy_fract_NpixClu,V_clu_3x3);
      Emax_vs_3x3->Fill(float(EPixMax),V_clu_3x3);
      NpixClu_vs_3x3->Fill(float(N),V_clu_3x3);
      E_CluAsy_vs_3x3->Fill(V_clu_Asy,V_clu_3x3);
      Emax_vs_NpixClu->Fill(float(N),float(EPixMax));
      E_CluAsy_fract_NpixClu_vs_E_CluAsy->Fill(E_CluAsy_fract_NpixClu,float(V_clu_Asy));
      E_CluAsy_fract_NpixClu_vs_NpixClu->Fill(E_CluAsy_fract_NpixClu,float(N));
      
    }                                            //FINE 1° CICLO SULLA LISTA DEI SEED
  
  /////////////////////////Conteggio Seed Vicini///////////////////////////
  
  for(size_t i=0; i<sl->Size(); i++)
    {
      Seed ts = sl->At(i);
      for(size_t j=i+1; j<sl->Size(); j++)
	{
	  Seed ts2 = sl->At(j);
	  
	  int distX = ts.GetCol()-ts2.GetCol();
	  int distY = ts.GetRow()-ts2.GetRow();
	  
	  if(distX <= 3 && distX >= -3 && distY <= 3 && distY >= -3 && (distX!=0 || distY!=0))
	    {
	      cont++;
	    }
	}
    }
  HistoContNeigh->Fill(cont);
  HistoNCluster->Fill(Ncluster);
  
  /* row_med /= sl->Size();
     col_med /= sl->Size();
     cout<<row_med<<" "<<col_med<<endl;
  */
  return(0);
  
}
