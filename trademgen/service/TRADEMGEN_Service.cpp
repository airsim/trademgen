// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <sstream>
// Boost
#include <boost/make_shared.hpp>
#if defined(SOCI_HEADERS_BURIED)
#include <soci/core/soci.h>
#else // SOCI_HEADERS_BURIED
#include <soci/soci.h>
#endif // SOCI_HEADERS_BURIED
// StdAir
#include <stdair/basic/BasChronometer.hpp>
#include <stdair/basic/BasConst_General.hpp>
#include <stdair/bom/BomRoot.hpp>
#include <stdair/bom/BookingRequestStruct.hpp>
#include <stdair/bom/AirlineStruct.hpp>
#include <stdair/bom/EventStruct.hpp>
#include <stdair/bom/EventQueue.hpp>
#include <stdair/command/DBManagerForAirlines.hpp>
#include <stdair/service/Logger.hpp>
#include <stdair/service/DBSessionManager.hpp>
#include <stdair/STDAIR_Service.hpp>
#include <stdair/factory/FacBomManager.hpp>
// SEvMgr
#include <sevmgr/SEVMGR_Service.hpp>
// TraDemGen
#include <trademgen/basic/BasConst_TRADEMGEN_Service.hpp>
#include <trademgen/bom/BomDisplay.hpp>
#include <trademgen/bom/DemandStreamKey.hpp>
#include <trademgen/bom/DemandStream.hpp>
#include <trademgen/factory/FacTRADEMGENServiceContext.hpp>
#include <trademgen/command/DemandParser.hpp>
#include <trademgen/command/DemandManager.hpp>
#include <trademgen/service/TRADEMGEN_ServiceContext.hpp>
#include <trademgen/TRADEMGEN_Service.hpp>

namespace TRADEMGEN {

  // //////////////////////////////////////////////////////////////////////
  TRADEMGEN_Service::TRADEMGEN_Service() : _trademgenServiceContext (NULL) {
    assert (false);
  }

  // //////////////////////////////////////////////////////////////////////
  TRADEMGEN_Service::TRADEMGEN_Service (const TRADEMGEN_Service& iService) 
    : _trademgenServiceContext (NULL) {
    assert (false);
  }

  // //////////////////////////////////////////////////////////////////////
  TRADEMGEN_Service::TRADEMGEN_Service (const stdair::BasLogParams& iLogParams,
                                        const stdair::RandomSeed_T& iRandomSeed)
    : _trademgenServiceContext (NULL) {
    
    // Initialise the STDAIR service handler
    stdair::STDAIR_ServicePtr_T lSTDAIR_Service_ptr =
      initStdAirService (iLogParams);
    
    // Initialise the service context
    initServiceContext (iRandomSeed);

    // Add the StdAir service context to the TRADEMGEN service context
    // \note TRADEMGEN owns the STDAIR service resources here.
    const bool ownStdairService = true;
    addStdAirService (lSTDAIR_Service_ptr, ownStdairService);

    // Initalise the SEvMgr service.
    initSEVMGRService();

    // Initialise the (remaining of the) context
    initTrademgenService();
  }

  // //////////////////////////////////////////////////////////////////////
  TRADEMGEN_Service::TRADEMGEN_Service (const stdair::BasLogParams& iLogParams,
                                        const stdair::BasDBParams& iDBParams,
                                        const stdair::RandomSeed_T& iRandomSeed)
    : _trademgenServiceContext (NULL) {
    
    // Initialise the STDAIR service handler
    stdair::STDAIR_ServicePtr_T lSTDAIR_Service_ptr =
      initStdAirService (iLogParams, iDBParams);
    
    // Initialise the service context
    initServiceContext (iRandomSeed);

    // Add the StdAir service context to the TRADEMGEN service context
    // \note TRADEMGEN owns the STDAIR service resources here.
    const bool ownStdairService = true;
    addStdAirService (lSTDAIR_Service_ptr, ownStdairService);

    // Initalise the SEvMgr service.
    initSEVMGRService();

    // Initialise the (remaining of the) context
    initTrademgenService();
  }

