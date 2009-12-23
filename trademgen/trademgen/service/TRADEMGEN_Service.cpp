// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <sstream>
// Boost
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/posix_time/ptime.hpp>
// TRADEMGEN
#include <trademgen/basic/BasConst_TRADEMGEN_Service.hpp>
#include <trademgen/basic/BasChronometer.hpp>
#include <trademgen/command/SociSessionManager.hpp>
#include <trademgen/command/DBManager.hpp>
#include <trademgen/factory/FacTrademgenServiceContext.hpp>
#include <trademgen/service/TRADEMGEN_ServiceContext.hpp>
#include <trademgen/service/Logger.hpp>
#include <trademgen/TRADEMGEN_Service.hpp>
// Flood Utilities
#include <Flood/Utilities/Vector.h>
#include <Flood/Utilities/Matrix.h>
#include <Flood/Utilities/InputTargetDataSet.h>
// Flood Network architecture
#include <Flood/MultilayerPerceptron/MultilayerPerceptron.h>
// Flood Objective functional includes
#include <Flood/ObjectiveFunctional/MeanSquaredError.h>
// Flood Training algorithm includes
#include <Flood/TrainingAlgorithm/QuasiNewtonMethod.h>

namespace TRADEMGEN {

  // //////////////////////////////////////////////////////////////////////
  TRADEMGEN_Service::
  TRADEMGEN_Service (std::ostream& ioLogStream, const DBParams& iDBParams)
    : _trademgenServiceContext (NULL) {
    init (ioLogStream, iDBParams);
  }

  // //////////////////////////////////////////////////////////////////////
  TRADEMGEN_Service::TRADEMGEN_Service ()
    : _trademgenServiceContext (NULL) {
    assert (false);
  }

  // //////////////////////////////////////////////////////////////////////
  TRADEMGEN_Service::TRADEMGEN_Service (const TRADEMGEN_Service& iService) {
    assert (false);
  }

  // //////////////////////////////////////////////////////////////////////
  TRADEMGEN_Service::~TRADEMGEN_Service () {
    // Delete/Clean all the objects from memory
    finalise();
  }

  // //////////////////////////////////////////////////////////////////////
  void logInit (const LOG::EN_LogLevel iLogLevel,
                std::ostream& ioLogOutputFile) {
    Logger::instance().setLogParameters (iLogLevel, ioLogOutputFile);
  }

  // //////////////////////////////////////////////////////////////////////
  void TRADEMGEN_Service::init (std::ostream& ioLogStream,
                               const DBParams& iDBParams) {
    // Set the log file
    logInit (LOG::DEBUG, ioLogStream);

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
    TRADEMGEN_ServiceContext& lTRADEMGEN_ServiceContext= *_trademgenServiceContext;

    // Get the date-time for the present time
    boost::posix_time::ptime lNowDateTime =
      boost::posix_time::second_clock::local_time();
    boost::gregorian::date lNowDate = lNowDateTime.date();

    // DEBUG
    TRADEMGEN_LOG_DEBUG (std::endl
                        << "==================================================="
                        << std::endl
                        << lNowDateTime);

    try {
      
      // Retrieve the SOCI Session
      soci::session& lSociSession =
        lTRADEMGEN_ServiceContext.getSociSessionHandler();
      
      // Delegate the query execution to the dedicated command
      BasChronometer lTrademgenChronometer;
      lTrademgenChronometer.start();

      //
      oStr << "That is my request: hello world!";

      const double lTrademgenMeasure = lTrademgenChronometer.elapsed();

      // DEBUG
      TRADEMGEN_LOG_DEBUG ("Match query on Xapian database (index): "
                           << lTrademgenMeasure);
      
    } catch (const std::exception& error) {
      TRADEMGEN_LOG_ERROR ("Exception: "  << error.what());
      throw TrademgenCalculationException();
    }
  
    return oStr.str();
  }

}
