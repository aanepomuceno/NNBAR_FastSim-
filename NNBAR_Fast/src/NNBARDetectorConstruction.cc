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
/// \brief Implementation of the NNBARDetectorConstruction class
/// Based on Geant4 NNBAR parameterisations example
//  Adapted by A Nepomuceno - Winter 2025

#include "NNBARDetectorConstruction.hh"
#include "G4ProductionCuts.hh"
#include "G4SystemOfUnits.hh"
#include "G4RegionStore.hh"
#include "G4GDMLParser.hh"
#include "G4AutoDelete.hh"
#include "G4GlobalMagFieldMessenger.hh"
#include "G4AutoDelete.hh"

#include "G4Material.hh"
#include "G4MaterialTable.hh"
#include "G4Element.hh"
#include "G4ProductionCuts.hh"
#include "G4ElementTable.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4SubtractionSolid.hh"
#include "G4LogicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4PVPlacement.hh"
#include "G4PVReplica.hh"
#include "G4SystemOfUnits.hh"
#include "G4NistManager.hh"
#include "G4RegionStore.hh"
#include "G4VisAttributes.hh"
#include "G4PhysicalConstants.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

NNBARDetectorConstruction::NNBARDetectorConstruction() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

NNBARDetectorConstruction::~NNBARDetectorConstruction() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* NNBARDetectorConstruction::Construct() {
  
   G4double a;
   G4double z;
   G4double density;
   G4int fCheckOverlaps = 0;
   G4NistManager* nistManager = G4NistManager::Instance();
   

//-----------Vacuum-------------------------------------------
  
   new G4Material("Galactic", z=1., a=1.01*g/mole, density= universe_mean_density,
                  kStateGas, 2.73*kelvin, 3.e-18*pascal);
   auto defaultMaterial = G4Material::GetMaterial("Galactic");
   
//-----------Lead Glass EMCAL----------------------------------
   //Elements for Lead-glass (Material will be named Abs)
   G4Element* elTi = nistManager->FindOrBuildElement("Ti");
   G4Element* elAs = nistManager->FindOrBuildElement("As");
   G4Element* elPb = nistManager->FindOrBuildElement("Pb");
   G4Element* elO = nistManager->FindOrBuildElement("O");
   G4Element* elSi = nistManager->FindOrBuildElement("Si");

   //Lead-glass Material
   G4Material* Abs = new G4Material("Abs", 3.86*g/cm3, 5);
   Abs->AddElement(elO, 0.156453);
   Abs->AddElement(elSi, 0.080866);
   Abs->AddElement(elTi, 0.008092);
   Abs->AddElement(elAs, .002651);
   Abs->AddElement(elPb, 0.751938);
   auto absorberMaterial = G4Material::GetMaterial("Abs");
   
//-----------Scintilator HCAL---------------------------------
   //Elements for scintilator (Material will be named Scint)
   G4Element* elH = nistManager->FindOrBuildElement("H");
   G4Element* elC = nistManager->FindOrBuildElement("C");

   //Scintilator Material
   G4Material* Scint = new G4Material("Scint", 1.023*g/cm3, 2);
   Scint->AddElement(elH, 0.524573);
   Scint->AddElement(elC, 1 - 0.524573);
   auto scintMaterial = G4Material::GetMaterial("Scint");
   
   //Target Material
   G4Material* Carbon_target = new G4Material("Carbon_target" , density=3.52*g/cm3, 1);
   Carbon_target->AddElement(elC, 1);
   auto carbonMaterial = G4Material::GetMaterial("Carbon_target");
    
   
    //Dimensions
    G4int  nofLayers = 10;
    G4double  absoThickness  = 25.*cm;
    G4double  scintThickness =  30.*cm;
    G4double  calorSizeXY    =  5.15*m;
    G4double  calorSizeZ     =  6.0*m;
    G4double  tubeThickness  =  2.*cm;

    auto calorThickness = scintThickness + absoThickness;
    auto worldSizeXY = 1.2 * calorSizeXY;
    auto worldSizeZ  = 1.2 * calorSizeZ;

//-----------Build World Volume--------------------------------------

   auto worldS = new G4Box("World", worldSizeXY/2, worldSizeXY/2, worldSizeZ/2); 
   auto worldLV = new G4LogicalVolume(worldS,defaultMaterial,"World");
   auto worldPV  = new G4PVPlacement(
                 0,                // no rotation
                 G4ThreeVector(),  // at (0,0,0)
                 worldLV,          // its logical volume                         
                 "World",          // its name
                 0,                // its mother  volume
                 false,            // no boolean operation
                 0,                // copy number
                 fCheckOverlaps);  // checking overlaps 

//-----------Build boxes for calorimeters construction---------------------------------
  
   auto box1 = new G4Box("Box1", calorSizeXY/2., calorSizeXY/2., calorSizeZ/2.); 
   auto box2 = new G4Box("Box2",(calorSizeXY-absoThickness)/2., (calorSizeXY-absoThickness)/2., calorSizeZ/2.);
   auto box3 = new G4Box("Box3", (calorSizeXY-calorThickness)/2., (calorSizeXY-calorThickness)/2., calorSizeZ/2.);

//-----------Build Lead glass calorimeter---------------------------------

   auto absorberS = new G4SubtractionSolid("Abso", box1, box2,0, G4ThreeVector(0.,0.,0.));
   auto absorberLV = new G4LogicalVolume(absorberS,  absorberMaterial, "AbsoLV"); 
   new G4PVPlacement(
                 0,                // no rotation
		 G4ThreeVector(0., 0., 0.),
                 absorberLV,       // its logical volume                         
                 "Abso",           // its name
                 worldLV,	   // its mother volume
                 false,            // no boolean operation
                 0,                // copy number
                 fCheckOverlaps);  // checking overlaps 
 
 //-----------Build Had Calorimeter--------------------------------------------
 
   auto scintS = new G4SubtractionSolid("Scint",box2,box3);
   auto scintLV = new G4LogicalVolume(scintS, scintMaterial, "ScintLV"); 
   new G4PVPlacement(
                    0,                   // no rotation
                    G4ThreeVector(0., 0., 0),
                    scintLV,             // its logical volume                         
                    "Scint",             // its name
                    worldLV,	         // its mother volume
                    false,               // no boolean operation
                    0,                   // copy number
                    fCheckOverlaps);     // checking overlaps 

 //-----------Build hallow box----------------------------------------------
 //This block has no function but vizualization

    auto hollowS = new G4Box("Hollow",(calorSizeXY-calorThickness)/2., (calorSizeXY-calorThickness)/2., calorSizeZ/2.);
    auto hollowLV = new G4LogicalVolume(hollowS,defaultMaterial,"HollowLV");

    new G4PVPlacement(
                 0,                // no rotation
		 G4ThreeVector(0., 0., 0.),
                 hollowLV,       // its logical volume                         
                 "Hollow",           // its name
                 worldLV,	   // its mother volume
                 false,            // no boolean operation
                 0,                // copy number
                 fCheckOverlaps);  // checking overlaps 
                 
 //-----------Build Carbon Target--------------------------------------------------
   G4double carbon_radius = 100.0*cm; 
   G4double carbon_len = 0.01*cm; 
   G4double carbon_angle = 360. * deg; 
   
   auto carbonS = new G4Tubs("CarbonS",0.,carbon_radius,carbon_len,0,carbon_angle);
   auto carbonLV = new G4LogicalVolume(carbonS,carbonMaterial,"CarbonLV");
   new G4PVPlacement(0,G4ThreeVector(0., 0., 0.),carbonLV,"CarbonPV",hollowLV,false,0,fCheckOverlaps);


 //-----------Visualization for hollowLV (make it same color as background)----------
//  auto black_color = new G4VisAttributes(G4Colour::Black()); 
//  black_color->SetVisibility(true);
//  carbonLV->SetVisAttributes(black_color);

//-------Makes absorber and scintilator volumes becoming a G4Region---------------------

   G4Region* caloRegion = new G4Region("EM_calo_region");
   caloRegion->AddRootLogicalVolume(absorberLV);
//   caloRegion->SetProductionCuts(new G4ProductionCuts());
//   caloRegion->GetProductionCuts()->SetProductionCut( 0.1*m, idxG4GammaCut );
//   caloRegion->GetProductionCuts()->SetProductionCut( 1.0*cm, idxG4GammaCut );
   
   G4Region* hadRegion = new G4Region("HAD_calo_region");   
   hadRegion->AddRootLogicalVolume(scintLV);

    return worldPV;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NNBARDetectorConstruction::ConstructSDandField() {
	
  
   G4RegionStore* regionStore = G4RegionStore::GetInstance();
   G4Region* caloRegion = regionStore->GetRegion("EM_calo_region");
   G4Region* hadRegion  = regionStore->GetRegion("HAD_calo_region"); 
  
  NNBARFastSimModelEMCal* fastSimModelEMCal
      = new NNBARFastSimModelEMCal( "fastSimModelEMCal", caloRegion,
                                    NNBARDetectorParametrisation::eNNBAR );
  // Register the EM fast simulation model for deleting
    G4AutoDelete::Register(fastSimModelEMCal);
    
  NNBARFastSimModelHCal* fastSimModelHCal
      = new NNBARFastSimModelHCal( "fastSimModelHCal", hadRegion,
                                   NNBARDetectorParametrisation::eNNBAR );
    // Register the HAD fast simulation model for deleting
    G4AutoDelete::Register( fastSimModelHCal );

 // Add global magnetic field
  G4ThreeVector fieldValue = G4ThreeVector(0.,0.,0.);
  fMagFieldMessenger = new G4GlobalMagFieldMessenger( fieldValue );
  fMagFieldMessenger->SetVerboseLevel(1);
}
 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