  // ////////////////////////////////////////////////////////////////////
  TRADEMGEN_Service::
  TRADEMGEN_Service (stdair::STDAIR_ServicePtr_T ioSTDAIR_Service_ptr,
                     const stdair::RandomSeed_T& iRandomSeed)
    : _trademgenServiceContext (NULL) {

    // Initialise the service context
    initServiceContext (iRandomSeed);

    // Add the StdAir service context to the TRADEMGEN service context
    // \note TraDemGen does not own the STDAIR service resources here.
    const bool doesNotOwnStdairService = false;
    addStdAirService (ioSTDAIR_Service_ptr, doesNotOwnStdairService);

    // Initalise the SEvMgr service.
    initSEVMGRService();

    // Initialise the context
    initTrademgenService();
  }

  // //////////////////////////////////////////////////////////////////////
  TRADEMGEN_Service::~TRADEMGEN_Service() {
    // Delete/Clean all the objects from memory
    finalise();
  }

  // ////////////////////////////////////////////////////////////////////
  void TRADEMGEN_Service::finalise() {
    assert (_trademgenServiceContext != NULL);
    // Reset the (Boost.)Smart pointer pointing on the STDAIR_Service object.
    _trademgenServiceContext->reset();
  }

  // //////////////////////////////////////////////////////////////////////
  void TRADEMGEN_Service::
  initServiceContext (const stdair::RandomSeed_T& iRandomSeed) {
    // Initialise the service context
    TRADEMGEN_ServiceContext& lTRADEMGEN_ServiceContext = 
      FacTRADEMGENServiceContext::instance().create (iRandomSeed);
    _trademgenServiceContext = &lTRADEMGEN_ServiceContext;
  }

  // ////////////////////////////////////////////////////////////////////
  void TRADEMGEN_Service::
  addStdAirService (stdair::STDAIR_ServicePtr_T ioSTDAIR_Service_ptr,
                    const bool iOwnStdairService) {
    // Retrieve the TraDemGen service context
    assert (_trademgenServiceContext != NULL);
    TRADEMGEN_ServiceContext& lTRADEMGEN_ServiceContext =
      *_trademgenServiceContext;

    // Store the STDAIR service object within the (TRADEMGEN) service context
    lTRADEMGEN_ServiceContext.setSTDAIR_Service (ioSTDAIR_Service_ptr,
                                                 iOwnStdairService);
  }
  
  // //////////////////////////////////////////////////////////////////////
  stdair::STDAIR_ServicePtr_T TRADEMGEN_Service::
  initStdAirService (const stdair::BasLogParams& iLogParams,
                     const stdair::BasDBParams& iDBParams) {

    /**
     * Initialise the STDAIR service handler.
     * \note The track on the object memory is kept thanks to the Boost
     * Smart Pointers component.
     */
    stdair::STDAIR_ServicePtr_T lSTDAIR_Service_ptr = 
      boost::make_shared<stdair::STDAIR_Service> (iLogParams, iDBParams);
    assert (lSTDAIR_Service_ptr != NULL);

    return lSTDAIR_Service_ptr;
  }
  
  // //////////////////////////////////////////////////////////////////////
  stdair::STDAIR_ServicePtr_T TRADEMGEN_Service::
  initStdAirService (const stdair::BasLogParams& iLogParams) {

    /**
     * Initialise the STDAIR service handler.
     * \note The track on the object memory is kept thanks to the Boost
     * Smart Pointers component.
     */
    stdair::STDAIR_ServicePtr_T lSTDAIR_Service_ptr = 
      boost::make_shared<stdair::STDAIR_Service> (iLogParams);
    assert (lSTDAIR_Service_ptr != NULL);

    return lSTDAIR_Service_ptr;
  }

