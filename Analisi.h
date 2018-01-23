#ifndef Analisi_HH
#define Analisi_HH

#include <vector>
#include <iostream>
#include "TObject.h"
#include "SeedList.h"
#include "Frame.h"
#include "Seed.h"

#include "TTree.h"
#include "TFile.h"

class ACPoint
{
 public:
  ACPoint(int _x=0, int _y=0) {
    x = _x;
    y = _y;
  };
  int x;
  int y;
};
  

class Analisi : public TObject
{
public:
  Analisi(const std::string OutFileName, const std::string BadFileName);
  virtual ~Analisi();
    
  void WriteOnFile();
  int AnalisiData (SeedList *sl);
    
  inline void SetSecondaryThr(double thr){fSecondaryThr=thr;};
  
 private:
  TH2F *map_seed;
  TH2F *map_seed_coarse;
  TH2F *map_seed_coarse2;
  TH2F *map_seed1_4;
  TH2F *map_seed2_4;
  TH2F *map_seed3_4;
  TH2F *map_seed4_4;
  TH1F *map_Row_seed;	
  TH1F *map_Col_seed;
  TH1F *R_center_vs_cluster_dist;
  TH1F *V_single;
  TH1F *V_clu_Asy;
  TH1F *V_clu_Asy_4;
  TH1F *V_clu_3x3_dist;
  TH1F *V_clu_5x5_dist;
  TH1F *V_clu_7x7_dist;
  TH1F *V_clu_9x9_dist;
  TH1F *Delta_5x5_vs_3x3_dist;
  TH1F *Delta_5x5_vs_3x3_norm_dist;
  TH1F *Delta_7x7_vs_5x5_dist;
  TH1F *Delta_7x7_vs_5x5_norm_dist;
  TH1F *Delta_9x9_vs_7x7_dist;
  TH1F *Delta_9x9_vs_7x7_norm_dist;
  TH2F *V_vs_npixel_cluster;
  TH2F *V_vs_npixel_cluster4;
  TH2F *npixel_cluster_vs_V;
  TH1F *N_pixel_per_cluster;
  TH1F *N_pixel_per_cluster4;
  TH1F *V_clu_fract_N;
  TH1F *V_clu4_fract_N;
  TH1F *VpixelMax;
  TH1F *VpixelMax4;
  TH1F *ContNeigh;
  TH2F *V_clu_vs_EPixMax;
  TH2F *V_clu_4_vs_EPixMax4; 
  TH2F *V_clu_fractN_vs_EPixMax;
  TH2F *V_clu4_fract_N_vs_EPixMax;
  TH1F *RMS;
  TH1F *RMS4;
  TH2F *RMS_vs_V_clu;
  TH2F *RMS4_vs_V_clu4;
  TH2F *RMS_vs_V_clu_fract_N;
  TH2F *RMS4_vs_V_clu4_fract_N;
  TH2F *N_vs_RMS;
  TH2F *N_vs_RMS4;
  TH2F *RMS_vs_3x3;	
  TH2F *RMS_vs_EPixMax;
  TH2F *RMS4_vs_EPixMax4;
  TH1F *MaxPixel_fract_EClusterA;
  TH1F *MaxPixel_fract_EClusterS;
  TH2F *V_S_vs_npixel_cluster;
  TH2F *npixel_cluster_vs_V_S;
  TH2F *V_clu_S_vs_EPixMax;
  TH2F *RMS_vs_V_clu_S;
  TH1F *R_seed_vs_seed;
  TH1F *V_vs_EPixMax;
  TH1F *V_vs_RMS;
  TH1F *EPixMax_vs_RMS;
  TH1F *Emax_div_E3x3_dist;
  TH1F *Eas_div_Npix_dist;
  TH2F *EasDivN_vs_3x3;
  TH2F *Emax_vs_3x3;	
  TH2F *N_vs_3x3;
  TH2F *Easy_vs_3x3;
  TH2F *Emax_vs_N;
  TH2F *EasDivN_vs_Easy;
  TH2F *EasDivN_vs_N;
  TTree *Albero;
  TFile *fOutFile;

  
  Double_t fSecondaryThr;
  Double_t  V_clu_3x3;
  Double_t RMS_nome_esteso; 
  int N;
  Double_t Eas_div_Npix;
   std::vector<ACPoint> Badpixels;
  ClassDef(Analisi,1);
};





#endif

 

