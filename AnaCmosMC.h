//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Mon May  8 16:25:54 2017 by ROOT version 6.08/02
// from TTree B1/physics
// found on file: B1histogram0.root
//////////////////////////////////////////////////////////

#ifndef AnaCmosMC_h
#define AnaCmosMC_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.
#include "vector"

class AnaCmosMC {
public :
	TFile *FrameFile;
	TFile *ImageFile;

   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain
	Double_t ConvFactor=10;

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   Double_t        Eabs;
   Double_t        PreCmosTrackN;
   vector<double>  *PreCmosPart;
   vector<double>  *PreCmosEn;
   Double_t        InCmosTrackN;
   vector<double>  *InCmosPart;
   vector<double>  *InCmosEn;
   Double_t        InCmosEnPhot;
   Double_t        InCmosEnEl;
   Double_t        InCmosEnPos;
   vector<double>  *PixelID;
   vector<double>  *EDepInPixel;
   vector<double>  *PixXPos;
   vector<double>  *PixYPos;
   Double_t        SourceX;
   Double_t        SourceY;
   Double_t        SourceZ;
   Double_t        SourceEne;
   Double_t        Nev;
	TH2F* cluster;
//	vector<TH2F*>  ClusterVec;

   // List of branches
   TBranch        *b_Eabs;   //!
   TBranch        *b_PreCmosTrackN;   //!
   TBranch        *b_PreCmosPart;   //!
   TBranch        *b_PreCmosEn;   //!
   TBranch        *b_InCmosTrackN;   //!
   TBranch        *b_InCmosPart;   //!
   TBranch        *b_InCmosEn;   //!
   TBranch        *b_InCmosEnPhot;   //!
   TBranch        *b_InCmosEnEl;   //!
   TBranch        *b_InCmosEnPos;   //!
   TBranch        *b_PixelID;   //!
   TBranch        *b_EDepInPixel;   //!
   TBranch        *b_PixXPos;   //!
   TBranch        *b_PixYPos;   //!
   TBranch        *b_SourceX;   //!
   TBranch        *b_SourceY;   //!
   TBranch        *b_SourceZ;   //!
   TBranch        *b_SourceEne;   //!
   TBranch        *b_Nev;   //!

   AnaCmosMC(TString filename);
   virtual ~AnaCmosMC();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef AnaCmosMC_cxx
AnaCmosMC::AnaCmosMC(TString filename) : fChain(0)
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
	
	
	TFile *f = new TFile(Form("%s.root",filename.Data()));
	TTree *tree = (TTree*)gDirectory->Get("B1");
	
	FrameFile = new TFile(Form("%s_Frame%d.root",filename.Data(), (int)(ConvFactor*10)),"RECREATE");
	ImageFile = new TFile(Form("%s_Image.root",filename.Data()),"RECREATE");
//	FrameFile->cd();
	Init(tree);
}

AnaCmosMC::~AnaCmosMC()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t AnaCmosMC::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t AnaCmosMC::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void AnaCmosMC::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set object pointer
   PreCmosPart = 0;
   PreCmosEn = 0;
   InCmosPart = 0;
   InCmosEn = 0;
   PixelID = 0;
   EDepInPixel = 0;
   PixXPos = 0;
   PixYPos = 0;
   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("Eabs", &Eabs, &b_Eabs);
   fChain->SetBranchAddress("PreCmosTrackN", &PreCmosTrackN, &b_PreCmosTrackN);
   fChain->SetBranchAddress("PreCmosPart", &PreCmosPart, &b_PreCmosPart);
   fChain->SetBranchAddress("PreCmosEn", &PreCmosEn, &b_PreCmosEn);
   fChain->SetBranchAddress("InCmosTrackN", &InCmosTrackN, &b_InCmosTrackN);
   fChain->SetBranchAddress("InCmosPart", &InCmosPart, &b_InCmosPart);
   fChain->SetBranchAddress("InCmosEn", &InCmosEn, &b_InCmosEn);
   fChain->SetBranchAddress("InCmosEnPhot", &InCmosEnPhot, &b_InCmosEnPhot);
   fChain->SetBranchAddress("InCmosEnEl", &InCmosEnEl, &b_InCmosEnEl);
   fChain->SetBranchAddress("InCmosEnPos", &InCmosEnPos, &b_InCmosEnPos);
   fChain->SetBranchAddress("PixelID", &PixelID, &b_PixelID);
   fChain->SetBranchAddress("EDepInPixel", &EDepInPixel, &b_EDepInPixel);
   fChain->SetBranchAddress("PixXPos", &PixXPos, &b_PixXPos);
   fChain->SetBranchAddress("PixYPos", &PixYPos, &b_PixYPos);
   fChain->SetBranchAddress("SourceX", &SourceX, &b_SourceX);
   fChain->SetBranchAddress("SourceY", &SourceY, &b_SourceY);
   fChain->SetBranchAddress("SourceZ", &SourceZ, &b_SourceZ);
   fChain->SetBranchAddress("SourceEne", &SourceEne, &b_SourceEne);
   fChain->SetBranchAddress("Nev", &Nev, &b_Nev);
   Notify();
}

Bool_t AnaCmosMC::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void AnaCmosMC::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t AnaCmosMC::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef AnaCmosMC_cxx
