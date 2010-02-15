// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
// StdAir
#include <stdair/STDAIR_Types.hpp>
#include <stdair/basic/DemandCharacteristics.hpp>
#include <stdair/basic/DemandDistribution.hpp>
#include <stdair/bom/EventStruct.hpp>
#include <stdair/bom/BomRoot.hpp>
#include <stdair/bom/DemandStream.hpp>
#include <stdair/bom/BookingRequestStruct.hpp>
#include <stdair/bom/EventQueue.hpp>
#include <stdair/bom/BomList.hpp>
#include <stdair/factory/FacBomContent.hpp>
#include <stdair/service/Logger.hpp>
// TraDemGen
#include <trademgen/bom/DemandStruct.hpp>
#include <trademgen/bom/DemandStream.hpp>
#include <trademgen/command/DemandManager.hpp>

namespace TRADEMGEN {

  // //////////////////////////////////////////////////////////////////////
  void DemandManager::
  addDemandStream (stdair::BomRoot& ioBomRoot,
                   const stdair::DemandStreamKey_T& iKey,
                   const stdair::DemandCharacteristics& iDemandCharacteristics,
                   const stdair::DemandDistribution& iDemandDistribution,
                   const stdair::RandomSeed_T& iNumberOfRequestsSeed,
                   const stdair::RandomSeed_T& iRequestDateTimeSeed,
                   const stdair::RandomSeed_T& iDemandCharacteristicsSeed) {

    stdair::DemandStream& lDemandStream =
      stdair::FacBomContent::
      instance().create<stdair::DemandStream> (iKey, iDemandCharacteristics,
                                               iDemandDistribution,
                                               iNumberOfRequestsSeed,
                                               iRequestDateTimeSeed,
                                               iDemandCharacteristicsSeed);

    // Insert the reference on the given DemandStream object into the
    // dedicated list
      // DEBUG
    STDAIR_LOG_DEBUG ("Add DemandStream: "
                      << lDemandStream.getKey().describe());
      
    const bool hasInsertBeenSuccessfull = stdair::FacBomContent::
      addDemandStream<stdair::DemandStream> (ioBomRoot, lDemandStream);
      
    if (hasInsertBeenSuccessfull == false) {
      STDAIR_LOG_ERROR ("The DemandStream object with key: " << iKey.describe()
                        << " can not be inserted into the dedicated list");
      STDAIR_LOG_ERROR ("DemandStream object: " << lDemandStream);
      assert (false);
    }
  }
    
  // ////////////////////////////////////////////////////////////////////
  const stdair::NbOfRequests_T& DemandManager::
  getTotalNumberOfRequestsToBeGenerated (const stdair::BomRoot& iBomRoot,
                                         const stdair::DemandStreamKeyStr_T& iKey) {

    // Retrieve the DemandStream which corresponds to the given key.
    const stdair::DemandStream& lDemandStream =
      iBomRoot.getDemandStream (iKey);
      
    return lDemandStream.getTotalNumberOfRequestsToBeGenerated ();
  }

  // ////////////////////////////////////////////////////////////////////
  const bool DemandManager::
  stillHavingRequestsToBeGenerated (const stdair::BomRoot& iBomRoot,
                                    const stdair::DemandStreamKeyStr_T& iKey) {
    // Retrieve the DemandStream which corresponds to the given key.
    const stdair::DemandStream& lDemandStream =
      iBomRoot.getDemandStream (iKey);
    
    const bool hasStillRequest = 
      DemandStream::stillHavingRequestsToBeGenerated(lDemandStream);
    return hasStillRequest;
  }

  // ////////////////////////////////////////////////////////////////////
  stdair::BookingRequestPtr_T DemandManager::
  generateNextRequest (const stdair::BomRoot& iBomRoot,
                       const stdair::DemandStreamKeyStr_T& iKey) {
    // Retrieve the DemandStream which corresponds to the given key.
    stdair::DemandStream& lDemandStream = iBomRoot.getDemandStream (iKey);
    
    return DemandStream::generateNextRequest (lDemandStream);
  }

  // ////////////////////////////////////////////////////////////////////
  void DemandManager::
  generateFirstRequests (stdair::EventQueue& ioEventQueue,
                         const stdair::BomRoot& iBomRoot) {

    // Retrieve the DemandStream list.
    const stdair::DemandStreamList_T lDemandStreamList =
      iBomRoot.getDemandStreamList();
    
    for (stdair::DemandStreamList_T::iterator itDemandStream =
           lDemandStreamList.begin();
         itDemandStream != lDemandStreamList.end(); ++itDemandStream) {
      stdair::DemandStream& lDemandStream = *itDemandStream;

      const bool stillHavingRequestsToBeGenerated =
        DemandStream::stillHavingRequestsToBeGenerated(lDemandStream);
   
      if (stillHavingRequestsToBeGenerated) {
        stdair::BookingRequestPtr_T lFirstRequest =
          DemandStream::generateNextRequest (lDemandStream);
        
        const stdair::DemandStreamKey_T& lKey = lDemandStream.getKey();
        const stdair::DateTime_T& lRequestDateTime =
          lFirstRequest->getRequestDateTime();
        stdair::EventStruct lEventStruct ("Request", lRequestDateTime,
                                          lKey.describe(), lFirstRequest);
        ioEventQueue.addEvent (lEventStruct);
      }
    }
  }

}
