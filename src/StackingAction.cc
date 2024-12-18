#include "G4SystemOfUnits.hh"
#include "G4Track.hh"
#include "G4AnalysisManager.hh"
#include "G4RunManager.hh"

#include "StackingAction.hh"


namespace B1
{

G4ClassificationOfNewTrack StackingAction::ClassifyNewTrack(const G4Track* aTrack){


  if(aTrack->GetParticleDefinition()->GetParticleName() == "gamma"){
    energy += aTrack->GetTotalEnergy() / MeV;
    return G4ClassificationOfNewTrack::fKill;
  } else {
    return G4ClassificationOfNewTrack::fUrgent;
  }
}

void StackingAction::PrepareNewEvent(){
  auto *manager = G4AnalysisManager::Instance();
  auto lastNtupleId = manager->GetNofNtuples() - 1;
  manager->FillNtupleDColumn(lastNtupleId, 0, energy);
  manager->AddNtupleRow(lastNtupleId);
  auto EvtId =  G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();
  std::cout << "EvtId: " << EvtId << " -> gammas totoal E = " << energy / MeV << std::endl;
  energy = 0;
}


}
