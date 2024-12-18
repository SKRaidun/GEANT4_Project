

#ifndef B1RunAction_h
#define B1RunAction_h 1

#include "G4UserRunAction.hh"
#include "G4Accumulable.hh"
#include "globals.hh"

class G4Run;

namespace B1
{


class RunAction : public G4UserRunAction
{
  public:
    RunAction(const std::string& fileName);
    ~RunAction();

    void BeginOfRunAction(const G4Run*) override;
    void   EndOfRunAction(const G4Run*) override;
    void SetHistName(const std::string& name){ histName = name; }

  private:
    std::string histName;

};

}

#endif

