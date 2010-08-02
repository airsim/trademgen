// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
// StdAir
#include <stdair/STDAIR_Types.hpp>
#include <stdair/basic/BasConst_BookingClass.hpp>
#include <stdair/basic/BasConst_Yield.hpp>
#include <stdair/basic/BasConst_General.hpp>
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
    // DEBUG
    // STDAIR_LOG_DEBUG ("Demand stream key: " << lDemandStreamKey.describe());
    
    // Arrival pattern
    stdair::ArrivalPatternCumulativeDistribution_T lArrivalPattern;
    iDemand.buildArrivalPattern (lArrivalPattern);
    // POS probability mass.
    stdair::POSProbabilityMassFunction_T lPOSProbMass;
    iDemand.buildPOSProbabilityMass (lPOSProbMass);
    // Channel probability mass.
    stdair::ChannelProbabilityMassFunction_T lChannelProbMass;
    iDemand.buildChannelProbabilityMass (lChannelProbMass);
    // Trip type probability mass.
    stdair::TripTypeProbabilityMassFunction_T lTripTypeProbMass;
    iDemand.buildTripTypeProbabilityMass (lTripTypeProbMass);
    // Stay duration probability mass.
    stdair::StayDurationProbabilityMassFunction_T lStayDurationProbMass;
    iDemand.buildStayDurationProbabilityMass (lStayDurationProbMass);
    // Frequent flyer probability mass.
    stdair::FrequentFlyerProbabilityMassFunction_T lFrequentFlyerProbMass;
    iDemand.buildFrequentFlyerProbabilityMass (lFrequentFlyerProbMass);
    // Preferred departure time cumulative distribution.
    stdair::PreferredDepartureTimeContinuousDistribution_T lPreferredDepartureTimeContinuousDistribution;
    iDemand.buildPreferredDepartureTimeContinuousDistribution (lPreferredDepartureTimeContinuousDistribution);
    // WTP cumulative distribution.
    stdair::WTPContinuousDistribution_T lWTPContinuousDistribution;
    iDemand.buildWTPContinuousDistribution (lWTPContinuousDistribution);
    // Value of time cumulative distribution.
    stdair::ValueOfTimeContinuousDistribution_T lValueOfTimeContinuousDistribution;
    iDemand.buildValueOfTimeContinuousDistribution (lValueOfTimeContinuousDistribution);

    const stdair::DemandDistribution lDemandDistribution (iDemand._demandMean,
                                                          iDemand._demandStdDev);
    
    // Seed
    stdair::RandomSeed_T lNumberOfRequestsSeed = stdair::DEFAULT_RANDOM_SEED;
    stdair::RandomSeed_T lRequestDateTimeSeed =  stdair::DEFAULT_RANDOM_SEED;
    stdair::RandomSeed_T lDemandCharacteristicsSeed =stdair::DEFAULT_RANDOM_SEED;
  
    // Delegate the call to the dedicated command
    DemandManager::addDemandStream(ioBomRoot, lDemandStreamKey,
                                   lArrivalPattern,
                                   lPOSProbMass,
                                   lChannelProbMass,
                                   lTripTypeProbMass,
                                   lStayDurationProbMass,
                                   lFrequentFlyerProbMass,
                                   lPreferredDepartureTimeContinuousDistribution,
                                   lWTPContinuousDistribution,
                                   lValueOfTimeContinuousDistribution,
                                   lDemandDistribution,
                                   lNumberOfRequestsSeed, lRequestDateTimeSeed,
                                   lDemandCharacteristicsSeed);
  }    
}
