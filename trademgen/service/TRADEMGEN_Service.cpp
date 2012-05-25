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
#include <stdair/basic/ProgressStatusSet.hpp>
#include <stdair/bom/BomRoot.hpp>
#include <stdair/bom/BookingRequestStruct.hpp>
#include <stdair/bom/AirlineStruct.hpp>
#include <stdair/bom/EventStruct.hpp>
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
#include <trademgen/bom/DemandStream.hpp>
#include <trademgen/bom/DemandStreamTypes.hpp>
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
		     SEVMGR::SEVMGR_ServicePtr_T ioSEVMGR_Service_ptr,
                     const stdair::RandomSeed_T& iRandomSeed)
    : _trademgenServiceContext (NULL) {

    // Initialise the service context
    initServiceContext (iRandomSeed);   

    // Add the StdAir service context to the TRADEMGEN service context
    // \note TraDemGen does not own the STDAIR service resources here.
    const bool doesNotOwnStdairService = false;
    addStdAirService (ioSTDAIR_Service_ptr, doesNotOwnStdairService);

    //Add the SEvMgr service to the TRADEMGEN service context.
    addSEVMGRService (ioSEVMGR_Service_ptr);

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

  // ////////////////////////////////////////////////////////////////////
  void TRADEMGEN_Service::
  addSEVMGRService (SEVMGR::SEVMGR_ServicePtr_T ioSEVMGR_Service_ptr) {

    // Retrieve the TraDemGen service context
    assert (_trademgenServiceContext != NULL);
    TRADEMGEN_ServiceContext& lTRADEMGEN_ServiceContext =
      *_trademgenServiceContext;

    // Store the STDAIR service object within the (TRADEMGEN) service context
    lTRADEMGEN_ServiceContext.setSEVMGR_Service (ioSEVMGR_Service_ptr);
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
  parseAndLoad (const DemandFilePath& iDemandFilePath) { 

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

    // Retrieve the StdAir service object from the (TRADEMGEN) service context
    stdair::STDAIR_Service& lSTDAIR_Service =
      lTRADEMGEN_ServiceContext.getSTDAIR_Service();

    // Retrieve the persistent BOM root object.
    stdair::BomRoot& lPersistentBomRoot = 
      lSTDAIR_Service.getPersistentBomRoot();

    // Retrieve the pointer on the SEvMgr service handler.
    SEVMGR::SEVMGR_ServicePtr_T lSEVMGR_Service_ptr =
      lTRADEMGEN_ServiceContext.getSEVMGR_ServicePtr();
    
    // Retrieve the shared generator
    stdair::RandomGeneration& lSharedGenerator =
      lTRADEMGEN_ServiceContext.getUniformGenerator();

    // Retrieve the default POS distribution
    const POSProbabilityMass_T& lDefaultPOSProbabilityMass =
      lTRADEMGEN_ServiceContext.getPOSProbabilityMass();

    /**
     * 1. Parse the input file and initialise the demand generators
     */
    stdair::BasChronometer lDemandGeneration; lDemandGeneration.start();
    DemandParser::generateDemand (iDemandFilePath, lSEVMGR_Service_ptr,
                                  lSharedGenerator, lDefaultPOSProbabilityMass);
    const double lGenerationMeasure = lDemandGeneration.elapsed();  

    /**
     * 2. Delegate the complementary building of objects and links by the
     *    appropriate levels/components
     * \note Nothing to do for now.
     */  

    /**
     * 3. Build the complementary links
     */
    buildComplementaryLinks (lPersistentBomRoot);

    // DEBUG
    STDAIR_LOG_DEBUG ("Demand generation time: " << lGenerationMeasure);

    /**
     * 4. Have TraDemGen clone the whole persistent BOM tree, only when the StdAir
     *    service is owned by the current component (TraDemGen here)
     */
    if (doesOwnStdairService == true) {
      //
      clonePersistentBom ();
    }
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

    // Retrieve the persistent BOM root object.
    stdair::BomRoot& lPersistentBomRoot = 
      lSTDAIR_Service.getPersistentBomRoot();

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
     * \note Nothing to do for now.
     */  

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

    // Retrieve the pointer on the SEvMgr service handler.
    SEVMGR::SEVMGR_ServicePtr_T lSEVMGR_Service_ptr =
      lTRADEMGEN_ServiceContext.getSEVMGR_ServicePtr();

    // Delegate the BOM building to the dedicated service
    DemandManager::buildSampleBom (lSEVMGR_Service_ptr, lSharedGenerator,
                                   lDefaultPOSProbabilityMass);
    // Build the complementary links
    buildComplementaryLinks (lPersistentBomRoot);

    /**
     * 4. Have TraDemGen clone the whole persistent BOM tree, only when the 
     *    StdAir service is owned by the current component (TraDemGen here)
     */
    if (doesOwnStdairService == true) {
      //
      clonePersistentBom ();
    }
  }  

  // ////////////////////////////////////////////////////////////////////
  void TRADEMGEN_Service::clonePersistentBom () {   

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
     * 1. Have StdAir clone the whole persistent BOM tree, only when the StdAir
     *    service is owned by the current component (TraDemGen here)
     */
    if (doesOwnStdairService == true) {
      //
      lSTDAIR_Service.clonePersistentBom ();
    }  
 
    /**
     * 2. Build the complementary links
     */
    stdair::BomRoot& lBomRoot = lSTDAIR_Service.getBomRoot();   
    buildComplementaryLinks (lBomRoot);
  } 

  // ////////////////////////////////////////////////////////////////////
  void TRADEMGEN_Service::buildComplementaryLinks (stdair::BomRoot& ioBomRoot) {
    // Currently, no more things to do by TRADEMGEN at that stage.
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

  // ////////////////////////////////////////////////////////////////////
  std::string TRADEMGEN_Service::
  jsonHandler (const stdair::JSONString& iJSONString) const {

    // Retrieve the TraDemGen service context
    if (_trademgenServiceContext == NULL) {
      throw stdair::NonInitialisedServiceException ("The TraDemGen service has "
                                                    "not been initialised");
    }
    assert (_trademgenServiceContext != NULL);
    
    TRADEMGEN_ServiceContext& lTRADEMGEN_ServiceContext =
      *_trademgenServiceContext;

    // Retrieve the pointer on the SEvMgr service handler.
    SEVMGR::SEVMGR_ServicePtr_T lSEVMGR_Service_ptr =
      lTRADEMGEN_ServiceContext.getSEVMGR_ServicePtr();

    return lSEVMGR_Service_ptr->jsonHandler (iJSONString);
    
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

    // Retrieve the pointer on the SEvMgr service handler.
    SEVMGR::SEVMGR_ServicePtr_T lSEVMGR_Service_ptr =
      lTRADEMGEN_ServiceContext.getSEVMGR_ServicePtr();
    
    // Delegate the BOM building to the dedicated service
    return BomDisplay::csvDisplay (lSEVMGR_Service_ptr);

  }

  // //////////////////////////////////////////////////////////////////////
  std::string TRADEMGEN_Service::list() const {

    // Retrieve the TraDemGen service context
    if (_trademgenServiceContext == NULL) {
      throw stdair::NonInitialisedServiceException ("The TraDemGen service has "
                                                    "not been initialised");
    }
    assert (_trademgenServiceContext != NULL);

    TRADEMGEN_ServiceContext& lTRADEMGEN_ServiceContext =
      *_trademgenServiceContext;

    // Retrieve the pointer on the SEvMgr service handler.
    SEVMGR::SEVMGR_ServicePtr_T lSEVMGR_Service_ptr =
      lTRADEMGEN_ServiceContext.getSEVMGR_ServicePtr();
    
    // Delegate the BOM building to the dedicated service
    return lSEVMGR_Service_ptr->list ();

  }  

  // //////////////////////////////////////////////////////////////////////
  std::string TRADEMGEN_Service::
  list(const stdair::EventType::EN_EventType& iEventType) const {

    // Retrieve the TraDemGen service context
    if (_trademgenServiceContext == NULL) {
      throw stdair::NonInitialisedServiceException ("The TraDemGen service has "
                                                    "not been initialised");
    }
    assert (_trademgenServiceContext != NULL);

    TRADEMGEN_ServiceContext& lTRADEMGEN_ServiceContext =
      *_trademgenServiceContext;

    // Retrieve the pointer on the SEvMgr service handler.
    SEVMGR::SEVMGR_ServicePtr_T lSEVMGR_Service_ptr =
      lTRADEMGEN_ServiceContext.getSEVMGR_ServicePtr();
    
    // Delegate the BOM building to the dedicated service
    return lSEVMGR_Service_ptr->list (iEventType);

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

    // Retrieve the SEvMgr service context
    SEVMGR::SEVMGR_Service& lSEVMGR_Service =
      lTRADEMGEN_ServiceContext.getSEVMGR_Service();
    
    // Delegate the call to the dedicated command
    const stdair::Count_T& oExpectedTotalNumberOfRequestsToBeGenerated =
      lSEVMGR_Service.getExpectedTotalNumberOfEventsToBeGenerated (stdair::EventType::BKG_REQ);

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

    // Retrieve the SEvMgr service context
    SEVMGR::SEVMGR_Service& lSEVMGR_Service =
      lTRADEMGEN_ServiceContext.getSEVMGR_Service();
    
    // Delegate the call to the dedicated command
    const stdair::Count_T& oActualTotalNumberOfRequestsToBeGenerated =
      lSEVMGR_Service.getActualTotalNumberOfEventsToBeGenerated (stdair::EventType::BKG_REQ);

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

    // Retrieve the pointer on the SEvMgr service handler.
    SEVMGR::SEVMGR_ServicePtr_T lSEVMGR_Service_ptr =
      lTRADEMGEN_ServiceContext.getSEVMGR_ServicePtr();
    
    // Delegate the call to the dedicated command
    const bool oStillHavingRequestsToBeGenerated =
      DemandManager::stillHavingRequestsToBeGenerated (lSEVMGR_Service_ptr,
                                                       iKey, ioPSS,
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

    // Retrieve the pointer on the SEvMgr service handler.
    SEVMGR::SEVMGR_ServicePtr_T lSEVMGR_Service_ptr =
      lTRADEMGEN_ServiceContext.getSEVMGR_ServicePtr();

    // Retrieve the random generator
    stdair::RandomGeneration& lGenerator =
      lTRADEMGEN_ServiceContext.getUniformGenerator();

    // Delegate the call to the dedicated command
    const stdair::Count_T& oActualTotalNbOfEvents =
      DemandManager::generateFirstRequests (lSEVMGR_Service_ptr, lGenerator,
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

    // Retrieve the pointer on the SEvMgr service handler.
    SEVMGR::SEVMGR_ServicePtr_T lSEVMGR_Service_ptr =
      lTRADEMGEN_ServiceContext.getSEVMGR_ServicePtr();
    
    // Retrieve the random generator
    stdair::RandomGeneration& lGenerator =
      lTRADEMGEN_ServiceContext.getUniformGenerator();

    // Delegate the call to the dedicated command
    return DemandManager::generateNextRequest (lSEVMGR_Service_ptr,
                                               lGenerator, iKey,
                                               iDemandGenerationMethod);
  }

  // ////////////////////////////////////////////////////////////////////
  stdair::ProgressStatusSet TRADEMGEN_Service::
  popEvent (stdair::EventStruct& ioEventStruct) const {

    // Retrieve the TraDemGen service context
    assert (_trademgenServiceContext != NULL);
    TRADEMGEN_ServiceContext& lTRADEMGEN_ServiceContext =
      *_trademgenServiceContext;

    // Retrieve the SEvMgr service context
    SEVMGR::SEVMGR_Service& lSEVMGR_Service =
      lTRADEMGEN_ServiceContext.getSEVMGR_Service();
    
    // Extract the next event from the queue
    return lSEVMGR_Service.popEvent (ioEventStruct);
  }

  // ////////////////////////////////////////////////////////////////////
  bool TRADEMGEN_Service::isQueueDone() const {

    // Retrieve the TraDemGen service context
    assert (_trademgenServiceContext != NULL);
    TRADEMGEN_ServiceContext& lTRADEMGEN_ServiceContext =
      *_trademgenServiceContext;

    // Retrieve the SEvMgr service context
    SEVMGR::SEVMGR_Service& lSEVMGR_Service =
      lTRADEMGEN_ServiceContext.getSEVMGR_Service();
    
    // Calculates whether the event queue has been fully emptied
    const bool isQueueDone = lSEVMGR_Service.isQueueDone();

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

    // Update the status of cancellation events within the event queue. 
    const bool hasProgressStatus = 
      lSEVMGR_Service.hasProgressStatus(stdair::EventType::CX); 
    if (hasProgressStatus == false) { 
      const stdair::Count_T lCancellationNumber = 1;
      lSEVMGR_Service.addStatus (stdair::EventType::CX, lCancellationNumber); 
    } else { 
      stdair::Count_T lCurrentCancellationNumber = 
	lSEVMGR_Service.getActualTotalNumberOfEventsToBeGenerated (stdair::EventType::CX);
      lCurrentCancellationNumber++;
      lSEVMGR_Service.updateStatus (stdair::EventType::CX, lCurrentCancellationNumber);
    }

    return hasCancellationBeenGenerated;

  }

  // ////////////////////////////////////////////////////////////////////
  void TRADEMGEN_Service::reset() const {

    // Retrieve the TraDemGen service context
    assert (_trademgenServiceContext != NULL);
    TRADEMGEN_ServiceContext& lTRADEMGEN_ServiceContext =
      *_trademgenServiceContext;

    // Retrieve the pointer on the SEvMgr service handler.
    SEVMGR::SEVMGR_ServicePtr_T lSEVMGR_Service_ptr =
      lTRADEMGEN_ServiceContext.getSEVMGR_ServicePtr();

    // Retrieve the shared generator
    stdair::RandomGeneration& lSharedGenerator =
      lTRADEMGEN_ServiceContext.getUniformGenerator();
    
    // Delegate the call to the dedicated command
    DemandManager::reset (lSEVMGR_Service_ptr,
                          lSharedGenerator.getBaseGenerator());
  }  

  //////////////////////////////////////////////////////////////////////
  const stdair::ProgressStatus& TRADEMGEN_Service::getProgressStatus() const {    

    // Retrieve the TraDemGen service context
    assert (_trademgenServiceContext != NULL);
    TRADEMGEN_ServiceContext& lTRADEMGEN_ServiceContext =
      *_trademgenServiceContext;

    // Retrieve the pointer on the SEvMgr service handler.
    SEVMGR::SEVMGR_ServicePtr_T lSEVMGR_Service_ptr =
      lTRADEMGEN_ServiceContext.getSEVMGR_ServicePtr();

    // Delegate the call to the dedicated service
    return lSEVMGR_Service_ptr->getStatus();
  
  }

  //////////////////////////////////////////////////////////////////////
  const stdair::ProgressStatus& TRADEMGEN_Service::
  getProgressStatus (const stdair::EventType::EN_EventType& iEventType) const {     

    // Retrieve the TraDemGen service context
    assert (_trademgenServiceContext != NULL);
    TRADEMGEN_ServiceContext& lTRADEMGEN_ServiceContext =
      *_trademgenServiceContext;

    // Retrieve the pointer on the SEvMgr service handler.
    SEVMGR::SEVMGR_ServicePtr_T lSEVMGR_Service_ptr =
      lTRADEMGEN_ServiceContext.getSEVMGR_ServicePtr();

    // Delegate the call to the dedicated service
    return lSEVMGR_Service_ptr->getStatus(iEventType);
  
  }
  
  //////////////////////////////////////////////////////////////////////
  bool TRADEMGEN_Service::
  hasDemandStream (const stdair::DemandStreamKeyStr_T& iDemandStreamKey) const {

    // Retrieve the TraDemGen service context
    assert (_trademgenServiceContext != NULL);
    TRADEMGEN_ServiceContext& lTRADEMGEN_ServiceContext =
      *_trademgenServiceContext;

    // Retrieve the pointer on the SEvMgr service handler.
    SEVMGR::SEVMGR_ServicePtr_T lSEVMGR_Service_ptr =
      lTRADEMGEN_ServiceContext.getSEVMGR_ServicePtr();

    // Delegate the call to the dedicated service
    return lSEVMGR_Service_ptr->hasEventGenerator<DemandStream,
                                                  stdair::DemandStreamKeyStr_T>(iDemandStreamKey);
  }

  //////////////////////////////////////////////////////////////////////
  std::string TRADEMGEN_Service::displayDemandStream () const {

    // Retrieve the TraDemGen service context
    assert (_trademgenServiceContext != NULL);
    TRADEMGEN_ServiceContext& lTRADEMGEN_ServiceContext =
      *_trademgenServiceContext;

    // Retrieve the pointer on the SEvMgr service handler.
    SEVMGR::SEVMGR_ServicePtr_T lSEVMGR_Service_ptr =
      lTRADEMGEN_ServiceContext.getSEVMGR_ServicePtr();

    // Delegate the call to the dedicated service
    const DemandStreamList_T lDemandStreamList =
      lSEVMGR_Service_ptr->getEventGeneratorList<DemandStream>();

    // Output stream to store the display of demand streams.
    std::ostringstream  oStream;

    for (DemandStreamList_T::const_iterator itDemandStream =
           lDemandStreamList.begin(); itDemandStream !=
           lDemandStreamList.end(); itDemandStream++) {
      DemandStream* lDemandStream_ptr = *itDemandStream;
      assert (lDemandStream_ptr != NULL);
      oStream << lDemandStream_ptr->describeKey() << std::endl; 
      
    }
    return oStream.str();
  }

}

namespace SEVMGR {

  /**
   * Implementation of the SEvMgr service template methods for TraDemGen own
   * EventGenerator type: DemandStream.
   *
   * \note The declaration of these methods can be found in the SEvMgr service
   * header file.
   */
  
  // ////////////////////////////////////////////////////////////////////
  template<class EventGenerator>
  void SEVMGR_Service::addEventGenerator (EventGenerator& iEventGenerator) const {
    // Retrieve the StdAir service
    const stdair::STDAIR_Service& lSTDAIR_Service =
      this->getSTDAIR_Service();
    
    // Retrieve the BOM root object instance
    stdair::BomRoot& lPersistentBomRoot = 
      lSTDAIR_Service.getPersistentBomRoot();

    // Link the DemandStream to its parent (EventQueue)
    stdair::FacBomManager::linkWithParent (lPersistentBomRoot, iEventGenerator);
    
    // Add the DemandStream to the dedicated list and map
    stdair::FacBomManager::addToListAndMap (lPersistentBomRoot, 
					    iEventGenerator);
    
  }
  
  // ////////////////////////////////////////////////////////////////////
  template<class EventGenerator, class Key>
  EventGenerator& SEVMGR_Service::getEventGenerator(const Key& iKey) const {

    // Retrieve the StdAir service
    const stdair::STDAIR_Service& lSTDAIR_Service =
      this->getSTDAIR_Service(); 

    // Retrieve the BOM root object instance
    stdair::BomRoot& lPersistentBomRoot = 
      lSTDAIR_Service.getPersistentBomRoot();

    // Retrieve the DemandStream which corresponds to the given key.
    EventGenerator& lEventGenerator = 
      stdair::BomManager::getObject<EventGenerator> (lPersistentBomRoot, 
						     iKey);

    return lEventGenerator;
    
  }
  
  // ////////////////////////////////////////////////////////////////////
  template<class EventGenerator, class Key>
  bool SEVMGR_Service::hasEventGenerator(const Key& iKey) const {

    bool hasEventGenerator = true;

    // Retrieve the StdAir service
    const stdair::STDAIR_Service& lSTDAIR_Service =
      this->getSTDAIR_Service(); 

    // Retrieve the BOM root object instance
    stdair::BomRoot& lPersistentBomRoot = 
      lSTDAIR_Service.getPersistentBomRoot();

    // Retrieve the DemandStream which corresponds to the given key.
    EventGenerator* lEventGenerator_ptr = 
      stdair::BomManager::getObjectPtr<EventGenerator> (lPersistentBomRoot, 
							iKey);
    if (lEventGenerator_ptr == NULL) {
      hasEventGenerator = false;
    } 

    return hasEventGenerator;
    
  }
  
  // ////////////////////////////////////////////////////////////////////
  template<class EventGenerator>
  const std::list<EventGenerator*> SEVMGR_Service::getEventGeneratorList() const {

    // Retrieve the StdAir service
    const stdair::STDAIR_Service& lSTDAIR_Service =
      this->getSTDAIR_Service();   

    // Retrieve the BOM root object instance
    stdair::BomRoot& lPersistentBomRoot = 
      lSTDAIR_Service.getPersistentBomRoot();
    
    // Retrieve the DemandStream list
    const std::list<EventGenerator*> lEventGeneratorList =
      stdair::BomManager::getList<EventGenerator> (lPersistentBomRoot);

    return lEventGeneratorList;
  }

  // ////////////////////////////////////////////////////////////////////
  template<class EventGenerator>
  bool SEVMGR_Service::hasEventGeneratorList() const {

    // Retrieve the StdAir service
    const stdair::STDAIR_Service& lSTDAIR_Service =
      this->getSTDAIR_Service();

    // Retrieve the BOM root object instance
    stdair::BomRoot& lPersistentBomRoot = 
      lSTDAIR_Service.getPersistentBomRoot();

    const bool hasListEventGenerator =
      stdair::BomManager::hasList<EventGenerator> (lPersistentBomRoot);

    return hasListEventGenerator;
  }

  // ////////////////////////////////////////////////////////////////////  
  /**
   * Explicit template instantiations with the TraDemGen own EventGenerator
   * type: DemandStream.
   */
  template void SEVMGR_Service::
  addEventGenerator<TRADEMGEN::DemandStream> (TRADEMGEN::DemandStream&) const;

  template TRADEMGEN::DemandStream& SEVMGR_Service::
  getEventGenerator<TRADEMGEN::DemandStream, stdair::DemandStreamKeyStr_T> (const stdair::DemandStreamKeyStr_T&) const;

  template bool SEVMGR_Service::
  hasEventGenerator<TRADEMGEN::DemandStream, stdair::DemandStreamKeyStr_T> (const stdair::DemandStreamKeyStr_T&) const;

  template const TRADEMGEN::DemandStreamList_T SEVMGR_Service::
  getEventGeneratorList<TRADEMGEN::DemandStream> () const;

  template bool SEVMGR_Service::hasEventGeneratorList<TRADEMGEN::DemandStream>() const;
  // ////////////////////////////////////////////////////////////////////

}
