#include <cstdlib>
#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <chrono>
#include <ctime>
#include <cstdlib>

#include "TChain.h"
#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TObjString.h"
#include "TSystem.h"
#include "TROOT.h"
#include "TFileCollection.h"
#include "TH1.h"
#include "TH2.h"
#include "TH3.h"
#include "TMath.h"
#include "TComplex.h"

#include "TMVA/Tools.h"
#include "TMVA/Reader.h"

//#include "initAnalysisBinning_pt_tmp.h"

using namespace std::chrono;
using namespace std;
using namespace TMVA;

//Part from header file                                                                                                                                                                                   
const int N_CENTBIN = 3;
Int_t min_centbin[N_CENTBIN]={0, 10, 30};
Int_t max_centbin[N_CENTBIN]={10, 30, 50};
Double_t centbinning[N_CENTBIN+1]={0, 10, 30, 50};
TString label_centbin[N_CENTBIN]={"cent0to10", "cent10to30", "cent30to50"};
TString centbin[N_CENTBIN]={"0 <= cent < 10 %", "10 <= cent < 30 %", "30 <= cent < 50 %"};

const int N_DCABIN = 1;
Float_t min_Dcabin[N_DCABIN]={0.0};
Float_t max_Dcabin[N_DCABIN]={0.01};
TString label_Dcabin[N_DCABIN]={"0to0p01"};
TString Dcabin[N_DCABIN]={"0 < dcaSig < 0.01"};
Double_t dcabinning[N_DCABIN+1]={0.0, 0.01};

const int N_PTBIN = 6;
Float_t min_pTbin[N_PTBIN]={4.0,6.0,8.0,10.0,15.0, 20.0};
Float_t max_pTbin[N_PTBIN]={6.0,8.0,10.0,15.0,20.0, 40.0};
TString label_pTbin[N_PTBIN]={"pT4to6","pT6to8","pT8to10","pT10to15","pT15to20", "pT20to40"};
TString pTbin[N_PTBIN]={"4.0 < pT < 6.0 GeV","6.0 < pT < 8.0","8.0 < pT < 10.0 GeV","10.0 < pT < 15.0 GeV","15.0 < pT < 20.0 GeV", "20.0 < pT < 40.0 GeV"};
Double_t ptbinning[N_PTBIN+1]={4.0, 6.0, 8.0, 10.0, 15.0, 20.0, 40.0};


/*                                                                                                        
                                                                                                   
#define N_DCABIN 18                                                                                                                                                                                               
Float_t min_Dcabin[N_DCABIN]={0.00,0.001,0.0023,0.0039,0.0059,0.0085,0.0118,0.0160,0.0214,0.0281,0.0367,0.0476,0.0615,0.0791,0.1015,0.1350,0.1700,0.230};                                                        
Float_t max_Dcabin[N_DCABIN]={0.001,0.0023,0.0039,0.0059,0.0085,0.0118,0.0160,0.0214,0.0281,0.0367,0.0476,0.0615,0.0791,0.1015,0.1350,0.1700,0.230,0.300};                                                       
TString label_Dcabin[N_DCABIN]={"0.00to0.001","0.001to0.0023","0.0023to0.0039","0.0039to0.0059","0.0059to0.0085","0.0085to0.0118","0.0118to0.0160","0.0160to0.0214","0.0214to0.0281","0.0281to0.0367","0.0367to0.0476
","0.0476to0.0615","0.0615to0.0791","0.0791to0.1015","0.1015to0.1350","0.1350to0.1700","0.1700to0.230","0.230to0.300"};                                                                                         
TString Dcabin[N_DCABIN]={ "0.00 < dca < 0.001","0.001 < dca < 0.0023","0.0023 < dca < 0.0039","0.0039 < dca < 0.0059","0.0059 < dca < 0.0085","0.0085 < dca < 0.0118","0.0118 < dca < 0.0160","0.0160 < dca < 0.0214
","0.0214 < dca < 0.0281","0.0281 < dca < 0.0367","0.0367 < dca < 0.0476","0.0476 < dca < 0.0615","0.0615 < dca < 0.0791","0.0791 < dca < 0.1015","0.1015 < dca < 0.1350","0.1350 < dca < 0.1700","0.1700 < dca <0.230","0.230 < dca < 0.300"};                                                                                                                                                                                   
Double_t dcabinning[N_DCABIN+1]={0.00,0.001,0.0023,0.0039,0.0059,0.0085,0.0118,0.0160,0.0214,0.0281,0.0367,0.0476,0.0615,0.0791,0.1015,0.1350,0.1700,0.230,0.300};                                                 
*/




const int N_VNBIN = 65;

Double_t min_vNbin[N_VNBIN]={-6.5, -6.3, -6.1, -5.9, -5.7, -5.5, -5.3, -5.1, -4.9, -4.7, -4.5, -4.3, -4.1, -3.9,-3.7,-3.5, -3.3, -3.1, -2.9, -2.7, -2.5, -2.3, -2.1, -1.9, -1.7, -1.5, -1.3, -1.1, -0.9, -0.7, -0.5, -0.3, -0.1, 0.1, 0.3, 0.5, 0.7, 0.9, 1.1, 1.3, 1.5, 1.7, 1.9, 2.1, 2.3, 2.5, 2.7, 2.9, 3.1, 3.3, 3.5, 3.7, 3.9, 4.1, 4.3, 4.5, 4.7, 4.9, 5.1, 5.3, 5.5, 5.7, 5.9, 6.1, 6.3};
Double_t max_vNbin[N_VNBIN]={ -6.3, -6.1, -5.9, -5.7, -5.5, -5.3, -5.1, -4.9, -4.7, -4.5, -4.3, -4.1, -3.9,-3.7,-3.5,-3.3, -3.1, -2.9, -2.7, -2.5, -2.3, -2.1, -1.9, -1.7, -1.5, -1.3, -1.1, -0.9, -0.7, -0.5, -0.3, -0.1, 0.1, 0.3, 0.5, 0.7, 0.9, 1.1, 1.3, 1.5, 1.7, 1.9, 2.1, 2.3, 2.5, 2.7, 2.9, 3.1, 3.3, 3.5, 3.7, 3.9, 4.1, 4.3, 4.5, 4.7, 4.9, 5.1, 5.3, 5.5, 5.7, 5.9, 6.1, 6.3, 6.5};
Double_t vnbinning[N_VNBIN+1]={-6.5, -6.3, -6.1, -5.9, -5.7, -5.5, -5.3, -5.1, -4.9, -4.7, -4.5, -4.3, -4.1, -3.9,-3.7,-3.5, -3.3, -3.1,-2.9, -2.7, -2.5, -2.3, -2.1, -1.9, -1.7, -1.5, -1.3, -1.1, -0.9,-0.7, -0.5, -0.3, -0.1, 0.1, 0.3, 0.5, 0.7, 0.9, 1.1, 1.3, 1.5, 1.7, 1.9, 2.1, 2.3, 2.5, 2.7, 2.9, 3.1, 3.3, 3.5, 3.7, 3.9, 4.1, 4.3, 4.5, 4.7, 4.9, 5.1, 5.3, 5.5, 5.7, 5.9, 6.1, 6.3, 6.5};


