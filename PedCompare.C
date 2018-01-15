

// ######################################################
// ######################################################
// Questa macro verifica la consistenza tra due run di buio diversi
// Prende in input due file NomeFile_PostPed.root che sono i risultati di PedAnalyzer.C
// Produce in output un file NomeFile1_PostPed_CheckDiff.root contenente l'istogramma delle differenze (attenzione, il file prende solo il nome del primo dei due confrontati!)

//######### LAST MODIFIED ON 2018-01-15 by Collamaf

//USAGE
/*
 .L PedCompare.C
 PedCompare("file1_PostPed","file2_PostPed")
 */

#include <TH2.h>
#include <TStyle.h>
#include <iostream>
#include <TCanvas.h>
void PedCompare(TString filename1, TString filename2)
{
	cout<<"Program to compare two pedestal (+ noise) files: Now comparing file:"<<endl<<filename1<<".root"<<endl<<"with file:"<<endl<<filename2<<".root"<<endl;
	//apertura dei file di input buio
	TFile fileCFR1 (Form("%s.root",filename1.Data()));
	TVectorD *fVectorPed1=(TVectorD*)fileCFR1.Get("VectorPed");
	TVectorD *fVectorNoise1=(TVectorD*)fileCFR1.Get("VectorNoise");

	TFile fileCFR2 (Form("%s.root",filename2.Data()));
	TVectorD* fVectorPed2 =  (TVectorD*) fileCFR2.Get("VectorPed");
	TVectorD* fVectorNoise2 =  (TVectorD*) fileCFR2.Get("VectorNoise");
	
	
	//apertura file di output
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
		
		ValMean1=fVectorPed1[0][kk];
		ValNoise1=fVectorNoise1[0][kk];
		ValMean2=fVectorPed2[0][kk];
		ValNoise2=fVectorNoise2[0][kk];
		
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
