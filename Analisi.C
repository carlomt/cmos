#include "Analisi.h"
#include <iostream>
#include <fstream>

//using namespace std;

using std::endl;
using std::cout;

ClassImp(Analisi);


Analisi::Analisi(const std::string OutFileName, const std::string BadFileName)
{
	map_seed = new TH2F("mapofseed","seedmap",480.,0.,480.,640.,0.,640.);
	map_seed_coarse = new TH2F("mapofseedcoarse8x8","seedmapcoarse",61.,0.,480.,81.,0.,640.); //mappa seed con bin larghi 8x8 pixel
	map_seed_coarse2 = new TH2F("mapofseedcoarse61x81","seedmapcoarse",8.,0.,480.,8.,0.,640.); //mappa seed con bin larghi 61x81 pixel
	map_seed1_4 = new TH2F("mapofseed 1di4", "mappa dei seed1",122.,0.,122.,648.,0.,648.); //istogramma1 di 4
	map_seed2_4 = new TH2F("mapofseed 2di4", "mappa dei seed2",122.,122.,244.,648.,0.,648.); //istogramma2 di 4
	map_seed3_4 = new TH2F("mapofseed 3di4", "mappa dei seed3",122.,244.,366.,648.,0.,648.); //istogramma3 di 4
	map_seed4_4 = new TH2F("mapofseed 4di4", "mappa dei seed4",122.,366.,488.,648.,0.,648.); //istogramma4 di 4
	map_Row_seed = new TH1F("mapofRow","posizione seed sulle righe",488.,0.,488.);
	map_Col_seed = new TH1F("mapofCol","posizione seed sulle colonne",648.,0.,648.);
	R_center_vs_cluster_dist = new TH1F("Dist_Cen_Clu","Distance between center and cluster",100,-50.,500.);
	V_single = new TH1F("Sigle_pixel_signal","Sigle pixel signal DV",47750,-50.,9500.);//0.2 per bin
	V_clu_Asy = new TH1F("Cluster_asymmetric","Cluster asymmetric signal",4025, -50., 8000.);///
	V_clu_3x3_dist = new TH1F("Cluster_ 3x3","Cluster signal 3x3",2500,-50.,200.);
	V_clu_5x5_dist = new TH1F("Cluster_5x5","Cluster signal 5x5",20500,-50.,2000.);
	V_clu_7x7_dist = new TH1F("Cluster_7x7","Cluster signal 7x7",20500,-50.,2000.);
	Delta_5x5_vs_3x3_dist = new TH1F("Delta 5x5 vs 3x3","",155,-50.,1500.);///
	Delta_5x5_vs_3x3_norm_dist = new TH1F("Delta 5x5 vs 3x3 norm","",155,-50.,1500.);///
	Delta_7x7_vs_5x5_dist = new TH1F("Delta 7x7 vs 5x5","",155,-50.,1500.);///
	Delta_7x7_vs_5x5_norm_dist = new TH1F("Delta 7x7 vs 5x5 norm","",155,-50.,1500.);///
	V_vs_npixel_cluster = new TH2F("V_vs_npixel_cluster","Signal vs number of pixels in a cluster",10001.,-10.,10000.,50.,0.,50.);
	npixel_cluster_vs_V = new TH2F("npixel_cluster_vs_V","Number of pixel in a cluster vs Signal",50.,0.,50.,10001.,-10.,10000.);
	N_pixel_per_cluster = new TH1F("Number of pixel in a cluster","",60,0,60);
	V_clu_fract_N = new TH1F("E_vs_N","Signal normalized to the number of pixels",4000,-50.,5050.);
	VpixelMax = new TH1F("EpixelMax","Energy of max pixel",500,0.,50.);///
	ContNeigh = new TH1F("ContNeigh","Frequency of neighbors pixels",100,-10.,200.);
	V_clu_vs_EPixMax = new TH2F("V_clu_vs_EPixMax","Cluster signal vs energy of max pixel",1000,0.,2000.,2500, 0.,5000.);
	V_clu_fractN_vs_EPixMax = new TH2F("V_clu_fractN_vs_EPixMax","Cluster signal normalized to N vs Energy of max pixel",500,0.,1000.,2500, 0.,5000.);
	RMS = new TH1F("RMS","RMS",1010,-10.,1000.);
	RMS_vs_V_clu = new TH2F("RMS_vs_V_clu","RMS vs Signal of Cluster",1010,-10.,1000.,1500,0.,3000.);///
	RMS_vs_V_clu_fract_N = new TH2F("RMS_vs_V_clu_fract_N","RMS vs Signal of Cluster Normalized to N",1010,-10.,1000.,1500,0.,3000.);///
	N_vs_RMS = new TH2F("N_vs_RMS","Number of pixel in a cluster vs RMS",500,-10.,300.,1010,-10.,1000.);
	RMS_vs_EPixMax = new TH2F("RMS_vs_EPixMax","RMS vs Energy of max pixel",1010,-10.,1000.,2500, 0.,5000.);
	Emax_div_E3x3_dist = new TH1F("Emax div E3x3","rapporto Energia pixel max ed energia matrice 3x3",500,0.,10.);
	Eas_div_Npix_dist = new TH1F("Eas div Npix","rapporto Energia cluster asimmetrico ed numero di pixel nel cluster",1500,0.,3000.);
	
	//****************************miei TH2F*************************
	EasDivN_vs_3x3 = new TH2F("EasDivN_vs_3x3","Eas/N ed energia del cluster3x3",500,0.,1000.,2500,0.,5000.); ///
	RMS_vs_3x3 = new TH2F ("RMS_vs_3x3","RMS e segnale del cluster3x3",1010,-10.,1000.,2500,0.,5000.);///
	Emax_vs_3x3 = new TH2F ("Emax_vs_3x3","Emax e energia del cluster 3x3",2500,0.,5000.,1000,0.,2000.);
	N_vs_3x3 = new TH2F ("N_vs_3x3","numero di pixel nel cluster e energia cluster 3x3",30,0.,30.,2500,0.,5000.);
	Easy_vs_3x3 = new TH2F ("Easy_vs_3x3","energia cluster asy ed energia cluster 3x3",1500,0.,3000.,1000,0.,2000.);
	Emax_vs_N = new TH2F ("Emax_vs_N","energia del pixel max e il numero di pixel nel cluster",2500,0.,5000.,500,0.,200.);
	EasDivN_vs_Easy = new TH2F ("EasDivN_vs_Easy","EasDivN ed energia del cluster asimmetrico",1000,0.,2000.,1500,0.,3000.);
	EasDivN_vs_N = new TH2F ("EasDivN_vs_N","EasDivN e numero di pixel nel cluster",1000,0.,2000.,500,0.,100.);
	
	//*********************ALBERO******************************
	Albero = new TTree ("Albero","Variabili per Fisher");
	// Albero->Branch("E3x3",&V_clu_3x3,"E3x3/D");
	Albero->Branch("RMS_nome_esteso",&RMS_nome_esteso,"RMS_nome_esteso/D");
	// Albero->Branch("Nclu",&N,"Nclu/I");
	// Albero->Branch("EasDivN",&Eas_div_Npix,"EasDivN/D");
	fOutFile= new TFile(OutFileName.c_str(),"Recreate");
	
	//std::ifstream BadPixInFile("badpixel.txt",std::ios::in);
	std::ifstream BadPixInFile(BadFileName.c_str(), std::ios::in);  //apro in lettura il file .txt dei bad pixels
	if (!BadPixInFile.is_open()) {
	  cout<<"FILE NON TROVATO!"<< BadFileName.c_str() <<endl;
	}
	else{
	  while(!BadPixInFile.eof()){
	    int IDpixel=0, row=0, col=0, control=0, sigmas=0;
	    BadPixInFile>>IDpixel>>col>>row>>control>>sigmas;
	    //		cout<< col<<" "<< row<<endl;
	    ACPoint bad(col,row);
	    Badpixels.push_back(bad);                                   //li metto nel vettore "Badpixels" (vedi Analisi.h)
	  }
	}
	BadPixInFile.close();
}