  // ////////////////////////////////////////////////////////////////////
  void TRADEMGEN_Service::initSEVMGRService() {

    // Retrieve the TraDemGen service context
    assert (_trademgenServiceContext != NULL);
    TRADEMGEN_ServiceContext& lTRADEMGEN_ServiceContext =
      *_trademgenServiceContext;

    // Retrieve the StdAir service context
    stdair::STDAIR_ServicePtr_T lSTDAIR_Service_ptr =
      lTRADEMGEN_ServiceContext.getSTDAIR_ServicePtr();

    /**
     * Initialise the SEvMgr service handler.
     *
     * \note The (Boost.)Smart Pointer keeps track of the references
     *       on the Service object, and deletes that object when it is
     *       no longer referenced (e.g., at the end of the process).
     */
    SEVMGR::SEVMGR_ServicePtr_T lSEVMGR_Service_ptr = 
      boost::make_shared<SEVMGR::SEVMGR_Service> (lSTDAIR_Service_ptr);
    
    // Store the SEvMgr service object within the (TraDemGen) service context
    lTRADEMGEN_ServiceContext.setSEVMGR_Service (lSEVMGR_Service_ptr);
  }
  
  
  // //////////////////////////////////////////////////////////////////////
  void TRADEMGEN_Service::initTrademgenService() {
    // Do nothing at this stage. A sample BOM tree may be built by
    // calling the buildSampleBom() method
  }

  // //////////////////////////////////////////////////////////////////////
  void TRADEMGEN_Service::
  parseAndLoad (const stdair::Filename_T& iDemandInputFilename) {

    // Retrieve the TraDemGen service context
    assert (_trademgenServiceContext != NULL);
    TRADEMGEN_ServiceContext& lTRADEMGEN_ServiceContext =
      *_trademgenServiceContext;

    // Retrieve the shared generator
    stdair::RandomGeneration& lSharedGenerator =
      lTRADEMGEN_ServiceContext.getUniformGenerator();

    // Retrieve the default POS distribution
    const POSProbabilityMass_T& lDefaultPOSProbabilityMass =
      lTRADEMGEN_ServiceContext.getPOSProbabilityMass();
    
    // Retrieve the StdAir service context
    stdair::STDAIR_Service& lSTDAIR_Service =
      lTRADEMGEN_ServiceContext.getSTDAIR_Service();

    // Retrieve the event queue
    stdair::EventQueue& lEventQueue = lSTDAIR_Service.getEventQueue();

    // Parse the input file and initialise the demand generators
    stdair::BasChronometer lDemandGeneration; lDemandGeneration.start();
    DemandParser::generateDemand (iDemandInputFilename, lEventQueue,
                                  lSharedGenerator, lDefaultPOSProbabilityMass);
    const double lGenerationMeasure = lDemandGeneration.elapsed();

    // DEBUG
    STDAIR_LOG_DEBUG ("Demand generation time: " << lGenerationMeasure);
  }

  // ////////////////////////////////////////////////////////////////////
  void TRADEMGEN_Service::buildSampleBom() {

    // Retrieve the TraDemGen service context
    if (_trademgenServiceContext == NULL) {
      throw stdair::NonInitialisedServiceException ("The TraDemGen service has "
                                                    "not been initialised");
    }
    assert (_trademgenServiceContext != NULL);

    // Retrieve the TraDemGen service context and whether it owns the Stdair
    // service
    TRADEMGEN_ServiceContext& lTRADEMGEN_ServiceContext =
      *_trademgenServiceContext;
    const bool doesOwnStdairService =
      lTRADEMGEN_ServiceContext.getOwnStdairServiceFlag();

    // Retrieve the StdAir service object from the (TraDemGen) service context
    stdair::STDAIR_Service& lSTDAIR_Service =
      lTRADEMGEN_ServiceContext.getSTDAIR_Service();

    /**
     * 1. Have StdAir build the whole BOM tree, only when the StdAir service is
     *    owned by the current component (TraDemGen here)
     */
    if (doesOwnStdairService == true) {
      //
      lSTDAIR_Service.buildSampleBom();
    }

    /**
     * 2. Delegate the complementary building of objects and links by the
     *    appropriate levels/components
     * 
     * \note: Currently, no more things to do by TraDemGen at that stage,
     *        as there is no child
     */
    /**
     * Let the Event Management Library (i.e., the SEvMgr component) complement
     * the BOM.
     *
     * \note As of now, the Event Management Library does not add anything to
     * the sample BOM.
     */
    SEVMGR::SEVMGR_Service& lSEVMGR_Service =
      lTRADEMGEN_ServiceContext.getSEVMGR_Service();
    lSEVMGR_Service.buildSampleBom();

    /**
     * 3. Build the complementary objects/links for the current component (here,
     *    TraDemGen)
     */
    // Retrieve the shared generator
    stdair::RandomGeneration& lSharedGenerator =
      lTRADEMGEN_ServiceContext.getUniformGenerator();

    // Retrieve the default POS distribution
    const POSProbabilityMass_T& lDefaultPOSProbabilityMass =
      lTRADEMGEN_ServiceContext.getPOSProbabilityMass();
    
    // Retrieve the event queue
    stdair::EventQueue& lEventQueue = lSTDAIR_Service.getEventQueue();

    // Delegate the BOM building to the dedicated service
    DemandManager::buildSampleBom (lEventQueue, lSharedGenerator,
                                   lDefaultPOSProbabilityMass);
  }

