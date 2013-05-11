#include "NeutronPrimaryGeneratorAction.hh"
#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "globals.hh"
#include "G4SystemOfUnits.hh"

//sets up a 4 MeV neutron gun at a position (-0,0,0)
NeutronPrimaryGeneratorAction::NeutronPrimaryGeneratorAction(G4double s_neutronEnergy, G4double s_neutronEnergySpread, G4double s_positronEnergy, G4double s_positronEnergySpread, G4double s_incidentAngle)
{
  G4int n_particle = 1;
  G4String particleName;
  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
 
  particleGunThermalNeutron = new G4ParticleGun(n_particle);
  particleGunThermalNeutron->SetParticleDefinition(particleTable->FindParticle(particleName="neutron"));
  neutronEnergy=s_neutronEnergy;
  neutronEnergySpread=s_neutronEnergySpread;
  
  
  particleGunPositron = new G4ParticleGun(n_particle);
  particleGunPositron->SetParticleDefinition(particleTable->FindParticle(particleName="e+"));
  positronEnergy=s_positronEnergy;
  positronEnergySpread=s_positronEnergySpread;

  incidentAngle=s_incidentAngle;
  
}

NeutronPrimaryGeneratorAction::~NeutronPrimaryGeneratorAction()
{
  delete particleGunPositron;
  delete particleGunThermalNeutron;
}

//generates primaries with slightly different directions (eventID used as seed)
void NeutronPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  double eventNeutronEnergy=neutronEnergy;
  if (neutronEnergySpread)
  	eventNeutronEnergy=G4RandGauss::shoot(neutronEnergy,neutronEnergySpread);  
  particleGunThermalNeutron->SetParticleEnergy(eventNeutronEnergy*keV);
  
  double eventPositronEnergy=positronEnergy;
  if (positronEnergySpread)
  	eventPositronEnergy=G4RandGauss::shoot(positronEnergy,positronEnergySpread);  
  particleGunPositron->SetParticleEnergy(eventPositronEnergy*keV);
  
  double vertexSpread=1*m;  
  G4ThreeVector vertexPosition(G4RandFlat::shoot(-vertexSpread/2.0, vertexSpread/2.0), G4RandFlat::shoot(-vertexSpread/2.0, vertexSpread/2.0), G4RandFlat::shoot(-vertexSpread/2.0, vertexSpread/2.0));
  particleGunThermalNeutron->SetParticlePosition(vertexPosition);
  particleGunPositron->SetParticlePosition(vertexPosition);
  
  G4double angleInRadians=incidentAngle*pi/180.0;
  G4ThreeVector v(sin(angleInRadians),cos(angleInRadians),0.0);
  particleGunThermalNeutron->SetParticleMomentumDirection(v);
  particleGunThermalNeutron->GeneratePrimaryVertex(anEvent);
  particleGunPositron->SetParticleMomentumDirection(v);
  particleGunPositron->GeneratePrimaryVertex(anEvent);
}