Analisi::~Analisi()
{
	delete map_seed;
	delete map_seed_coarse;
	delete map_seed_coarse2;
	delete map_seed1_4;
	delete map_seed2_4;
	delete map_seed3_4;
	delete map_seed4_4;
	delete map_Row_seed;
	delete map_Col_seed;
	delete R_center_vs_cluster_dist;
	delete V_single;
	delete V_clu_Asy;
	delete V_clu_3x3_dist;
	delete V_clu_5x5_dist;
	delete V_clu_7x7_dist;
	delete Delta_5x5_vs_3x3_dist;
	delete Delta_5x5_vs_3x3_norm_dist;
	delete Delta_7x7_vs_5x5_dist;
	delete Delta_7x7_vs_5x5_norm_dist;
	delete V_vs_npixel_cluster;
	delete npixel_cluster_vs_V;
	delete N_pixel_per_cluster;
	delete V_clu_fract_N;
	delete VpixelMax;
	delete ContNeigh;
	delete V_clu_vs_EPixMax;
	delete V_clu_fractN_vs_EPixMax;
	delete RMS;
	delete RMS_vs_3x3;
	delete RMS_vs_V_clu;
	delete RMS_vs_V_clu_fract_N;
	delete N_vs_RMS;
	delete RMS_vs_EPixMax;
	delete Emax_div_E3x3_dist;   //Energia pixel max :energia cluster3x3
	delete Eas_div_Npix_dist;   //energia cluster asimmetrico:numero di pixel nel cluster
	delete EasDivN_vs_3x3;
	delete Emax_vs_3x3;
	delete N_vs_3x3;
	delete Easy_vs_3x3;
	delete Emax_vs_N;
	delete EasDivN_vs_Easy;
	delete EasDivN_vs_N;
	delete Albero;
}

