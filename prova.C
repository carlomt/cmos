#include <iostream>
#include "Frame.h"
#include "Seed.h"
#include "SeedList.h"

#include "TTree.h"
#include "TFile.h"

using std::cout;
using std::endl;

int main()
{
  TFile f("prova.root","RECREATE");
  TTree t("prova","una prova");
  Frame *frame = new Frame(640,480);
  //  std::vector<Seed> 
  SeedList *seeds = new SeedList();
  t.Branch("frame",&frame);
  t.Branch("seeds",&seeds);
  
  frame->ReadFile("../cmos_test/MT9V011_new_90Sr_G01_TI200ms_Thre10_spessore5_9mm_00001.txt");

  //  *seeds=
  SeedList pp=frame->FindSeeds(10);
  cout<<"seeds.size(): "<<pp.Size()<<endl;

  Seed thisSeed=pp(0);
  double adc=thisSeed(-1,-1);

  *seeds=pp;
  // cout<<"File"<<endl;
  // cout.flush();

  //  frame->Dump();

  // for(int k=1;k<5;k++)
  //   {

  //     for(int i=0;i<4;i++)
  // 	{
  // 	  for(int j=0;j<4;j++)
  // 	    {
  // 	      // cout<<"Set("<<i<<","<<j<<")"<<endl;
  // 	      // cout.flush();
  // 	      frame->Set(i,j,k);
  // 	    }
  // 	}
  //     frame->At(10,10);
  //     // cout<<"Filling"<<endl;
  //     // cout.flush();
  t.Fill();
  // cout<<"GetTH2F"<<endl;
  // cout.flush();
  int k=1;
  TH2F* h=frame->GetTH2F( Form("h-%d",k),"prova h");
  //  h->Write();
  //  frame->Clear();
  //}
  f.Write();
  //  t.Print();
  
  f.Close();
  return 1;
}
