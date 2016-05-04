#include "Analisi.h"

Analisi::Analisi()
{
  map_seed = new TH2F("mapofseed","seedmap",480.,0.,480.,640.,0.,640.);
  V_single = new TH1F("Sigle pixel signal DV","",8000,-50.,950.);
  V_clu_Asy = new TH1F("Cluster asymmetric signal","",8000, -50., 5050.);
  V_clu_3x3_dist = new TH1F("Cluster signal 3x3","",8000,-50.,5050.);
  V_clu_5x5_dist = new TH1F("Cluster signal 5x5","",8000,-50.,5050.);
  V_clu_7x7_dist = new TH1F("Cluster signal 7x7","",8000,-50.,5050.);
  Delta_5x5_vs_3x3_dist = new TH1F("Delta 5x5 vs 3x3","",8000,-50.,5050.);
  Delta_5x5_vs_3x3_norm_dist = new TH1F("Delta 5x5 vs 3x3 norm","",8000,-50.,5050.);
  Delta_7x7_vs_5x5_dist = new TH1F("Delta 7x7 vs 5x5","",8000,-50.,5050.);
  Delta_7x7_vs_5x5_norm_dist = new TH1F("Delta 7x7 vs 5x5 norm","",8000,-50.,5050.);
  V_vs_npixel_cluster = new TH2F("Signal vs number of pixels in a cluster","",2000.,-10.,1000.,50.,0.,50.);
  N_pixel_per_cluster = new TH1F("Number of pixel in a cluster","",49,0.,49.);
  
}

Analisi::~Analisi()
{
  delete map_seed;
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
  delete N_pixel_per_cluster;
}

void Analisi::WriteOnFile(const std::string fname)
{
  TFile f(fname.c_str(),"Recreate");
  map_seed->Write();
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
  N_pixel_per_cluster->Write();
  
  f.Close();
  
}


int Analisi::AnalisiData (SeedList &sl)
{ 
  int Row_seed = 0;
  int Col_seed = 0;
  double V_adja = 2.0;
  
  for(size_t i=0; i<sl.Size(); i++)
    {
      Seed ts = sl.At(i);
      Row_seed = ts.GetRow();
      Col_seed = ts.GetCol();

      //Questa variabile non la usi mai?!?!
      double adccountsCenter=ts(0,0);
      /////////////////////////////////////

      map_seed->Fill(float(Row_seed),float(Col_seed));
      int N = 0;
      double V_clu_A = 0.;
      double V_clu_3x3 = 0.;
      double V_clu_5x5 = 0.;
      double V_clu_7x7 = 0.;
      double Delta_5x5_vs_3x3 = 0.;
      double Delta_5x5_vs_3x3_norm = 0.;
      double Delta_7x7_vs_5x5 = 0.;
      double Delta_7x7_vs_5x5_norm = 0.;
      
      
      for(int j=ts.GetPixelMin(); j<ts.GetPixelMax(); j++)
	{
	  for(int i=ts.GetPixelMin(); i<ts.GetPixelMax(); i++)
	    {
	      V_single->Fill(ts(i,j));
	      
	      if(ts(i,j)>V_adja)
		{
		  V_clu_A += ts(i,j);
		  N++;
		}
	      
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
	      
	      Delta_5x5_vs_3x3 = V_clu_5x5 - V_clu_3x3;
	      Delta_5x5_vs_3x3_norm = (V_clu_5x5 - V_clu_3x3)/16;
	      Delta_7x7_vs_5x5 = V_clu_7x7 -V_clu_5x5;
	      Delta_7x7_vs_5x5_norm = (V_clu_7x7 - V_clu_5x5)/24;
	      
	      
	      // cout<<i<<" "<<j<<" "<< ts(i,j) << " ";
	    }
	  // cout << std::endl;
	}
      V_vs_npixel_cluster->Fill(float(V_clu_A),float(N));
      N_pixel_per_cluster->Fill(float(N));
      V_clu_Asy->Fill(V_clu_A);
      V_clu_3x3_dist->Fill(V_clu_3x3);
      V_clu_5x5_dist->Fill(V_clu_5x5);
      V_clu_7x7_dist->Fill(V_clu_7x7);
      Delta_5x5_vs_3x3_dist->Fill(Delta_5x5_vs_3x3);
      Delta_5x5_vs_3x3_norm_dist->Fill(Delta_5x5_vs_3x3_norm);
      Delta_7x7_vs_5x5_dist->Fill(Delta_7x7_vs_5x5);
      Delta_7x7_vs_5x5_norm_dist->Fill(Delta_7x7_vs_5x5_norm);
      N = 0;
      V_clu_A = 0.;
      V_clu_3x3 = 0.;
      V_clu_5x5 = 0.;
      V_clu_7x7 = 0.;
    }
  return(0);     
  
}