//#define N_MASSBIN 52                                                                                                                                                                                    
const int N_MASSBIN = 52;
Double_t massbinning[N_MASSBIN+1];


///Function to get optimized BDT cuts
/*Double_t GetBDTCut(Double_t pt, Int_t CentBin) {
  Double_t bdtcut = -1.;
  Double_t bdtcut_[N_PTBIN] = {0.12, 0.16, 0.12, 0.15, 0.30};
  Double_t ptBDTbin[N_PTBIN+1] = {4.0, 6.0, 8.0, 10.0, 15.0, 20.0};

  if (CentBin > 0 || CentBin < 80) {
    for (int i=0; i < N_PTBIN; i++)  if (ptBDTbin[i]<pt && ptBDTbin[i+1]>pt) bdtcut = bdtcut_[i];
  }
  return bdtcut;
  }*/

Double_t GetBDTCut(Double_t pt, Int_t CentBin) {
  Double_t bdtcut = -1.;

  //for S/sqrt(S+B)
  /*Double_t bdtcut_0_10[N_PTBIN] = {0.105, 0.165, 0.125, 0.145, 0.105};
  Double_t bdtcut_10_30[N_PTBIN] = {0.085, 0.115, 0.055, 0.065, 0.065};
  Double_t bdtcut_30_50[N_PTBIN] = {-0.045, -0.065, -0.085, -0.155, -0.285};*/

  //for S/S_error
  /*Double_t bdtcut_0_10[N_PTBIN] = {0.105, 0.205, 0.135, 0.155, -0.055};
  Double_t bdtcut_10_30[N_PTBIN] = {0.265, 0.095, 0.075, 0.105, 0.215};
  Double_t bdtcut_30_50[N_PTBIN] = {-0.075, -0.085, -0.155, -0.065, -0.295};*/

  //New BDT cuts
  Double_t bdtcut_0_10[N_PTBIN] = {0.141, 0.135, 0.144, 0.189, 0.08, 0.1764 };
  Double_t bdtcut_10_30[N_PTBIN] = {0.095, 0.102, 0.113, 0.189, 0.0757, 0.1781};
  Double_t bdtcut_30_50[N_PTBIN] = {0.0795, 0.109, 0.053, 0.154, 0.178, 0.0746};


  Double_t ptBDTbin[N_PTBIN+1] = {4.0, 6.0, 8.0, 10.0, 15.0, 20.0, 40.0};

  if (CentBin < 10) {
    for (int i=0; i<N_PTBIN; i++)  if (ptBDTbin[i]<pt && ptBDTbin[i+1]>pt) bdtcut = bdtcut_0_10[i];
  }
  if (CentBin >= 10 && CentBin < 30) {
    for (int i=0; i<N_PTBIN; i++)  if (ptBDTbin[i]<pt && ptBDTbin[i+1]>pt) bdtcut = bdtcut_10_30[i];
  }

  if (CentBin >= 30 && CentBin < 50) {
    for (int i=0; i<N_PTBIN; i++)  if (ptBDTbin[i]<pt && ptBDTbin[i+1]>pt) bdtcut = bdtcut_30_50[i];
  }

  return bdtcut;
}


