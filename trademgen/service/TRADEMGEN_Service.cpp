// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <sstream>
// Boost
#include <boost/make_shared.hpp>
// SOCI
#include <soci/core/soci.h>
#include <soci/backends/mysql/soci-mysql.h>
// StdAir
#include <stdair/basic/BasChronometer.hpp>
#include <stdair/basic/BasFileMgr.hpp>
#include <stdair/basic/BasConst_General.hpp>
#include <stdair/bom/BomManager.hpp> // for display()
#include <stdair/bom/BomRoot.hpp>
#include <stdair/bom/AirlineStruct.hpp>
#include <stdair/bom/BookingRequestStruct.hpp>
#include <stdair/bom/EventStruct.hpp>
#include <stdair/bom/EventQueue.hpp>
#include <stdair/command/DBManagerForAirlines.hpp>
#include <stdair/service/Logger.hpp>
#include <stdair/service/DBSessionManager.hpp>
#include <stdair/STDAIR_Service.hpp>
// TraDemGen
#include <trademgen/basic/BasConst_TRADEMGEN_Service.hpp>
#include <trademgen/bom/DemandStreamKey.hpp>
#include <trademgen/factory/FacTRADEMGENServiceContext.hpp>
#include <trademgen/command/DemandParser.hpp>
#include <trademgen/command/DemandManager.hpp>
#include <trademgen/service/TRADEMGEN_ServiceContext.hpp>
#include <trademgen/TRADEMGEN_Service.hpp>

namespace TRADEMGEN {

  // //////////////////////////////////////////////////////////////////////
  TRADEMGEN_Service::TRADEMGEN_Service ()
    : _trademgenServiceContext (NULL) {
    assert (false);
  }

  // //////////////////////////////////////////////////////////////////////
  TRADEMGEN_Service::TRADEMGEN_Service (const TRADEMGEN_Service& iService) 
    : _trademgenServiceContext (NULL) {
    assert (false);
  }

  // ////////////////////////////////////////////////////////////////////
  TRADEMGEN_Service::
  TRADEMGEN_Service (stdair::STDAIR_ServicePtr_T ioSTDAIR_ServicePtr,
                     const stdair::Filename_T& iDemandInputFilename)
    : _trademgenServiceContext (NULL) {

    // Initialise the service context
    initServiceContext();
    
    // Retrieve the Trademgen service context
    assert (_trademgenServiceContext != NULL);
    TRADEMGEN_ServiceContext& lTRADEMGEN_ServiceContext =
      *_trademgenServiceContext;
    
    // Store the STDAIR service object within the (TRADEMGEN) service context
    lTRADEMGEN_ServiceContext.setSTDAIR_Service (ioSTDAIR_ServicePtr);
    
    // Initialise the context
    init (iDemandInputFilename);
  }

  // //////////////////////////////////////////////////////////////////////
  TRADEMGEN_Service::
  TRADEMGEN_Service (const stdair::BasLogParams& iLogParams,
                     const stdair::BasDBParams& iDBParams,
                     const stdair::Filename_T& iDemandInputFilename)
    : _trademgenServiceContext (NULL) {
    
    // Initialise the service context
    initServiceContext ();
    
    // Initialise the STDAIR service handler
    initStdAirService (iLogParams, iDBParams);
    
    // Initialise the (remaining of the) context
    init (iDemandInputFilename);
  }

  // //////////////////////////////////////////////////////////////////////
  TRADEMGEN_Service::
  TRADEMGEN_Service (const stdair::BasLogParams& iLogParams,
                     const stdair::Filename_T& iDemandInputFilename)
    : _trademgenServiceContext (NULL) {
    
    // Initialise the service context
    initServiceContext ();
    
    // Initialise the STDAIR service handler
    initStdAirService (iLogParams);
    
    // Initialise the (remaining of the) context
    init (iDemandInputFilename);
  }

  // //////////////////////////////////////////////////////////////////////
  TRADEMGEN_Service::~TRADEMGEN_Service () {

    // Retrieve the TraDemGen service context
    assert (_trademgenServiceContext != NULL);
    TRADEMGEN_ServiceContext& lTRADEMGEN_ServiceContext =
      *_trademgenServiceContext;

    // Retrieve the StdAir service context
    stdair::STDAIR_ServicePtr_T lSTDAIR_Service =
      lTRADEMGEN_ServiceContext.getSTDAIR_ServicePtr();
    assert (lSTDAIR_Service != NULL);
  }

