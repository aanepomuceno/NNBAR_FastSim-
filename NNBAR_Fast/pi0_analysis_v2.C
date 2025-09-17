
#include <iostream>  
#include <fstream>
#include <string>  
#include "TVector3.h"

void pi0_analysis_v2()
  {

   TH1F *hmass = new TH1F("InvMass","Pi0 InvMass",50,0,250);
   TH2  *hE2d  = new TH2F("E2D", "Photon E1 X E2", 50, 0, 400, 50, 0, 400);
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
   double angle = 0;
   double mass = 0;

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
     if (em_e->size() < 2 ) continue;
     v1.Clear();
     v2.Clear();
     v1.SetXYZ( (*em_x)[0],(*em_y)[0],(*em_z)[0] );
     v2.SetXYZ( (*em_x)[1],(*em_y)[1],(*em_z)[1] );
     angle = v1.Angle(v2);
     mass = sqrt( 2*(*em_e)[0]*(*em_e)[1]*(1.-cos(angle) ) );

     hmass->Fill(mass);
     hE2d->Fill( (*em_e)[0], (*em_e)[1]);
     for (int j=0;j< em_x->size(); j++) {
       hXY->Fill((*em_x)[j]/10.,(*em_y)[j]/10.);
       hYZ->Fill((*em_y)[j]/10.,(*em_z)[j]/10.);
     }
   
   }
   delete t1;
   delete f;

   TCanvas * c1 = new TCanvas("c1", "c1", 600, 500);
   hE2d->Draw();
   TCanvas * c2 = new TCanvas("c2", "c2", 600, 500);
   hmass->Draw();
   TCanvas * c3 = new TCanvas("c3", "c3", 600, 500);
   hXY->Draw();
   TCanvas * c4 = new TCanvas("c4", "c4", 600, 500);
   hYZ->Draw();

   
   }
   




