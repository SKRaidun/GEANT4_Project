#ifndef PrimitivePhotonCounter_h
#define PrimitivePhotonCounter_h

#include "G4VSensitiveDetector.hh"

namespace B1 {

class PrimitivePhotonCounter : public G4VSensitiveDetector {
public:
    PrimitivePhotonCounter(const G4String& name);
    ~PrimitivePhotonCounter() = default;

    void Initialize(G4HCofThisEvent*) override;
    void EndOfEvent(G4HCofThisEvent*) override;

    G4bool ProcessHits(G4Step*, G4TouchableHistory*) override;

    private:
  void ResetCounter();
  G4int GetCounter();

  G4int count;
};
}
#endif
