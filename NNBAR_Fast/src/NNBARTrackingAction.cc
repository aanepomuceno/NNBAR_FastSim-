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
/// \file NNBARTrackingAction.cc
/// \brief Implementation of the NNBARTrackingAction class

#include "NNBARTrackingAction.hh"
#include "NNBAREventInformation.hh"
#include "NNBARPrimaryParticleInformation.hh"
#include "NNBAROutput.hh"

#include "G4ThreeVector.hh"
#include "G4EventManager.hh"
#include "G4RunManager.hh"

#include "Randomize.hh"
#include "G4SystemOfUnits.hh"
#include "G4TrackingManager.hh"
#include <iomanip>
#include <vector>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

NNBARTrackingAction::NNBARTrackingAction() : G4UserTrackingAction() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

NNBARTrackingAction::~NNBARTrackingAction() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NNBARTrackingAction::PreUserTrackingAction( const G4Track* aTrack ) {
  // Kill the tracks that have a small transverse momentum or that are not
  // in the central region.
//  if ( aTrack->GetMomentum().perp() < 1.0*MeV  ||
//       std::abs( aTrack->GetMomentum().pseudoRapidity() ) > 5.5 ) {
//    ( (G4Track*) aTrack )->SetTrackStatus( fStopAndKill );
//  }
  if ( aTrack->GetMomentum().perp() < 1.0*MeV )
 {
     ( (G4Track*) aTrack )->SetTrackStatus( fStopAndKill );
  }

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NNBARTrackingAction::PostUserTrackingAction( const G4Track* aTrack ) {
  if ( aTrack->GetTrackStatus() == fStopAndKill  &&  aTrack->GetParentID() == 0 ) {
    NNBARPrimaryParticleInformation* info = (NNBARPrimaryParticleInformation*) 
       aTrack->GetDynamicParticle()->GetPrimaryParticle()->GetUserInformation();
    //info->Print();
    NNBAROutput::Instance()->SaveTrack( NNBAROutput::eSaveMC,
                                        info->GetPartID(),
                                        info->GetPDG(),
                                        info->GetMCKineticEnergy(),
                                        info->GetMCPosition()/mm );
                                        //info->GetMCMomentum()/MeV);
                                        
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

