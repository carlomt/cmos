#include "Analisi.h"
#include <iostream>
#include <fstream>

//using namespace std;

using std::endl;
using std::cout;

ClassImp(Analisi);


Analisi::Analisi(const std::string OutFileName, Double_t fcal)
{
    
    ped = new TH1F("ped","ped",100,0.,100.);
    
    noise = new TH1F("noise","noise",50,0.,6.);
    
    NCluster = new TH1F("NCluster","Numero di cluster nei frame",200,0.,200.);
    NCluster->GetXaxis()->SetTitle("Numero di cluster");
    NCluster->Draw();
    gPad->Update();
    TPaveStats *st = (TPaveStats*)NCluster->FindObject("stats");
    st->SetOptStat(1001111);   //Stampa l'integrale
    
    map_seed = new TH2F("mapofseed","seedmap",488.,0.,488.,648.,0.,648.);
    
    map_seed_80 = new TH2F("mapofseed80","seedmap80",6.,0.,488.,8.,0.,648.);
    
    map_seed_col = new TH2F("mapofseed col", "mappa seed colonna centrale di 1 mm",488.,0.,488.,179.,461.,648.); //istogramma colonna centrale di 1 mm
    map_seed_col->Draw();
    gPad->Update();
    TPaveStats *stC = (TPaveStats*)map_seed_col->FindObject("stats");
    stC->SetOptStat(1001111);
    
    map_seed1_5 = new TH2F("mapofseed 1di5", "mappa dei seed1",89.,10.,99.,648.,0.,648.); //istogramma1 di 5
    map_seed1_5->Draw();
    gPad->Update();
    TPaveStats *st1 = (TPaveStats*)map_seed1_5->FindObject("stats");
    st1->SetOptStat(1001111);
    
    map_seed2_5 = new TH2F("mapofseed 2di5", "mappa dei seed2",89.,100.,189.,648.,0.,648.); //istogramma2 di 5
    map_seed2_5->Draw();
    gPad->Update();
    TPaveStats *st2 = (TPaveStats*)map_seed2_5->FindObject("stats");
    st2->SetOptStat(1001111);
    
    map_seed3_5 = new TH2F("mapofseed 3di5", "mappa dei seed3",89.,189.,278.,648.,0.,648.); //istogramma3 di 5
    map_seed3_5->Draw();
    gPad->Update();
    TPaveStats *st3 = (TPaveStats*)map_seed3_5->FindObject("stats");
    st3->SetOptStat(1001111);
    
    map_seed4_5 = new TH2F("mapofseed 4di5", "mappa dei seed4",89.,278.,367.,648.,0.,648.); //istogramma4 di 5
    map_seed4_5->Draw();
    gPad->Update();
    TPaveStats *st4 = (TPaveStats*)map_seed4_5->FindObject("stats");
    st4->SetOptStat(1001111);
    
    map_seed5_5 = new TH2F("mapofseed 5di5", "mappa dei seed5",125.,367.,456.,648.,0.,648.); //istogramma5 di 5
    map_seed5_5->Draw();
    gPad->Update();
    TPaveStats *st5 = (TPaveStats*)map_seed5_5->FindObject("stats");
    st5->SetOptStat(1001111);
    
    map_Row_seed = new TH1F("mapofRow","Posizione seed sulle righe",488.,0.,488.);
    map_Col_seed = new TH1F("mapofCol","Posizione seed sulle colonne",648.,0.,648.);
    
    dist_min_cluster = new TH1F("Distanza minima cluster","Distanza minima cluster",850.,0.,850.);
    dist_min_cluster->Draw();
    gPad->Update();
    TPaveStats *st6 = (TPaveStats*)dist_min_cluster->FindObject("stats");
    st6->SetOptStat(1111111);   //Stampa l'integrale
    
    R_center_vs_cluster_dist = new TH1F("Dist_Cen_Clu","Distance between center and cluster",100,-50.,500.);
    V_single = new TH1F("Sigle_pixel_signal","Sigle pixel signal DV",5000,-50.,950.);
    
    V_clu_Asy = new TH1F("Cluster_asymmetric","Cluster asymmetric signal",100,-50.,600.);
    V_clu_Asy->GetXaxis()->SetTitle("Energia Cluster (ADC)");
    V_clu_Asy->GetYaxis()->SetTitle("Conteggi");
    
    V_clu_3x3_dist = new TH1F("Cluster_3x3","Cluster signal 3x3",300,0.,600.);
    V_clu_3x3_dist->GetXaxis()->SetTitle("Energia Cluster (ADC)");
    V_clu_3x3_dist->GetYaxis()->SetTitle("Conteggi");
    
    V_clu_3x3_dist_keV = new TH1F("Cluster_3x3keV","Cluster signal 3x3 keV",600,0.,100.);
    V_clu_3x3_dist_keV->GetXaxis()->SetTitle("Energia Cluster (keV)");
    V_clu_3x3_dist_keV->GetYaxis()->SetTitle("Conteggi");
    
    V_clu_5x5_dist = new TH1F("Cluster_5x5","Cluster signal 5x5",100,-50.,600.);
    V_clu_5x5_dist->GetXaxis()->SetTitle("Energia Cluster (ADC)");
    V_clu_5x5_dist->GetYaxis()->SetTitle("Conteggi");
    
    V_clu_7x7_dist = new TH1F("Cluster_7x7","Cluster signal 7x7",100,-50.,600.);
    V_clu_7x7_dist->GetXaxis()->SetTitle("Energia Cluster (ADC)");
    V_clu_7x7_dist->GetYaxis()->SetTitle("Conteggi");
    
    Delta_5x5_vs_3x3_dist = new TH1F("Delta 5x5 vs 3x3","",100,-50.,50.);
    Delta_5x5_vs_3x3_norm_dist = new TH1F("Delta 5x5 vs 3x3 norm","",100,-50.,150.);
    Delta_7x7_vs_5x5_dist = new TH1F("Delta 7x7 vs 5x5","",100,-50.,150.);
    Delta_7x7_vs_5x5_norm_dist = new TH1F("Delta 7x7 vs 5x5 norm","",100,-50.,150.);
    
    V_vs_npixel_cluster = new TH2F("V_vs_npixel_cluster","Signal vs number of pixels in a cluster",5000.,-10.,990.,50.,0.,50.);
    npixel_cluster_vs_V = new TH2F("npixel_cluster_vs_V","Number of pixel in a cluster vs Signal",50.,0.,50.,5000.,-10.,990.);
    N_pixel_per_cluster = new TH1F("Number of pixel in a cluster","",50.,0.,50.);
    V_clu_fract_N = new TH1F("E_vs_N","Signal normalized to the number of pixels",4000,-50.,5050.);
    VpixelMax = new TH1F("EpixelMax","Energy of max pixel",2000,0.,500.);
    ContNeigh = new TH1F("ContNeigh","Frequency of neighbors pixels",100, 0.,100.);
    V_clu_vs_EPixMax = new TH2F("V_clu_vs_EPixMax","Cluster signal vs energy of max pixel",500,0.,200.,500,-50.,500.);
    V_clu_fractN_vs_EPixMax = new TH2F("V_clu_fractN_vs_EPixMax","Cluster signal normalized to N vs Energy of max pixel",500,0.,1000.,1000,-50.,5050.);
    RMS = new TH1F("RMS","RMS",100,-5.,50.);
    RMS_vs_V_clu = new TH2F("RMS_vs_V_clu","RMS vs Signal of Cluster",500,0.,200.,500,0.,150.);
    RMS_vs_V_clu_fract_N = new TH2F("RMS_vs_V_clu_fract_N","RMS vs Signal of Cluster Normalized to N",500,-50.,100.,500,-10.,150.);
    N_vs_RMS = new TH2F("N_vs_RMS","Number of pixel in a cluster vs RMS",500,-10.,300.,49,0.,49.);
    RMS_vs_EPixMax = new TH2F("RMS_vs_EPixMax","RMS vs Energy of max pixel",500,0.,300.,500,-10.,300.);
    Emax_div_E3x3_dist = new TH1F("Emax div E3x3","Rapporto Energia pixel max ed energia matrice 3x3",500,0.,10.);
    Eas_div_Npix_dist = new TH1F("Eas div Npix","Rapporto Energia cluster asimmetrico e numero di pixel nel cluster",500,0.,1000.);
    
    //****************************miei TH2F*************************
    EasDivN_vs_3x3 = new TH2F("EasDivN_vs_3x3","Eas/N ed energia del cluster3x3",500,0.,100.,500,0.,200.);
    RMS_vs_3x3 = new TH2F ("RMS_vs_3x3","RMS e segnale del cluster3x3",500,0.,70.,500,0.,200.);
    Emax_vs_3x3 = new TH2F ("Emax_vs_3x3","Emax e energia del cluster 3x3",500,0.,500.,200,0.,200.);
    N_vs_3x3 = new TH2F ("N_vs_3x3","Numero di pixel nel cluster e energia cluster 3x3",49,0.,25.,500,0.,200.);
    Easy_vs_3x3 = new TH2F ("Easy_vs_3x3","Energia cluster asy ed energia cluster 3x3",100,0.,500.,500,0.,200.);
    Emax_vs_N = new TH2F ("Emax_vs_N","Energia del pixel max e il numero di pixel nel cluster",100,0.,50.,500,0.,200.);
    EasDivN_vs_Easy = new TH2F ("EasDivN_vs_Easy","EasDivN ed energia del cluster asimmetrico",100,0.,150.,500,0.,200.);
    EasDivN_vs_N = new TH2F ("EasDivN_vs_N","EasDivN e numero di pixel nel cluster",100,0.,40.,500,0.,100.);
    
    //************************Cumulative*****************************
    
    cumulative_raggix = new TFile ("~/repos/cmos/cumulative/CumulativeX.root");
    cumulative_90Sr = new TFile ("~/repos/cmos/cumulative/Cumulative90Sr.root");
    cumulative_137Cs = new TFile ("~/repos/cmos/cumulative/Cumulative137Cs.root");
    cumulative_60Co = new TFile ("~/repos/cmos/cumulative/Cumulative60Co.root");
    
    h1 = (TH1F*)cumulative_raggix->Get("Cluster_3x3_cumulative");
    h2 = (TH1F*)cumulative_137Cs->Get("Cluster_3x3_cumulative");
    h3 = (TH1F*)cumulative_60Co->Get("Cluster_3x3_cumulative");
    h4 = (TH1F*)cumulative_90Sr->Get("Cluster_3x3_cumulative");
    
    /*
     cumulative_raggix = new TFile ("~/repos/cmos/cumulative/isocumulative_x.root");
     cumulative_90Sr = new TFile ("~/repos/cmos/cumulative/isocumulative_90Sr.root");
     cumulative_137Cs = new TFile ("~/repos/cmos/cumulative/isocumulative_137Cs.root");
     cumulative_60Co = new TFile ("~/repos/cmos/cumulative/isocumulative_60Co.root");
     
     h1 = (TH1F*)cumulative_raggix->Get("hC3x3_like_4");
     h2 = (TH1F*)cumulative_137Cs->Get("hC3x3_like_2");
     h3 = (TH1F*)cumulative_60Co->Get("hC3x3_like_3");
     h4 = (TH1F*)cumulative_90Sr->Get("hC3x3_like_1");
     */
    
    Prob_100keV = new TH1F("Prob.<100keV","Prob.<100keV",100,0.,1.);
    Prob_up_1MeV = new TH1F("Prob.>1MeV","Prob.>1MeV",100,0.,1.);
    Prob_1MeV = new TH1F("Prob.~1MeV","Prob.~1MeV",100,0.,1.);
    Prob_500keV = new TH1F("Prob.~500keV","Prob.~500keV",100,0.,1.);
    
    
    //*********************ALBERO******************************
    // fOutFile= new TFile(OutFileName.c_str(),"Recreate");
    
    /*
     
     Albero = new TTree ("Albero","Variabili per Fisher");
     // Albero->Branch("E3x3",&V_clu_3x3,"E3x3/D");
     Albero->Branch("RMS_nome_esteso",&RMS_nome_esteso,"RMS_nome_esteso/D");
     // Albero->Branch("Nclu",&N,"Nclu/I");
     // Albero->Branch("EasDivN",&Eas_div_Npix,"EasDivN/D");
     */
    fOutFile= new TFile(OutFileName.c_str(),"Recreate");
    
    
    
    std::ifstream out1("badpixel.txt",std::ios::in);
    while(!out1.eof()){
        int row=0, col=0, n=0;
        long double prob=0;
        out1>>row>>col>>prob>>n;
        //	cout<< row<<" "<< col<<endl;
        ACPoint bad(row,col);
        Badpixels.push_back(bad);
    }
    out1.close();
}

