// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
// StdAir
#include <stdair/STDAIR_Types.hpp>
#include <stdair/basic/DemandCharacteristics.hpp>
#include <stdair/basic/DemandDistribution.hpp>
#include <stdair/bom/BomRoot.hpp>
#include <stdair/bom/DemandStream.hpp>
#include <stdair/factory/FacBomContent.hpp>
#include <stdair/service/Logger.hpp>
// TraDemGen
#include <trademgen/bom/DemandStruct.hpp>
#include <trademgen/bom/DemandStream.hpp>
#include <trademgen/command/DemandManager.hpp>

namespace TRADEMGEN {

  // //////////////////////////////////////////////////////////////////////
  void DemandManager::
  addDemandStream (stdair::DemandStreamList_T& ioDemandStreamList,
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

    const stdair::DemandStreamKeyStr_T iKeyStr = iKey.describe();

    // Insert the reference on the given DemandStream object into the
    // dedicated list
    const bool hasInsertBeenSuccessfull = ioDemandStreamList.
      insert (stdair::DemandStreamList_T::value_type (iKeyStr,
                                                      &lDemandStream)).second;
    if (hasInsertBeenSuccessfull == false) {
      STDAIR_LOG_ERROR ("The DemandStream object with key: " << iKeyStr
                        << " can not be inserted into the dedicated list");
      STDAIR_LOG_ERROR ("DemandStream object: " << lDemandStream);
      assert (false);
    }
  }
    
  // ////////////////////////////////////////////////////////////////////
  const stdair::NbOfRequests_T& DemandManager::
  getTotalNumberOfRequestsToBeGenerated (const stdair::DemandStreamList_T& iDemandStreamList,
                                         const stdair::DemandStreamKey_T& iKey) {
    stdair::DemandStreamList_T::const_iterator itDemandStream =
      iDemandStreamList.find (iKey.describe());
    // TODO
    assert (itDemandStream != iDemandStreamList.end());
    const stdair::DemandStream* lDemandStream_ptr = itDemandStream->second;
    assert (lDemandStream_ptr != NULL);
      
    return lDemandStream_ptr->getTotalNumberOfRequestsToBeGenerated ();
  }

  // ////////////////////////////////////////////////////////////////////
  const bool DemandManager::
  stillHavingRequestsToBeGenerated (const stdair::DemandStreamList_T& iDemandStreamList,
                                    const stdair::DemandStreamKey_T& iKey) {
    stdair::DemandStreamList_T::const_iterator itDemandStream =
      iDemandStreamList.find (iKey.describe());
    // TODO
    assert (itDemandStream != iDemandStreamList.end());
    const stdair::DemandStream* lDemandStream_ptr = itDemandStream->second;
    assert (lDemandStream_ptr != NULL);

    const bool hasStillRequest = 
      DemandStream::stillHavingRequestsToBeGenerated(*lDemandStream_ptr);
    return hasStillRequest;
  }

  // ////////////////////////////////////////////////////////////////////
  stdair::BookingRequestPtr_T DemandManager::
  generateNextRequest (stdair::DemandStreamList_T& ioDemandStreamList,
                       const stdair::DemandStreamKey_T& iKey) {
    stdair::DemandStreamList_T::iterator itDemandStream =
      ioDemandStreamList.find (iKey.describe());
    // TODO
    assert (itDemandStream != ioDemandStreamList.end());
    stdair::DemandStream* lDemandStream_ptr = itDemandStream->second;
    assert (lDemandStream_ptr != NULL);

    return DemandStream::generateNextRequest (*lDemandStream_ptr);
  }

}
