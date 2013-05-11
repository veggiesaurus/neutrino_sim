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
// $Id$
//

#ifndef NeutronDetectorConstruction_H
#define NeutronDetectorConstruction_H 1

#include "G4VUserDetectorConstruction.hh"
#include "G4Material.hh"
#include "globals.hh"


class G4LogicalVolume;
class G4VPhysicalVolume;
class NeutronDetectorMessenger;
class G4UserLimits;


class NeutronDetectorConstruction : public G4VUserDetectorConstruction
{
  public:

    NeutronDetectorConstruction(G4double s_boronLoading, G4double s_boronEnrichment, G4double s_hexRadius, G4double s_hexLength, G4int s_edgeCells);
    ~NeutronDetectorConstruction();

    G4VPhysicalVolume* Construct();
	
	void SetMaxStep (G4double );

  private:
	void ConstructHexDetector(G4Material* material);
	void ConstructSquareDetector(G4Material* material);

    //parameters
    G4double boronLoading, boronEnrichment;
    G4double hexRadius, hexLength;
    G4int edgeCells;
    
    // Logical volumes
    //
    G4LogicalVolume* experimentalHall_log;
    G4LogicalVolume* tracker_log;

    // Physical volumes
    //
    G4VPhysicalVolume* experimentalHall_phys;
    G4VPhysicalVolume* tracker_phys;

	//Materials
	G4Material* PVT, *PPO, *DPA, *PVT_doped, *BC454, *BC454_enr;

	G4UserLimits* fStepLimit;            // pointer to user step limits
    NeutronDetectorMessenger*  fMessenger;   // messenger
};

#endif

