//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
/// \file exampleB1.cc
/// \brief Main program of the B1 example

#include "ActionInitialization.hh"
#include "DetectorConstruction.hh"
#include "QBBC.hh"
#include "RunAction.hh"

#include "G4RunManagerFactory.hh"
#include "G4SteppingVerbose.hh"
#include "G4UIExecutive.hh"
#include "G4UImanager.hh"
#include "G4VisExecutive.hh"
#include "PrimaryGeneratorAction.hh"
#include "G4OpticalPhysics.hh"
#include "G4EmStandardPhysics_option4.hh"
#include "FTFP_BERT.hh"

#include "PrimitivePhotonCounter.hh"


// #include "Randomize.hh"

using namespace B1;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int main(int argc, char** argv)
{
  // Detect interactive mode (if no arguments) and define UI session
  //
  G4UIExecutive* ui = nullptr;
  if (argc == 1) {
    ui = new G4UIExecutive(argc, argv);
  }

  // Optionally: choose a different Random engine...
  // G4Random::setTheEngine(new CLHEP::MTwistEngine);

  // use G4SteppingVerboseWithUnits
  G4int precision = 4;
  G4SteppingVerbose::UseBestUnit(precision);

  // Construct the default run manager
  //
  auto runManager = new G4RunManager;

  // Set mandatory initialization classes
  //

  // Physics list
  auto physicsList = new QBBC;
  physicsList->ReplacePhysics(new G4EmStandardPhysics_option4());
  G4OpticalPhysics* opticalPhysics = new G4OpticalPhysics();
  physicsList->RegisterPhysics(opticalPhysics);
  physicsList->SetVerboseLevel(1);
  runManager->SetUserInitialization(physicsList);

  // User action initialization
   runManager->SetUserInitialization(new B1::DetectorConstruction);


  // Initialize visualization with the default graphics system
  auto visManager = new G4VisExecutive(argc, argv);

  // Constructors can also take optional arguments:
  // - a graphics system of choice, eg. "OGL"
  // - and a verbosity argument - see /vis/verbose guidance.
  // auto visManager = new G4VisExecutive(argc, argv, "OGL", "Quiet");
  // auto visManager = new G4VisExecutive("Quiet");


  // Get the pointer to the User Interface manager
  auto UImanager = G4UImanager::GetUIpointer();


  // Process macro or start UI session
  //



  std::vector<double> z_pos = {-0.9, -0.8, -0.7, -0.6, -0.5, -0.4, -0.3, -0.2, -0.1, 0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9};

  ActionInitialization* ai = new ActionInitialization();
  PrimaryGeneratorAction* PGA = new PrimaryGeneratorAction();
  ai->NewGeneratorAction(PGA);
  RunAction* RA = new RunAction("/Users/semenraydun/Desktop/GEANT4/examples/basic/B1/build/positronsZ.csv");
  ai->NewRunAction(RA);
  runManager->SetUserInitialization(ai);
  runManager->Initialize();
  //visManager->Initialize();

  // if (!ui) {
  //   // batch mode
  //   G4String command = "/control/execute ";
  //   G4String fileName = argv[1];
  //   UImanager->ApplyCommand(command + fileName);
  // }
  // else {
  //   // interactive mode
  //   UImanager->ApplyCommand("/control/execute /Users/semenraydun/Desktop/GEANT4/examples/basic/B1/build/init_vis.mac");
  //   ui->SessionStart();
  //   delete ui;
  // }


  for (const double& z : z_pos){
    std::cout << "z = " << z << std::endl;
    PGA->SetPosition(G4ThreeVector(0, 0, z));
    RA->SetHistName(std::to_string(z));
    runManager->GeometryHasBeenModified();
    runManager->BeamOn(1000);
  }





  delete visManager;
  delete runManager;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....
