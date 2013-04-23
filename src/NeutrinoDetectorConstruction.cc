#include "NeutrinoDetectorConstruction.hh"
#include "NeutrinoDetectorConstruction.hh"
#include "G4Material.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4TwoVector.hh"
#include "G4VisAttributes.hh"
#include "G4ExtrudedSolid.hh"
#include "G4UserLimits.hh"
#include "G4PhysicalConstants.hh"
#include "G4RotationMatrix.hh"
#include "G4PVPlacement.hh"
#include "globals.hh"
#include "G4SystemOfUnits.hh"
#include "G4NistManager.hh"
#include <vector>

NeutrinoDetectorConstruction::NeutrinoDetectorConstruction(G4double s_boronLoading, G4double s_boronEnrichment, G4double s_hexRadius, G4double s_hexLength, G4int s_edgeCells)
 :  boronLoading(s_boronLoading), boronEnrichment(s_boronEnrichment),
    hexRadius(s_hexRadius), hexLength(s_hexLength), edgeCells(s_edgeCells),
    experimentalHall_log(0), tracker_log(0),   
    experimentalHall_phys(0), tracker_phys(0),
	fStepLimit(0)
{;}

NeutrinoDetectorConstruction::~NeutrinoDetectorConstruction()
{
	delete fStepLimit;
}

G4VPhysicalVolume* NeutrinoDetectorConstruction::Construct()
{

  //------------------------------------------------------ materials

   G4double a, z;
  G4double density;
  G4int ncomponents, natoms, nisotopes;
  
  //Elements
  G4Element* H = new G4Element("Hydrogen", "H", z=1., a= 1.008*g/mole);
  G4Element* C = new G4Element("Carbon"  , "C", z=6., a= 12.01*g/mole);
  G4Isotope* B10 = new G4Isotope("B10",  5, 10, 10.0*g/mole);
  G4Isotope* B11 = new G4Isotope("B11",  5, 11, 11.0*g/mole);
  G4Element* BoronEnriched = new G4Element("BoronEnriched", "B_enr",nisotopes=2);
  BoronEnriched->AddIsotope(B10, boronEnrichment*perCent);
  BoronEnriched->AddIsotope(B11, (100-boronEnrichment)*perCent);
  G4Element* B = new G4Element("Boron", "B", z=5., a= 10.81*g/mole);
  G4Element* O = new G4Element("Oxygen", "O", z=8., a= 15.9994*g/mole);
  G4Element* N = new G4Element("Nitrogen", "N", z=7., a= 14.0067*g/mole);

  //Polyvinyl tourine matrix
  G4Material* PVT = new G4Material("PVT", density= 1032*mg/cm3, ncomponents=2, kStateSolid);
  PVT->AddElement(H, 8.14*perCent);
  PVT->AddElement(C, 91.86*perCent);

  //PPO dye
  G4Material* PPO = new G4Material("PPO", density= 1094*mg/cm3, ncomponents=4, kStateSolid);
  PPO->AddElement(C, natoms=15);
  PPO->AddElement(H, natoms=11);
  PPO->AddElement(N, natoms=1);
  PPO->AddElement(O, natoms=1);

 //DPA dye
  G4Material* DPA = new G4Material("DPA", density= 1220*mg/cm3, ncomponents=2, kStateSolid);
  DPA->AddElement(C, natoms=26);
  DPA->AddElement(H, natoms=18);
  
  //Density is an estimate. 
  G4double estimatedDensity=(69.8*1032+0.2*1220+(30-boronLoading)*1094+boronLoading*2080)/100.0;
  G4Material* PVT_doped = new G4Material("EJ299-33-doped", density= estimatedDensity*mg/cm3, ncomponents=4, kStateSolid);
  PVT_doped->AddMaterial(PVT, 69.8*perCent);
  PVT_doped->AddMaterial(PPO, (30-boronLoading)*perCent);
  PVT_doped->AddMaterial(DPA, 0.2*perCent);
  PVT_doped->AddElement(BoronEnriched, boronLoading*perCent);
  std::cout<<"Density= "<<estimatedDensity<<std::endl;
  G4NistManager* man = G4NistManager::Instance();
  G4Material* Air  = man->FindOrBuildMaterial("G4_AIR");

  //------------------------------------------------------ volumes
  
  //------------------------------ experimental hall (world volume)
  //------------------------------ beam line along x axis

  G4double expHall_x = 1.0*m;
  G4double expHall_y = 0.5*m;
  G4double expHall_z = 0.5*m;
  G4Box* experimentalHall_box
    = new G4Box("expHall_box",expHall_x,expHall_y,expHall_z);
  experimentalHall_log = new G4LogicalVolume(experimentalHall_box,
                                             Air,"expHall_log",0,0,0);
  experimentalHall_log->SetVisAttributes (G4VisAttributes::Invisible);
  experimentalHall_phys = new G4PVPlacement(0,G4ThreeVector(), experimentalHall_log,"expHall",0,false,0);

 //Hexagon
  
  std::vector<G4TwoVector> hexPoints; 
  G4double centerX=0.*mm;
  G4double centerY=0.*mm;  
  for (int i=0;i<6;i++)
  {
  	//the minus is used in order to define the polygon clockwise, rather than counterclockwise
  	G4double x=hexRadius*std::cos(-2*pi/6.0*i)+centerX;
  	G4double y=hexRadius*std::sin(-2*pi/6.0*i)+centerY;
  	G4TwoVector p(x,y);
  	hexPoints.push_back(p);
  }
  
  G4TwoVector offset1, offset2;
  G4double scale1=1, scale2=1;
  
  G4ExtrudedSolid* hexExtruded=new G4ExtrudedSolid("hex_extruded", hexPoints, hexLength/2.0, offset1, scale1, offset2, scale2);

  tracker_log = new G4LogicalVolume(hexExtruded,PVT_doped,"tracker_log",0,0,0);
  G4double trackerPos_x = 0.*m;
  G4double trackerPos_y = 0.*m;
  G4double trackerPos_z = 0.*m;
  G4RotationMatrix* rm=new G4RotationMatrix();
  rm->rotateX(pi/2.0);
  rm->rotateZ(pi/6.0);
  
  G4int index=0;
  G4int rows=edgeCells*2-1;
  char volumeName[24];
  for (int i=-edgeCells+1;i<=edgeCells-1;i++)
  {
  	for (int j=0;j<(rows-abs(i));j++)
  	{

  		G4double rowOffset_x=(sqrt(3.0)/2.0*hexRadius*abs(i));
		G4double rowOffset_z=(3.0/2.0*hexRadius*i);
  		sprintf(volumeName, "tracker_%d", index);
  		tracker_phys = new G4PVPlacement(rm,
             	G4ThreeVector(trackerPos_x+rowOffset_x+(sqrt(3)*hexRadius*(j-edgeCells+1)),trackerPos_y, trackerPos_z+rowOffset_z),
             	tracker_log,volumeName,experimentalHall_log,false,index);
             	index++;
  	}  	
  }  
  //------------------------------------------------------------------

  G4double maxStep = 2.0*mm;
  //fStepLimit = new G4UserLimits(maxStep);
  //tracker_log->SetUserLimits(fStepLimit);
  
  return experimentalHall_phys;
}

void NeutrinoDetectorConstruction::SetMaxStep(G4double maxStep)
{
  if ((fStepLimit)&&(maxStep>0.)) fStepLimit->SetMaxAllowedStep(maxStep);
}

