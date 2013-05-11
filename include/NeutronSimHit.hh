#ifndef NeutronSimHit_h
#define NeutronSimHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "G4LogicalVolume.hh"
#include "G4Transform3D.hh"
#include "G4RotationMatrix.hh"

class G4AttDef;

class NeutronSimHit : public G4VHit
{
  public:

      NeutronSimHit();
      NeutronSimHit(G4LogicalVolume* logVol,G4ThreeVector s_pos);
      ~NeutronSimHit();
      NeutronSimHit(const NeutronSimHit &right);
      const NeutronSimHit& operator=(const NeutronSimHit &right);
      G4int operator==(const NeutronSimHit &right) const;

      inline void *operator new(size_t);
      inline void operator delete(void *aHit);

      void Draw();
      const std::map<G4String,G4AttDef>* GetAttDefs() const;
      std::vector<G4AttValue>* CreateAttValues() const;
      void Print();

  private:
      G4double edep;
      G4ThreeVector pos;
      G4RotationMatrix rot;
      const G4LogicalVolume* pLogV;
      static std::map<G4String,G4AttDef> fAttDefs;

  public:
      inline void SetEdep(G4double de)
      { edep = de; }
      inline void AddEdep(G4double de)
      { edep += de; }
      inline G4double GetEdep()
      { return edep; }
      inline void SetPos(G4ThreeVector xyz)
      { pos = xyz; }
      inline G4ThreeVector GetPos()
      { return pos; }
      inline void SetRot(G4RotationMatrix rmat)
      { rot = rmat; }
      inline G4RotationMatrix GetRot()
      { return rot; }
      inline const G4LogicalVolume * GetLogV()
      { return pLogV; }

};

typedef G4THitsCollection<NeutronSimHit> NeutronSimHitsCollection;

extern G4Allocator<NeutronSimHit> NeutronSimHitAllocator;

inline void* NeutronSimHit::operator new(size_t)
{
  void *aHit;
  aHit = (void *) NeutronSimHitAllocator.MallocSingle();
  return aHit;
}

inline void NeutronSimHit::operator delete(void *aHit)
{
  NeutronSimHitAllocator.FreeSingle((NeutronSimHit*) aHit);
}

#endif


