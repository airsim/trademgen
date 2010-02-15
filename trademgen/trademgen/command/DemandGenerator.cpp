// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
// StdAir
#include <stdair/STDAIR_Types.hpp>
#include <stdair/basic/BasConst_BookingClass.hpp>
#include <stdair/basic/BasConst_Yield.hpp>
#include <stdair/basic/DemandCharacteristics.hpp>
#include <stdair/basic/DemandDistribution.hpp>
#include <stdair/bom/BomRoot.hpp>
#include <stdair/factory/FacBomContent.hpp>
#include <stdair/command/CmdBomManager.hpp>
#include <stdair/service/Logger.hpp>
// TraDemGen
#include <trademgen/bom/DemandStruct.hpp>
#include <trademgen/command/DemandManager.hpp>
#include <trademgen/command/DemandGenerator.hpp>

namespace TRADEMGEN {

  // //////////////////////////////////////////////////////////////////////
  void DemandGenerator::
  createDemandCharacteristics (stdair::BomRoot& ioBomRoot,
                               const DemandStruct_T& iDemand) {

    const stdair::DemandStreamKey_T lDemandStreamKey (iDemand._origin,
                                                      iDemand._destination,
                                                      iDemand._prefDepDate,
                                                      iDemand._prefCabin);

    stdair::DemandCharacteristics lDemandCharacteristics (lDemandStreamKey);

    const stdair::DemandDistribution lDemandDistribution (iDemand._demandMean,
                                                          iDemand._demandStdDev);

    // Arrival pattern
    stdair::ArrivalPatternCumulativeDistribution_T arrivalPatternCumulativeDistribution;
    
    for (DTDProbDist_T::const_iterator it = iDemand._dtdProbDist.begin();
         it != iDemand._dtdProbDist.end(); ++it) {
      const stdair::DayDuration_T& lDTD = it->first;
      const DTDProbMass_T& lDTDProbMass = it->second;

      const stdair::FloatDuration_T lZeroDTDFloat = 0.0;
      stdair::FloatDuration_T lDTDFloat =
        static_cast<stdair::FloatDuration_T> (lDTD);
      lDTDFloat = lZeroDTDFloat - lDTD;

      arrivalPatternCumulativeDistribution.
        insert (stdair::ArrivalPatternCumulativeDistribution_T::
                value_type (lDTDFloat, lDTDProbMass));
    }
    
    const stdair::ContinuousFloatDuration_T lArrivalPattern (arrivalPatternCumulativeDistribution);
    lDemandCharacteristics.setArrivalPattern (lArrivalPattern);

    // Display
    STDAIR_LOG_DEBUG ("Demand: " << std::endl
                      << lDemandCharacteristics.display()
                      << lDemandDistribution.display());

    // Seed
    stdair::RandomSeed_T lNumberOfRequestsSeed = 2;
    stdair::RandomSeed_T lRequestDateTimeSeed = 2;
    stdair::RandomSeed_T lDemandCharacteristicsSeed = 2;
  
    // Delegate the call to the dedicated command
    DemandManager::addDemandStream (ioBomRoot, lDemandStreamKey,
                                    lDemandCharacteristics, lDemandDistribution,
                                    lNumberOfRequestsSeed, lRequestDateTimeSeed,
                                    lDemandCharacteristicsSeed);
  }
    
}
