/*Verifica consistenza tra run di buio diversi*/
#include <TH2.h>
#include <TStyle.h>
#include <iostream>
#include <TCanvas.h>
void PedCompare(TString filename1, TString filename2)
{
	cout<<"Program to compare two pedestal (+ noise) files: Now comparing file:"<<endl<<filename1<<".root"<<endl<<"with file:"<<endl<<filename2<<".root"<<endl;
	//apertura file di input buio
//	TFile fileCFR1 ("DatiProva/RootBuoni/2018-01-10_MT9V115_90Y_Decay05_Primi100_PostPed.root");
	TFile fileCFR1 (Form("%s.root",filename1.Data()));
	TVectorD *fVectorMean1=(TVectorD*)fileCFR1.Get("VectorMean");
	TVectorD *fVectorNoise1=(TVectorD*)fileCFR1.Get("VectorNoise");
//	fVectorMean1->Print();

	
//	TFile fileCFR2 ("DatiProva/RootBuoni/2018-01-09_MT9V115_90Y_Decay01_Primi100_PostPed.root", "READ");
	TFile fileCFR2 (Form("%s.root",filename2.Data()));
	TVectorD* fVectorMean2 =  (TVectorD*) fileCFR2.Get("VectorMean");
	TVectorD* fVectorNoise2 =  (TVectorD*) fileCFR2.Get("VectorNoise");
	
	/*
	fVectorMean1->Print();
	fVectorMean2->Print();
	fVectorNoise1->Print();
	fVectorNoise2->Print();
	*/
	//apertura file di output
//	TFile fout ("outputDiff.root","RECREATE");
	TFile fout (Form("%s_CheckDiff.root",filename1.Data()),"RECREATE");
	
	cout<<"Output in file:"<<endl<< Form("%s_CheckDiff.root",filename1.Data()) <<endl;
	fout.cd();
	TH1F *diff=new TH1F("diff","Differenze", 100, -10, 10); //istogramma per mettere le differenze
	
	int nrow=648, ncol=488;
	double ValMean1=-99;
	double ValNoise1=-99;
	double ValMean2=-99;
	double ValNoise2=-99;
	
	for (int kk=0; kk<nrow*ncol; kk++) {
		
		ValMean1=fVectorMean1[0][kk];
		ValNoise1=fVectorNoise1[0][kk];
		ValMean2=fVectorMean2[0][kk];
		ValNoise2=fVectorNoise2[0][kk];
//		cout<<fVectorMean1[0][0];
		
//		cout<<kk<<" "<<ValMean1<<" "<<ValMean2<<" "<<ValNoise1<<" "<<ValNoise2<<" "<<  (ValMean2-ValMean1)/sqrt(pow(ValNoise2, 2)+pow(ValNoise1,2)) <<endl;
		
		diff->Fill((ValMean2-ValMean1)/sqrt(pow(ValNoise2, 2)+pow(ValNoise1,2))); //riempiamo l'istogramma con le differenze
	}

	cout<<endl;
	cout<<endl;
	cout<<"root -l "<<Form("%s_CheckDiff.root",filename1.Data()) <<endl;
	cout<<"diff->Draw()"<<endl<<endl;
	
	
	diff->Write();//salva l'istogramma sul file di ouptut
	fout.Write();
	TCanvas* canvas;
	diff->Draw();
}
