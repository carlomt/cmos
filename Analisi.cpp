#include <iostream>
#include <fstream>
#include "Riostream.h"

#include "Frame.h"
#include "Seed.h"
#include "SeedList.h"

#include "TTree.h"
#include "TFile.h"

//#include "getFileCreationTime.h"

using std::cout;
using std::endl;
using std::ifstream;
using std::string;


int Analisi (SeedList &sl , string outfname)
{ 
  int Row_seed = 0;
  int Col_seed = 0;
  double V_adja = 2.;
  
  
  
  TFile f ("/Users/Amedeo/repos/cmos/Analisi.root","recreate");
  TH2F *map_seed = new TH2F("mapofseed","seedmap",480.,0.,480.,640.,0.,640.);
  TH2F *map_seed_coarse = new TH2F("mapofseedcoarse","seedmapcoarso",61.,0.,480.,81.,0.,640.);
  TH2F *map_seed_coarse2 = new TH2F("mapofseedcoarse61x81","seedmapcoarse",8.,0.,480.,8.,0.,640.); //mappa seed con bin larghi 61x81 pixel
  TH2F *map_seed_2 = new TH2F("mapofseed2","seedmap2",480.,0.,480.,640.,0.,640.);
  TH2F *trid_map_seed = new TH2F("3D map of seed","",480,2.,480.,640,2.,640.);
  TH1F *V_single = new TH1F("Sigle_pixel_signal","Sigle pixel signal DV",47750,-50.,9500.);//0.2 per bin
  TH1F *V_clu_Asy = new TH1F("Cluster_asymmetric","Cluster asymmetric signal",2025, -50., 4000.);
  TH1F *V_clu_3x3_dist = new TH1F("Cluster_ 3x3","Cluster signal 3x3",2500,-50.,200.);
  TH1F *V_clu_5x5_dist = new TH1F("Cluster_5x5","Cluster signal 5x5",20500,-50.,2000.);
  TH1F *V_clu_7x7_dist = new TH1F("Cluster_7x7","Cluster signal 7x7",10500,-50.,2000.);
  TH1F *Delta_5x5_vs_3x3_dist = new TH1F("Delta 5x5 vs 3x3","",105,-50.,1000.);
  TH1F *Delta_5x5_vs_3x3_norm_dist = new TH1F("Delta 5x5 vs 3x3 norm","",205,-50.,2000.);
  TH1F *Delta_7x7_vs_5x5_dist = new TH1F("Delta 7x7 vs 5x5","",100,-50.,150.);
  TH1F *Delta_7x7_vs_5x5_norm_dist = new TH1F("Delta 7x7 vs 5x5 norm","",205,-50.,2000.);

  TH2F *V_vs_npixel_cluster = new TH2F("V_vs_npixel_cluster","Signal vs number of pixels in a cluster",10001.,-10.,10000.,50.,0.,50.);
  TH1F *N_pixel_per_cluster = new TH1F("Number of pixel in a cluster","",60,0,60);
	

  
  
  for(size_t i=0; i<sl.Size(); i++)
    {
      Seed ts = sl.At(i);
      Row_seed = ts.GetRow();
      Col_seed = ts.GetCol();
      double adccountsCenter=ts(0,0);
      map_seed->Fill(float(Col_seed),float(Row_seed));
      map_seed_coarse->Fill(float(Col_seed),float(Row_seed));
      map_seed_coarse2->Fill(float(Col_seed),float(Row_seed));
      map_seed_2->Fill(float(Col_seed),float(Row_seed),float(adccountsCenter));
      trid_map_seed->Fill(Col_seed,Row_seed,adccountsCenter);

      //  double adccountsCenter=ts(0,0);
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
  map_seed->Write();
  map_seed->Write();
  map_seed_coarse->Write();
  map_seed_coarse2->Write();
  map_seed_2->Write();
  trid_map_seed->Write();
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
  return(0);     
  
}
