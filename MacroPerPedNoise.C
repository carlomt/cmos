/*Verifica consistenza tra run di buio diversi*/

void MacroPerPed1()
{
  gROOT->SetBatch(kTRUE); //evita che la funzione Draw disegni davvero millemila canvas
  
  //apertura file di input buio
  TFile f1 ("buio1.root", "READ");
  TTree *Tree1 = (TTree*)f1.Get("CMOSDataTree");
  TFile f2 ("buio2.root", "READ");
  TTree *Tree2 = (TTree*)f2.Get("CMOSDataTree");
  //apertura file di input noise
  TFile f3 ("noise1.root", "READ");
  TTree *Tree3 = (TTree*)f3.Get("CMOSDataTree");
  TFile f4 ("noise2.root", "READ");
  TTree *Tree4 = (TTree*)f4.Get("CMOSDataTree");
  //apertura file di output
  TFile fout ("outputDiff.root","RECREATE");
  
  TH1F *diff=new TH1F("diff","Differenze", 100, -10, 10); //istogramma per mettere le differenze
  
  //int nrow=648, ncol=488;
  int nrow=64, ncol=48; //versione con poche righe per vedere se funziona, poi riattivare quello vero (lento..:D)
  double Val1[nrow][ncol];
  double Val2[nrow][ncol];
  double temp1, temp2, temp3, temp4;
  //TH1F *histo=new TH1F();
  for (int ii=0; ii<nrow; ii++) {
    cout<<"Inizio riga ii= "<<ii<<endl;
    for (int jj=0; jj<ncol; jj++) {
      Tree1->Draw(Form("fData[%d][%d]",ii, jj)); //Finge di disegnarlo, ma in realtà per farlo lo schiaffa in un TH1F temporaneo chiamato "htemp"
      TH1 *myh1 = (TH1*)gPad->GetPrimitive("htemp"); //ci copiamo da qualche parte questo istogramma per non sovrascriverlo subito
      //Val1[ii][jj]=myh1->GetMean();  //in teoria potremmo salvare tutti i valori, ma se non serve risparmiamo memoria e tempo
      temp1=myh1->GetMean(); //prendiamo il valore medio, tanto c'è un solo valore
      
      Tree2->Draw(Form("fData[%d][%d]",ii, jj)); //idem per il secondo tree, cioè per il secondo run
      TH1 *myh2 = (TH1*)gPad->GetPrimitive("htemp");
      //Val2[ii][jj]=myh2->GetMean();
      temp2=myh2->GetMean();

      Tree3->Draw(Form("fData[%d][%d]",ii, jj)); //idem per il terzo tree
      TH1 *myh3 = (TH1*)gPad->GetPrimitive("htemp");
      temp3=myh3->GetMean();

      Tree4->Draw(Form("fData[%d][%d]",ii, jj)); //idem per il quarto tree
      TH1 *myh4 = (TH1*)gPad->GetPrimitive("htemp");
      temp4=myh4->GetMean();

      
      diff->Fill((temp2-temp1)/sqrt(pow(temp3, 2)+pow(temp4,2))); //riempiamo l'istogramma con le differenze
    }
    
  }
  //CMOSDataTree->Draw("fData[10][10]");
  diff->Write();//salva l'istogramma sul file di ouptut
  fout.Write();
  
}
