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
/// \file NeutronSimTrackingAction.hh
/// \brief Definition of the NeutronSimTrackingAction class

#ifndef NeutronSimTrackingAction_h
#define NeutronSimTrackingAction_h 1

#include "G4UserTrackingAction.hh"
#include "G4ThreeVector.hh"
#include "globals.hh"
#include "Randomize.hh"
#include <fstream>

using namespace std;

/// Event action class

class NeutronSimTrackingAction : public G4UserTrackingAction
{
  public:
    NeutronSimTrackingAction(ofstream* s_verboseOut, G4double s_verticalResolution, bool s_outputRealistic);
    virtual ~NeutronSimTrackingAction();
	void GetStatistics(G4int& numTracks, G4double& alphaMeanX, G4double& alphaSigmaX, G4double& alphaMeanY, G4double& alphaSigmaY, G4double& alphaMeanZ, G4double& alphaSigmaZ, G4double& recMeanTheta, G4double& recSigmaTheta, G4double& alphaMeanT, G4double& alphaSigmaT);
	virtual void PreUserTrackingAction(const G4Track*);    
	virtual void PostUserTrackingAction(const G4Track*);
private:
	//neutron initial positions
	G4ThreeVector latestNeutronStartPosition;
	G4ThreeVector latestNeutronStartVolumePosition;
	//alpha positions
	G4double alphaSumX, alphaSumXSquared, alphaSumY, alphaSumYSquared, alphaSumZ, alphaSumZSquared;
	//reconstructions
	G4double recSumTheta, recSumThetaSquared;
	//timing
	G4double alphaSumT, alphaSumTSquared;
	G4int  numAlphaTracks;
	G4double verticalResolution;
	
	bool outputRealistic;
	bool validTrack;
	ofstream* verboseOut;
};


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
