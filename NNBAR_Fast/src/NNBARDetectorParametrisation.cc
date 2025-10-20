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
//----------------------------------------------------------------------------
/// \file NNBARDetectorParametrisation.cc
// Based on Geant4 NNBAR parametrization example
// Andre Nepomuceno - November 2024
////---------------------------------------------------------------------------

#include "NNBARDetectorParametrisation.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"
#include "Randomize.hh"
G4double p1 = 1.0; //probability for sorting double gaussian

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

NNBARDetectorParametrisation::NNBARDetectorParametrisation() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

NNBARDetectorParametrisation::~NNBARDetectorParametrisation() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4double NNBARDetectorParametrisation::GetResolution( Detector aDetector, 
                                                      Parametrisation aParam, 
                                                      G4double aKenergy, G4int pdg ) {

G4double res = 1.0;
//-------------------------------------------------------------------------- 
//Generic Detector (based on different detectors and test beam data)
  if ( aParam == eGENERIC ) {
    aKenergy /= GeV;  //kinetic energy must be in GeV
    switch ( aDetector ) {
      case NNBARDetectorParametrisation::eEMCAL :
           res = 0.056/std::sqrt(aKenergy) + 0.011;
           break;
      case NNBARDetectorParametrisation::eHCAL :
           res = std::sqrt( std::pow( 0.51/std::sqrt( aKenergy ),2) + std::pow( 0.07, 2 ) );
           break;
      case NNBARDetectorParametrisation::eTRACKER :
           res = 0.013;
           break;
    }
   }
 //--------------------------------------------------------------------------
 //NNBAR Detector
   else if ( aParam == eNNBAR ) {
    aKenergy /= GeV;  //aMomentum must be in GeV
         if (aDetector == NNBARDetectorParametrisation::eEMCAL ) {
             if (abs(pdg) == 11 || pdg == 22 || abs(pdg) == 13 )  res = 0.056/std::sqrt(aKenergy) + 0.011;
         }
         
        if (aDetector == NNBARDetectorParametrisation::eHCAL ) {
           if ( abs(pdg) == 211) res = 0.11;  //using FWHM
        }
       
         //if (aDetector == NNBARDetectorParametrisation::eTRACKER ) {
           // res = 1.0;
         //}
   }  
  
  return res;
 }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4double NNBARDetectorParametrisation::GetMedian( Detector aDetector, 
                                                      Parametrisation aParam,
                                                      G4double aKenergy,G4int pdg ) {
    G4double med = 1.0;
    if ( aParam == eNNBAR  ) { 
        aKenergy /= GeV;  //aMomentum in MeV
        if (aDetector == NNBARDetectorParametrisation::eEMCAL ) {
           med = 1.0;
        }
   
       if (aDetector == NNBARDetectorParametrisation::eHCAL ) {
         med = 1.0;
       }

//     if (aDetector == NNBARDetectorParametrisation::eTRACKER ) {
//         med = 1.0;
//       }

    }
  return med;
 }  

//--------------------------------------------------------------------------------------
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4double NNBARDetectorParametrisation::GetEfficiency( Detector aDetector, 
                                                      Parametrisation /*aParam*/,
                                                      G4double /*aMomentum*/ ) {
  // For the time being, we set the efficiency to 1.0
  G4double eff = 1.0;
  switch ( aDetector ) {
    case NNBARDetectorParametrisation::eTRACKER :
      eff = 1.0;
      break;
    case NNBARDetectorParametrisation::eEMCAL :
      eff = 1.0;
      break;
    case NNBARDetectorParametrisation::eHCAL :
      eff = 1.0;
      break;
  }
  return eff;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

