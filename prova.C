#include <iostream>
#include <fstream>

#include "Frame.h"
#include "Seed.h"
#include "SeedList.h"

#include "TTree.h"
#include "TFile.h"

using std::cout;
using std::endl;
using std::ifstream;

int main(int argc, char *argv[])
{
  TFile f("prova.root","RECREATE");
  TTree t("prova","una prova");
  Frame *frame = new Frame(640,480);
  //  std::vector<Seed> 
  SeedList *seeds = new SeedList();
  t.Branch("frame",&frame);  
  frame->ReadFile("test/MT9V011_new_90Sr_G01_TI200ms_noThre_00000.txt");

  Frame *bkg = new Frame(640,480);
  bkg->ReadFile("test/MT9V011_G01_TI200ms_Thre0_0_buio.txt");

  bkg->Multiply(-1);
  frame->Add(*bkg);

  //  *seeds=
  SeedList pp=frame->FindSeeds(2);
  cout<<"seeds.size(): "<<pp.Size()<<endl;


  // Seed thisSeed=pp(0);
  // double adc=thisSeed(-1,-1);

  // *seeds=pp;
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