Analisi::~Analisi()
{
    delete ped;
    delete noise;
    cout<<" Numero Totale Cluster "<<map_seed->GetEntries()<<endl;
    cout<<" Zona Centrale "<<map_seed_col->Integral()<<endl;
    delete dist_min_cluster;
    delete NCluster;
    delete map_seed;
    delete map_seed_80;
    delete map_seed_col;
    delete map_seed1_5;
    delete map_seed2_5;
    delete map_seed3_5;
    delete map_seed4_5;
    delete map_seed5_5;
    delete map_Row_seed;
    delete map_Col_seed;
    delete R_center_vs_cluster_dist;
    delete V_single;
    delete V_clu_Asy;
    delete V_clu_3x3_dist;
    delete V_clu_3x3_dist_keV;
    delete V_clu_5x5_dist;
    delete V_clu_7x7_dist;
    delete Delta_5x5_vs_3x3_dist;
    delete Delta_5x5_vs_3x3_norm_dist;
    delete Delta_7x7_vs_5x5_dist;
    delete Delta_7x7_vs_5x5_norm_dist;
    delete V_vs_npixel_cluster;
    delete npixel_cluster_vs_V;
    delete N_pixel_per_cluster;
    delete V_clu_fract_N;
    delete VpixelMax;
    delete ContNeigh;
    delete V_clu_vs_EPixMax;
    delete V_clu_fractN_vs_EPixMax;
    delete RMS;
    delete RMS_vs_3x3;
    delete RMS_vs_V_clu;
    delete RMS_vs_V_clu_fract_N;
    delete N_vs_RMS;
    delete RMS_vs_EPixMax;
    delete Emax_div_E3x3_dist;   //Energia pixel max :energia cluster3x3
    delete Eas_div_Npix_dist;   //energia cluster asimmetrico:numero di pixel nel cluster
    delete EasDivN_vs_3x3;
    delete Emax_vs_3x3;
    delete N_vs_3x3;
    delete Easy_vs_3x3;
    delete Emax_vs_N;
    delete EasDivN_vs_Easy;
    delete EasDivN_vs_N;
    delete Prob_100keV;
    delete Prob_up_1MeV;
    delete Prob_1MeV;
    delete Prob_500keV;
    
    //delete Albero;
    
}

