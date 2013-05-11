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

#ifndef NeutronPrimaryGeneratorAction_h
#define NeutronPrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "Randomize.hh"
#include "globals.hh"

class G4ParticleGun;
class G4Event;

class NeutronPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
  public:
    NeutronPrimaryGeneratorAction();
    NeutronPrimaryGeneratorAction(G4double s_neutronEnergy, G4double s_neutronEnergySpread, G4double s_positronEnergy, G4double s_positronEnergySpread, G4double s_incidentAngle);
    ~NeutronPrimaryGeneratorAction();

  public:
    void GeneratePrimaries(G4Event* anEvent);

  private:
    G4ParticleGun* particleGunThermalNeutron;
    G4double neutronEnergy, neutronEnergySpread;
    G4ParticleGun* particleGunPositron;
    G4double positronEnergy, positronEnergySpread;
	G4double incidentAngle;
};

#endif

