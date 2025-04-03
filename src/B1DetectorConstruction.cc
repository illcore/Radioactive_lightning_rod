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
/// \file B1DetectorConstruction.cc
/// \brief Implementation of the B1DetectorConstruction class

#include "B1DetectorConstruction.hh"

#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Cons.hh"
#include "G4Orb.hh"
#include "G4Sphere.hh"
#include "G4Tubs.hh"
#include "G4RotationMatrix.hh"
#include "G4SubtractionSolid.hh"
#include "G4IntersectionSolid.hh"
#include "G4VSolid.hh"
#include "G4Trd.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1DetectorConstruction::B1DetectorConstruction()
: G4VUserDetectorConstruction()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1DetectorConstruction::~B1DetectorConstruction()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* B1DetectorConstruction::Construct()
{  
  // Get nist material manager
  G4NistManager* nist = G4NistManager::Instance();
     
  // Option to switch on/off checking of volumes overlaps
  //
  G4bool checkOverlaps = true;

  //     
  // World
  //
  
G4Material* world_mat = nist->FindOrBuildMaterial("G4_AIR");
  
G4Box* solidWorld =    
    new G4Box("WorldBox",                       //its name
       150*cm, 150*cm, 150*cm);     //its size
      
G4LogicalVolume* logicWorld =                         
    new G4LogicalVolume(solidWorld,          //its solid
                        world_mat,           //its material
                        "LogicalWorld");            //its name
                                   
G4VPhysicalVolume* physWorld = 
    new G4PVPlacement(0,                     //no rotation
                      G4ThreeVector(),       //at (0,0,0)
                      logicWorld,            //its logical volume
                      "World",               //its name
                      0,                     //its mother  volume
                      false,                 //no boolean operation
                      0,                     //copy number
                      checkOverlaps);        //overlaps checking

//Source                      
G4Material* source_mat = nist->FindOrBuildMaterial("G4_Cs");
G4Orb* Source = new G4Orb("SolidSource", 1*cm);
G4LogicalVolume* logicsource = new G4LogicalVolume(Source, source_mat, "LogicalSource");        
new G4PVPlacement(0, G4ThreeVector(), logicsource, "Source", logicWorld, false, 0, checkOverlaps);

//Sphere                                            
G4Material* sphere_mat = nist->FindOrBuildMaterial("G4_Al");
G4Sphere* AluSphere = new G4Sphere("AluSphere", 4.75*cm, 5.0*cm, 0.0*rad, 360*deg, 0.0*rad, 180*deg);
G4LogicalVolume* Sphere_logic = new G4LogicalVolume(AluSphere, sphere_mat, "LogicSphere");
new G4PVPlacement(0, G4ThreeVector(), Sphere_logic, "Sphere", logicWorld, false, 0, checkOverlaps);

//Disc
G4Material* material = G4NistManager::Instance()->FindOrBuildMaterial("G4_Al");
G4RotationMatrix* rotationMatrix = new G4RotationMatrix();
rotationMatrix->rotateX(90.*deg);   
G4Tubs* solidDisc = new G4Tubs("SolidDisc", 5.0*cm, 8.0*cm, 0.25*cm, 0.0*deg, 360.0*deg);
G4LogicalVolume* logicDisc = new G4LogicalVolume(solidDisc, material, "LogicDisc");
new G4PVPlacement(rotationMatrix, G4ThreeVector(), logicDisc, "Disc", logicWorld, false, 0, checkOverlaps); 

//Rod
G4Material* rod_mat = nist->FindOrBuildMaterial("G4_Fe");
G4Tubs* rod_solidI = new G4Tubs("SolidRodI", 0, 0.5*cm, 50*cm, 0, 360*deg);
G4LogicalVolume* rod_logicI = new G4LogicalVolume(rod_solidI, rod_mat, "LogicRodI");
new G4PVPlacement(rotationMatrix, G4ThreeVector(0*cm,-55*cm,0*cm), rod_logicI, "Rod", logicWorld, false, 0, checkOverlaps);
G4Tubs* rod_solidII = new G4Tubs("SoldiRodII", 0, 0.5*cm, 10*cm, 0, 360*deg);
G4LogicalVolume* rod_logicII = new G4LogicalVolume(rod_solidII, rod_mat, "LogicRodII");
new G4PVPlacement(rotationMatrix, G4ThreeVector(0*cm,15*cm,0*cm), rod_logicII, "Rod", logicWorld, false, 0, checkOverlaps);                     
return physWorld;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
