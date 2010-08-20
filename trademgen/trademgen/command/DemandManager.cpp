// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
// STDAIR
#include <stdair/STDAIR_Types.hpp>
#include <stdair/bom/BomManager.hpp>
#include <stdair/bom/EventStruct.hpp>
#include <stdair/bom/BomRoot.hpp>
#include <stdair/bom/BookingRequestStruct.hpp>
#include <stdair/bom/EventQueue.hpp>
#include <stdair/factory/FacBomManager.hpp>
#include <stdair/service/Logger.hpp>
// TRADEMGEN
#include <trademgen/basic/DemandCharacteristics.hpp>
#include <trademgen/basic/DemandDistribution.hpp>
#include <trademgen/bom/DemandStruct.hpp>
#include <trademgen/bom/DemandStream.hpp>
#include <trademgen/factory/FacDemandStream.hpp>
#include <trademgen/command/DemandManager.hpp>

namespace TRADEMGEN {

  // //////////////////////////////////////////////////////////////////////
  void DemandManager::addDemandStream
  (stdair::BomRoot& ioBomRoot,
   const DemandStreamKey& iKey,
   const ArrivalPatternCumulativeDistribution_T& iArrivalPattern,
   const POSProbabilityMassFunction_T& iPOSProbMass,
   const ChannelProbabilityMassFunction_T& iChannelProbMass,
   const TripTypeProbabilityMassFunction_T& iTripTypeProbMass,
   const StayDurationProbabilityMassFunction_T& iStayDurationProbMass,
   const FrequentFlyerProbabilityMassFunction_T& iFrequentFlyerProbMass,
   const PreferredDepartureTimeContinuousDistribution_T& iPreferredDepartureTimeContinuousDistribution,
   const WTPContinuousDistribution_T& iWTPContinuousDistribution,
   const ValueOfTimeContinuousDistribution_T& iValueOfTimeContinuousDistribution,
   const DemandDistribution& iDemandDistribution,
   const stdair::RandomSeed_T& iNumberOfRequestsSeed,
   const stdair::RandomSeed_T& iRequestDateTimeSeed,
   const stdair::RandomSeed_T& iDemandCharacteristicsSeed) {
    
    DemandStream& lDemandStream = FacDemandStream::
      instance().create(iKey, iArrivalPattern, iPOSProbMass,
                        iChannelProbMass, iTripTypeProbMass,
                        iStayDurationProbMass, iFrequentFlyerProbMass,
                        iPreferredDepartureTimeContinuousDistribution,
                        iWTPContinuousDistribution,
                        iValueOfTimeContinuousDistribution,
                        iDemandDistribution, iNumberOfRequestsSeed,
                        iRequestDateTimeSeed, iDemandCharacteristicsSeed);
    
    // Insert the reference on the given DemandStream object into the
    // dedicated list
      // DEBUG
    // STDAIR_LOG_DEBUG ("Add DemandStream: \n"
    //                   << lDemandStream.getDemandCharacteristics().display()
    //                   << lDemandStream.getDemandDistribution().display());
    stdair::FacBomManager::addToListAndMap (ioBomRoot, lDemandStream);
  }
    
  // ////////////////////////////////////////////////////////////////////
  const stdair::NbOfRequests_T& DemandManager::
  getTotalNumberOfRequestsToBeGenerated (const stdair::BomRoot& iBomRoot,
                                         const stdair::DemandStreamKeyStr_T& iKey) {
    // Retrieve the DemandStream which corresponds to the given key.
    const DemandStream& lDemandStream =
      stdair::BomManager::getChild<DemandStream> (iBomRoot, iKey);
      
    return lDemandStream.getTotalNumberOfRequestsToBeGenerated ();
  }

  // ////////////////////////////////////////////////////////////////////
  const bool DemandManager::
  stillHavingRequestsToBeGenerated (const stdair::BomRoot& iBomRoot,
                                    const stdair::DemandStreamKeyStr_T& iKey) {
    // Retrieve the DemandStream which corresponds to the given key.
    const DemandStream& lDemandStream =
      stdair::BomManager::getChild<DemandStream> (iBomRoot, iKey);
    
    return lDemandStream.stillHavingRequestsToBeGenerated ();
  }

  // ////////////////////////////////////////////////////////////////////
  stdair::BookingRequestPtr_T DemandManager::
  generateNextRequest (const stdair::BomRoot& iBomRoot,
                       const stdair::DemandStreamKeyStr_T& iKey) {
    // Retrieve the DemandStream which corresponds to the given key.
    DemandStream& lDemandStream = 
      stdair::BomManager::getChild<DemandStream> (iBomRoot, iKey);
    
    return lDemandStream.generateNextRequest ();
  }

  // ////////////////////////////////////////////////////////////////////
  void DemandManager::
  generateFirstRequests (stdair::EventQueue& ioEventQueue,
                         const stdair::BomRoot& iBomRoot) {
    // Retrieve the DemandStream list.
    const DemandStreamList_T& lDemandStreamList =
      stdair::BomManager::getList<DemandStream> (iBomRoot);
    
    for (DemandStreamList_T::const_iterator itDemandStream =
           lDemandStreamList.begin();
         itDemandStream != lDemandStreamList.end(); ++itDemandStream) {
      DemandStream* lDemandStream_ptr = *itDemandStream;
      assert (lDemandStream_ptr != NULL);
      
      const bool stillHavingRequestsToBeGenerated =
        lDemandStream_ptr->stillHavingRequestsToBeGenerated ();
   
      if (stillHavingRequestsToBeGenerated) {
        stdair::BookingRequestPtr_T lFirstRequest =
          lDemandStream_ptr->generateNextRequest ();
        
        const DemandStreamKey& lKey = lDemandStream_ptr->getKey();
        const stdair::DateTime_T& lRequestDateTime =
          lFirstRequest->getRequestDateTime();
        stdair::EventStruct lEventStruct ("Request", lRequestDateTime,
                                          lKey.toString(), lFirstRequest);
        ioEventQueue.addEvent (lEventStruct);
      }
    }
  }

  // ////////////////////////////////////////////////////////////////////
  void DemandManager::reset (const stdair::BomRoot& iBomRoot) {
    DemandStreamList_T& lDemandStreamList =
      stdair::BomManager::getList<DemandStream> (iBomRoot);
    for (DemandStreamList_T::iterator itDS = lDemandStreamList.begin();
         itDS != lDemandStreamList.end(); ++itDS) {
      DemandStream* lCurrentDS_ptr = *itDS;
      assert (lCurrentDS_ptr != NULL);
      lCurrentDS_ptr->reset();
    }
  }

}
