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
// $Id$
// 
/// \file NeutrinoDetectorMessenger.cc
/// \brief Implementation of the NeutrinoDetectorMessenger class

#include "NeutrinoDetectorMessenger.hh"
#include "NeutrinoDetectorConstruction.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

NeutrinoDetectorMessenger::NeutrinoDetectorMessenger(NeutrinoDetectorConstruction* Det)
 : G4UImessenger(),
   fDetectorConstruction(Det)
{
  fNeutrinoDirectory = new G4UIdirectory("/neutrino/");
  fNeutrinoDirectory->SetGuidance("UI commands specific to this simulation.");

  fDetDirectory = new G4UIdirectory("/neutrino/det/");
  fDetDirectory->SetGuidance("Detector construction control");
 

  fStepMaxCmd = new G4UIcmdWithADoubleAndUnit("/neutrino/det/stepMax",this);
  fStepMaxCmd->SetGuidance("Define a step max");
  fStepMaxCmd->SetParameterName("stepMax",false);
  fStepMaxCmd->SetUnitCategory("Length");
  fStepMaxCmd->AvailableForStates(G4State_Idle);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

NeutrinoDetectorMessenger::~NeutrinoDetectorMessenger()
{ 
  delete fStepMaxCmd;
  delete fNeutrinoDirectory;
  delete fDetDirectory;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NeutrinoDetectorMessenger::SetNewValue(G4UIcommand* command,G4String newValue)
{

  if( command == fStepMaxCmd ) 
  {
    fDetectorConstruction->SetMaxStep(fStepMaxCmd->GetNewDoubleValue(newValue));
	G4cout << "\n----> Setting new step limit to  " << newValue << G4endl;
  }   
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