void Analisi::WriteOnFile()
{
	
	map_seed->Write();
	map_seed_coarse->Write();
	map_seed_coarse2->Write();
	map_seed1_4->Write();
	map_seed2_4->Write();
	map_seed3_4->Write();
	map_seed4_4->Write();
	map_Col_seed->Write();
	map_Row_seed->Write();
	R_center_vs_cluster_dist->Write();
	V_single->Write();
	V_clu_Asy->Write();
	V_clu_3x3_dist->Write();
	V_clu_5x5_dist->Write();
	V_clu_7x7_dist->Write();
	Delta_5x5_vs_3x3_dist->Write();
	Delta_5x5_vs_3x3_norm_dist->Write();
	Delta_7x7_vs_5x5_dist->Write();
	Delta_7x7_vs_5x5_norm_dist->Write();
	V_vs_npixel_cluster->Write();
	npixel_cluster_vs_V->Write();
	N_pixel_per_cluster->Write();
	V_clu_fract_N->Write();
	VpixelMax->Write();
	ContNeigh->Write();
	V_clu_vs_EPixMax->Write();
	V_clu_fractN_vs_EPixMax->Write();
	RMS->Write();
	RMS_vs_3x3->Write();
	RMS_vs_V_clu->Write();
	RMS_vs_V_clu_fract_N->Write();
	N_vs_RMS->Write();
	RMS_vs_EPixMax->Write();
	Emax_div_E3x3_dist->Write(); //scrivo
	Eas_div_Npix_dist->Write();  //scrivo
	EasDivN_vs_3x3->Write();
	Emax_vs_3x3->Write();
	N_vs_3x3->Write();
	Easy_vs_3x3->Write();
	Emax_vs_N->Write();
	EasDivN_vs_Easy->Write();
	EasDivN_vs_N->Write();
	Albero->Write();
	fOutFile->Write();
	fOutFile->Close();
	
}