  // //////////////////////////////////////////////////////////////////////
  void TRADEMGEN_Service::initServiceContext () {
    // Initialise the service context
    TRADEMGEN_ServiceContext& lTRADEMGEN_ServiceContext = 
      FacTRADEMGENServiceContext::instance().create ();
    _trademgenServiceContext = &lTRADEMGEN_ServiceContext;
  }

  // //////////////////////////////////////////////////////////////////////
  void TRADEMGEN_Service::
  initStdAirService (const stdair::BasLogParams& iLogParams,
                     const stdair::BasDBParams& iDBParams) {

    // Retrieve the TraDemGen service context
    assert (_trademgenServiceContext != NULL);
    TRADEMGEN_ServiceContext& lTRADEMGEN_ServiceContext =
      *_trademgenServiceContext;
    
    /**
       Initialise the STDAIR service handler.
       \note The track on the object memory is kept thanks to the Boost
       Smart Pointers component.
    */
    stdair::STDAIR_ServicePtr_T lSTDAIR_Service_ptr = 
      boost::make_shared<stdair::STDAIR_Service> (iLogParams, iDBParams);
    assert (lSTDAIR_Service_ptr != NULL);
    
    // Store the STDAIR service object within the (TRADEMGEN) service context
    lTRADEMGEN_ServiceContext.setSTDAIR_Service (lSTDAIR_Service_ptr);
  }
  
  // //////////////////////////////////////////////////////////////////////
  void TRADEMGEN_Service::
  initStdAirService (const stdair::BasLogParams& iLogParams) {

    // Retrieve the TraDemGen service context
    assert (_trademgenServiceContext != NULL);
    TRADEMGEN_ServiceContext& lTRADEMGEN_ServiceContext =
      *_trademgenServiceContext;
    
    /**
       Initialise the STDAIR service handler.
       \note The track on the object memory is kept thanks to the Boost
       Smart Pointers component.
    */
    stdair::STDAIR_ServicePtr_T lSTDAIR_Service_ptr = 
      boost::make_shared<stdair::STDAIR_Service> (iLogParams);
    assert (lSTDAIR_Service_ptr != NULL);
    
    // Store the STDAIR service object within the (TRADEMGEN) service context
    lTRADEMGEN_ServiceContext.setSTDAIR_Service (lSTDAIR_Service_ptr);
  }
  
  // //////////////////////////////////////////////////////////////////////
  void TRADEMGEN_Service::
  init (const stdair::Filename_T& iDemandInputFilename) {

    // Check that the file path given as input corresponds to an actual file
    const bool doesExistAndIsReadable =
      stdair::BasFileMgr::doesExistAndIsReadable (iDemandInputFilename);
    if (doesExistAndIsReadable == false) {
      STDAIR_LOG_ERROR ("The demand input file '" << iDemandInputFilename
                        << "' does not exist or can not be read");
      
      throw DemandInputFileNotFoundException ("The demand file '"
                                              + iDemandInputFilename
                                              + "' does not exist or can not "
                                              + "be read");
    }

    // Retrieve the TraDemGen service context
    assert (_trademgenServiceContext != NULL);
    TRADEMGEN_ServiceContext& lTRADEMGEN_ServiceContext =
      *_trademgenServiceContext;

    // Retrieve the StdAir service context
    stdair::STDAIR_ServicePtr_T lSTDAIR_Service =
      lTRADEMGEN_ServiceContext.getSTDAIR_ServicePtr();
    assert (lSTDAIR_Service != NULL);

    // Retrieve the shared generator
    stdair::UniformGenerator_T& lSharedGenerator =
      lTRADEMGEN_ServiceContext.getUniformGenerator();

    // Retrieve the default POS distribution
    const POSProbabilityMass_T& lDefaultPOSProbabilityMass =
      lTRADEMGEN_ServiceContext.getPOSProbabilityMass();
    
    // Get the root of the BOM tree, on which all of the other BOM objects
    // will be attached
    stdair::BomRoot& lBomRoot = lSTDAIR_Service->getBomRoot();

    // Initialise the event queue
    stdair::EventQueue& lEventQueue = DemandManager::initEventQueue (lBomRoot);

    // Store a reference of the EventQueue object instance within the
    // service context
    lTRADEMGEN_ServiceContext.setEventQueue (lEventQueue);
    
    // Parse the input file and initialise the demand generators
    DemandParser::generateDemand (iDemandInputFilename, lBomRoot,
                                  lSharedGenerator, lDefaultPOSProbabilityMass);
  }
  