  // //////////////////////////////////////////////////////////////////////
  stdair::BookingRequestStruct TRADEMGEN_Service::
  buildSampleBookingRequest (const bool isForCRS) {

    // Retrieve the TraDemGen service context
    if (_trademgenServiceContext == NULL) {
      throw stdair::NonInitialisedServiceException ("The TraDemGen service has "
                                                    "not been initialised");
    }
    assert (_trademgenServiceContext != NULL);

    TRADEMGEN_ServiceContext& lTRADEMGEN_ServiceContext =
      *_trademgenServiceContext;

    // Retrieve the STDAIR service object from the (TraDemGen) service context
    stdair::STDAIR_Service& lSTDAIR_Service =
      lTRADEMGEN_ServiceContext.getSTDAIR_Service();

    // Delegate the BOM building to the dedicated service
    return lSTDAIR_Service.buildSampleBookingRequest (isForCRS);
  }

  // //////////////////////////////////////////////////////////////////////
  std::string TRADEMGEN_Service::csvDisplay() const {

    // Retrieve the TraDemGen service context
    if (_trademgenServiceContext == NULL) {
      throw stdair::NonInitialisedServiceException ("The TraDemGen service has "
                                                    "not been initialised");
    }
    assert (_trademgenServiceContext != NULL);

    TRADEMGEN_ServiceContext& lTRADEMGEN_ServiceContext =
      *_trademgenServiceContext;

    // Retrieve the STDAIR service object from the (TraDemGen) service context
    stdair::STDAIR_Service& lSTDAIR_Service =
      lTRADEMGEN_ServiceContext.getSTDAIR_Service();

    // Retrieve the event queue
    stdair::EventQueue& lEventQueue = lSTDAIR_Service.getEventQueue();

    // Delegate the BOM building to the dedicated service
    return BomDisplay::csvDisplay (lEventQueue);
  }

  // //////////////////////////////////////////////////////////////////////
  void TRADEMGEN_Service::displayAirlineListFromDB() const {

    // Retrieve the TraDemGen service context
    if (_trademgenServiceContext == NULL) {
      throw stdair::NonInitialisedServiceException ("The TraDemGen service has "
                                                    "not been initialised");
    }
    assert (_trademgenServiceContext != NULL);
    // TRADEMGEN_ServiceContext& lTRADEMGEN_ServiceContext =
    //  *_trademgenServiceContext;

    // Get the date-time for the present time
    boost::posix_time::ptime lNowDateTime =
      boost::posix_time::second_clock::local_time();
    //boost::gregorian::date lNowDate = lNowDateTime.date();

    // DEBUG
    STDAIR_LOG_DEBUG (std::endl
                      << "==================================================="
                      << std::endl
                      << lNowDateTime);

    // Delegate the query execution to the dedicated command
    stdair::BasChronometer lAirListChronometer;
    lAirListChronometer.start();

    // Retrieve the database session handler
    stdair::DBSession_T& lDBSession =
      stdair::DBSessionManager::instance().getDBSession();
    
    // Prepare and execute the select statement
    stdair::AirlineStruct lAirline;
    stdair::DBRequestStatement_T lSelectStatement (lDBSession);
    stdair::DBManagerForAirlines::prepareSelectStatement (lDBSession,
                                                          lSelectStatement,
                                                          lAirline);
    
    // Prepare the SQL request corresponding to the select statement
    bool hasStillData = true;
    unsigned int idx = 0;
    while (hasStillData == true) {
      hasStillData =
        stdair::DBManagerForAirlines::iterateOnStatement (lSelectStatement,
                                                          lAirline);
      
      // DEBUG
      STDAIR_LOG_DEBUG ("[" << idx << "]: " << lAirline);
      
      // Iteration
      ++idx;
    }
    
    const double lAirListMeasure = lAirListChronometer.elapsed();
    
    // DEBUG
    STDAIR_LOG_DEBUG ("Sample service for airline list retrieval: "
                      << lAirListMeasure);
  }

