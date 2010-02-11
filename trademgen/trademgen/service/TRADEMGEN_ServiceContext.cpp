// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <sstream>
// StdAir
#include <stdair/STDAIR_Service.hpp>
#include <stdair/bom/DemandStream.hpp>
#include <stdair/factory/FacBomContent.hpp>
// Trademgen
#include <trademgen/bom/DemandStream.hpp>
#include <trademgen/service/TRADEMGEN_ServiceContext.hpp>

namespace TRADEMGEN {

  // //////////////////////////////////////////////////////////////////////
  TRADEMGEN_ServiceContext::TRADEMGEN_ServiceContext () {
  }

  // //////////////////////////////////////////////////////////////////////
  TRADEMGEN_ServiceContext::
  TRADEMGEN_ServiceContext (const std::string& iServiceName) {
  }

  // //////////////////////////////////////////////////////////////////////
  TRADEMGEN_ServiceContext::~TRADEMGEN_ServiceContext() {
  }
  
  // //////////////////////////////////////////////////////////////////////
  stdair::STDAIR_Service& TRADEMGEN_ServiceContext::getSTDAIR_Service() const {
    assert (_stdairService != NULL);
    return *_stdairService;
  }
  
  // //////////////////////////////////////////////////////////////////////
  void TRADEMGEN_ServiceContext::
  setSTDAIR_Service (stdair::STDAIR_ServicePtr_T ioSTDAIR_ServicePtr) {
    _stdairService = ioSTDAIR_ServicePtr;
  }
  
  // //////////////////////////////////////////////////////////////////////
  const std::string TRADEMGEN_ServiceContext::shortDisplay() const {
    std::ostringstream oStr;
    oStr << "TRADEMGEN_ServiceContext: " << std::endl
         << std::endl;
    return oStr.str();
  }

  // //////////////////////////////////////////////////////////////////////
  const std::string TRADEMGEN_ServiceContext::display() const {
    std::ostringstream oStr;
    oStr << shortDisplay();
    return oStr.str();
  }

  // ////////////////////////////////////////////////////////////////////
  void TRADEMGEN_ServiceContext::
  addDemandStream (const stdair::DemandStreamKey_T& iKey,
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
    const bool succeed =
      _demandStreamList.
      insert(stdair::DemandStreamList_T::value_type (iKey, &lDemandStream)).second;
    // TODO
    assert (succeed == true);
  }

  // ////////////////////////////////////////////////////////////////////
  const stdair::Count_T& TRADEMGEN_ServiceContext::
  getTotalNumberOfRequestsToBeGenerated (const stdair::DemandStreamKey_T& iKey) const {
    stdair::DemandStreamList_T::const_iterator itDemandStream =
      _demandStreamList.find (iKey);
    // TODO
    assert (itDemandStream != _demandStreamList.end());
    const stdair::DemandStream* lDemandStream_ptr = itDemandStream->second;
    assert (lDemandStream_ptr != NULL);
      
    return lDemandStream_ptr->getTotalNumberOfRequestsToBeGenerated ();
  }

  // ////////////////////////////////////////////////////////////////////
  const bool TRADEMGEN_ServiceContext::
  stillHavingRequestsToBeGenerated (const stdair::DemandStreamKey_T& iKey) const{
    stdair::DemandStreamList_T::const_iterator itDemandStream =
      _demandStreamList.find (iKey);
    // TODO
    assert (itDemandStream != _demandStreamList.end());
    const stdair::DemandStream* lDemandStream_ptr = itDemandStream->second;
    assert (lDemandStream_ptr != NULL);

    return DemandStream::stillHavingRequestsToBeGenerated (*lDemandStream_ptr);
  }

  // ////////////////////////////////////////////////////////////////////
  stdair::BookingRequestPtr_T TRADEMGEN_ServiceContext::
  generateNextRequest (const stdair::DemandStreamKey_T& iKey) {
    stdair::DemandStreamList_T::iterator itDemandStream =
      _demandStreamList.find (iKey);
    // TODO
    assert (itDemandStream != _demandStreamList.end());
    stdair::DemandStream* lDemandStream_ptr = itDemandStream->second;
    assert (lDemandStream_ptr != NULL);

    return DemandStream::generateNextRequest (*lDemandStream_ptr);
  }
}
