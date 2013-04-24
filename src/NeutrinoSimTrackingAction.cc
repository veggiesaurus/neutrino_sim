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
/// \file NeutrinoSimTrackingAction.cc
/// \brief Implementation of the NeutrinoSimTrackingAction class

#include "NeutrinoSimTrackingAction.hh"

#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4TrajectoryContainer.hh"
#include "G4Trajectory.hh"
#include "G4ios.hh"
#include "G4Alpha.hh"
#include <boost/format.hpp>
using boost::format;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

NeutrinoSimTrackingAction::NeutrinoSimTrackingAction(ofstream* s_verboseOut, G4double s_verticalResolution, bool s_outputRealistic)
	: G4UserTrackingAction()
{	
	alphaSumX=0,alphaSumXSquared=0,alphaSumY=0, alphaSumYSquared=0, alphaSumZ=0, alphaSumZSquared=0;
	//reconstructions
	recSumTheta=0, recSumThetaSquared=0;
	numAlphaTracks=0;
	verboseOut=s_verboseOut;
	verticalResolution=s_verticalResolution;
	outputRealistic=s_outputRealistic;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

NeutrinoSimTrackingAction::~NeutrinoSimTrackingAction()
{

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NeutrinoSimTrackingAction::PostUserTrackingAction(const G4Track* track)
{
	//alpha particle originating from primary neutron
	if (track->GetParticleDefinition()==G4Alpha::Alpha() && track->GetParentID()==1)
	{
		G4ThreeVector pos=track->GetVertexPosition();
		G4double x_f=pos.x();
		G4double y_f=pos.y();
		G4double z_f=pos.z();
		G4double x_i=0;
		G4double y_i=0;
		G4double z_i=0;

		//take into account the finite vertical resolution
		if (outputRealistic)
		{
			y_i=G4RandGauss::shoot(y_i,verticalResolution*mm);
			y_f=G4RandGauss::shoot(y_f,verticalResolution*mm);
		}
		alphaSumX+=x_f;
		alphaSumXSquared+=x_f*x_f;
		alphaSumY+=y_f;
		alphaSumYSquared+=y_f*y_f;
		alphaSumZ+=z_f;
		alphaSumZSquared+=z_f*z_f;

		//tan(theta)=dY/dX
		G4double theta=atan2((x_f-x_i),(y_f-y_i));
		if (verboseOut)
		{
			(*verboseOut)  << format("%9.4f  %9.4f  %9.4f  %9.4f") %x_f%y_f%z_f% theta<<endl;			
		}

		recSumTheta+=theta;
		recSumThetaSquared+=theta*theta;
		numAlphaTracks++;
	}

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NeutrinoSimTrackingAction::GetStatistics(G4int& numTracks, G4double& alphaMeanX, G4double& alphaSigmaX, G4double& alphaMeanY, G4double& alphaSigmaY, G4double& alphaMeanZ, G4double& alphaSigmaZ, G4double& recMeanTheta, G4double& recSigmaTheta)
{
	if (numAlphaTracks)
	{
		alphaMeanX=alphaSumX/numAlphaTracks;
		alphaMeanY=alphaSumY/numAlphaTracks;
		alphaMeanZ=alphaSumZ/numAlphaTracks;
		recMeanTheta=recSumTheta/numAlphaTracks;

		alphaSigmaX=sqrt(alphaSumXSquared/numAlphaTracks-alphaMeanX*alphaMeanX);
		alphaSigmaY=sqrt(alphaSumYSquared/numAlphaTracks-alphaMeanY*alphaMeanY);
		alphaSigmaZ=sqrt(alphaSumZSquared/numAlphaTracks-alphaMeanZ*alphaMeanZ);
		recSigmaTheta=sqrt(recSumThetaSquared/numAlphaTracks-recMeanTheta*recMeanTheta);		
	}
}