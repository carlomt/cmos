#include "Analisi.h"

//using namespace std;

using std::endl;
using std::cout;

ClassImp(Analisi);


Analisi::Analisi()
{
  map_seed = new TH2F("mapofseed","seedmap",480.,0.,480.,640.,0.,640.);
  R_center_vs_cluster_dist = new TH1F("Dist_Cen_Clu","Distance between center and cluster",8000,-50.,1800.);
  V_single = new TH1F("Sigle_pixel_signal","Sigle pixel signal DV",8000,-50.,950.);
  V_clu_Asy = new TH1F("Cluster_asymmetric","Cluster asymmetric signal",8000, -50., 5050.);
  V_clu_3x3_dist = new TH1F("Cluster_ 3x3","Cluster signal 3x3",8000,-50.,9050.);
  V_clu_5x5_dist = new TH1F("Cluster_5x5","Cluster signal 5x5",8000,-50.,9050.);
  V_clu_7x7_dist = new TH1F("Cluster_7x7","Cluster signal 7x7",8000,-50.,9050.);
  Delta_5x5_vs_3x3_dist = new TH1F("Delta 5x5 vs 3x3","",8000,-50.,9050.);
  Delta_5x5_vs_3x3_norm_dist = new TH1F("Delta 5x5 vs 3x3 norm","",8000,-50.,9050.);
  Delta_7x7_vs_5x5_dist = new TH1F("Delta 7x7 vs 5x5","",8000,-50.,9050.);
  Delta_7x7_vs_5x5_norm_dist = new TH1F("Delta 7x7 vs 5x5 norm","",8000,-50.,9050.);
  V_vs_npixel_cluster = new TH2F("V_vs_npixel_cluster","Signal vs number of pixels in a cluster",2000.,-10.,1000.,50.,0.,50.);
  npixel_cluster_vs_V = new TH2F("npixel_cluster_vs_V","Number of pixel in a cluster vs Signal",50.,0.,50.,2000.,-10.,1000.);
  N_pixel_per_cluster = new TH1F("Number of pixel in a cluster","",49,0.,49.);
  V_clu_fract_N = new TH1F("E_vs_N","Signal normalized to the number of pixels",8000,-50.,5050.);
  VpixelMax = new TH1F("EpixelMax","Energy of max pixel",1000,0.,1000.);
  ContNeigh = new TH1F("ContNeigh","Frequency of neighbors pixels",100,-10.,200.);
  V_clu_vs_EPixMax = new TH2F("V_clu_vs_EPixMax","Cluster signal vs energy of max pixel",1000,0.,1000.,8000,-50.,5050.);
  V_clu_fractN_vs_EPixMax = new TH2F("V_clu_fractN_vs_EPixMax","Cluster signal normalized to N vs Energy of max pixel",1000,0.,1000.,8000,-50.,5050.);
  RMS = new TH1F("RMS","RMS",500,-10.,300.);
  RMS_vs_V_clu = new TH2F("RMS_vs_V_clu","RMS vs Signal of Cluster",8000,-50.,5050.,500,-10.,300.);
  RMS_vs_V_clu_fract_N = new TH2F(" RMS_vs_V_clu_fract_N","RMS vs Signal of Cluster Normalized to N",8000,-50.,5050.,500,-10.,300.);
  N_vs_RMS = new TH2F("N_vs_RMS","Number of pixel in a cluster vs RMS",500,-10.,300.,49,0.,49.);
  RMS_vs_EPixMax = new TH2F("RMS_vs_EPixMax","RMS vs Energy of max pixel",1000,0.,1000.,500,-10.,300.);
  
  
}

Analisi::~Analisi()
{
  delete map_seed;
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
  delete RMS_vs_V_clu;
  delete RMS_vs_V_clu_fract_N;
  delete N_vs_RMS;
  delete RMS_vs_EPixMax;
}

void Analisi::WriteOnFile(const std::string fname)
{
  TFile f(fname.c_str(),"Recreate");
  map_seed->Write();
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
  RMS_vs_V_clu->Write();
  RMS_vs_V_clu_fract_N->Write();
  N_vs_RMS->Write();
  RMS_vs_EPixMax->Write();
  
  f.Close();
  
}



