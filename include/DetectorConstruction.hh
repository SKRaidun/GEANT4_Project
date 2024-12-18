
#ifndef B1DetectorConstruction_h
#define B1DetectorConstruction_h 1


#include "G4VUserDetectorConstruction.hh"
#include "G4ThreeVector.hh"

class G4VPhysicalVolume;
class G4LogicalVolume;

namespace B1
{

class DetectorConstruction : public G4VUserDetectorConstruction
{
  public:
    DetectorConstruction() = default;
    ~DetectorConstruction() override = default;
 

    G4VPhysicalVolume* Construct() override;

    G4LogicalVolume* GetScoringVolume() const;

    std::vector<G4ThreeVector> CalculateDodecahedronVertices(G4double radius);
    std::vector<G4ThreeVector> CalculateIcosahedronVertices(G4double radius);

    void ConstructSDandField() override;

  protected:
    G4LogicalVolume* fScoringVolume;
};

}  

#endif