  // //////////////////////////////////////////////////////////////////////
  void TRADEMGEN_Service::displayAirlineListFromDB () const {

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
    boost::gregorian::date lNowDate = lNowDateTime.date();

    // DEBUG
    STDAIR_LOG_DEBUG (std::endl
                      << "==================================================="
                      << std::endl
                      << lNowDateTime);

    // Delegate the query execution to the dedicated command
    stdair::BasChronometer lDsimChronometer;
    lDsimChronometer.start();

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
    
    const double lDsimMeasure = lDsimChronometer.elapsed();
    
    // DEBUG
    STDAIR_LOG_DEBUG ("Sample service for Dsim: " << lDsimMeasure);
  }

  // ////////////////////////////////////////////////////////////////////
  /*
  const stdair::NbOfRequests_T& TRADEMGEN_Service::
  getTotalNumberOfRequestsToBeGenerated (const stdair::DemandStreamKeyStr_T& iKey) const {
    
    // Retrieve the TraDemGen service context
    assert (_trademgenServiceContext != NULL);
    TRADEMGEN_ServiceContext& lTRADEMGEN_ServiceContext =
      *_trademgenServiceContext;

    // Retrieve the StdAir service context
    stdair::STDAIR_ServicePtr_T lSTDAIR_Service =
      lTRADEMGEN_ServiceContext.getSTDAIR_ServicePtr();
    assert (lSTDAIR_Service != NULL);
    
    // Get the root of the BOM tree, on which all of the other BOM objects
    // will be attached
    stdair::BomRoot& lBomRoot = lSTDAIR_Service->getBomRoot();

    // Delegate the call to the dedicated command
    const stdair::NbOfRequests_T& oTotalNumberOfRequestsToBeGenerated =
      DemandManager::getTotalNumberOfRequestsToBeGenerated (lBomRoot, iKey);

    //
    return oTotalNumberOfRequestsToBeGenerated;
  }
  */

  // ////////////////////////////////////////////////////////////////////
  const stdair::NbOfRequests_T& TRADEMGEN_Service::
  getTotalNumberOfRequestsToBeGenerated() const {
    
    // Retrieve the TraDemGen service context
    assert (_trademgenServiceContext != NULL);
    TRADEMGEN_ServiceContext& lTRADEMGEN_ServiceContext =
      *_trademgenServiceContext;

    // Retrieve the StdAir service context
    stdair::STDAIR_ServicePtr_T lSTDAIR_Service =
      lTRADEMGEN_ServiceContext.getSTDAIR_ServicePtr();
    assert (lSTDAIR_Service != NULL);
    
    // Get the root of the BOM tree, on which all of the other BOM objects
    // will be attached
    stdair::BomRoot& lBomRoot = lSTDAIR_Service->getBomRoot();

    // Delegate the call to the dedicated command
    const stdair::NbOfRequests_T& oExpectedTotalNumberOfRequestsToBeGenerated =
      DemandManager::getTotalNumberOfRequestsToBeGenerated (lBomRoot);

    //
    return oExpectedTotalNumberOfRequestsToBeGenerated;
  }

  // ////////////////////////////////////////////////////////////////////
  const bool TRADEMGEN_Service::
  stillHavingRequestsToBeGenerated (const stdair::DemandStreamKeyStr_T& iKey) const {
    
    // Retrieve the TraDemGen service context
    assert (_trademgenServiceContext != NULL);
    TRADEMGEN_ServiceContext& lTRADEMGEN_ServiceContext =
      *_trademgenServiceContext;

    // Retrieve the StdAir service context
    stdair::STDAIR_ServicePtr_T lSTDAIR_Service =
      lTRADEMGEN_ServiceContext.getSTDAIR_ServicePtr();
    assert (lSTDAIR_Service != NULL);
    
    // Get the root of the BOM tree, on which all of the other BOM objects
    // will be attached
    stdair::BomRoot& lBomRoot = lSTDAIR_Service->getBomRoot();

    // Delegate the call to the dedicated command
    const bool oStillHavingRequestsToBeGenerated =
      DemandManager::stillHavingRequestsToBeGenerated (lBomRoot, iKey);

    //
    return oStillHavingRequestsToBeGenerated;
  }

