

#include "RunAction.hh"
#include "PrimaryGeneratorAction.hh"
#include "DetectorConstruction.hh"
// #include "Run.hh"

#include "G4RunManager.hh"
#include "G4Run.hh"
#include "G4AccumulableManager.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4AnalysisManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4Track.hh"

namespace B1
{

RunAction::RunAction(const std::string& fileName)
{

  G4AnalysisManager *manager = G4AnalysisManager::Instance();

  manager->OpenFile(fileName.c_str());
}

void RunAction::BeginOfRunAction(const G4Run*)
{

  G4AnalysisManager *manager = G4AnalysisManager::Instance();
   manager->CreateNtuple(histName.c_str(), histName.c_str());
    manager->CreateNtupleDColumn(histName.c_str());
    manager->FinishNtuple();

}

void RunAction::EndOfRunAction(const G4Run* run)
{
  G4AnalysisManager *manager = G4AnalysisManager::Instance();
  G4int nofEvents = run->GetNumberOfEvent();
  if (nofEvents == 0) return;
}

RunAction::~RunAction(){
    G4AnalysisManager *manager = G4AnalysisManager::Instance();
    manager->Write();
    manager->CloseFile();

}

}