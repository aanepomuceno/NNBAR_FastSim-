//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
/// \file NNBAROutput.cc
/// \brief Implementation of the NNBAROutput class

#include "NNBAROutput.hh"
#include "NNBAREventInformation.hh"
#include <vector>
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4AnalysisManager.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

NNBAROutput* NNBAROutput::fNNBAROutput = 0;
//G4ThreadLocal G4int NNBAROutput::fCurrentNtupleId = 0;
//G4ThreadLocal G4int NNBAROutput::fCurrentID = 0; 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

NNBAROutput::NNBAROutput() : fFileNameWithRunNo( false ) {
  fFileName = "NNBARFastOutput.root";
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

NNBAROutput::~NNBAROutput() {
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

NNBAROutput* NNBAROutput::Instance() {
  if ( ! fNNBAROutput ) {
    fNNBAROutput = new NNBAROutput();
  }
  return fNNBAROutput;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NNBAROutput::SetFileName( G4String aName ) {
  fFileName = aName;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NNBAROutput::AppendName( G4bool aApp ) {
  fFileNameWithRunNo = aApp;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4String NNBAROutput::GetFileName() {
  return fFileName;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NNBAROutput::StartAnalysis( G4int aRunID ) {
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  if ( fFileNameWithRunNo ) {
    fFileName +=  "_run";
    fFileName += G4UIcommand::ConvertToString( aRunID );
  }
  analysisManager->SetDefaultFileType("root");
  analysisManager->SetVerboseLevel( 1 );
  analysisManager->SetFileName( fFileName );
  analysisManager->OpenFile( fFileName );
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NNBAROutput::EndAnalysis() {
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  analysisManager->Write();
  analysisManager->CloseFile();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NNBAROutput::CreateNtuples() {
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  analysisManager->SetNtupleMerging(true);

  analysisManager->CreateNtuple("MC","MC Truth");
  analysisManager->CreateNtupleIColumn("particleID", fParticleIDVec);  
  analysisManager->CreateNtupleIColumn("PID", fPIDVec);
  analysisManager->CreateNtupleDColumn("MC_KE",fMC_KEnergy);
  analysisManager->CreateNtupleDColumn("MC_X", fMC_XVec);
  analysisManager->CreateNtupleDColumn("MC_Y", fMC_YVec);
  analysisManager->CreateNtupleDColumn("MC_Z", fMC_ZVec);
  analysisManager->FinishNtuple(0);

  //Uncomment for tracker. Be careful with the ntuple number in FinishNtuple()
  //analysisManager->CreateNtuple("Tracker","Tracker");
  //analysisManager->CreateNtupleDColumn("tracker_res", fTrackerResVec);
  //analysisManager->CreateNtupleDColumn("tracker_eff", fTrackerEffVec);
  //analysisManager->CreateNtupleDColumn("tracker_pX", fTracker_pXVec);
  //analysisManager->CreateNtupleDColumn("tracker_pY", fTracker_pYVec);
  //analysisManager->CreateNtupleDColumn("tracker_pZ", fTracker_pZVec);
  //analysisManager->FinishNtuple(1);
  
  analysisManager->CreateNtuple("EMCAL","EMCAL");
  analysisManager->CreateNtupleIColumn( "emcal_PDG" ,fEmcalPDGVec); 
  analysisManager->CreateNtupleDColumn( "emcal_ETruth",fEmcalETruthVec);   
  analysisManager->CreateNtupleDColumn( "emcal_res",fEmcalResVec );
  analysisManager->CreateNtupleDColumn( "emcal_eff",fEmcalEffVec );
  analysisManager->CreateNtupleDColumn( "emcal_X", fEmcalXVec );  
  analysisManager->CreateNtupleDColumn( "emcal_Y" ,fEmcalYVec); 
  analysisManager->CreateNtupleDColumn( "emcal_Z",fEmcalZVec ); 
  analysisManager->CreateNtupleDColumn( "emcal_E", fEmcalEVec ); 
  analysisManager->CreateNtupleDColumn( "emcal_Time" ,fEmcalTimeVec); 
  analysisManager->FinishNtuple(1);
  
  //Uncomment for HCAL. Be careful with the ntuple number in FinishNtuple()
  analysisManager->CreateNtuple("HCAL","HCAL");
  analysisManager->CreateNtupleIColumn( "hcal_PDG",fHcalPDGVec);  
  analysisManager->CreateNtupleDColumn( "hcal_ETruth",fHcalETruthVec);
  analysisManager->CreateNtupleDColumn( "hcal_res",fHcalResVec); 
  analysisManager->CreateNtupleDColumn( "hcal_eff",fHcalEffVec); 
  analysisManager->CreateNtupleDColumn( "hcal_X",fHcalXVec);  
  analysisManager->CreateNtupleDColumn( "hcal_Y",fHcalYVec); 
  analysisManager->CreateNtupleDColumn( "hcal_Z",fHcalZVec); 
  analysisManager->CreateNtupleDColumn( "hcal_E",fHcalEVec); 
  analysisManager->CreateNtupleDColumn( "hcal_Time",fHcalTimeVec);
  analysisManager->FinishNtuple(2);

 }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NNBAROutput::CreateHistograms()
{
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  analysisManager->CreateH1( "Pdiff", "momentum smeared in tracker", 100, 0.8, 1.2 );
  analysisManager->SetH1XAxisTitle( 0, "p_{smeared}/p_{true}" );
  analysisManager->SetH1YAxisTitle( 0, "Entries" );
  analysisManager->CreateH1( "EMCalEdiff", "energy smeared in EMCal", 100, 0.6, 1.2 );
  analysisManager->SetH1XAxisTitle( 1, "E_{smeared}/E_{true}" );
  analysisManager->SetH1YAxisTitle( 1, "Entries" );
  analysisManager->CreateH1( "HCalEdiff", "energy smeared in HCal", 100, 0.0, 2.0 );
  analysisManager->SetH1XAxisTitle( 2, "E_{smeared}/E_{true}" );
  analysisManager->SetH1YAxisTitle( 2, "Entries" );
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NNBAROutput::SaveTrack( SaveType aWhatToSave, G4int aPartID,  G4int aPDG, G4double aETruth,
                             G4ThreeVector aVector, G4double aResolution, 
                             G4double aEfficiency, G4double aEnergy,  G4double aTime ) {
 
   switch (aWhatToSave) {
   case NNBAROutput::eNoSave:
   break;
     
     case NNBAROutput::eSaveMC: {
      fParticleIDVec.push_back(aPartID);
      fPIDVec.push_back(aPDG);
      fMC_KEnergy.push_back(aETruth);
      fMC_XVec.push_back(aVector.x());
      fMC_YVec.push_back(aVector.y());
      fMC_ZVec.push_back(aVector.z());
      break;
    }

    //case NNBAROutput::eSaveTracker: {
    //  std::cout <<"here 2" << std::endl;
    //  fTrackerResVec.push_back(aResolution);
    //  fTrackerEffVec.push_back(aEfficiency);
     // fTracker_pXVec.push_back(aVector.x());
     // fTracker_pYVec.push_back(aVector.y());
     // fTracker_pZVec.push_back(aVector.z());
     // break;
  //  }
    
    case NNBAROutput::eSaveEMCal : {
     fEmcalPDGVec.push_back(aPDG);
     fEmcalETruthVec.push_back(aETruth);
     fEmcalResVec.push_back(aResolution);
     fEmcalEffVec.push_back(aEfficiency);
     fEmcalXVec.push_back(aVector.x() );
     fEmcalYVec.push_back( aVector.y() );
     fEmcalZVec.push_back( aVector.z() );
     fEmcalEVec.push_back(aEnergy);
     fEmcalTimeVec.push_back(aTime);
     break;
   }    
    
    case NNBAROutput::eSaveHCal : {
    fHcalPDGVec.push_back(aPDG);
    fHcalETruthVec.push_back(aETruth);
    fHcalResVec.push_back(aResolution);
    fHcalEffVec.push_back(aEfficiency);
    fHcalXVec.push_back(aVector.x() );
    fHcalYVec.push_back( aVector.y() );
    fHcalZVec.push_back( aVector.z() );
    fHcalEVec.push_back(aEnergy);
    fHcalTimeVec.push_back(aTime);
    break;
   }
 }
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NNBAROutput::SaveEvent() {
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();

  analysisManager->AddNtupleRow(0);
  analysisManager->AddNtupleRow(1);
  analysisManager->AddNtupleRow(2);
  //analysisManager->AddNtupleRow(3);

  //Clear vectors for next event
 
  fParticleIDVec.clear();
  fPIDVec.clear();
  fMC_KEnergy.clear();
  fMC_XVec.clear();
  fMC_YVec.clear();
  fMC_ZVec.clear();
 // fTrackerResVec.clear();
 // fTrackerEffVec.clear();
 // fTracker_pXVec.clear();
 // fTracker_pYVec.clear();
 // fTracker_pZVec.clear();
  fEmcalPDGVec.clear();
  fEmcalETruthVec.clear();
  fEmcalResVec.clear();
  fEmcalEffVec.clear();
  fEmcalXVec.clear();
  fEmcalYVec.clear();
  fEmcalZVec.clear();
  fEmcalEVec.clear();
  fEmcalTimeVec.clear();
  fHcalPDGVec.clear();
  fHcalETruthVec.clear();
  fHcalResVec.clear();
  fHcalEffVec.clear();
  fHcalXVec.clear();
  fHcalYVec.clear();
  fHcalZVec.clear();
  fHcalEVec.clear();
  fHcalTimeVec.clear();  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void NNBAROutput::FillHistogram( G4int aHistNo, G4double aValue ) const {
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  analysisManager->FillH1( aHistNo, aValue );
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

