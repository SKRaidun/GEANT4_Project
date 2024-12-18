
#ifndef B1StackingAction_h
#define B1StackingAction_h

#include "globals.hh"
#include "G4UserStackingAction.hh"


namespace B1
{

class StackingAction : public G4UserStackingAction
{
  public:
  StackingAction() = default;
  virtual ~StackingAction() override = default;
  virtual void PrepareNewEvent() override;
  virtual G4ClassificationOfNewTrack ClassifyNewTrack(const G4Track* aTrack) override;
  private:
  double energy{0.0};
};
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
