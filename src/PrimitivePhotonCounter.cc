#include "PrimitivePhotonCounter.hh"
#include "G4Event.hh"
#include "G4Step.hh"
#include "G4TouchableHistory.hh"

#include "Randomize.hh"
#include "G4AnalysisManager.hh"
#include "G4RunManager.hh"
#include "G4RunManager.hh"

namespace B1 {

PrimitivePhotonCounter::PrimitivePhotonCounter(const G4String& name)
    : count(0), G4VSensitiveDetector(name) {std::cout << "OLOLO" << std::endl;}


void PrimitivePhotonCounter::ResetCounter(){
  count = 0;
}

G4int PrimitivePhotonCounter::GetCounter(){
  return count;
}


void PrimitivePhotonCounter::Initialize(G4HCofThisEvent*) {
    ResetCounter();
}

G4bool PrimitivePhotonCounter::ProcessHits(G4Step* step, G4TouchableHistory*) {
    G4Track *track = step->GetTrack();

    
  if(track->GetParticleDefinition()->GetParticleName() == "opticalphoton"){
    if (G4UniformRand() < 0.28) ++count;
  }
  track->SetTrackStatus(fStopAndKill);
  return true;
}

void PrimitivePhotonCounter::EndOfEvent(G4HCofThisEvent*){
  auto *manager = G4AnalysisManager::Instance();
  auto EvtId =  G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();

  
  G4int count = GetCounter();
  auto lastNtupleId = G4AnalysisManager::Instance()->GetNofNtuples() - 1;
  if(lastNtupleId == -1){
    return;
  }
  manager->FillNtupleDColumn(lastNtupleId, 0, count);
  manager->AddNtupleRow(lastNtupleId);
}

}
