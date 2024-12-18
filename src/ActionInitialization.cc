// ActionInitialization.cc
#include "ActionInitialization.hh"
#include "PrimaryGeneratorAction.hh"
#include "RunAction.hh"
#include "StackingAction.hh"


namespace B1
{

void ActionInitialization::NewGeneratorAction(PrimaryGeneratorAction* PGA) {newPrimaryGeneratorAction = PGA;}
void ActionInitialization::NewRunAction(RunAction* RA) { newRunAction = RA;}


void ActionInitialization::Build() const
{
  if(newPrimaryGeneratorAction){
    SetUserAction(newPrimaryGeneratorAction);
  }
  if(newRunAction){
    SetUserAction(newRunAction);
  }
}

}
