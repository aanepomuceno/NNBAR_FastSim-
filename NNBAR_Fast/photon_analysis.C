
#include <iostream>  
#include <fstream>
#include <string>  
#include "TVector3.h"

void photon_analysis()
  {
   double x_min = 150;
   double x_max = 500;
   TH1F *hEnergy = new TH1F("Photon Energy","Photon Energy",50,x_min,x_max);
   TH2  *hXY   = new TH2F("EXY", "EMCAL XY", 50, -35, 35, 50, -35, 35);
   TH2  *hYZ   = new TH2F("EYZ", "EMCAL YZ", 50, -35, 35, 50, -55, 55);

   std::vector<int>    *pdg;
   std::vector<double> *mc_x;
   std::vector<double> *mc_y;
   std::vector<double> *mc_z;
   std::vector<double> *em_e;
   std::vector<double> *em_x;
   std::vector<double> *em_y;
   std::vector<double> *em_z;
   TVector3 v1; 
   TVector3 v2; 
   double energy = 0;

   //Intitialize vectors
   em_e = 0;
   em_x = 0;
   em_y = 0;
   em_z = 0;
   
   TFile *f = new TFile("NNBARFastOutput_t0.root");
   TTree *t1 = (TTree*)f->Get("EMCAL");
   t1->SetBranchAddress("emcal_E",&em_e);
   t1->SetBranchAddress("emcal_X",&em_x);
   t1->SetBranchAddress("emcal_Y",&em_y);
   t1->SetBranchAddress("emcal_Z",&em_z);
  
   
   for (int i = 0; i < t1->GetEntries(); i++) {
     t1->GetEntry(i);
     if (em_e->size() < 1 ) continue;
     hEnergy->Fill((*em_e)[0]);
   }
   delete t1;
   delete f;

   TCanvas * c1 = new TCanvas("c1", "c1", 600, 500);
   hEnergy->Draw();
  
   }
   