int Analisi::AnalisiData (SeedList *sl)
{
	const int TOLERANCE_PIXELS = 2;
	
	int Row_seed = 0;
	int Col_seed = 0;
	
	double V_adja = 2.; //fSecondaryThr prima era 4.0//prima era 2.6
	int cont = 0;
	
	
	for(size_t i=0; i<sl->Size(); i++)
	{
		Seed ts = sl->At(i);
		Row_seed = ts.GetRow();
		Col_seed = ts.GetCol();
		double EPixMax=0.;
		N = 1;
		
		
		//////////////////////////////////////ELIMINO BAD PIXEL//////////////////////////////////
		int flag=0;
		for(int i=0; i<Badpixels.size();i++){
			if(Badpixels.at(i).x == Col_seed && Badpixels.at(i).y == Row_seed){
				flag=1;
				
				cout<<"Row seed: "<<Col_seed <<" Col_seed: "<< Row_seed<<endl;
				cout<<"BadPixel x: "<<Badpixels.at(i).x <<" BadPixel y: "<< Badpixels.at(i).y<<endl;
				break;
			}
		}
		//cout<<"flag: "<<flag<<endl;
		if(flag==1) continue;
		
		
		/////////////////////////////////////////////////////////////////////////////////////////
		
		map_Row_seed->Fill(Row_seed);
		map_Col_seed->Fill(Col_seed);
		map_seed->Fill((Col_seed),(Row_seed)); //perche cast?
		map_seed_coarse->Fill((Col_seed),(Row_seed)); //perche cast?
		map_seed_coarse2->Fill((Col_seed),(Row_seed));
		map_seed1_4->Fill((Col_seed),(Row_seed));
		map_seed2_4->Fill((Col_seed),(Row_seed));
		map_seed3_4->Fill((Col_seed),(Row_seed));
		map_seed4_4->Fill((Col_seed),(Row_seed));
		double R_center_vs_cluster;
		R_center_vs_cluster = sqrt(pow((240.-Col_seed),2) +pow((320.-Row_seed),2));
		R_center_vs_cluster_dist->Fill(R_center_vs_cluster);
		
		
		double V_clu_fract_n = 0;
		double V_clu_A = 0.;
		V_clu_3x3 = 0.;// xk già in .h
		double V_clu_5x5 = 0.;
		double V_clu_7x7 = 0.;
		double Delta_5x5_vs_3x3 = 0.;
		double Delta_5x5_vs_3x3_norm = 0.;
		double Delta_7x7_vs_5x5 = 0.;
		double Delta_7x7_vs_5x5_norm = 0.;
		double RMS_2 = 0.;
		double RMS_2_1 = 0.;
		RMS_nome_esteso = 0.;
		Double_t Emax_div_E3x3 =0.;
		Eas_div_Npix =0.;
		//RF definire
		// char logStr[10000];
		// char logStr3[10000];
		// sprintf(logStr, ""); //?!?!
		
		std::vector<ACPoint> preCluster;// = vector<ACPoint>{};
		std::vector<ACPoint> cluster;// = vector<ACPoint>{};
		cluster.push_back(ACPoint(0,0));
		
		for (int k = ts.GetPixelMin(); k < ts.GetPixelMax(); k++)
		{
			for (int l = ts.GetPixelMin(); l < ts.GetPixelMax(); l++)
			{
				float tsValue = ts(l,k);
				if(tsValue>EPixMax)
				{
					EPixMax=tsValue;
				}
				
				bool isOverThreshold = tsValue > V_adja; //cerco tutti i pixel maggiori della soglia secondaria
				
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
					ACPoint currentPoint = ACPoint(l,k); //creo il punto corrispondente al k e l attuali nel ciclo
					preCluster.push_back(currentPoint);//scrivo il  pixel elegibile
					
				}
			}
			//	  sprintf(logStr, "%s\n", logStr);
		}
		
		//      sprintf(logStr, "%s\n\n", logStr);
		
		bool found; // found = esiste almeno un pixel nei precluster adiacente ad uno di quelli nel cluster?
		
		do {
			found = false;
			
			for (size_t i = 0; i < preCluster.size(); i++)
			{
				
				ACPoint preClusterPoint = preCluster.at(i); //dammi il punto all'i-sima posizione della lista di precluster
				for (size_t j = 0; j < cluster.size(); j++)
				{
					ACPoint clusterPoint = cluster.at(j);
					if (std::abs(preClusterPoint.x-clusterPoint.x) <= TOLERANCE_PIXELS && std::abs(preClusterPoint.y-clusterPoint.y) <= TOLERANCE_PIXELS)
					{
						
						//		    sprintf(logStr, "%s\n(%d, %d) is near (%d, %d)", logStr, preClusterPoint.x, preClusterPoint.y, clusterPoint.x, clusterPoint.y);
						N++;//numero pixel nel cluster
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
		//      sprintf(logStr, "%s\n\n", logStr);
		
		
		//      cout << logStr;
		// char logStr2[10000];
		//      sprintf(logStr2, "Result: ");
		
		///////////////////////Calcolo Cluster Asimmetrico/////////////////////////
		//      if(N==4)cout<<" cluster a 4 pixels"<< endl; //debug 4pixels
		//cout<< cluster.size()<< " "<< N << endl;
		for (size_t i = 0; i < cluster.size(); i++)
		{
			ACPoint currentPoint = cluster.at(i);
			//	  sprintf(logStr2, "%s (%d, %d) -", logStr2, currentPoint.x, currentPoint.y);
			V_clu_A += ts(currentPoint.x, currentPoint.y);
			//  if(N==4)
			// cout<<currentPoint.x<<" " <<currentPoint.y<<" "<<ts(currentPoint.x,currentPoint.y)<<endl;//debug 4pixels
		}
		V_clu_fract_n = (V_clu_A)/cluster.size();
		//      sprintf(logStr2, "%s\n\n\n\n\n\n", logStr2);
		//      cout << logStr2;
		
		//////////////////////Calcolo RMS Cluster Asimmetrico/////////////////////
		
		for (size_t i = 0; i < cluster.size(); i++)
		{
			ACPoint currentPoint = cluster.at(i);
			float tsValue = ts(currentPoint.x,currentPoint.y);
			//	  sprintf(logStr3, "%s[[%f]]\n ",logStr3,tsValue);
			RMS_2_1+= (pow((tsValue - V_clu_fract_n),2));
			
		}
		RMS_2=cluster.size()>1 ? RMS_2_1 /(cluster.size()-1):0;
		RMS_nome_esteso = sqrt(RMS_2);
		
		//////////////////////////////DEBUG RMS//////////////////////////////////////
		
		/* if (RMS_2 <1  && cluster.size()==15)
		 {
		 //cout << logStr;
		 //cout << logStr2;
		 //	  cout<<logStr3;
		 cout<<"V_clu ="<<V_clu_A<<endl;
		 cout<<"Npix ="<<cluster.size()<<endl;
		 cout<<"V_clu_fractN ="<<V_clu_fract_n<<endl;
		 cout<<"RMS2_1 ="<<RMS_2_1<<endl;
		 cout<<"RMS2 ="<<RMS_2<<endl;
		 cout<<"RMS =" << RMS_<<endl;
		 }*/
		
		///////////////////////Calcolo Cluster Simmetrici/////////////////////
		
		for(int j=ts.GetPixelMin(); j<ts.GetPixelMax(); j++)
		{
			for(int i=ts.GetPixelMin(); i<ts.GetPixelMax(); i++)
			{
				V_single->Fill(ts(i,j));
				
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
		
		////////////////////////////Calcolo del Delta///////////////////////////
		
		Delta_5x5_vs_3x3 = V_clu_5x5 - V_clu_3x3;
		Delta_5x5_vs_3x3_norm = (V_clu_5x5 - V_clu_3x3)/16;
		Delta_7x7_vs_5x5 = V_clu_7x7 -V_clu_5x5;
		Delta_7x7_vs_5x5_norm = (V_clu_7x7 - V_clu_5x5)/24;
		Emax_div_E3x3 =(float(EPixMax) / V_clu_3x3);
		Eas_div_Npix = (V_clu_A /float(N));
		//RF qui calcoli
		
		///////////////////////Riempimento Istogrammi//////////////////////////
		
		V_vs_npixel_cluster->Fill(float(V_clu_A),float(N));
		npixel_cluster_vs_V->Fill(float(N),float(V_clu_A));
		V_clu_fract_N->Fill(float(V_clu_fract_n));
		N_pixel_per_cluster->Fill(float(N));
		VpixelMax->Fill(float(EPixMax));
		RMS->Fill(float(RMS_nome_esteso));
		V_clu_vs_EPixMax->Fill(float(EPixMax),float(V_clu_A));
		V_clu_fractN_vs_EPixMax->Fill(float(EPixMax),float(V_clu_fract_n));
		RMS_vs_V_clu->Fill(float(V_clu_A),float(RMS_nome_esteso));
		RMS_vs_V_clu_fract_N->Fill(float(V_clu_fract_n),float(RMS_nome_esteso));
		//********************
		RMS_vs_3x3->Fill(float(RMS_nome_esteso),V_clu_3x3);
		//**************************
		N_vs_RMS->Fill(float(RMS_nome_esteso),float(N));
		RMS_vs_EPixMax->Fill(float(EPixMax),float(RMS_nome_esteso));
		V_clu_Asy->Fill(V_clu_A);
		V_clu_3x3_dist->Fill(V_clu_3x3);
		V_clu_5x5_dist->Fill(V_clu_5x5);
		V_clu_7x7_dist->Fill(V_clu_7x7);
		Delta_5x5_vs_3x3_dist->Fill(Delta_5x5_vs_3x3);
		Delta_5x5_vs_3x3_norm_dist->Fill(Delta_5x5_vs_3x3_norm);
		Delta_7x7_vs_5x5_dist->Fill(Delta_7x7_vs_5x5);
		Delta_7x7_vs_5x5_norm_dist->Fill(Delta_7x7_vs_5x5_norm);
		Emax_div_E3x3_dist->Fill(Emax_div_E3x3); // qui calcoli
		Eas_div_Npix_dist->Fill(Eas_div_Npix);
		EasDivN_vs_3x3->Fill(Eas_div_Npix,V_clu_3x3);
		Emax_vs_3x3->Fill(float(EPixMax),V_clu_3x3);
		N_vs_3x3->Fill(float(N),V_clu_3x3);
		Easy_vs_3x3->Fill(V_clu_A,V_clu_3x3);
		Emax_vs_N->Fill(float(N),float(EPixMax));
		EasDivN_vs_Easy->Fill(Eas_div_Npix,float(V_clu_A));
		EasDivN_vs_N->Fill(Eas_div_Npix,float(N));
		//      if(N>0)
		{
			Albero->Fill();
			//cout<<N<< " "<<Eas_div_Npix<< endl;
		}
		
	}
	
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
	ContNeigh->Fill(cont);
	
	/* row_med /= sl->Size();
	 col_med /= sl->Size();
	 cout<<row_med<<" "<<col_med<<endl;
	 */
	return(0);
	
}