void Analisi::WriteOnFile()
{
    map_seed->Write();
    map_seed_80->Write();
    map_seed_col->Write();
    map_seed1_5->Write();
    map_seed2_5->Write();
    map_seed3_5->Write();
    map_seed4_5->Write();
    map_seed5_5->Write();
    ped->Write();
    noise->Write();
    dist_min_cluster->Write();
    Prob_100keV->Write();
    Prob_500keV->Write();
    Prob_1MeV->Write();
    Prob_up_1MeV->Write();
    NCluster->Write();
    map_Col_seed->Write();
    map_Row_seed->Write();
    R_center_vs_cluster_dist->Write();
    V_single->Write();
    V_clu_Asy->Write();
    V_clu_3x3_dist->Write();
    V_clu_3x3_dist_keV->Write();
    V_clu_5x5_dist->Write();
    V_clu_7x7_dist->Write();
    Delta_5x5_vs_3x3_dist->Write();
    Delta_5x5_vs_3x3_norm_dist->Write();
    Delta_7x7_vs_5x5_dist->Write();
    Delta_7x7_vs_5x5_norm_dist->Write();
    V_vs_npixel_cluster->Write();
    npixel_cluster_vs_V->Write();
    N_pixel_per_cluster->Write();
    V_clu_fract_N->Write();
    VpixelMax->Write();
    ContNeigh->Write();
    V_clu_vs_EPixMax->Write();
    V_clu_fractN_vs_EPixMax->Write();
    RMS->Write();
    RMS_vs_3x3->Write();
    RMS_vs_V_clu->Write();
    RMS_vs_V_clu_fract_N->Write();
    N_vs_RMS->Write();
    RMS_vs_EPixMax->Write();
    Emax_div_E3x3_dist->Write(); //scrivo
    Eas_div_Npix_dist->Write();  //scrivo
    EasDivN_vs_3x3->Write();
    Emax_vs_3x3->Write();
    N_vs_3x3->Write();
    Easy_vs_3x3->Write();
    Emax_vs_N->Write();
    EasDivN_vs_Easy->Write();
    EasDivN_vs_N->Write();
    
    //Albero->Write();
    fOutFile->Write();
    fOutFile->Close();
    
}



