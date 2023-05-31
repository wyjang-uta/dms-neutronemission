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
/// \file DMSSteppingAction.cc
/// \brief Implementation of the DMSSteppingAction class

#include "DMSSteppingAction.hh"
#include "DMSEventAction.hh"
#include "DMSDetectorConstruction.hh"

#include "G4Step.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4LogicalVolume.hh"
#include "G4ParticleTypes.hh"
#include "g4root.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DMSSteppingAction::DMSSteppingAction()
: G4UserSteppingAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DMSSteppingAction::~DMSSteppingAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DMSSteppingAction::UserSteppingAction(const G4Step* step)
{
  auto analysisManager = G4AnalysisManager::Instance();
  G4StepPoint* presp = step->GetPreStepPoint();
  G4StepPoint* postsp = step->GetPostStepPoint();

  // Record all neutrons with prestep volume name == "Dump" but poststep volume name != "Dump".
  // This means that particle is exiting the dump volume.
  G4String presp_volume;
  G4String postsp_volume;
  if( presp == nullptr ) return;
  if( postsp == nullptr ) return;
  if( step->GetTrack()->GetParticleDefinition()->GetPDGEncoding() != 2112 ) return; // only neutrons
  if( postsp->GetTouchableHandle()->GetVolume() == nullptr ) return;
  if( presp->GetTouchableHandle()->GetVolume() == nullptr ) return;
  presp_volume = presp->GetTouchableHandle()->GetVolume()->GetLogicalVolume()->GetName();
  postsp_volume = postsp->GetTouchableHandle()->GetVolume()->GetLogicalVolume()->GetName();
  // select events
  if( !(presp_volume == "Dump" && postsp_volume != "Dump") ) return;

  G4double KE=(G4double)postsp->GetKineticEnergy()/CLHEP::MeV;    // Kinetic energy
  G4double TE=(G4double)postsp->GetTotalEnergy()/CLHEP::MeV;      // Total energy
  G4double vx=(G4double)postsp->GetPosition().getX()/CLHEP::cm;   // Position neutron exits the dump.
  G4double vy=(G4double)postsp->GetPosition().getY()/CLHEP::cm;
  G4double vz=(G4double)postsp->GetPosition().getZ()/CLHEP::cm;
  G4double px=(G4double)postsp->GetMomentum().getX()/CLHEP::MeV;  // Momentum of the neutron.
  G4double py=(G4double)postsp->GetMomentum().getY()/CLHEP::MeV;
  G4double pz=(G4double)postsp->GetMomentum().getZ()/CLHEP::MeV;
  G4double theta=acos(pz/sqrt(px*px+py*py+pz*pz))*180./M_PI;      // Direction of the neutron with respect to the z-axis (beam direction).
  G4double gt=(G4double)postsp->GetGlobalTime()/CLHEP::ns;
  G4double lt=(G4double)postsp->GetLocalTime()/CLHEP::ns;

  // kineticEnergy
  analysisManager->FillNtupleDColumn(0, KE);
  // total energy
  analysisManager->FillNtupleDColumn(1, TE);
  // vertex coordinates
  analysisManager->FillNtupleDColumn(2, vx);
  analysisManager->FillNtupleDColumn(3, vy);
  analysisManager->FillNtupleDColumn(4, vz);
  analysisManager->FillNtupleDColumn(5, theta);
  // momentum
  analysisManager->FillNtupleDColumn(6, px);
  analysisManager->FillNtupleDColumn(7, py);
  analysisManager->FillNtupleDColumn(8, pz);
  // time info
  analysisManager->FillNtupleDColumn(9, gt);
  analysisManager->FillNtupleDColumn(10, lt);
  analysisManager->AddNtupleRow();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

