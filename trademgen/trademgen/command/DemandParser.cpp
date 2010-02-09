// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <string>
// StdAir
#include <stdair/bom/Inventory.hpp>
#include <stdair/bom/Network.hpp>
// Trademgen
#include <trademgen/command/DemandParserHelper.hpp>
#include <trademgen/command/DemandParser.hpp>
//#include <trademgen/command/DemandGenerator.hpp>

namespace TRADEMGEN {

  // //////////////////////////////////////////////////////////////////////
  void DemandParser::generateDemand (const stdair::Filename_T& iFilename,
                                     stdair::BomRoot& ioBomRoot) {

    // Initialise the demand file parser.
    DemandFileParser lDemandParser (ioBomRoot, iFilename);

    // Parse the CSV-formatted demand input file, and generate the
    // corresponding DemandCharacteristic objects.
    // lDemandParser.generateDemand ();
  }

}
