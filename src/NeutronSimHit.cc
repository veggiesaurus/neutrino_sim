#include "NeutronSimHit.hh"
#include "G4Color.hh"
#include "G4Circle.hh"
#include "G4ThreeVector.hh"
#include "G4ios.hh"
#include "G4VVisManager.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"
#include "G4LogicalVolume.hh"
#include "G4UIcommand.hh"
#include "G4UnitsTable.hh"
#include "G4AttValue.hh"
#include "G4AttDef.hh"
#include "G4AttCheck.hh"

G4Allocator<NeutronSimHit> NeutronSimHitAllocator;

NeutronSimHit::NeutronSimHit()
{pLogV=0;}

NeutronSimHit::NeutronSimHit(G4LogicalVolume* logVol,G4ThreeVector s_pos)
: pos(s_pos), pLogV(logVol)
{;}

NeutronSimHit::~NeutronSimHit()
{;}

NeutronSimHit::NeutronSimHit(const NeutronSimHit &right)
  : G4VHit()
{
  edep = right.edep;
  pos = right.pos;
  rot = right.rot;
  pLogV = right.pLogV;
}

const NeutronSimHit& NeutronSimHit::operator=(const NeutronSimHit &right)
{
  edep = right.edep;
  pos = right.pos;
  rot = right.rot;
  pLogV = right.pLogV;
  return *this;
}

G4int NeutronSimHit::operator==(const NeutronSimHit &right) const
{
  return (pos==right.pos && edep==right.edep);
}

std::map<G4String,G4AttDef> NeutronSimHit::fAttDefs;

void NeutronSimHit::Draw()
{
  G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();
  if(pVVisManager)
  {
	   G4Circle circle(pos); // Instantiate a circle with its 3D
                            // position. The argument "position"
                            // is defined as G4Point3D instance
 circle.SetScreenDiameter (1.0); // Should be circle.SetScreenDiameter
                                 //  (1.0 * pixels) - to be implemented
 circle.SetFillStyle (G4Circle::filled); // Make it a filled circle
 G4Colour colour(1.,0.,0.);              // Define red color
 G4VisAttributes attribs(colour);        // Define a red visualization attribute
 circle.SetVisAttributes(attribs);       // Assign the red attribute to the circle
		pVVisManager->Draw(circle);
    ;
  }
  G4cout<<"Marker"<<G4endl<<G4endl;
}

const std::map<G4String,G4AttDef>* NeutronSimHit::GetAttDefs() const
{
  // G4AttDefs have to have long life.  Use static member...
  if (fAttDefs.empty()) {
    fAttDefs["HitType"] =
      G4AttDef("HitType","Type of hit","Physics","","G4String");
    fAttDefs["EDep"] =
      G4AttDef("EDep","Energy deposited","Physics","G4BestUnit","G4double");
  }
  return &fAttDefs;
}

std::vector<G4AttValue>* NeutronSimHit::CreateAttValues() const
{
  // Create expendable G4AttsValues for picking...
  std::vector<G4AttValue>* attValues = new std::vector<G4AttValue>;
  attValues->push_back
    (G4AttValue("HitType","NeutronSimHit",""));
  attValues->push_back
    (G4AttValue("EDep",G4BestUnit(edep,"Energy"),""));
  //G4cout << "Checking...\n" << G4AttCheck(attValues, GetAttDefs());
  return attValues;
}

void NeutronSimHit::Print()
{;}
