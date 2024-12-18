#ifndef B1PrimaryGeneratorAction_h
#define B1PrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"
#include "globals.hh"

class G4ParticleGun;
class G4Event;

namespace B1
{



class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
  public:
    PrimaryGeneratorAction();
    ~PrimaryGeneratorAction() override;

    // method from the base class
    void GeneratePrimaries(G4Event*) override;

    G4ThreeVector RandomDirection();

    G4ThreeVector AlongParticlePosition();

    // method to access particle gun
    const G4ParticleGun* GetParticleGun() const { return fParticleGun; }
    void SetPosition(const G4ThreeVector& position) { primaryPosition = position; }

  private:
    G4ParticleGun* fParticleGun = nullptr;
    G4ThreeVector primaryPosition;
};

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
