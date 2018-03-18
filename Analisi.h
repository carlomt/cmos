#ifndef Analisi_HH
#define Analisi_HH

#include <vector>
#include <iostream>
#include "TObject.h"
#include "SeedList.h"
#include "Frame.h"
#include "Seed.h"

#include "TStyle.h"
#include "TCanvas.h"
#include "TPaveStats.h"
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
  Analisi(const std::string OutFileName, const std::string BadFileName, int FrameNCol, int FrameNRow, Double_t fcal); //costruttore con argomenti
  virtual ~Analisi();  //distruttore: rilascia la memoria associata ai membri della classe
    
  void WriteOnFile();
  int AnalisiData (SeedList *sl, int FrameNCol, int FrameNRow, Double_t fcal);
    
  inline void SetSecondaryThr(double thr){fSecondaryThr=thr;};
  
 private:
  TPaveStats *st; //dichiaro i membri della classe (che definirò in Analisi.C)
  TH1F *HistoNSeeds;
  TH2F *map_seed;
  TH2F *map_seedDist10;
  TH2F *map_seed_coarse;
  TH2F *map_seed_coarse2;
  TH2F *map_seed1_4;
  TH2F *map_seed2_4;
  TH2F *map_seed3_4;
  TH2F *map_seed4_4;
  TH1F *map_Row_seed;	
  TH1F *map_Col_seed;
  TH1F *HistoDistSeeds;
  TH1F *HistoR_vs_Cluster;
  TH1F *HistoV_single;
  TH1F *HistoClusterAsy;
  TH1F *HistoCluster3x3;
  TH1F *HistoCluster3x3keV;
  TH1F *HistoCluster5x5;
  TH1F *HistoCluster7x7;
  TH1F *HistoCluster9x9;
  TH1F *HistoDelta_5x5_vs_3x3;
  TH1F *HistoDelta_5x5_vs_3x3_norm;
  TH1F *HistoDelta_7x7_vs_5x5;
  TH1F *HistoDelta_7x7_vs_5x5_norm;
  TH1F *HistoDelta_9x9_vs_7x7;
  TH1F *HistoDelta_9x9_vs_7x7_norm;
  TH2F *V_vs_NpixClu;
  TH2F *NpixClu_vs_V;
  TH1F *HistoNpixClu;
  TH1F *HistoV_fract_NpixClu;
  TH1F *HistoEPixMax;
  TH2F *V_vs_EPixMax; 
  TH2F *V_fract_NpixClu_vs_EPixMax;
  TH1F *HistoRMSclu_Asy;
  TH2F *RMS_vs_3x3;
  TH2F *RMS_vs_V;
  TH2F *RMS_vs_V_fract_NpixClu;
  TH2F *NpixClu_vs_RMS;
  TH2F *RMS_vs_EPixMax;
  TH1F *HistoEmax_fract_E3x3;
  TH1F *HistoE_CluAsy_fract_NpixClu;
  TH2F *E_CluAsy_fract_NpixClu_vs_3x3;
  TH2F *Emax_vs_3x3;	
  TH2F *NpixClu_vs_3x3;
  TH2F *E_CluAsy_vs_3x3;
  TH2F *Emax_vs_NpixClu;
  TH2F *E_CluAsy_fract_NpixClu_vs_E_CluAsy;
  TH2F *E_CluAsy_fract_NpixClu_vs_NpixClu;
  TFile *fOutFile;
  
  Double_t fSecondaryThr;
  std::vector<ACPoint> Badpixels;
  ClassDef(Analisi,1);
};

#endif //Analisi_HH

 

