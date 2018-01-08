#define ClassePed_cxx
#include "ClassePed.h"
#include <TH2.h>
#include <TStyle.h>
#include <iostream>
#include <TCanvas.h>

void ClassePed::Loop()
{
	//   In a ROOT session, you can do:
	//      root> .L ClassePed.C
	//      root> ClassePed t
	//      root> t.GetEntry(12); // Fill t data members with entry number 12
	//      root> t.Show();       // Show values of entry 12
	//      root> t.Show(16);     // Read and show values of entry 16
	//      root> t.Loop();       // Loop on all entries
	//
	
	//     This is the loop skeleton where:
	//    jentry is the global entry number in the chain
	//    ientry is the entry number in the current Tree
	//  Note that the argument to GetEntry must be:
	//    jentry for TChain::GetEntry
	//    ientry for TTree::GetEntry and TBranch::GetEntry
	//
	//       To read only selected branches, Insert statements like:
	// METHOD1:
	//    fChain->SetBranchStatus("*",0);  // disable all branches
	//    fChain->SetBranchStatus("branchname",1);  // activate branchname
	// METHOD2: replace line
	//    fChain->GetEntry(jentry);       //read all branches
	//by  b_branchname->GetEntry(ientry); //read only this branch
	if (fChain == 0) return;
	
	Long64_t nentries = fChain->GetEntriesFast();
	
	vector<double> VectVal;
	vector<double> VectValQuad;
	
//	FILE * PedFile;
//	PedFile=fopen("File_buio.txt","RECREATE");
//	TString prova=Form("%s_buio",nomefile);
//	ofstream PedFile(Form("%sFile_buio.txt",(char)nomefile));
	ofstream PedFile("File_buio.txt");
	ofstream NoiseFile("File_noise.txt");
//	prova<<"ciao"<<endl;
	/*
	FILE * NoiseFile;
	NoiseFile=fopen("File_noise.txt","RECREATE");
	*/
	
	int kk=0;
	//	temp[488][648];
	Long64_t nbytes = 0, nb = 0;
	for (Long64_t jentry=0; jentry<nentries;jentry++) {
		Long64_t ientry = LoadTree(jentry);
		if (ientry < 0) break;
		nb = fChain->GetEntry(jentry);   nbytes += nb;
		
		if (jentry==0) {
			int Size=fNRow*fNCol;
			cout<<"Size= "<<Size<<endl;
			VectVal.resize(Size);
			VectValQuad.resize(Size);
			for (kk=0; kk<Size; kk++) {
				VectVal[kk]=0;
				VectValQuad.at(kk)=0;
				
				
			}
		}
		
		
		for (kk=0; kk<VectVal.size(); kk++) {
			VectVal.at(kk)+=fData[kk];
			VectValQuad.at(kk)+=(fData[kk]*fData[kk]);
			if (kk==0) cout<<"File num= "<<jentry<<", pixel num= "<<kk<<", valore aggiunto= "<<fData[kk]<<", somma attuale= "<<VectVal.at(kk)<<", somma Quad attuale= "<<VectValQuad.at(kk)<<endl;
		}
		
		
		
	} //chiude ciclo su files
	
	double STDDev=0;
	double media=0;
	
	for (kk=0; kk<VectVal.size(); kk++) {
		media=VectVal.at(kk)/nentries;
		STDDev=sqrt(VectValQuad.at(kk)/nentries-(media*media));
		if (kk==0) cout<<"#############  FINITO! ############### "<<endl<<"Somma finale= "<<VectVal.at(kk)<<", numero files= "<<nentries<<", media= "<<", somma Quad finale= "<<VectValQuad.at(kk)<<", somma Quad finale/N = "<<VectValQuad.at(kk)/nentries<<" ### DevSTD= "<<STDDev<<endl;
		
		PedFile<<media<<" ";
		NoiseFile<<STDDev<<" ";
		
		
	}
	
	
	PedFile.close();
	NoiseFile.close();
	
	
	
}