void hist_vn_mass(TString input_file, int itxtoutFile, TString out_file ){
  
  TH1::StatOverflows(kTRUE);
  
  const char* date = "Jan10";
  std::string outfilename = Form("%s/Hist_QQnVsMass_MB0to19_withDCA0p01_%s_%d" , out_file.Data(), date, itxtoutFile);
  TFile* fout = new TFile(Form("%s.root", outfilename.c_str()), "recreate");

  //std::string outfilename = Form("%s/Hist_cent080_QQnVsMass_MB2to19_%s_%d" , out_file.Data(), date, itxtoutFile);
  //TFile* fout = new TFile("test.root", "recreate");

  TH1D *hist_Q2Q2_HFmHFp_Re_centbin[N_CENTBIN];//centrality
  TH1D *hist_Q2Q2_HFmHFp_Im_centbin[N_CENTBIN];//centrality
  TH1D *aux_hist_Q2Q2_HFmHFp = new TH1D("aux_hist_Q2Q2_HFmHFp","",6000,-30000,30000);
  
  TH1D *hist_Q2Q2_HFmTrk_Re_centbin[N_CENTBIN];//centrality
  TH1D *hist_Q2Q2_HFmTrk_Im_centbin[N_CENTBIN];//centrality
  TH1D *aux_hist_Q2Q2_HFmTrk = new TH1D("aux_hist_Q2Q2_HFmTrk","",6000,-30000,30000);
  
  TH1D *hist_Q2Q2_HFpTrk_Re_centbin[N_CENTBIN];//centrality
  TH1D *hist_Q2Q2_HFpTrk_Im_centbin[N_CENTBIN];//centrality
  TH1D *aux_hist_Q2Q2_HFpTrk = new TH1D("aux_hist_Q2Q2_HFpTrk","",6000,-30000,30000);
  
  TH1D *hist_Q3Q3_HFmHFp_Re_centbin[N_CENTBIN];//centrality
  TH1D *hist_Q3Q3_HFmHFp_Im_centbin[N_CENTBIN];//centrality
  TH1D *aux_hist_Q3Q3_HFmHFp = new TH1D("aux_hist_Q3Q3_HFmHFp","",3000,-15000,15000);
  
  TH1D *hist_Q3Q3_HFmTrk_Re_centbin[N_CENTBIN];//centrality
  TH1D *hist_Q3Q3_HFmTrk_Im_centbin[N_CENTBIN];//centrality
  TH1D *aux_hist_Q3Q3_HFmTrk = new TH1D("aux_hist_Q3Q3_HFmTrk","",3000,-15000,15000);
  
  TH1D *hist_Q3Q3_HFpTrk_Re_centbin[N_CENTBIN];//centrality
  TH1D *hist_Q3Q3_HFpTrk_Im_centbin[N_CENTBIN];//centrality
  TH1D *aux_hist_Q3Q3_HFpTrk = new TH1D("aux_hist_Q3Q3_HFpTrk","",3000,-15000,15000);
  
  TH1D *hist_Q1Q1_HFmHFp_Re_centbin[N_CENTBIN];//centrality
  TH1D *hist_Q1Q1_HFmHFp_Im_centbin[N_CENTBIN];//centrality
  TH1D *aux_hist_Q1Q1_HFmHFp = new TH1D("aux_hist_Q1Q1_HFmHFp","",6000,-30000,30000);
  
  /*TH1D *hist_Q1Q1Q2Q2_HFmHFpHFmHFp_Re_centbin[N_CENTBIN];//centrality
    TH1D *hist_Q1Q1Q2Q2_HFmHFpHFmHFp_Im_centbin[N_CENTBIN];//centrality
    TH1D *aux_hist_Q1Q1Q2Q2_HFmHFpHFmHFp = new TH1D("aux_hist_Q1Q1Q2Q2_HFmHFpHFmHFp","",20000,-1000000,1000000);
    
    TH1D *hist_Q2Q2Q2Q2_HFmHFmHFpHFp_Re_centbin[N_CENTBIN];//centrality
    TH1D *hist_Q2Q2Q2Q2_HFmHFmHFpHFp_Im_centbin[N_CENTBIN];//centrality
    TH1D *aux_hist_Q2Q2Q2Q2_HFmHFmHFpHFp = new TH1D("aux_hist_Q2Q2Q2Q2_HFmHFmHFpHFp","",20000,-1000000,1000000);*/
  
  for(Int_t i=0; i<N_CENTBIN; i++){
    hist_Q2Q2_HFmHFp_Re_centbin[i] = (TH1D *)aux_hist_Q2Q2_HFmHFp->Clone("Q2Q2_HFmHFp_Re_"+label_centbin[i]);
    hist_Q2Q2_HFmHFp_Im_centbin[i] = (TH1D *)aux_hist_Q2Q2_HFmHFp->Clone("Q2Q2_HFmHFp_Im_"+label_centbin[i]);
    hist_Q2Q2_HFmTrk_Re_centbin[i] = (TH1D *)aux_hist_Q2Q2_HFmTrk->Clone("Q2Q2_HFmTrk_Re_"+label_centbin[i]);
    hist_Q2Q2_HFmTrk_Im_centbin[i] = (TH1D *)aux_hist_Q2Q2_HFmTrk->Clone("Q2Q2_HFmTrk_Im_"+label_centbin[i]);
    hist_Q2Q2_HFpTrk_Re_centbin[i] = (TH1D *)aux_hist_Q2Q2_HFpTrk->Clone("Q2Q2_HFpTrk_Re_"+label_centbin[i]);
    hist_Q2Q2_HFpTrk_Im_centbin[i] = (TH1D *)aux_hist_Q2Q2_HFpTrk->Clone("Q2Q2_HFpTrk_Im_"+label_centbin[i]);
    
    hist_Q3Q3_HFmHFp_Re_centbin[i] = (TH1D *)aux_hist_Q3Q3_HFmHFp->Clone("Q3Q3_HFmHFp_Re_"+label_centbin[i]);
    hist_Q3Q3_HFmHFp_Im_centbin[i] = (TH1D *)aux_hist_Q3Q3_HFmHFp->Clone("Q3Q3_HFmHFp_Im_"+label_centbin[i]);
    hist_Q3Q3_HFmTrk_Re_centbin[i] = (TH1D *)aux_hist_Q3Q3_HFmTrk->Clone("Q3Q3_HFmTrk_Re_"+label_centbin[i]);
    hist_Q3Q3_HFmTrk_Im_centbin[i] = (TH1D *)aux_hist_Q3Q3_HFmTrk->Clone("Q3Q3_HFmTrk_Im_"+label_centbin[i]);
    hist_Q3Q3_HFpTrk_Re_centbin[i] = (TH1D *)aux_hist_Q3Q3_HFpTrk->Clone("Q3Q3_HFpTrk_Re_"+label_centbin[i]);
    hist_Q3Q3_HFpTrk_Im_centbin[i] = (TH1D *)aux_hist_Q3Q3_HFpTrk->Clone("Q3Q3_HFpTrk_Im_"+label_centbin[i]);
    
    hist_Q1Q1_HFmHFp_Re_centbin[i] = (TH1D *)aux_hist_Q1Q1_HFmHFp->Clone("Q1Q1_HFmHFp_Re_"+label_centbin[i]);
    hist_Q1Q1_HFmHFp_Im_centbin[i] = (TH1D *)aux_hist_Q1Q1_HFmHFp->Clone("Q1Q1_HFmHFp_Im_"+label_centbin[i]);
    
    /*hist_Q1Q1Q2Q2_HFmHFpHFmHFp_Re_centbin[i] = (TH1D *)aux_hist_Q1Q1Q2Q2_HFmHFpHFmHFp->Clone("Q1Q1Q2Q2_HFmHFpHFmHFp_Re_"+label_centbin[i]);
      hist_Q1Q1Q2Q2_HFmHFpHFmHFp_Im_centbin[i] = (TH1D *)aux_hist_Q1Q1Q2Q2_HFmHFpHFmHFp->Clone("Q1Q1Q2Q2_HFmHFpHFmHFp_Im_"+label_centbin[i]);  
      hist_Q2Q2Q2Q2_HFmHFmHFpHFp_Re_centbin[i] = (TH1D *)aux_hist_Q2Q2Q2Q2_HFmHFmHFpHFp->Clone("Q2Q2Q2Q2_HFmHFmHFpHFp_Re_"+label_centbin[i]);
      hist_Q2Q2Q2Q2_HFmHFmHFpHFp_Im_centbin[i] = (TH1D *)aux_hist_Q2Q2Q2Q2_HFmHFmHFpHFp->Clone("Q2Q2Q2Q2_HFmHFmHFpHFp_Im_"+label_centbin[i]);*/
    
  }
  
  
  Double_t vn_den = -99999999.9;
  Double_t v2_den[N_CENTBIN];
  Double_t v3_den[N_CENTBIN];
  TH1D *hQ2Q2_HFmHFp[N_CENTBIN];
  TH1D *hQ2Q2_HFmTrk[N_CENTBIN];
  TH1D *hQ2Q2_HFpTrk[N_CENTBIN];
  TH1D *hQ3Q3_HFmHFp[N_CENTBIN];
  TH1D *hQ3Q3_HFmTrk[N_CENTBIN];
  TH1D *hQ3Q3_HFpTrk[N_CENTBIN];
  
  TH2D *h_Ds_mass = new TH2D("Ds_mass",";m_{inv};QQ_{A}^{*}",50,1.9,2.1,N_VNBIN,vnbinning);
  TH2D *h_Ds_mass_v2[N_CENTBIN][N_PTBIN][N_DCABIN];//pT,y,CentBin                                                                                               
  TH2D *h_Ds_mass_v3[N_CENTBIN][N_PTBIN][N_DCABIN];//pT,y,CentBin                                                                                                                   

   for(Int_t i_centbin=0; i_centbin<N_CENTBIN; i_centbin++){
     for(Int_t i_pTbin=0; i_pTbin<N_PTBIN; i_pTbin++){
       for(Int_t i_Dcabin=0; i_Dcabin<N_DCABIN; i_Dcabin++){
	 h_Ds_mass_v2[i_centbin][i_pTbin][i_Dcabin] = (TH2D*)h_Ds_mass->Clone("hist_Ds_mass_v2_"+label_centbin[i_centbin]+"_"+label_pTbin[i_pTbin]+"_"+label_Dcabin[i_Dcabin]);
	 h_Ds_mass_v3[i_centbin][i_pTbin][i_Dcabin] = (TH2D*)h_Ds_mass->Clone("hist_Ds_mass_v3_"+label_centbin[i_centbin]+"_"+label_pTbin[i_pTbin]+"_"+label_Dcabin[i_Dcabin]);

       }
     }
   }
  
   auto start = high_resolution_clock::now();
      
     
   TString Str;
   ifstream fpr(Form("%s",input_file.Data()), ios::in);
   //ifstream fpr("T2_MB2to19_copy.txt", ios::in);

   if(!fpr.is_open()){
     cout << "List of input files not founded!" << endl;
     return;
   }


   std::vector<TString> file_name_vector;
   string file_chain;
   while(getline(fpr, file_chain))
     {
       file_name_vector.push_back(file_chain);
     }
   //fpr.close();                                                                                                                                                                                                  
   TChain *tree = new TChain("Dfinder/ntDPhikkpi");

   for (std::vector<TString>::iterator listIterator = file_name_vector.begin(); listIterator != file_name_vector.end(); listIterator++)
     {
       TFile *file = TFile::Open(*listIterator);
       cout << "Adding file:--- " << *listIterator << "--- to the chains" << endl;
       tree->Add(*listIterator);
     } 

   Int_t MAXDsCandSize=50000;
   
   Int_t Dsize;
   Int_t CentBin;
   Float_t ephfpAngle[3];
   Float_t ephfmAngle[3];
   Float_t ephfpQ[3];
   Float_t ephfmQ[3];
   Float_t eptkAngle[2];
   Float_t eptkQ[2];
   Float_t Dpt[MAXDsCandSize];
   Float_t Dphi[MAXDsCandSize];
   //Float_t mva[MAXDsCandSize];
   //Double_t mvaNp[MAXDsCandSize];
   Float_t Dy[MAXDsCandSize];
   Float_t flavor[MAXDsCandSize];
   Float_t Dmass[MAXDsCandSize];
   Float_t Ddca[MAXDsCandSize];
   
   Float_t Dalpha[MAXDsCandSize];
   Float_t DsvpvDistance[MAXDsCandSize];
   Float_t DsvpvDisErr[MAXDsCandSize];
   Float_t Dchi2cl[MAXDsCandSize];
   Float_t DtktkResmass[MAXDsCandSize];
   Float_t DtktkRes_alpha[MAXDsCandSize];
   Float_t DtktkRes_chi2cl[MAXDsCandSize];
   Float_t DtktkRes_svpvDistance[MAXDsCandSize];
   Float_t DtktkRes_svpvDisErr[MAXDsCandSize];
   Float_t Dtrk1Pt[MAXDsCandSize];
   Float_t Dtrk2Pt[MAXDsCandSize];
   
   
   tree->SetBranchAddress("Dsize", &Dsize);
   tree->SetBranchAddress("CentBin", &CentBin);
   tree->SetBranchAddress("ephfpAngle", &ephfpAngle);
   tree->SetBranchAddress("ephfmAngle", &ephfmAngle);
   tree->SetBranchAddress("ephfpQ", &ephfpQ);
   tree->SetBranchAddress("ephfmQ", &ephfmQ);
   tree->SetBranchAddress("eptkAngle", &eptkAngle);
   tree->SetBranchAddress("eptkQ", &eptkQ);
   tree->SetBranchAddress("Dpt", &Dpt);
   tree->SetBranchAddress("Dphi", &Dphi);
   tree->SetBranchAddress("Dmass", &Dmass);
   tree->SetBranchAddress("Dy", &Dy);
   tree->SetBranchAddress("Ddca", &Ddca);
   tree->SetBranchAddress("Dalpha", &Dalpha);
   tree->SetBranchAddress("DsvpvDistance", &DsvpvDistance);
   tree->SetBranchAddress("DsvpvDisErr", &DsvpvDisErr);
   tree->SetBranchAddress("Dchi2cl", &Dchi2cl);
   tree->SetBranchAddress("DtktkResmass", &DtktkResmass);
   tree->SetBranchAddress("DtktkRes_alpha", &DtktkRes_alpha);
   tree->SetBranchAddress("DtktkRes_chi2cl", &DtktkRes_chi2cl);
   tree->SetBranchAddress("DtktkRes_svpvDistance", &DtktkRes_svpvDistance);
   tree->SetBranchAddress("DtktkRes_svpvDisErr", &DtktkRes_svpvDisErr);
   tree->SetBranchAddress("Dtrk1Pt", &Dtrk1Pt);
   tree->SetBranchAddress("Dtrk2Pt", &Dtrk2Pt);
   
   tree->SetBranchStatus("*", 0);
   for (const auto& p : {"Dsize", "CentBin", "ephfpAngle", "ephfmAngle", "ephfpQ", "ephfmQ", "eptkAngle", "eptkQ", "Dpt", "Dphi", "Dmass", "Dy", "Ddca", "Dalpha", "DsvpvDistance", "DsvpvDisErr", "Dchi2cl", "DtktkResmass", "DtktkRes_alpha", "DtktkRes_chi2cl", "DtktkRes_svpvDistance", "DtktkRes_svpvDisErr", "Dtrk1Pt", "Dtrk2Pt"})
     tree->SetBranchStatus(p, 1);
   
    
   Double_t dmass;
   //Long64_t n_entries = tree->GetEntriesFast();
   Int_t n_entries = tree->GetEntries();

   std::cout<<"nevents of first loop = "<<n_entries<<std::endl;

   for(Int_t ii=0; ii<n_entries; ii++){//loop in events
     
     tree->GetEntry(ii);
     //treeMva->GetEntry(ii);
     if(ii % 100000 == 0)
       printf("current entry of first loop = %lld out of %lld : %.3f %%\n", ii, n_entries, (Double_t)ii / n_entries * 100);
     
     if (CentBin >= max_centbin[N_CENTBIN-1]) continue;
     
     ///For vn denominator - EP resolution
     TComplex aux_Q2_HFm(ephfmQ[1]*TMath::Cos(2.0*ephfmAngle[1]),ephfmQ[1]*TMath::Sin(2.0*ephfmAngle[1]),0);
     TComplex aux_Q2_HFp(ephfpQ[1]*TMath::Cos(2.0*ephfpAngle[1]),ephfpQ[1]*TMath::Sin(2.0*ephfpAngle[1]),0); 
     TComplex aux_Q2_Trk(eptkQ[0]*TMath::Cos(2.0*eptkAngle[0]),eptkQ[0]*TMath::Sin(2.0*eptkAngle[0]),0);
     
     TComplex aux_Q3_HFm(ephfmQ[2]*TMath::Cos(3.0*ephfmAngle[2]),ephfmQ[2]*TMath::Sin(3.0*ephfmAngle[2]),0);
     TComplex aux_Q3_HFp(ephfpQ[2]*TMath::Cos(3.0*ephfpAngle[2]),ephfpQ[2]*TMath::Sin(3.0*ephfpAngle[2]),0);
     TComplex aux_Q3_Trk(eptkQ[1]*TMath::Cos(3.0*eptkAngle[1]),eptkQ[1]*TMath::Sin(3.0*eptkAngle[1]),0); 
     
     TComplex aux_Q1_HFm(ephfmQ[0]*TMath::Cos(1.0*ephfmAngle[0]),ephfmQ[0]*TMath::Sin(1.0*ephfmAngle[0]),0);
     TComplex aux_Q1_HFp(ephfpQ[0]*TMath::Cos(1.0*ephfpAngle[0]),ephfpQ[0]*TMath::Sin(1.0*ephfpAngle[0]),0);
     
     for(Int_t i=0; i<N_CENTBIN; i++){

       if(CentBin>min_centbin[i] && CentBin<max_centbin[i]){	 
	 Double_t aux_Q2Q2_HFmHFp_Re = (aux_Q2_HFm*TComplex::Conjugate(aux_Q2_HFp)).Re();
	 Double_t aux_Q2Q2_HFmHFp_Im = (aux_Q2_HFm*TComplex::Conjugate(aux_Q2_HFp)).Im();
	 Double_t aux_Q2Q2_HFmTrk_Re = (aux_Q2_HFm*TComplex::Conjugate(aux_Q2_Trk)).Re();
	 Double_t aux_Q2Q2_HFmTrk_Im = (aux_Q2_HFm*TComplex::Conjugate(aux_Q2_Trk)).Im();
	 Double_t aux_Q2Q2_HFpTrk_Re = (aux_Q2_HFp*TComplex::Conjugate(aux_Q2_Trk)).Re();
	 Double_t aux_Q2Q2_HFpTrk_Im = (aux_Q2_HFp*TComplex::Conjugate(aux_Q2_Trk)).Im();
	 hist_Q2Q2_HFmHFp_Re_centbin[i]->Fill(aux_Q2Q2_HFmHFp_Re);
	 //hist_Q2Q2_HFmHFp_Im_centbin[i]->Fill(aux_Q2Q2_HFmHFp_Im);
	 hist_Q2Q2_HFmTrk_Re_centbin[i]->Fill(aux_Q2Q2_HFmTrk_Re);
	 //hist_Q2Q2_HFmTrk_Im_centbin[i]->Fill(aux_Q2Q2_HFmTrk_Im);
	 hist_Q2Q2_HFpTrk_Re_centbin[i]->Fill(aux_Q2Q2_HFpTrk_Re);
	 //hist_Q2Q2_HFpTrk_Im_centbin[i]->Fill(aux_Q2Q2_HFpTrk_Im);
	 
	 Double_t aux_Q3Q3_HFmHFp_Re = (aux_Q3_HFm*TComplex::Conjugate(aux_Q3_HFp)).Re();
	 Double_t aux_Q3Q3_HFmHFp_Im = (aux_Q3_HFm*TComplex::Conjugate(aux_Q3_HFp)).Im();
	 Double_t aux_Q3Q3_HFmTrk_Re = (aux_Q3_HFm*TComplex::Conjugate(aux_Q3_Trk)).Re();
	 Double_t aux_Q3Q3_HFmTrk_Im = (aux_Q3_HFm*TComplex::Conjugate(aux_Q3_Trk)).Im();
	 Double_t aux_Q3Q3_HFpTrk_Re = (aux_Q3_HFp*TComplex::Conjugate(aux_Q3_Trk)).Re();
	 Double_t aux_Q3Q3_HFpTrk_Im = (aux_Q3_HFp*TComplex::Conjugate(aux_Q3_Trk)).Im();
	 hist_Q3Q3_HFmHFp_Re_centbin[i]->Fill(aux_Q3Q3_HFmHFp_Re);
	 //hist_Q3Q3_HFmHFp_Im_centbin[i]->Fill(aux_Q3Q3_HFmHFp_Im);
	 hist_Q3Q3_HFmTrk_Re_centbin[i]->Fill(aux_Q3Q3_HFmTrk_Re);
	 //hist_Q3Q3_HFmTrk_Im_centbin[i]->Fill(aux_Q3Q3_HFmTrk_Im);
	 hist_Q3Q3_HFpTrk_Re_centbin[i]->Fill(aux_Q3Q3_HFpTrk_Re);
	 //hist_Q3Q3_HFpTrk_Im_centbin[i]->Fill(aux_Q3Q3_HFpTrk_Im);
	 
	 Double_t aux_Q1Q1_HFmHFp_Re = (aux_Q1_HFm*TComplex::Conjugate(aux_Q1_HFp)).Re();
	 Double_t aux_Q1Q1_HFmHFp_Im = (aux_Q1_HFm*TComplex::Conjugate(aux_Q1_HFp)).Im();

	 /*hist_Q1Q1_HFmHFp_Re_centbin[i]->Fill(aux_Q1Q1_HFmHFp_Re);
	   hist_Q1Q1_HFmHFp_Im_centbin[i]->Fill(aux_Q1Q1_HFmHFp_Im);*/
	 
	 /*Double_t aux_Q1Q1Q2Q2_HFmHFpHFmHFp_Re = (aux_Q1_HFm*TComplex::Conjugate(aux_Q1_HFp)*aux_Q2_HFm*TComplex::Conjugate(aux_Q2_HFp)).Re();
	   Double_t aux_Q1Q1Q2Q2_HFmHFpHFmHFp_Im = (aux_Q1_HFm*TComplex::Conjugate(aux_Q1_HFp)*aux_Q2_HFm*TComplex::Conjugate(aux_Q2_HFp)).Im();
	   hist_Q1Q1Q2Q2_HFmHFpHFmHFp_Re_centbin[i]->Fill(aux_Q1Q1Q2Q2_HFmHFpHFmHFp_Re);
	   hist_Q1Q1Q2Q2_HFmHFpHFmHFp_Im_centbin[i]->Fill(aux_Q1Q1Q2Q2_HFmHFpHFmHFp_Im);
	   
	   Double_t aux_Q2Q2Q2Q2_HFmHFmHFpHFp_Re = (aux_Q2_HFm*aux_Q2_HFm*TComplex::Conjugate(aux_Q2_HFp)*TComplex::Conjugate(aux_Q2_HFp)).Re();
	   Double_t aux_Q2Q2Q2Q2_HFmHFmHFpHFp_Im = (aux_Q2_HFm*aux_Q2_HFm*TComplex::Conjugate(aux_Q2_HFp)*TComplex::Conjugate(aux_Q2_HFp)).Im();
	   hist_Q2Q2Q2Q2_HFmHFmHFpHFp_Re_centbin[i]->Fill(aux_Q2Q2Q2Q2_HFmHFmHFpHFp_Re);
	   hist_Q2Q2Q2Q2_HFmHFmHFpHFp_Im_centbin[i]->Fill(aux_Q2Q2Q2Q2_HFmHFmHFpHFp_Im);*/	 
       }     
     }//CENTBIN loop  
     
   }//end first event loops
   
   
   //calculated centrality bin wise
   for(Int_t i=0; i<N_CENTBIN; i++){
     v2_den[i] = TMath::Sqrt( (hist_Q2Q2_HFmHFp_Re_centbin[i]->GetMean()*hist_Q2Q2_HFmTrk_Re_centbin[i]->GetMean()) / (hist_Q2Q2_HFpTrk_Re_centbin[i]->GetMean()) );
     v3_den[i] = TMath::Sqrt( (hist_Q3Q3_HFmHFp_Re_centbin[i]->GetMean()*hist_Q3Q3_HFmTrk_Re_centbin[i]->GetMean()) / (hist_Q3Q3_HFpTrk_Re_centbin[i]->GetMean()) );
   }
   
   TH3D *h_binning = new TH3D("centbins","",N_CENTBIN,centbinning,N_PTBIN,ptbinning,N_DCABIN,dcabinning);
   
   
   const char *path[6]={"/afs/cern.ch/work/n/nsaha/tmva/TMVA_HFgroup/tutorialTMVABntupleRunII_46/myTMVA/weights/TMVA_BDT_PbPb_pt_4p0_6p0.weights.xml",
			"/afs/cern.ch/work/n/nsaha/tmva/TMVA_HFgroup/tutorialTMVABntupleRunII_68/myTMVA/weights/TMVA_BDT_PbPb_pt_6p0_8p0.weights.xml",
			"/afs/cern.ch/work/n/nsaha/tmva/TMVA_HFgroup/tutorialTMVABntupleRunII_810/myTMVA/weights/TMVA_BDT_PbPb_pt_8p0_10p0.weights.xml",
			"/afs/cern.ch/work/n/nsaha/tmva/TMVA_HFgroup/tutorialTMVABntupleRunII_1015/myTMVA/weights/TMVA_BDT_PbPb_pt_10p0_15p0.weights.xml",
			"/afs/cern.ch/work/n/nsaha/tmva/TMVA_HFgroup/tutorialTMVABntupleRunII_1520/myTMVA/weights/TMVA_BDT_PbPb_pt_15p0_20p0.weights.xml",
			"/afs/cern.ch/work/n/nsaha/tmva/TMVA_HFgroup/tutorialTMVABntupleRunII_1520/myTMVA/weights/TMVA_BDT_PbPb_pt_15p0_20p0.weights.xml"};
   
   Float_t dlengthsign, dalpha,dchi2cl, dca, phiAlpha, phiChi2cl, phiLengthSign, myVar ;
   
   TMVA::Tools::Instance();
   
   /*TMVA::Reader *reader[N_CENTBIN][N_PTBIN];
   for(int icent =0; icent< N_CENTBIN; icent++){
     for(int ipt = 0; ipt<N_PTBIN; ipt++){
       reader[icent][ipt] = new TMVA::Reader( "!Color:!Silent" );
     }
   }

   for(int icent =0; icent< N_CENTBIN; icent++){
     for(int ipt = 0; ipt<N_PTBIN; ipt++){
       reader[icent][ipt]->AddVariable( "DsvpvDistance/DsvpvDisErr", &dlengthsign );
       reader[icent][ipt]->AddVariable( "Dalpha", &dalpha );
       reader[icent][ipt]->AddVariable( "Dchi2cl", &dchi2cl );
       reader[icent][ipt]->AddVariable( "DtktkRes_alpha", &phiAlpha );
       reader[icent][ipt]->AddVariable( "DtktkRes_chi2cl", &phiChi2cl );
       reader[icent][ipt]->AddVariable( "DtktkRes_svpvDistance/DtktkRes_svpvDisErr", &phiLengthSign );
       reader[icent][ipt]->AddVariable( "TMath::Abs((Dtrk1Pt-Dtrk2Pt)/(Dtrk1Pt+Dtrk2Pt))", &myVar );
       reader[icent][ipt]->BookMVA(Form("BDT_%d%d",icent, ipt ), path[ipt]);
     }
     }*/


   TMVA::Reader *reader[N_PTBIN];
   for(int ipt = 0; ipt<N_PTBIN; ipt++){
     reader[ipt] = new TMVA::Reader( "!Color:!Silent" );
   }
   
   for(int ipt = 0; ipt<N_PTBIN; ipt++){
     reader[ipt]->AddVariable( "DsvpvDistance/DsvpvDisErr", &dlengthsign );
     reader[ipt]->AddVariable( "Dalpha", &dalpha );
     reader[ipt]->AddVariable( "Dchi2cl", &dchi2cl );
     reader[ipt]->AddVariable( "DtktkRes_alpha", &phiAlpha );
     reader[ipt]->AddVariable( "DtktkRes_chi2cl", &phiChi2cl );
     reader[ipt]->AddVariable( "DtktkRes_svpvDistance/DtktkRes_svpvDisErr", &phiLengthSign );
     reader[ipt]->AddVariable( "TMath::Abs((Dtrk1Pt-Dtrk2Pt)/(Dtrk1Pt+Dtrk2Pt))", &myVar );
     reader[ipt]->BookMVA(Form("BDT_%d",ipt ), path[ipt]);
     }
   

   
   double weight;
   //weight = 1.0/hist_eff->GetBinContent(hist_eff->GetXaxis()->FindBin(CentBin),hist_eff->GetYaxis()->FindBin(pT[i]));                                                                               
   //if (hist_eff->GetBinContent(hist_eff->GetXaxis()->FindBin(CentBin),hist_eff->GetYaxis()->FindBin(pT[i])) <0.0005) weight =1;   


   Int_t nevents = tree->GetEntries();
   
   std::cout<<"Events for 2nd loop="<<nevents<<std::endl;

   for(Int_t iii=0; iii<nevents; iii++){//loop in events                                                                 
     
     tree->GetEntry(iii);
     
     if(iii % 100000 == 0)
       printf("current entry of 2nd loop = %lld out of %lld : %.3f %%\n", iii, n_entries, (Double_t)iii / n_entries * 100);
     
     if (CentBin >= max_centbin[N_CENTBIN-1]) continue;

     TComplex aux_Q2_HFm(ephfmQ[1]*TMath::Cos(2.0*ephfmAngle[1]),ephfmQ[1]*TMath::Sin(2.0*ephfmAngle[1]),0);
     TComplex aux_Q2_HFp(ephfpQ[1]*TMath::Cos(2.0*ephfpAngle[1]),ephfpQ[1]*TMath::Sin(2.0*ephfpAngle[1]),0);
     TComplex aux_Q2_Trk(eptkQ[0]*TMath::Cos(2.0*eptkAngle[0]),eptkQ[0]*TMath::Sin(2.0*eptkAngle[0]),0);
     
     TComplex aux_Q3_HFm(ephfmQ[2]*TMath::Cos(3.0*ephfmAngle[2]),ephfmQ[2]*TMath::Sin(3.0*ephfmAngle[2]),0);
     TComplex aux_Q3_HFp(ephfpQ[2]*TMath::Cos(3.0*ephfpAngle[2]),ephfpQ[2]*TMath::Sin(3.0*ephfpAngle[2]),0);
     TComplex aux_Q3_Trk(eptkQ[1]*TMath::Cos(3.0*eptkAngle[1]),eptkQ[1]*TMath::Sin(3.0*eptkAngle[1]),0);
     
     
     Int_t i_centbin = h_binning->GetXaxis()->FindBin(CentBin)-1;
     Int_t np = Dsize;
     
     for(Int_t i=0; i<np; i++){//loop in Ds+/- Candidates                                                    

       if (TMath::IsNaN(DsvpvDistance[i]/DsvpvDisErr[i])) continue;
       if (TMath::IsNaN(DtktkRes_svpvDistance[i]/DtktkRes_svpvDisErr[i])) continue;

       if (fabs(Dy[i]) > 2.4)continue;
       if (Dalpha[i] > 0.15)continue;
       //if (Ddca[i] > 0.008)continue;
       //if (Dchi2cl[i] < 0.05 ) continue;
       if (TMath::Abs(DtktkResmass[i]-1.019)>0.009) continue; 

       if (Dpt[i] > max_pTbin[N_PTBIN-1] || Dpt[i] < min_pTbin[0]) continue;
       if (Ddca[i] > max_Dcabin[N_DCABIN-1] || Ddca[i] < min_Dcabin[0]) continue;                                                          



       //==============BDT training variables=========================
       dlengthsign = DsvpvDistance[i]/DsvpvDisErr[i] ;
       dalpha = Dalpha[i];
       dchi2cl = Dchi2cl[i];
       phiAlpha = DtktkRes_alpha[i];
       phiChi2cl = DtktkRes_chi2cl[i];
       phiLengthSign = DtktkRes_svpvDistance[i]/DtktkRes_svpvDisErr[i];
       myVar = TMath::Abs((Dtrk1Pt[i]-Dtrk2Pt[i])/(Dtrk1Pt[i]+Dtrk2Pt[i]));
       //=============================================================

       Int_t i_pTbin = (h_binning->GetYaxis()->FindBin(Dpt[i]))-1;
       Int_t i_Dcabin = (h_binning->GetZaxis()->FindBin(Ddca[i]))-1;       

       
       double mvaFi = reader[i_pTbin]->EvaluateMVA(Form("BDT_%d", i_pTbin));
       //double mvaFi = reader[i_centbin][i_pTbin]->EvaluateMVA(Form("BDT_%d%d",i_centbin, i_pTbin));
       
       if(mvaFi <=GetBDTCut(Dpt[i],CentBin)) continue;

       
       weight = 1.0;
       
       if(Dy[i]>0){
	 TComplex aux_Q2_Ds(TMath::Cos(2.0*Dphi[i]),TMath::Sin(2.0*Dphi[i]),0);
	 Double_t aux_Q2Q2_DsHF_Re_centbin_pTbin_Dcabin_massbin = (aux_Q2_Ds*TComplex::Conjugate(aux_Q2_HFm)).Re();
	 h_Ds_mass_v2[i_centbin][i_pTbin][i_Dcabin]->Fill(Dmass[i],aux_Q2Q2_DsHF_Re_centbin_pTbin_Dcabin_massbin/v2_den[i_centbin],weight);
	 
	 TComplex aux_Q3_Ds(TMath::Cos(3.0*Dphi[i]),TMath::Sin(3.0*Dphi[i]),0);
	 Double_t aux_Q3Q3_DsHF_Re_centbin_pTbin_Dcabin_massbin = (aux_Q3_Ds*TComplex::Conjugate(aux_Q3_HFm)).Re();
	 h_Ds_mass_v3[i_centbin][i_pTbin][i_Dcabin]->Fill(Dmass[i],aux_Q3Q3_DsHF_Re_centbin_pTbin_Dcabin_massbin/v3_den[i_centbin],weight);
	 
       }
       else{
	 
	 TComplex aux_Q2_Ds(TMath::Cos(2.0*Dphi[i]),TMath::Sin(2.0*Dphi[i]),0);
	 Double_t aux_Q2Q2_DsHF_Re_centbin_pTbin_Dcabin_massbin = (aux_Q2_Ds*TComplex::Conjugate(aux_Q2_HFp)).Re();
	 h_Ds_mass_v2[i_centbin][i_pTbin][i_Dcabin]->Fill(Dmass[i],aux_Q2Q2_DsHF_Re_centbin_pTbin_Dcabin_massbin/v2_den[i_centbin],weight);
	 
	 TComplex aux_Q3_Ds(TMath::Cos(3.0*Dphi[i]),TMath::Sin(3.0*Dphi[i]),0);
	 Double_t aux_Q3Q3_DsHF_Re_centbin_pTbin_Dcabin_massbin = (aux_Q3_Ds*TComplex::Conjugate(aux_Q3_HFp)).Re();
	 h_Ds_mass_v3[i_centbin][i_pTbin][i_Dcabin]->Fill(Dmass[i],aux_Q3Q3_DsHF_Re_centbin_pTbin_Dcabin_massbin/v3_den[i_centbin],weight);
	 
       }
     }//---end particle loop                                                                                                                     
   }//--end 2nd event loop
   
   delete tree;
   //file->Close();
   //delete file;

   /*for(int icent=0; icent<N_CENTBIN; icent++){
     for(int ipt = 0; ipt<N_PTBIN; ipt++){
       delete reader[icent][ipt];
     }
     }*/

   
   for(int ipt = 0; ipt<N_PTBIN; ipt++){
     delete reader[ipt];
   }
   


   
   fout->cd();
   
   for(Int_t i_centbin=0; i_centbin<N_CENTBIN; i_centbin++){
     for(Int_t i_pTbin=0; i_pTbin<N_PTBIN; i_pTbin++){
       for(Int_t i_Dcabin=0; i_Dcabin<N_DCABIN; i_Dcabin++){
	 
	 h_Ds_mass_v2[i_centbin][i_pTbin][i_Dcabin]->Write();
	 h_Ds_mass_v3[i_centbin][i_pTbin][i_Dcabin]->Write();
	 
       }
     }
   }
   
   fout->Close();	

   auto stop = high_resolution_clock::now();
   auto duration = duration_cast<minutes>(stop - start);   
   cout << "Total time taken: "<< duration.count() << "minutes" << endl;
   
   std::cout<<" \n Done!!! "<<std::endl;
   
  }


int main(int argc, char **argv)
{
  using namespace std;
  
  TString inputfile;
  int itxtout;
  TString outfile;

  if(argc == 1)
    {
      std::cout<<"You did not pass any argument to the code other than the program name"<<std::endl;
      std::cout<<"You need to pass 4 arguments including the program name"<<std::endl;
    }

  if(argc >=1 && argc <=3)
    {
      std::cout<<"Only "<<argc<<" arguments you have given including the program name"<<std::endl;
      std::cout<<"You need to pass 4 arguments including the program name"<<std::endl;
    }

  if(argc == 4)
    {
      std::cout<<std::endl;
      std::cout<<"You have given 4 arguments including the program name;  Your program will run"<<std::endl;
      std::cout<<std::endl;

      inputfile = argv[1];
      itxtout = atoi(argv[2]);
      outfile = argv[3];

      hist_vn_mass(inputfile, itxtout, outfile);
    }
  return 0;
}

