// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <sstream>
// StdAir
#include <stdair/basic/BasChronometer.hpp>
#include <stdair/bom/BomManager.hpp> // for display()
#include <stdair/bom/BookingRequestStruct.hpp>
#include <stdair/service/Logger.hpp>
// TraDemGen
#include <trademgen/basic/BasConst_TRADEMGEN_Service.hpp>
#include <trademgen/command/SociSessionManager.hpp>
#include <trademgen/command/DBManager.hpp>
#include <trademgen/factory/FacTrademgenServiceContext.hpp>
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

  // //////////////////////////////////////////////////////////////////////
  TRADEMGEN_Service::TRADEMGEN_Service (const DBParams& iDBParams)
    : _trademgenServiceContext (NULL) {

    // Initialise the context
    init (iDBParams);
  }

  // //////////////////////////////////////////////////////////////////////
  TRADEMGEN_Service::TRADEMGEN_Service (const stdair::BasLogParams& iLogParams,
                                        const DBParams& iDBParams)
    : _trademgenServiceContext (NULL) {
    
    // Set the log file
    logInit (iLogParams);

    // Initialise the (remaining of the) context
    init (iDBParams);
  }

  // //////////////////////////////////////////////////////////////////////
  TRADEMGEN_Service::~TRADEMGEN_Service () {
    // Delete/Clean all the objects from memory
    finalise();
  }

  // //////////////////////////////////////////////////////////////////////
  void TRADEMGEN_Service::logInit (const stdair::BasLogParams& iLogParams) {
    stdair::Logger::init (iLogParams);
  }

  // //////////////////////////////////////////////////////////////////////
  void TRADEMGEN_Service::init (const DBParams& iDBParams) {

    // Initialise the context
    TRADEMGEN_ServiceContext& lTRADEMGEN_ServiceContext = 
      FacTrademgenServiceContext::instance().create ("DEBUG_TO_BE_REMOVED");
    _trademgenServiceContext = &lTRADEMGEN_ServiceContext;

    // Initialise the SOCI Session
    soci::session* lSociSession_ptr = lTRADEMGEN_ServiceContext.getSociSession();
    SociSessionManager::init (lSociSession_ptr, iDBParams);
    assert (lSociSession_ptr != NULL);
    lTRADEMGEN_ServiceContext.setSociSession (*lSociSession_ptr);

    // Instanciate an empty World object
    // World& lWorld = FacWorld::instance().create();
    // lTRADEMGEN_ServiceContext.setWorld (lWorld);
  }
  
  // //////////////////////////////////////////////////////////////////////
  void TRADEMGEN_Service::finalise () {
    assert (_trademgenServiceContext != NULL);

    // Terminate the SOCI Session
    soci::session* lSociSession_ptr = _trademgenServiceContext->getSociSession();
    SociSessionManager::finalise (lSociSession_ptr);
  }

  // //////////////////////////////////////////////////////////////////////
  std::string TRADEMGEN_Service::calculateTrademgen () {
    std::ostringstream oStr;

    if (_trademgenServiceContext == NULL) {
      throw NonInitialisedServiceException();
    }
    assert (_trademgenServiceContext != NULL);
    TRADEMGEN_ServiceContext& lTRADEMGEN_ServiceContext =
      *_trademgenServiceContext;

    // Get the date-time for the present time
    boost::posix_time::ptime lNowDateTime =
      boost::posix_time::second_clock::local_time();
    boost::gregorian::date lNowDate = lNowDateTime.date();

    // DEBUG
    STDAIR_LOG_DEBUG (std::endl
                      << "==================================================="
                      << std::endl
                      << lNowDateTime);

    try {
      
      // Retrieve the SOCI Session
      soci::session& lSociSession =
        lTRADEMGEN_ServiceContext.getSociSessionHandler();
      
      // Delegate the query execution to the dedicated command
      stdair::BasChronometer lTrademgenChronometer;
      lTrademgenChronometer.start();

      //
      oStr << "That is my request: hello world!";

      const double lTrademgenMeasure = lTrademgenChronometer.elapsed();

      // DEBUG
      STDAIR_LOG_DEBUG ("Match query on Xapian database (index): "
                        << lTrademgenMeasure);
      
    } catch (const std::exception& error) {
      STDAIR_LOG_ERROR ("Exception: "  << error.what());
      throw TrademgenCalculationException();
    }
  
    return oStr.str();
  }

  // ////////////////////////////////////////////////////////////////////
  stdair::BookingRequestStruct TRADEMGEN_Service::
  generateBookingRequest () const {
    
      // TODO: remove this hardcoded section
      // Hardcode a booking request in order to simulate a sale.
      // Departure airport code
      stdair::AirportCode_T lOrigin ("LHR");
      // Arrival airport code
      stdair::AirportCode_T lDestination ("JFK");
      // Departure date
      stdair::Date_T lDepartureDate (2010, 01, 19);
      // Passenger type
      stdair::PassengerType_T lPaxType ("L");
      // Number of passengers in the travelling group
      stdair::NbOfSeats_T lPartySize = 5;

      // Booking request
      return stdair::BookingRequestStruct (lOrigin, lDestination,
                                           lDepartureDate, lPaxType, lPartySize);
  }

}
