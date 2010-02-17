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
    // Arrival pattern
    const stdair::ContinuousFloatDuration_T lArrivalPattern =
      iDemand.getArrivalPattern ();
    // POS probability mass.
    const stdair::POSProbabilityMass_T lPOSProbMass =
      iDemand.getPOSProbabilityMass ();
    // Channel probability mass.
    const stdair::ChannelProbabilityMass_T lChannelProbMass =
      iDemand.getChannelProbabilityMass ();
    // Trip type probability mass.
    const stdair::TripTypeProbabilityMass_T lTripTypeProbMass =
      iDemand.getTripTypeProbabilityMass ();
    // Stay duration probability mass.
    const stdair::StayDurationProbabilityMass_T lStayDurationProbMass =
      iDemand.getStayDurationProbabilityMass ();
    // Frequent flyer probability mass.
    const stdair::FrequentFlyerProbabilityMass_T lFrequentFlyerProbMass =
      iDemand.getFrequentFlyerProbabilityMass ();
    // Preferred departure time cumulative distribution.
    const stdair::PreferredDepartureTimeCumulativeDistribution_T lPreferredDepartureTimeCumulativeDistribution =
      iDemand.getPreferredDepartureTimeCumulativeDistribution ();
    // WTP cumulative distribution.
    const stdair::WTPCumulativeDistribution_T lWTPCumulativeDistribution =
      iDemand.getWTPCumulativeDistribution ();
    // Value of time cumulative distribution.
    const stdair::ValueOfTimeCumulativeDistribution_T lValueOfTimeCumulativeDistribution = iDemand.getValueOfTimeCumulativeDistribution ();

    stdair::DemandCharacteristics lDemandCharacteristics (lDemandStreamKey,
                                                          lArrivalPattern,
                                                          lPOSProbMass,
                                                          lChannelProbMass,
                                                          lTripTypeProbMass,
                                                          lStayDurationProbMass,
                                                          lFrequentFlyerProbMass,
                                                          lPreferredDepartureTimeCumulativeDistribution,
                                                          lWTPCumulativeDistribution,
                                                          lValueOfTimeCumulativeDistribution);

    const stdair::DemandDistribution lDemandDistribution (iDemand._demandMean,
                                                          iDemand._demandStdDev);
    
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
