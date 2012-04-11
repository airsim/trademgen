// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
// StdAir
#include <stdair/basic/BasFileMgr.hpp>
#include <stdair/basic/RandomGeneration.hpp>
#include <stdair/bom/Inventory.hpp>
// TraDemGen
#include <trademgen/command/DemandParserHelper.hpp>
#include <trademgen/command/DemandParser.hpp>

namespace TRADEMGEN {

  // //////////////////////////////////////////////////////////////////////
  void DemandParser::
  generateDemand (const DemandFilePath& iDemandFilename,
                  SEVMGR::SEVMGR_ServicePtr_T ioSEVMGR_ServicePtr,
                  stdair::RandomGeneration& ioSharedGenerator,
                  const POSProbabilityMass_T& iDefaultPOSProbablityMass) {

    const stdair::Filename_T lFilename = iDemandFilename.name();

    // Check that the file path given as input corresponds to an actual file
    const bool doesExistAndIsReadable =
      stdair::BasFileMgr::doesExistAndIsReadable (lFilename);
    if (doesExistAndIsReadable == false) {
      STDAIR_LOG_ERROR ("The demand input file '" << lFilename
                        << "' does not exist or can not be read");
      
      throw DemandInputFileNotFoundException ("The demand file '" + lFilename
                                              + "' does not exist or can not "
                                              "be read");
    }

    // Initialise the demand file parser.
    DemandFileParser lDemandParser (ioSEVMGR_ServicePtr, ioSharedGenerator,
                                    iDefaultPOSProbablityMass, lFilename);

    // Parse the CSV-formatted demand input file, and generate the
    // corresponding DemandCharacteristic objects.
    lDemandParser.generateDemand();
  }

}
