//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Mon Jan  8 17:45:09 2018 by ROOT version 6.11/02
// from TTree CMOSDataTree/CMOS exp data
// found on file: capture0000.root
//////////////////////////////////////////////////////////

#ifndef PedAnalyzer_h
#define PedAnalyzer_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.
#include "TObject.h"

class PedAnalyzer {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain
	TFile* out;
// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
 //Frame           *frame;
   UInt_t          fUniqueID;
   UInt_t          fBits;
   ULong_t         fNRow;
   ULong_t         fNCol;
   Int_t           fId;
   vector<double>  fData;

   // List of branches
   TBranch        *b_frame_fUniqueID;   //!
   TBranch        *b_frame_fBits;   //!
   TBranch        *b_frame_fNRow;   //!
   TBranch        *b_frame_fNCol;   //!
   TBranch        *b_frame_fId;   //!
   TBranch        *b_frame_fData;   //!

//	PedAnalyzer(TTree *tree=0);
	PedAnalyzer(TString filename);
	TString nomefile;
   virtual ~PedAnalyzer();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef PedAnalyzer_cxx
PedAnalyzer::PedAnalyzer(TString filename)
{
	nomefile=filename;
	// if parameter tree is not specified (or zero), connect the file
	// used to generate this class and read the Tree.
	TFile *f = new TFile(Form("%s.root",filename.Data()));
	TTree* tree = (TTree*)gDirectory->Get("CMOSDataTree");
//	ofstream prova("provaaa");
//	out=new TFile(Form("%s_PostPed.root",filename.Data()),"RECREATE");
//	out->cd();
	Init(tree);
}
/*
PedAnalyzer::PedAnalyzer(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("capture0010.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("capture0010.root");
      }
      f->GetObject("CMOSDataTree",tree);

   }
   Init(tree);
}
 */

PedAnalyzer::~PedAnalyzer()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t PedAnalyzer::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t PedAnalyzer::LoadTree(Long64_t entry)
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

void PedAnalyzer::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("fUniqueID", &fUniqueID, &b_frame_fUniqueID);
   fChain->SetBranchAddress("fBits", &fBits, &b_frame_fBits);
   fChain->SetBranchAddress("fNRow", &fNRow, &b_frame_fNRow);
   fChain->SetBranchAddress("fNCol", &fNCol, &b_frame_fNCol);
   fChain->SetBranchAddress("fId", &fId, &b_frame_fId);
   fChain->SetBranchAddress("fData", &fData, &b_frame_fData);
   Notify();
}

Bool_t PedAnalyzer::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void PedAnalyzer::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t PedAnalyzer::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef PedAnalyzer_cxx