int Analisi::AnalisiData (SeedList *sl, Double_t fcal)
{
    const int TOLERANCE_PIXELS = 2;
    
    int Row_seed = 0;
    int Col_seed = 0;
    int Row_seed2 = 0;
    int Col_seed2 = 0;
    double dist_min = 10000, dist = 10000;
    int flag1=0;
    
    double soglia_vicini = 10; //soglia vicinanza cluster
    double V_adja = 2.610; //fSecondaryThr prima era 4.0
    int cont = 0;
    
    int Ncluster = 0;
    
    //cout<<"Frame"<<endl;
    
    for(size_t i=0; i<sl->Size(); i++){
        int  flag2=0;
        Seed ts = sl->At(i);
        Row_seed = ts.GetRow();
        Col_seed = ts.GetCol();
        double EPixMax=0.;
        N = 1;
        
        //////////////////////////////////////ELIMINAZIONE BAD PIXEL//////////////////////////////////
        
        int flag=0;
        for(int i=0; i<Badpixels.size();i++){
            if(Badpixels.at(i).x == Row_seed && Badpixels.at(i).y == Col_seed){
                flag=1;
                break;
            }
        }
        if(flag==1) continue;
        
        /////////////////////////////////ELIMINAZIONE CLUSTER VICINI//////////////////////////////////
        
        
        //cout<<"1  "<<Row_seed<<" "<<Col_seed<<endl;
        for(size_t j=0; j<sl->Size(); j++){
            
            Seed ts2 = sl->At(j);
            Row_seed2 = ts2.GetRow();
            Col_seed2 = ts2.GetCol();
            if((Row_seed == Row_seed2) && (Col_seed == Col_seed2)) continue;
            
            int flag=0;
            for(int i=0; i<Badpixels.size();i++){
                if(Badpixels.at(i).x == Row_seed2 && Badpixels.at(i).y == Col_seed2){
                    flag=1;
                    break;
                }
            }
            if(flag==1) continue;
            
            
            //cout<<"2  "<<Row_seed2<<" "<<Col_seed2<<endl;
            
            int distX = Row_seed - Row_seed2;
            int distY = Col_seed - Col_seed2;
            
            dist = sqrt(distX*distX+distY*distY);
            flag1=0;
            //cout<<"Distanza "<<dist<<" Energia 1 "<<ts(0,0)<<" Energia 2 "<<ts2(0,0)<<endl;
            
            if(dist <= soglia_vicini && (ts(0,0) < ts2(0,0))){
                //cout<<"Soglia"<<endl;
                flag1=1;
                break;
            }
            if(dist<=soglia_vicini)flag2=1;
            //if(dist > soglia_vicini){
            //dist_min_cluster->Fill(dist);
            //}
            if(dist<dist_min){
                dist_min = dist;
            }
            
            if(distX <= 3 && distX >= -3 && distY <= 3 && distY >= -3 && (distX!=0 || distY!=0)){
                cont++;
            }
        }
        
        if(flag1==1) continue;
        
        //if(dist_min != 10000){
        //cout<<endl<<"Dist min "<<dist_min<<endl<<endl;
        if(flag2==0)dist_min_cluster->Fill(dist_min);
        //}
        dist_min = 10000;
        Ncluster += 1;
        
        ///////////////////////////////////////////////////////////////////////////////////////////
        
        map_Row_seed->Fill(Row_seed);
        map_Col_seed->Fill(Col_seed);
        map_seed->Fill((Row_seed),(Col_seed));//perche cast?
        map_seed_80->Fill((Row_seed),(Col_seed));
        map_seed_col->Fill((Row_seed),(Col_seed));
        map_seed1_5->Fill((Row_seed),(Col_seed));
        map_seed2_5->Fill((Row_seed),(Col_seed));
        map_seed3_5->Fill((Row_seed),(Col_seed));
        map_seed4_5->Fill((Row_seed),(Col_seed));
        map_seed5_5->Fill((Row_seed),(Col_seed));
        double R_center_vs_cluster;
        R_center_vs_cluster = sqrt(pow((240.-Row_seed),2) +pow((320.-Col_seed),2));
        R_center_vs_cluster_dist->Fill(R_center_vs_cluster);
        
        double V_clu_fract_n = 0;
        double V_clu_A = 0.;
        V_clu_3x3 = 0.;// xk già in .h
        double V_clu_3x3_keV=0.;
        double V_clu_5x5 = 0.;
        double V_clu_7x7 = 0.;
        double Delta_5x5_vs_3x3 = 0.;
        double Delta_5x5_vs_3x3_norm = 0.;
        double Delta_7x7_vs_5x5 = 0.;
        double Delta_7x7_vs_5x5_norm = 0.;
        double RMS_2 = 0.;
        double RMS_2_1 = 0.;
        RMS_nome_esteso = 0.;
        Double_t Emax_div_E3x3 =0.;
        Eas_div_Npix =0.;
        //RF definire
        // char logStr[10000];
        // char logStr3[10000];
        // sprintf(logStr, ""); //?!?!
        
        std::vector<ACPoint> preCluster;// = vector<ACPoint>{};
        std::vector<ACPoint> cluster;// = vector<ACPoint>{};
        cluster.push_back(ACPoint(0,0));
        
        for (int k = ts.GetPixelMin(); k < ts.GetPixelMax(); k++)
        {
            for (int l = ts.GetPixelMin(); l < ts.GetPixelMax(); l++)
            {
                float tsValue = ts(l,k);
                if(tsValue>EPixMax)
                {
                    EPixMax=tsValue;
                }
                
                bool isOverThreshold = tsValue > V_adja; //cerco tutti i pixel maggiori della soglia secondaria
                
                // if (isOverThreshold)
                // 	{
                // 	  sprintf(logStr, "%s[[%+04.0f]] ", logStr, tsValue);
                // 	}
                // else
                // 	{
                // 	  sprintf(logStr, "%s (%+04.0f)  ", logStr, tsValue);
                // 	}
                
                
                if (k == 0 && l == 0)
                {
                    continue;
                }
                
                if (isOverThreshold)
                {
                    ACPoint currentPoint = ACPoint(l,k); //creo il punto corrispondente al k e l attuali nel ciclo
                    preCluster.push_back(currentPoint);//scrivo il  pixel elegibile
                    
                }
            }
            //	  sprintf(logStr, "%s\n", logStr);
        }
        
        //      sprintf(logStr, "%s\n\n", logStr);
        
        bool found; // found = esiste almeno un pixel nei precluster adiacente ad uno di quelli nel cluster?
        
        do {
            found = false;
            
            for (size_t i = 0; i < preCluster.size(); i++)
            {
                
                ACPoint preClusterPoint = preCluster.at(i); //dammi il punto all'i-sima posizione della lista di precluster
                for (size_t j = 0; j < cluster.size(); j++)
                {
                    ACPoint clusterPoint = cluster.at(j);
                    if (std::abs(preClusterPoint.x-clusterPoint.x) <= TOLERANCE_PIXELS && std::abs(preClusterPoint.y-clusterPoint.y) <= TOLERANCE_PIXELS)
                    {
                        
                        //		    sprintf(logStr, "%s\n(%d, %d) is near (%d, %d)", logStr, preClusterPoint.x, preClusterPoint.y, clusterPoint.x, clusterPoint.y);
                        N++;//numero pixel nel cluster
                        found = true;
                        cluster.push_back(preClusterPoint);
                        preCluster.erase(preCluster.begin()+i);
                        break;
                    }
                }
                
                if (found)
                {
                    break;
                }
            }
            
        } while (found);
        //      sprintf(logStr, "%s\n\n", logStr);
        
        
        //      cout << logStr;
        // char logStr2[10000];
        //      sprintf(logStr2, "Result: ");
        
        ///////////////////////Calcolo Cluster Asimmetrico/////////////////////////
        //      if(N==4)cout<<" cluster a 4 pixels"<< endl; //debug 4pixels
        //cout<< cluster.size()<< " "<< N << endl;
        for (size_t i = 0; i < cluster.size(); i++)
        {
            ACPoint currentPoint = cluster.at(i);
            //	  sprintf(logStr2, "%s (%d, %d) -", logStr2, currentPoint.x, currentPoint.y);
            V_clu_A += ts(currentPoint.x, currentPoint.y);
            //  if(N==4)
            // cout<<currentPoint.x<<" " <<currentPoint.y<<" "<<ts(currentPoint.x,currentPoint.y)<<endl;//debug 4pixels
        }
        V_clu_fract_n = (V_clu_A)/cluster.size();
        //      sprintf(logStr2, "%s\n\n\n\n\n\n", logStr2);
        //      cout << logStr2;
        
        //////////////////////Calcolo RMS Cluster Asimmetrico/////////////////////
        
        for (size_t i = 0; i < cluster.size(); i++)
        {
            ACPoint currentPoint = cluster.at(i);
            float tsValue = ts(currentPoint.x,currentPoint.y);
            //	  sprintf(logStr3, "%s[[%f]]\n ",logStr3,tsValue);
            RMS_2_1+= (pow((tsValue - V_clu_fract_n),2));
            
        }
        RMS_2=cluster.size()>1 ? RMS_2_1 /(cluster.size()-1):0;
        RMS_nome_esteso = sqrt(RMS_2);
        
        //////////////////////////////DEBUG RMS//////////////////////////////////////
        
        /* if (RMS_2 <1  && cluster.size()==15)
         {
         //cout << logStr;
         //cout << logStr2;
         //	  cout<<logStr3;
         cout<<"V_clu ="<<V_clu_A<<endl;
         cout<<"Npix ="<<cluster.size()<<endl;
         cout<<"V_clu_fractN ="<<V_clu_fract_n<<endl;
         cout<<"RMS2_1 ="<<RMS_2_1<<endl;
         cout<<"RMS2 ="<<RMS_2<<endl;
         cout<<"RMS =" << RMS_<<endl;
         }*/
        
        ///////////////////////Calcolo Cluster Simmetrici/////////////////////
        
        for(int j=ts.GetPixelMin(); j<ts.GetPixelMax(); j++)
        {
            for(int i=ts.GetPixelMin(); i<ts.GetPixelMax(); i++)
            {
                V_single->Fill(ts(i,j));
                
                if(j>=-1 && j<=1 && i>=-1 && i<=1)
                {
                    V_clu_3x3+=ts(i,j);
                }
                
                if(j>=-2 && j<=2 && i>=-2 && i<=2)
                {
                    V_clu_5x5+=ts(i,j);
                }
                
                if(j>=-3 && j<=3 && i>=-3 && i<=3)
                {
                    V_clu_7x7+=ts(i,j);
                }
                
            }
        }
        
        ////////////////////////////Calcolo del Delta///////////////////////////
        
        Delta_5x5_vs_3x3 = V_clu_5x5 - V_clu_3x3;
        Delta_5x5_vs_3x3_norm = (V_clu_5x5 - V_clu_3x3)/16;
        Delta_7x7_vs_5x5 = V_clu_7x7 -V_clu_5x5;
        Delta_7x7_vs_5x5_norm = (V_clu_7x7 - V_clu_5x5)/24;
        Emax_div_E3x3 =(float(EPixMax) / V_clu_3x3);
        Eas_div_Npix = (V_clu_A /float(N));
        //RF qui calcoli
        
        ///////////////////////Riempimento Istogrammi//////////////////////////
        
        V_vs_npixel_cluster->Fill(float(V_clu_A),float(N));
        npixel_cluster_vs_V->Fill(float(N),float(V_clu_A));
        V_clu_fract_N->Fill(float(V_clu_fract_n));
        N_pixel_per_cluster->Fill(float(N));
        VpixelMax->Fill(float(EPixMax));
        RMS->Fill(float(RMS_nome_esteso));
        V_clu_vs_EPixMax->Fill(float(EPixMax),float(V_clu_A));
        V_clu_fractN_vs_EPixMax->Fill(float(EPixMax),float(V_clu_fract_n));
        RMS_vs_V_clu->Fill(float(V_clu_A),float(RMS_nome_esteso));
        RMS_vs_V_clu_fract_N->Fill(float(V_clu_fract_n),float(RMS_nome_esteso));
        //********************
        RMS_vs_3x3->Fill(float(RMS_nome_esteso),V_clu_3x3);
        //**************************
        N_vs_RMS->Fill(float(RMS_nome_esteso),float(N));
        RMS_vs_EPixMax->Fill(float(EPixMax),float(RMS_nome_esteso));
        V_clu_Asy->Fill(V_clu_A);
        V_clu_3x3_dist->Fill(V_clu_3x3);
        
        ///////////////////////////Conversione in keV//////////////////////////
        
        V_clu_3x3_keV = V_clu_3x3/fcal;//Fattore di calibrazione ADC/keV
        V_clu_3x3_dist_keV->Fill(V_clu_3x3_keV);
        
        ///////////////////////////////////////////////////////////////////////
        
        V_clu_5x5_dist->Fill(V_clu_5x5);
        V_clu_7x7_dist->Fill(V_clu_7x7);
        Delta_5x5_vs_3x3_dist->Fill(Delta_5x5_vs_3x3);
        Delta_5x5_vs_3x3_norm_dist->Fill(Delta_5x5_vs_3x3_norm);
        Delta_7x7_vs_5x5_dist->Fill(Delta_7x7_vs_5x5);
        Delta_7x7_vs_5x5_norm_dist->Fill(Delta_7x7_vs_5x5_norm);
        Emax_div_E3x3_dist->Fill(Emax_div_E3x3); // qui calcoli
        Eas_div_Npix_dist->Fill(Eas_div_Npix);
        EasDivN_vs_3x3->Fill(Eas_div_Npix,V_clu_3x3);
        Emax_vs_3x3->Fill(float(EPixMax),V_clu_3x3);
        N_vs_3x3->Fill(float(N),V_clu_3x3);
        Easy_vs_3x3->Fill(V_clu_A,V_clu_3x3);
        Emax_vs_N->Fill(float(N),float(EPixMax));
        EasDivN_vs_Easy->Fill(Eas_div_Npix,float(V_clu_A));
        EasDivN_vs_N->Fill(Eas_div_Npix,float(N));
        
        /////////////////////////////////Calcolo cumulative///////////////////////////////
        
        int bin = V_clu_3x3; //Impostazione dovuta al binnaggio dell'istogramma
        
        double bin_content_1 = h1 -> GetBinContent(bin);
        double bin_content_2 = h2 -> GetBinContent(bin);
        double bin_content_3 = h3 -> GetBinContent(bin);
        double bin_content_4 = h4 -> GetBinContent(bin);
        
        double sum = bin_content_1 + bin_content_2 + bin_content_3 + bin_content_4;
        
        bin_content_1 /= sum;
        bin_content_2 /= sum;
        bin_content_3 /= sum;
        bin_content_4 /= sum;

        if(sum!=0){
            Prob_100keV->Fill(bin_content_1);
            Prob_500keV->Fill(bin_content_2);
            Prob_1MeV->Fill(bin_content_3);
            Prob_up_1MeV->Fill(bin_content_4);
        }
    }
    
    /////////////////////////////////////////////////////////////////////////////////////
    /*
     //      if(N>0)
     {
     Albero->Fill();
     //cout<<N<< " "<<Eas_div_Npix<< endl;
     }
     */
    
    /////////////////////////Conteggio Seed Vicini///////////////////////////
    /*
     dist_min = 10000, dist = 10000;
     
     //cout<<"Frame"<<endl;
     
     for(size_t i=0; i<sl->Size(); i++){
     
     Seed ts = sl->At(i);
     Row_seed = ts.GetRow();
     Col_seed = ts.GetCol();
     
     int flag=0;
     for(int i=0; i<Badpixels.size();i++){
     if(Badpixels.at(i).x == Row_seed && Badpixels.at(i).y == Col_seed){
     flag=1;
     break;
     }
     }
     if(flag==1) continue;
     
     for(size_t j=i+1; j<sl->Size(); j++){
     
     Seed ts2 = sl->At(j);
     Row_seed2 = ts2.GetRow();
     Col_seed2 = ts2.GetCol();
     
     int flag=0;
     for(int i=0; i<Badpixels.size();i++){
     if(Badpixels.at(i).x == Row_seed2 && Badpixels.at(i).y == Col_seed2){
     flag=1;
     break;
     }
     }
     if(flag==1) continue;
     
     //cout<<"1  "<<Row_seed<<" "<<Col_seed<<endl;
     //cout<<"2  "<<Row_seed2<<" "<<Col_seed2<<endl;
     
     if((Row_seed == Row_seed2) && (Col_seed == Col_seed2)){
     dist_min = 10000;
     continue;
     }
     
     int distX = Row_seed - Row_seed2;
     int distY = Col_seed - Col_seed2;
     
     dist = sqrt(distX*distX+distY*distY);
     
     if(dist<dist_min){
     dist_min = dist;
     }
     
     if(distX <= 3 && distX >= -3 && distY <= 3 && distY >= -3 && (distX!=0 || distY!=0)){
     cont++;
     }
     }
     //cout<<endl<<"Dist min "<<dist_min<<endl<<endl;
     //dist_min_cluster->Fill(dist_min);
     //dist_min = 10000;
     
     }
     */
    ContNeigh->Fill(cont);
    
    NCluster->Fill(Ncluster);
    
    return(1);
    
}
