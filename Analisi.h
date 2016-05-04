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
  int AnalisiData (SeedList &sl);
  
  
 private:
  TH2F *map_seed;
  TH1F *V_single;
  TH1F *V_clu_Asy;
  TH1F *V_clu_3x3_dist;
  TH1F *V_clu_5x5_dist;
  TH1F *V_clu_7x7_dist;
  TH1F *Delta_5x5_vs_3x3_dist;
  TH1F *Delta_5x5_vs_3x3_norm_dist;
  TH1F *Delta_7x7_vs_5x5_dist;
  TH1F *Delta_7x7_vs_5x5_norm_dist;
  TH2F *V_vs_npixel_cluster;
  TH1F *N_pixel_per_cluster;
};

#endif

 
