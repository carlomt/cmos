//prova
#include <iostream>
#include <fstream>

#include "Frame.h"
#include "Seed.h"
#include "SeedList.h"

#include "TTree.h"
#include "TFile.h"

#include "Rtypes.h"

#include <sstream>

#include <iomanip>

using std::cout;
using std::endl;
using std::ifstream;
using std::string;



int main()
{
  /* for(int i=0;i<10;i++)
    {
      //questo for solo per farti vedere come prendere diversi file
      //in caso prendi il codice sotto lo metti nel for e dentro ReadFile metti filename
      //la lettura del buio lasciala fuori del for (tanto mi pare sia la stessa per tutti i run)
      std::ostringstream fnamer;
      fnamer<<"MT9V011_new_nosource_G01_TI200ms_Thre0_spessore8x270um_"<< std::setfill('0') << std::setw(5)<<i;
      string filename=fnamer.str();
      cout<<filename<<endl;
    }
  */
  Frame fr(480,640);
  fr.ReadFile("/Users/Amedeo/Desktop/CMOS/MT9V011_newprograms/Data/MT9V011_new_90Sr_G01_TI200ms_noThre_7000_8000/MT9V011_new_90Sr_G01_TI200ms_noThre_07101.txt");
  Frame bkg(480,640);
  bkg.ReadFile("/Users/Amedeo/Desktop/CMOS/MT9V011_newprograms/MT9V011_G01_TI200ms_Thre0_0_buio.txt");
  
  //Seed se(0,0,1,7);

  fr.Subtract(bkg);
  
  //  SeedList sl=fr.FindSeeds(3.36,3,3);
  SeedList* sl=fr.FindSeeds(2);
  cout<<"seeds.size(): "<<sl->Size()<<endl;

  //per scrivere su schermo l'elenco dei seed
  //comunque questo e' solo per confrontarlo con la vecchia analisi
  //settimana prossima vediamo come fare l'analisi ma senza scrivere e leggere da txt
  for(size_t i=0; i<sl->Size(); i++)
    {
      Seed ts=sl->At(i);
      cout<<" evento 0 scritto il seed "<<i<<" row "<< ts.GetRow() <<" col "<<ts.GetCol();
      cout << std::fixed << std::setprecision(2) << std::setfill('0');
      cout<<" "<<ts(0,0)<<endl;

    }
  

  //  cout<<fr<<endl;
}
/*
int main(int argc, char *argv[])
{
  TFile f("prova.root","RECREATE");
  TTree t("prova","una prova");
  //  Frame *frame = new Frame(480,640);
  //  std::vector<Seed> 
  SeedList *seeds = new SeedList();
  t.Branch("frame",&frame);  
  //  frame->



  
  Frame *bkg = new Frame(480,640);
  bkg->ReadFile("/Users/Amedeo/Desktop/CMOS/MT9V011_newprograms/MT9V011_G01_TI200ms_Thre0_0_buio.txt");

  bkg->Multiply(-1);
  frame->Add(*bkg);

  //  *seeds=
  SeedList pp=frame->FindSeeds(2);
  cout<<"seeds.size(): "<<pp.Size()<<endl;

  for(size_t i=0; i<pp.Size(); i++)
    {
      Seed ts=pp(i);
      cout<<ts.GetCol()<<"\t"<<ts.GetRow()<<"\t"<<ts(0,0)<<endl;
    }
  
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
*/
