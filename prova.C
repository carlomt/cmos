#include <iostream>
#include "Frame.h"
#include "TTree.h"
#include "TFile.h"

using std::cout;
using std::endl;

int main()
{
  TFile f("prova.root","RECREATE");
  TTree t("prova","una prova");
  Frame *frame = new Frame(4,4);
  t.Branch("frame",&frame);
  

  for(int k=1;k<5;k++)
    {
      frame->Dump();
      for(int i=0;i<4;i++)
	{
	  for(int j=0;j<4;j++)
	    {
	      // cout<<"Set("<<i<<","<<j<<")"<<endl;
	      // cout.flush();
	      frame->Set(i,j,k);
	    }
	}
      frame->At(10,10);
      // cout<<"Filling"<<endl;
      // cout.flush();
      t.Fill();
      // cout<<"GetTH2F"<<endl;
      // cout.flush();
      TH2F* h=frame->GetTH2F( Form("h-%d",k),"prova h");
      h->Write();
      frame->Clear();
    }
  f.Write();
  t.Print();

  f.Close();
  return 1;
}