  // ////////////////////////////////////////////////////////////////////
  const stdair::Count_T& TRADEMGEN_Service::
  getExpectedTotalNumberOfRequestsToBeGenerated() const {
    
    // Retrieve the TraDemGen service context
    assert (_trademgenServiceContext != NULL);
    TRADEMGEN_ServiceContext& lTRADEMGEN_ServiceContext =
      *_trademgenServiceContext;

    // Retrieve the StdAir service context
    stdair::STDAIR_Service& lSTDAIR_Service =
      lTRADEMGEN_ServiceContext.getSTDAIR_Service();

    // Retrieve the event queue object instance
    const stdair::EventQueue& lQueue = lSTDAIR_Service.getEventQueue();
    
    // Delegate the call to the dedicated command
    const stdair::Count_T& oExpectedTotalNumberOfRequestsToBeGenerated =
      lQueue.getExpectedTotalNbOfEvents (stdair::EventType::BKG_REQ);

    //
    return oExpectedTotalNumberOfRequestsToBeGenerated;
  }

  // ////////////////////////////////////////////////////////////////////
  const stdair::Count_T& TRADEMGEN_Service::
  getActualTotalNumberOfRequestsToBeGenerated() const {
    
    // Retrieve the TraDemGen service context
    assert (_trademgenServiceContext != NULL);
    TRADEMGEN_ServiceContext& lTRADEMGEN_ServiceContext =
      *_trademgenServiceContext;

    // Retrieve the StdAir service context
    stdair::STDAIR_Service& lSTDAIR_Service =
      lTRADEMGEN_ServiceContext.getSTDAIR_Service();

    // Retrieve the event queue object instance
    const stdair::EventQueue& lQueue = lSTDAIR_Service.getEventQueue();
    
    // Delegate the call to the dedicated command
    const stdair::Count_T& oActualTotalNumberOfRequestsToBeGenerated =
      lQueue.getActualTotalNbOfEvents (stdair::EventType::BKG_REQ);

    //
    return oActualTotalNumberOfRequestsToBeGenerated;
  }

  // ////////////////////////////////////////////////////////////////////
  const bool TRADEMGEN_Service::
  stillHavingRequestsToBeGenerated (const stdair::DemandStreamKeyStr_T& iKey,
                                    stdair::ProgressStatusSet& ioPSS,
                                    const stdair::DemandGenerationMethod& iDemandGenerationMethod) const {
    
    // Retrieve the TraDemGen service context
    assert (_trademgenServiceContext != NULL);
    TRADEMGEN_ServiceContext& lTRADEMGEN_ServiceContext =
      *_trademgenServiceContext;

    // Retrieve the StdAir service context
    stdair::STDAIR_Service& lSTDAIR_Service =
      lTRADEMGEN_ServiceContext.getSTDAIR_Service();

    // Retrieve the event queue object instance
    const stdair::EventQueue& lQueue = lSTDAIR_Service.getEventQueue();
    
    // Delegate the call to the dedicated command
    const bool oStillHavingRequestsToBeGenerated =
      DemandManager::stillHavingRequestsToBeGenerated (lQueue, iKey, ioPSS,
                                                       iDemandGenerationMethod);

    //
    return oStillHavingRequestsToBeGenerated;
  }

