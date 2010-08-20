// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
// STDAIR
#include <stdair/STDAIR_Types.hpp>
#include <stdair/basic/BasConst_BookingClass.hpp>
#include <stdair/basic/BasConst_Yield.hpp>
#include <stdair/basic/BasConst_General.hpp>
#include <stdair/bom/BomRoot.hpp>
#include <stdair/command/CmdBomManager.hpp>
#include <stdair/service/Logger.hpp>
// TRADEMGEN
#include <trademgen/basic/DemandCharacteristics.hpp>
#include <trademgen/basic/DemandDistribution.hpp>
#include <trademgen/bom/DemandStruct.hpp>
#include <trademgen/factory/FacDemandStream.hpp>
#include <trademgen/command/DemandManager.hpp>
#include <trademgen/command/DemandGenerator.hpp>

namespace TRADEMGEN {

  // //////////////////////////////////////////////////////////////////////
  void DemandGenerator::
  createDemandCharacteristics (stdair::BomRoot& ioBomRoot,
                               const DemandStruct& iDemand) {
    
    const DemandStreamKey lDemandStreamKey (iDemand._origin,
                                            iDemand._destination,
                                            iDemand._prefDepDate,
                                            iDemand._prefCabin);
    // DEBUG
    // STDAIR_LOG_DEBUG ("Demand stream key: " << lDemandStreamKey.describe());
    
    // Arrival pattern
    ArrivalPatternCumulativeDistribution_T lArrivalPattern;
    iDemand.buildArrivalPattern (lArrivalPattern);
    // POS probability mass.
    POSProbabilityMassFunction_T lPOSProbMass;
    iDemand.buildPOSProbabilityMass (lPOSProbMass);
    // Channel probability mass.
    ChannelProbabilityMassFunction_T lChannelProbMass;
    iDemand.buildChannelProbabilityMass (lChannelProbMass);
    // Trip type probability mass.
    TripTypeProbabilityMassFunction_T lTripTypeProbMass;
    iDemand.buildTripTypeProbabilityMass (lTripTypeProbMass);
    // Stay duration probability mass.
    StayDurationProbabilityMassFunction_T lStayDurationProbMass;
    iDemand.buildStayDurationProbabilityMass (lStayDurationProbMass);
    // Frequent flyer probability mass.
    FrequentFlyerProbabilityMassFunction_T lFrequentFlyerProbMass;
    iDemand.buildFrequentFlyerProbabilityMass (lFrequentFlyerProbMass);
    // Preferred departure time cumulative distribution.
    PreferredDepartureTimeContinuousDistribution_T lPreferredDepartureTimeContinuousDistribution;
    iDemand.buildPreferredDepartureTimeContinuousDistribution (lPreferredDepartureTimeContinuousDistribution);
    // WTP cumulative distribution.
    WTPContinuousDistribution_T lWTPContinuousDistribution;
    iDemand.buildWTPContinuousDistribution (lWTPContinuousDistribution);
    // Value of time cumulative distribution.
    ValueOfTimeContinuousDistribution_T lValueOfTimeContinuousDistribution;
    iDemand.buildValueOfTimeContinuousDistribution (lValueOfTimeContinuousDistribution);

    const DemandDistribution lDemandDistribution (iDemand._demandMean,
                                                          iDemand._demandStdDev);
    
    // Seed
    stdair::RandomSeed_T lNumberOfRequestsSeed = stdair::DEFAULT_RANDOM_SEED;
    stdair::RandomSeed_T lRequestDateTimeSeed =  stdair::DEFAULT_RANDOM_SEED;
    stdair::RandomSeed_T lDemandCharacteristicsSeed =stdair::DEFAULT_RANDOM_SEED;
  
    // Delegate the call to the dedicated command
    DemandManager::addDemandStream(ioBomRoot, lDemandStreamKey,
                                   lArrivalPattern, lPOSProbMass,
                                   lChannelProbMass, lTripTypeProbMass,
                                   lStayDurationProbMass, lFrequentFlyerProbMass,
                                   lPreferredDepartureTimeContinuousDistribution,
                                   lWTPContinuousDistribution,
                                   lValueOfTimeContinuousDistribution,
                                   lDemandDistribution,
                                   lNumberOfRequestsSeed, lRequestDateTimeSeed,
                                   lDemandCharacteristicsSeed);
  }    
}
