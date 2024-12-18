#include "DetectorConstruction.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4NistManager.hh"
#include "G4Sphere.hh"
#include "G4Tubs.hh"
#include "G4Box.hh"
#include "G4Material.hh"
#include "G4SystemOfUnits.hh"
#include "G4VisAttributes.hh"
#include <cmath>
#include <vector>
#include "PrimitivePhotonCounter.hh"
#include "G4SubtractionSolid.hh"
#include "G4UnionSolid.hh"
#include "CLHEP/Vector/ThreeVector.h" 
#include "G4LogicalVolumeStore.hh"
#include "G4SDManager.hh"
#include "ActionInitialization.hh"

namespace B1 {

std::vector<G4ThreeVector> DetectorConstruction::CalculateDodecahedronVertices(G4double radius) {
    static const G4double phi = (1.0 + std::sqrt(5.0)) / 2.0;

    std::vector<G4ThreeVector> vertices = {
        {-1, phi, 0}, {1, phi, 0}, {-1, -phi, 0}, {1, -phi, 0},
        {0, -1, phi}, {0, 1, phi}, {0, -1, -phi}, {0, 1, -phi},
        {phi, 0, -1}, {phi, 0, 1}, {-phi, 0, -1}, {-phi, 0, 1}
    };

    for (auto& vertex : vertices) {
        vertex *= (radius / std::sqrt(1 + phi * phi));
    }

    return vertices;
}

std::vector<G4ThreeVector> DetectorConstruction::CalculateIcosahedronVertices(G4double radius) {
    static const G4double phi = (1.0 + std::sqrt(5.0)) / 2.0;

    std::vector<G4ThreeVector> vertices = {
        {0, 1, phi}, {0, 1, -phi}, {0, -1, phi}, {0, -1, -phi},
        {1, phi, 0}, {1, -phi, 0}, {-1, phi, 0}, {-1, -phi, 0},
        {phi, 0, 1}, {-phi, 0, 1}, {phi, 0, -1}, {-phi, 0, -1}
    };

    for (auto& vertex : vertices) {
        vertex *= (radius / std::sqrt(1 + phi * phi));
    }

    return vertices;
}

G4LogicalVolume* DetectorConstruction::GetScoringVolume() const {
    return fScoringVolume;
}

void DetectorConstruction::ConstructSDandField() {

        bool isExists = false;
        if (!isExists) {
        PrimitivePhotonCounter* SD = new PrimitivePhotonCounter("PrimitivePhotonCounter");

        G4LogicalVolume* logicalVolume = G4LogicalVolumeStore::GetInstance()->GetVolume("Photocathode");
        if (logicalVolume) {
            G4SDManager::GetSDMpointer()->AddNewDetector(SD);
            logicalVolume->SetSensitiveDetector(SD);
        } else {
            G4cerr << "Logical volume 'PMTPhotocatod' not found!" << G4endl;
        }

        if (logicalVolume->GetSensitiveDetector()) {
    G4cout << "Sensitive detector successfully set for 'PMTPhotocatod'" << G4endl;
} else {
    G4cerr << "Failed to set sensitive detector for 'PMTPhotocatod'" << G4endl;
}
        }
        isExists = true;

}

G4VPhysicalVolume* DetectorConstruction::Construct() {
    G4NistManager* nist = G4NistManager::Instance();

    G4Material* PMMA = nist->FindOrBuildMaterial("G4_PLEXIGLASS"); 
    G4Material* steel = nist->FindOrBuildMaterial("G4_STAINLESS-STEEL"); 
    G4Material* glass = nist->FindOrBuildMaterial("G4_GLASS"); 

    std::vector<G4double> photonEnergy = { 5.1 * keV, 5.2 * keV, 5.3 * keV};;

    G4Material* fLAB = new G4Material("LAB", 0.86 * g / cm3, 2);
    fLAB->AddElement(nist->FindOrBuildElement("C", false), 10); 
    fLAB->AddElement(nist->FindOrBuildElement("H", false), 22); 

    std::vector<G4double> LAB_RIND = { 1.7, 1.75, 1.8 };
    std::vector<G4double> LAB_ABSL = { 12.0 * m, 12.0 * m, 12.0 * m };

    G4MaterialPropertiesTable* LAB_mt = new G4MaterialPropertiesTable();
    LAB_mt->AddProperty("RINDEX", photonEnergy, LAB_RIND);
    LAB_mt->AddProperty("ABSLENGTH", photonEnergy, LAB_ABSL);
    fLAB->SetMaterialPropertiesTable(LAB_mt);


    G4Material* PPO = new G4Material("PPO", 1.02 * g/cm3, 4); 
    PPO->AddElement(nist->FindOrBuildElement("C", false), 15);
    PPO->AddElement(nist->FindOrBuildElement("H", false), 11);
    PPO->AddElement(nist->FindOrBuildElement("N", false), 1);
    PPO->AddElement(nist->FindOrBuildElement("O", false), 1); 
    
    G4Material* gadolinium = new G4Material("Gadolinium", 7.90 * g/cm3, 1); 
    gadolinium->AddElement(nist->FindOrBuildElement("Gd", false), 1);

    G4Material* fScintillator = new G4Material("Scintillator", 0.86 * g / cm3, 3);
    G4double ppoFraction = 3.0 / (860.0 + 3.0 + 0.5);
    G4double gadoliniumFraction = 0.5 / (860.0 + 3.0 + 0.5);
    fScintillator->AddMaterial(fLAB, 1.0 - ppoFraction - gadoliniumFraction); 
    fScintillator->AddMaterial(PPO, ppoFraction);
    fScintillator->AddMaterial(gadolinium, gadoliniumFraction);

    G4MaterialPropertiesTable* Scintillator_mt = new G4MaterialPropertiesTable();

    std::vector<G4double> Scintillator_SCINT = { 0.1, 0.8, 0.99 };
    std::vector<G4double> Scintillator_RIND  = { 1.7, 1.75, 1.8 };
    std::vector<G4double> Scintillator_ABSL  = { 5.0 * m, 5.0 * m, 5.0 * m };
    Scintillator_mt->AddProperty("SCINTILLATIONCOMPONENT1", photonEnergy, Scintillator_SCINT);
    Scintillator_mt->AddProperty("RINDEX", photonEnergy, Scintillator_RIND);
    Scintillator_mt->AddProperty("ABSLENGTH", photonEnergy, Scintillator_ABSL);
    Scintillator_mt->AddConstProperty("SCINTILLATIONYIELD", 5000. / MeV);
    Scintillator_mt->AddConstProperty("RESOLUTIONSCALE", 1.0);
    Scintillator_mt->AddConstProperty("SCINTILLATIONTIMECONSTANT1", 0.0 * ns);

    fScintillator->SetMaterialPropertiesTable(Scintillator_mt);

    // Stainless Steel
    G4Material* fStainlessSteel = new G4Material("StainlessSteel", 7.92*g/cm3, 2);  
    fStainlessSteel->AddElement(nist->FindOrBuildElement("Fe", false), 90. * perCent);   
    fStainlessSteel->AddElement(nist->FindOrBuildElement("Cr", false), 10 * perCent);  


    // PMMA (Plexiglass) properties
    G4Material* fPMMA = new G4Material("PMMA", 1.19*g/cm3, 3);
    fPMMA->AddElement(nist->FindOrBuildElement("C", false), 5);
    fPMMA->AddElement(nist->FindOrBuildElement("O", false), 2);
    fPMMA->AddElement(nist->FindOrBuildElement("H", false), 8);
    std::vector<G4double> PMMA_RIND = { 1.51, 1.52, 1.53 };
    std::vector<G4double> PMMA_ABSL = { 9.0 * m, 9.0 * m, 9.0 * m };
    G4MaterialPropertiesTable* PMMA_mt = new G4MaterialPropertiesTable();
    PMMA_mt->AddProperty("RINDEX", photonEnergy, PMMA_RIND);
    PMMA_mt->AddProperty("ABSLENGTH", photonEnergy, PMMA_ABSL);
    fPMMA->SetMaterialPropertiesTable(PMMA_mt);

    G4Box* worldBox = new G4Box("World", 2 * m, 2 * m, 2 * m);
    G4LogicalVolume* worldLog = new G4LogicalVolume(worldBox, nist->FindOrBuildMaterial("G4_AIR"), "World");
    G4VPhysicalVolume* worldPhys = new G4PVPlacement(nullptr, {}, worldLog, "World", nullptr, false, 0);

    G4Sphere* PMMASphere = new G4Sphere("PMMASphereOuter", 0, 0.6253505 * m, 0, 360 * deg, 0, 180 * deg); 
    G4LogicalVolume* PMMALog = new G4LogicalVolume(PMMASphere, fPMMA, "PMMASphere");
    new G4PVPlacement(nullptr, G4ThreeVector(0, 0, 0), PMMALog, "PMMASphere", worldLog, false, 0);

    G4Sphere* scintillatorSphere = new G4Sphere("Scintillator", 0, 0.6203505 * m, 0, 360 * deg, 0, 180 * deg);
    G4LogicalVolume* scintillatorLog = new G4LogicalVolume(scintillatorSphere, fScintillator, "Scintillator");
    new G4PVPlacement(nullptr, G4ThreeVector(0, 0, 0), scintillatorLog, "Scintillator", PMMALog, false, 0);

    G4double TankH = 1858.0 * mm;
    G4double tankR = 1858.0 * mm / 2;
    auto solidTank = new G4Tubs("Tank", 0.0, tankR, TankH / 2.0, 0.0, 360 * deg);
    auto logicTank = new G4LogicalVolume(solidTank, fStainlessSteel, "Tank"); 
    auto physTank = new G4PVPlacement(nullptr, G4ThreeVector(), logicTank, "Tank", worldLog, false, 0);  

    G4double LABH = 1858.0 * mm - 2 * mm;
    G4double LABR = 929 * mm - 2 * mm;
    auto solidLAB = new G4Tubs("LAB", 0.0, LABR, LABH / 2.0, 0.0, 360 * deg);
    auto logicLAB = new G4LogicalVolume(solidLAB, fLAB, "LAB"); 
    auto physLAB = new G4PVPlacement(nullptr, G4ThreeVector(), logicLAB, "LAB", logicTank, false, 0);



    G4double pmtWindowRadius = 101 * mm;
    G4double pmtTubeRadius1 = 42 * mm;
    G4double pmtTubeHeight1 = 101 * mm;
    G4double pmtTubeRadius2 = 26 * mm;
    G4double pmtTubeHeight2 = 55 * mm;
    G4double pmtPhotocatodRadius = 95 * mm;

    G4Material* pmtTubeMaterial = nist->FindOrBuildMaterial("G4_STAINLESS-STEEL");
    G4Material* pmtWindowMaterial = PMMA; 

    if (!pmtWindowMaterial) {
        G4cerr << "Error: PMT window material is not found!" << G4endl;
        return nullptr;
    }

    G4Element* potassium = nist->FindOrBuildElement("K"); // Калий
    G4Element* antimony = nist->FindOrBuildElement("Sb"); // Висмут
    G4Element* cesium = nist->FindOrBuildElement("Cs"); // Цезий

    G4Material* photocathodeMaterial = new G4Material("Photocathode", 0.86 * g / cm3, 2); 
    photocathodeMaterial->AddElement(nist->FindOrBuildElement("C", false), 10); 
    photocathodeMaterial->AddElement(nist->FindOrBuildElement("H", false), 22); 

    

    G4Sphere* pmtPhotocatod = new G4Sphere("Photocathode", 0, pmtPhotocatodRadius, 0 , 360 * deg, 0, 90 * deg );
    G4LogicalVolume* pmtPhotocatodLog = new G4LogicalVolume(pmtPhotocatod, fLAB, "Photocathode");
    pmtPhotocatodLog->SetVisAttributes(new G4VisAttributes(G4Colour(0.45,0.25,0.0)));
    // Создаем верхний цилиндр
    G4Tubs* pmtBodyUpper = new G4Tubs("PMTBodyUpper", 0, pmtTubeRadius1, pmtTubeHeight1, 0, 360 * deg);
    G4LogicalVolume* pmtBodyUpperLog = new G4LogicalVolume(pmtBodyUpper, pmtTubeMaterial, "PMTBodyUpper");

    // Создаем нижний цилиндр
    G4Tubs* pmtBodyLower = new G4Tubs("PMTBodyLower", 0, pmtTubeRadius2, pmtTubeHeight2 / 2, 0, 360 * deg);
    G4LogicalVolume* pmtBodyLowerLog = new G4LogicalVolume(pmtBodyLower, pmtTubeMaterial, "PMTBodyLower");

    // Размещаем верхний цилиндр, прилегающий к окну
    G4ThreeVector upperOffset(0, 0, -pmtWindowRadius); // Смещение по оси Z
    new G4PVPlacement(nullptr, upperOffset, pmtBodyUpperLog, "PMTBodyUpper", pmtPhotocatodLog, false, 0);

    // Размещаем нижний цилиндр, прилегающий к верхнему цилиндру
    G4ThreeVector lowerOffset(0, 0, -(pmtTubeHeight1 / 2 + pmtTubeHeight2 / 2)); // Смещение по оси Z
    new G4PVPlacement(nullptr, lowerOffset, pmtBodyLowerLog, "PMTBodyLower", pmtBodyUpperLog, false, 1);

    
    fScoringVolume = new G4LogicalVolume(worldBox, nist->FindOrBuildMaterial("G4_AIR"), "ScoringVolume");


    ConstructSDandField();

    G4double radius = 0.73 * m;

    auto vertices = CalculateDodecahedronVertices(radius);

for (size_t i = 0; i < vertices.size(); ++i) {
    G4ThreeVector direction = -vertices[i].unit();  

    G4ThreeVector zAxis(0, 0, 1);

    G4double angle = std::acos(direction.dot(zAxis));

    G4ThreeVector axis= direction.cross(zAxis);
    axis = axis.unit();

    G4RotationMatrix* rotatePMT = new G4RotationMatrix();

        rotatePMT->rotate(angle, axis); 



    new G4PVPlacement(rotatePMT, vertices[i], pmtPhotocatodLog, "Photocathode", logicLAB, false, i);
}

G4cout << "PMTs placed successfully!" << G4endl;



    return worldPhys;
}
}



   