  // ////////////////////////////////////////////////////////////////////
  stdair::Count_T TRADEMGEN_Service::
  generateFirstRequests (const stdair::DemandGenerationMethod& iDemandGenerationMethod) const {

    // Retrieve the TraDemGen service context
    assert (_trademgenServiceContext != NULL);
    TRADEMGEN_ServiceContext& lTRADEMGEN_ServiceContext =
      *_trademgenServiceContext;

    // Retrieve the StdAir service context
    stdair::STDAIR_Service& lSTDAIR_Service =
      lTRADEMGEN_ServiceContext.getSTDAIR_Service();

    // Retrieve the event queue object instance
    stdair::EventQueue& lQueue = lSTDAIR_Service.getEventQueue();

    // Retrieve the random generator
    stdair::RandomGeneration& lGenerator =
      lTRADEMGEN_ServiceContext.getUniformGenerator();

    // Delegate the call to the dedicated command
    const stdair::Count_T& oActualTotalNbOfEvents =
      DemandManager::generateFirstRequests (lQueue, lGenerator,
                                            iDemandGenerationMethod);

    //
    return oActualTotalNbOfEvents;
  }

  // ////////////////////////////////////////////////////////////////////
  stdair::BookingRequestPtr_T TRADEMGEN_Service::
  generateNextRequest (const stdair::DemandStreamKeyStr_T& iKey,
                       const stdair::DemandGenerationMethod& iDemandGenerationMethod) const {

    // Retrieve the TraDemGen service context
    assert (_trademgenServiceContext != NULL);
    TRADEMGEN_ServiceContext& lTRADEMGEN_ServiceContext =
      *_trademgenServiceContext;

    // Retrieve the StdAir service context
    stdair::STDAIR_Service& lSTDAIR_Service =
      lTRADEMGEN_ServiceContext.getSTDAIR_Service();

    // Retrieve the event queue object instance
    stdair::EventQueue& lQueue = lSTDAIR_Service.getEventQueue();
    
    // Retrieve the random generator
    stdair::RandomGeneration& lGenerator =
      lTRADEMGEN_ServiceContext.getUniformGenerator();

    // Delegate the call to the dedicated command
    return DemandManager::generateNextRequest (lQueue, lGenerator, iKey,
                                               iDemandGenerationMethod);
  }

  // ////////////////////////////////////////////////////////////////////
  stdair::ProgressStatusSet TRADEMGEN_Service::
  popEvent (stdair::EventStruct& ioEventStruct) const {

    // Retrieve the TraDemGen service context
    assert (_trademgenServiceContext != NULL);
    TRADEMGEN_ServiceContext& lTRADEMGEN_ServiceContext =
      *_trademgenServiceContext;

    // Retrieve the StdAir service context
    stdair::STDAIR_Service& lSTDAIR_Service =
      lTRADEMGEN_ServiceContext.getSTDAIR_Service();

    // Retrieve the event queue object instance
    stdair::EventQueue& lQueue = lSTDAIR_Service.getEventQueue();
    
    // Extract the next event from the queue
    return lQueue.popEvent (ioEventStruct);
  }

  // ////////////////////////////////////////////////////////////////////
  bool TRADEMGEN_Service::isQueueDone() const {

    // Retrieve the TraDemGen service context
    assert (_trademgenServiceContext != NULL);
    TRADEMGEN_ServiceContext& lTRADEMGEN_ServiceContext =
      *_trademgenServiceContext;

    // Retrieve the StdAir service context
    stdair::STDAIR_Service& lSTDAIR_Service =
      lTRADEMGEN_ServiceContext.getSTDAIR_Service();

    // Retrieve the event queue object instance
    const stdair::EventQueue& lQueue = lSTDAIR_Service.getEventQueue();
    
    // Calculates whether the event queue has been fully emptied
    const bool isQueueDone = lQueue.isQueueDone();

    //
    return isQueueDone;
  }

