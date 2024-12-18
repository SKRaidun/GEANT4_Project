#ifndef B1ActionInitialization_h
#define B1ActionInitialization_h 1

#include "G4VUserActionInitialization.hh"
#include "PrimaryGeneratorAction.hh"
#include "RunAction.hh"

namespace B1
{


class ActionInitialization : public G4VUserActionInitialization
{
  public:
    ActionInitialization() = default;
    ~ActionInitialization() override = default;

    void NewGeneratorAction(PrimaryGeneratorAction* PGA);
    void NewRunAction(RunAction* RA);

    void Build() const override;
  private:
    PrimaryGeneratorAction* newPrimaryGeneratorAction;
    RunAction* newRunAction;
};

}

#endif
