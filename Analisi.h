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

class Analisi : public TObject
{
public:
  Analisi();
  virtual ~Analisi();
    
  void WriteOnFile(const std::string fname);
  int AnalisiData (SeedList *sl);
    
  inline void SetSecondaryThr(double thr){fSecondaryThr=thr;};
  
 private:
  TH2F *map_seed;
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

  double fSecondaryThr;
  
  ClassDef(Analisi,1);
};


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
  


#endif

 