  // ////////////////////////////////////////////////////////////////////
  bool TRADEMGEN_Service::
  generateCancellation (const stdair::TravelSolutionStruct& iTravelSolution,
                        const stdair::PartySize_T& iPartySize,
                        const stdair::DateTime_T& iRequestTime,
                        const stdair::Date_T& iDepartureDate) const {

    // Retrieve the TraDemGen service context
    assert (_trademgenServiceContext != NULL);
    TRADEMGEN_ServiceContext& lTRADEMGEN_ServiceContext =
      *_trademgenServiceContext;
    
    // Retrieve the random generator
    stdair::RandomGeneration& lGenerator =
      lTRADEMGEN_ServiceContext.getUniformGenerator();

    // Build an event structure with the default constructor.
    stdair::EventStruct lEventStruct;
    stdair::EventStruct& lRefEventStruct = lEventStruct;

    // Generate the cancellation event
    const bool hasCancellationBeenGenerated =
      DemandManager::generateCancellation (lGenerator, iTravelSolution,
                                           iPartySize, iRequestTime,
                                           iDepartureDate, lRefEventStruct);

    // If the cancellation has been not sucessfully gerenerated, return.
    if (hasCancellationBeenGenerated == false) {
      return  hasCancellationBeenGenerated;
    }
    assert (hasCancellationBeenGenerated == true);

    // Retrieve the SEvMgr service context
    SEVMGR::SEVMGR_Service& lSEVMGR_Service =
      lTRADEMGEN_ServiceContext.getSEVMGR_Service();
      
    /**
       \note When adding an event in the event queue, the event can be
       altered. That happens when an event already exists, in the
       event queue, with exactly the same date-time stamp. In that
       case, the date-time stamp is altered for the newly added event,
       so that the unicity on the date-time stamp can be guaranteed.
    */
    // Add the gerenerated cancellation event into the queue
    lSEVMGR_Service.addEvent (lRefEventStruct);

    return hasCancellationBeenGenerated;

  }

  // ////////////////////////////////////////////////////////////////////
  void TRADEMGEN_Service::reset() const {

    // Retrieve the TraDemGen service context
    assert (_trademgenServiceContext != NULL);
    TRADEMGEN_ServiceContext& lTRADEMGEN_ServiceContext =
      *_trademgenServiceContext;

    // Retrieve the StdAir service context
    stdair::STDAIR_Service& lSTDAIR_Service =
      lTRADEMGEN_ServiceContext.getSTDAIR_Service();
    // Retrieve the event queue object instance
    stdair::EventQueue& lQueue = lSTDAIR_Service.getEventQueue();

    // Retrieve the shared generator
    stdair::RandomGeneration& lSharedGenerator =
      lTRADEMGEN_ServiceContext.getUniformGenerator();
    
    // Delegate the call to the dedicated command
    DemandManager::reset (lQueue, lSharedGenerator.getBaseGenerator());
    
    // Retrieve the SEvMgr service context
    SEVMGR::SEVMGR_Service& lSEVMGR_Service =
      lTRADEMGEN_ServiceContext.getSEVMGR_Service();

    /**
     * Reset the EventQueue object.
     *
     * \note As the DemandStream objects are attached to the EventQueue
     * instance, that latter has to be resetted after the DemandStream
     * objects.
     */
    lSEVMGR_Service.reset();
  }
}

namespace SEVMGR {

  /**
   * 
   */
  
  // ////////////////////////////////////////////////////////////////////
  template<class EventGenerator>
  void SEVMGR_Service::addEventGenerator (EventGenerator& iEventGenerator) const {

    // Retrieve the StdAir service
    const stdair::STDAIR_Service& lSTDAIR_Service =
      this->getSTDAIR_Service();
    
    // Retrieve the event queue object instance
    stdair::EventQueue& lQueue = lSTDAIR_Service.getEventQueue();

    // Link the DemandStream to its parent (EventQueue)
    stdair::FacBomManager::linkWithParent (lQueue, iEventGenerator);
    
    // Add the DemandStream to the dedicated list and map
    stdair::FacBomManager::addToListAndMap (lQueue, iEventGenerator);
    
  }
  
  // ////////////////////////////////////////////////////////////////////
  // Explicit template instantiation
  template void SEVMGR::SEVMGR_Service::
  addEventGenerator<TRADEMGEN::DemandStream> (TRADEMGEN::DemandStream&) const;

  // ////////////////////////////////////////////////////////////////////


}