  // ////////////////////////////////////////////////////////////////////
  stdair::Count_T TRADEMGEN_Service::generateFirstRequests() const {

    // Retrieve the TraDemGen service context
    assert (_trademgenServiceContext != NULL);
    TRADEMGEN_ServiceContext& lTRADEMGEN_ServiceContext =
      *_trademgenServiceContext;

    // Retrieve the StdAir service context
    stdair::STDAIR_ServicePtr_T lSTDAIR_Service =
      lTRADEMGEN_ServiceContext.getSTDAIR_ServicePtr();
    assert (lSTDAIR_Service != NULL);
    
    // Get the root of the BOM tree, on which all of the other BOM objects
    // will be attached
    stdair::BomRoot& lBomRoot = lSTDAIR_Service->getBomRoot();

    // Delegate the call to the dedicated command
    const stdair::Count_T& oActualTotalNbOfEvents =
      DemandManager::generateFirstRequests (lBomRoot);

    //
    return oActualTotalNbOfEvents;
  }

  // ////////////////////////////////////////////////////////////////////
  stdair::BookingRequestPtr_T TRADEMGEN_Service::
  generateNextRequest (const stdair::DemandStreamKeyStr_T& iKey) const {

    // Retrieve the TraDemGen service context
    assert (_trademgenServiceContext != NULL);
    TRADEMGEN_ServiceContext& lTRADEMGEN_ServiceContext =
      *_trademgenServiceContext;

    // Retrieve the StdAir service context
    stdair::STDAIR_ServicePtr_T lSTDAIR_Service =
      lTRADEMGEN_ServiceContext.getSTDAIR_ServicePtr();
    assert (lSTDAIR_Service != NULL);
    
    // Get the root of the BOM tree, on which all of the other BOM objects
    // will be attached
    stdair::BomRoot& lBomRoot = lSTDAIR_Service->getBomRoot();

    // Delegate the call to the dedicated command
    return DemandManager::generateNextRequest (lBomRoot, iKey);
  }

  // ////////////////////////////////////////////////////////////////////
  stdair::EventStruct TRADEMGEN_Service::popEvent() const {

    // Retrieve the TraDemGen service context
    assert (_trademgenServiceContext != NULL);
    TRADEMGEN_ServiceContext& lTRADEMGEN_ServiceContext =
      *_trademgenServiceContext;

    // Retrieve the event queue object instance
    stdair::EventQueue& lQueue = lTRADEMGEN_ServiceContext.getEventQueue();
    
    // Extract the next event from the queue
    const stdair::EventStruct& oEventStruct = lQueue.popEvent();

    //
    return oEventStruct;
  }

  // ////////////////////////////////////////////////////////////////////
  bool TRADEMGEN_Service::isQueueDone() const {

    // Retrieve the TraDemGen service context
    assert (_trademgenServiceContext != NULL);
    TRADEMGEN_ServiceContext& lTRADEMGEN_ServiceContext =
      *_trademgenServiceContext;

    // Retrieve the event queue object instance
    stdair::EventQueue& lQueue = lTRADEMGEN_ServiceContext.getEventQueue();
    
    // Calculates whether the event queue has been fully emptied
    const bool isQueueDone = lQueue.isQueueDone();

    //
    return isQueueDone;
  }

  // ////////////////////////////////////////////////////////////////////
  void TRADEMGEN_Service::
  initProgressDisplays (stdair::ProgressDisplayMap_T& ioProgressDisplayMap) const {

    // Retrieve the TraDemGen service context
    assert (_trademgenServiceContext != NULL);
    TRADEMGEN_ServiceContext& lTRADEMGEN_ServiceContext =
      *_trademgenServiceContext;

    // Retrieve the event queue object instance
    stdair::EventQueue& lQueue = lTRADEMGEN_ServiceContext.getEventQueue();
    
    // Initialise the map of Boost progress_display objects
    lQueue.initProgressDisplays (ioProgressDisplayMap);
  }

  // ////////////////////////////////////////////////////////////////////
  void TRADEMGEN_Service::reset() const {

    // Retrieve the TraDemGen service context
    assert (_trademgenServiceContext != NULL);
    TRADEMGEN_ServiceContext& lTRADEMGEN_ServiceContext =
      *_trademgenServiceContext;
    
    // Retrieve the StdAir service context
    stdair::STDAIR_ServicePtr_T lSTDAIR_Service =
      lTRADEMGEN_ServiceContext.getSTDAIR_ServicePtr();
    assert (lSTDAIR_Service != NULL);

    // Get the root of the BOM tree, on which all of the other BOM
    // objects will be attached
    stdair::BomRoot& lBomRoot = lSTDAIR_Service->getBomRoot();

    // Delegate the call to the dedicated command
    DemandManager::reset (lBomRoot);
  }
}