int Analisi::AnalisiData (SeedList *sl)
{
  const int TOLERANCE_PIXELS = 2;
  
  int Row_seed = 0;
  int Col_seed = 0;
  double V_adja = 4.0; //fSecondaryThr
  int cont = 0;
  
  for(size_t i=0; i<sl->Size(); i++)
    {
      Seed ts = sl->At(i);
      Row_seed = ts.GetRow();
      Col_seed = ts.GetCol();
      double EPixMax=0.;
      int N = 1;
  

      map_seed->Fill((Row_seed),(Col_seed)); //perche cast?
      double R_center_vs_cluster;
      R_center_vs_cluster = sqrt(pow((240.-Row_seed),2) +pow((320.-Col_seed),2));
      R_center_vs_cluster_dist->Fill(R_center_vs_cluster);
  

      double V_clu_fract_n = 0;
      double V_clu_A = 0.;
      double V_clu_3x3 = 0.;
      double V_clu_5x5 = 0.;
      double V_clu_7x7 = 0.;
      double Delta_5x5_vs_3x3 = 0.;
      double Delta_5x5_vs_3x3_norm = 0.;
      double Delta_7x7_vs_5x5 = 0.;
      double Delta_7x7_vs_5x5_norm = 0.;
      double RMS_2 = 0.;
      double RMS_2_1 = 0.;
      double RMS_ = 0.;

      char logStr[10000];
      char logStr3[10000];
      //      sprintf(logStr, ""); //?!?!
      
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
        
	for (int i = 0; i < preCluster.size(); i++)
	  {
	    
	    ACPoint preClusterPoint = preCluster.at(i); //dammi il punto all'i-sima posizione della lista di precluster
	    for (int j = 0; j < cluster.size(); j++)
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
      char logStr2[10000];
      //      sprintf(logStr2, "Result: ");
      
      ///////////////////////Calcolo Cluster Asimmetrico/////////////////////////
      
      for (int i = 0; i < cluster.size(); i++)
	{
	  ACPoint currentPoint = cluster.at(i);
	  //	  sprintf(logStr2, "%s (%d, %d) -", logStr2, currentPoint.x, currentPoint.y);
	  V_clu_A += ts(currentPoint.x, currentPoint.y);
        }
      V_clu_fract_n = (V_clu_A)/cluster.size();
      //      sprintf(logStr2, "%s\n\n\n\n\n\n", logStr2);
      //      cout << logStr2;
      
     //////////////////////Calcolo RMS Cluster Asimmetrico/////////////////////
      
      for (int i = 0; i < cluster.size(); i++)
	{
	  ACPoint currentPoint = cluster.at(i);
	  float tsValue = ts(currentPoint.x,currentPoint.y);
	  //	  sprintf(logStr3, "%s[[%f]]\n ",logStr3,tsValue);
      	  RMS_2_1+= (pow((tsValue - V_clu_fract_n),2));
	  RMS_2= (RMS_2_1) /(cluster.size()-1);
	}
      RMS_ = sqrt(RMS_2);
      
      //////////////////////////////DEBUG RMS//////////////////////////////////////
      if (RMS_2 <1  && cluster.size()==15)
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
	}

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
      
      
      ///////////////////////Riempimento Istogrammi//////////////////////////
      
      V_vs_npixel_cluster->Fill(float(V_clu_A),float(N));
      npixel_cluster_vs_V->Fill(float(N),float(V_clu_A));
      V_clu_fract_N->Fill(float(V_clu_fract_n));
      N_pixel_per_cluster->Fill(float(N));
      VpixelMax->Fill(float(EPixMax));
      RMS->Fill(float(RMS_));
      V_clu_vs_EPixMax->Fill(float(EPixMax),float(V_clu_A));
      V_clu_fractN_vs_EPixMax->Fill(float(EPixMax),float(V_clu_fract_n));
      RMS_vs_V_clu->Fill(float(V_clu_A),float(RMS_));
      RMS_vs_V_clu_fract_N->Fill(float(V_clu_fract_n),float(RMS_));
      N_vs_RMS->Fill(float(RMS_),float(N));
      RMS_vs_EPixMax->Fill(float(EPixMax),float(RMS_));
      V_clu_Asy->Fill(V_clu_A);
      V_clu_3x3_dist->Fill(V_clu_3x3);
      V_clu_5x5_dist->Fill(V_clu_5x5);
      V_clu_7x7_dist->Fill(V_clu_7x7);
      Delta_5x5_vs_3x3_dist->Fill(Delta_5x5_vs_3x3);
      Delta_5x5_vs_3x3_norm_dist->Fill(Delta_5x5_vs_3x3_norm);
      Delta_7x7_vs_5x5_dist->Fill(Delta_7x7_vs_5x5);
      Delta_7x7_vs_5x5_norm_dist->Fill(Delta_7x7_vs_5x5_norm);
    }

  /////////////////////////Conteggio Seed Vicini///////////////////////////
  
  for(size_t i=0; i<sl->Size(); i++)
    {
      Seed ts = sl->At(i);
      for(size_t j=i+1; j<sl->Size(); j++)
	{
	  Seed ts2 = sl->At(j);

	  int distX = ts.GetRow()-ts2.GetRow();
	  int distY = ts.GetCol()-ts2.GetCol();

	  if(distX <= 3 && distX >= -3 && distY <= 3 && distY >= -3 && (distX!=0 || distY!=0))
	    {
	      cont++;
	    }
	}
    }
  ContNeigh->Fill(cont);
  return(0);     
  
}
