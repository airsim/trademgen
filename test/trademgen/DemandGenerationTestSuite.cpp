/*!
 * \page DemandGenerationTestSuite_cpp Command-Line Test to Demonstrate How To Use TraDemGen elements
 * \code
 */
// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <sstream>
#include <fstream>
#include <map>
#include <cmath>
// Boost Unit Test Framework (UTF)
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#define BOOST_TEST_MODULE DemandGenerationTest
#include <boost/test/unit_test.hpp>
// StdAir
#include <stdair/stdair_basic_types.hpp>
#include <stdair/basic/BasConst_General.hpp>
#include <stdair/basic/BasLogParams.hpp>
#include <stdair/basic/BasDBParams.hpp>
#include <stdair/basic/BasFileMgr.hpp>
#include <stdair/basic/ProgressStatusSet.hpp>
#include <stdair/bom/EventStruct.hpp>
#include <stdair/bom/BookingRequestStruct.hpp>
#include <stdair/service/Logger.hpp>
// TraDemGen
#include <trademgen/TRADEMGEN_Exceptions.hpp>
#include <trademgen/TRADEMGEN_Service.hpp>
#include <trademgen/bom/DemandStreamKey.hpp>
#include <trademgen/config/trademgen-paths.hpp>

namespace boost_utf = boost::unit_test;

// (Boost) Unit Test XML Report
std::ofstream utfReportStream ("DemandGenerationTestSuite_utfresults.xml");

/**
 * Configuration for the Boost Unit Test Framework (UTF)
 */
struct UnitTestConfig {
  /** Constructor. */
  UnitTestConfig() {
    boost_utf::unit_test_log.set_stream (utfReportStream);
#if BOOST_VERSION_MACRO >= 105900
    boost_utf::unit_test_log.set_format (boost_utf::OF_XML);
#else // BOOST_VERSION_MACRO
    boost_utf::unit_test_log.set_format (boost_utf::XML);
#endif // BOOST_VERSION_MACRO
    boost_utf::unit_test_log.set_threshold_level (boost_utf::log_test_units);
    //boost_utf::unit_test_log.set_threshold_level (boost_utf::log_successful_tests);
  }
  
  /** Destructor. */
  ~UnitTestConfig() {
  }
};

// Specific type definitions
typedef std::pair<stdair::Count_T, stdair::Count_T> NbOfEventsPair_T;
typedef std::map<const stdair::DemandStreamKeyStr_T,
                 NbOfEventsPair_T> NbOfEventsByDemandStreamMap_T;

// //////////////////////////////////////////////////////////////////////
/**
 * Generate booking requests using demand streams.
 */
void testDemandGenerationHelper (const unsigned short iTestFlag,
                                 const stdair::Filename_T& iDemandInputFilename,
                                 const stdair::DemandGenerationMethod& iDemandGenerationMethod,
                                 const bool isBuiltin) {

  // Seed for the random generation
  const stdair::RandomSeed_T lRandomSeed = stdair::DEFAULT_RANDOM_SEED;
  
  // Output log File
  std::ostringstream oStr;
  oStr << "DemandGenerationTestSuite_" << iTestFlag << ".log";
  const stdair::Filename_T lLogFilename (oStr.str());

  // Set the log parameters
  std::ofstream logOutputFile;
  // Open and clean the log outputfile
  logOutputFile.open (lLogFilename.c_str());
  logOutputFile.clear();

  // Initialise the TraDemGen service object
  const stdair::BasLogParams lLogParams (stdair::LOG::DEBUG, logOutputFile);
  TRADEMGEN::TRADEMGEN_Service trademgenService (lLogParams, lRandomSeed);

  /**
   * Initialise the current number of generated events and the
   * expected total numbers of requests to be generated, depending on
   * the demand streams.
   *
   * The current number of generated events starts at one, for each demand
   * stream, because the initialisation step generates exactly one event
   * for each demand stream.
   */
  NbOfEventsByDemandStreamMap_T lNbOfEventsMap;
  
  // Total number of events
  stdair::Count_T lRefExpectedNbOfEvents (0);
  stdair::Count_T lRefActualNbOfEvents (0);

  // Check whether or not a (CSV) input file should be read
  if (isBuiltin == true) {

    // Build the default sample BOM tree (filled with demand streams) for TraDemGen
    trademgenService.buildSampleBom();

    lNbOfEventsMap.insert (NbOfEventsByDemandStreamMap_T::
                           value_type ("SIN-BKK 2010-Feb-08 Y",
                                       NbOfEventsPair_T (4, 60)));
    lNbOfEventsMap.insert (NbOfEventsByDemandStreamMap_T::
                           value_type ("BKK-HKG 2010-Feb-08 Y",
                                       NbOfEventsPair_T (4, 60)));
    lNbOfEventsMap.insert (NbOfEventsByDemandStreamMap_T::
                           value_type ("SIN-HKG 2010-Feb-08 Y",
                                       NbOfEventsPair_T (4, 60)));

    // Total number of events, for the 3 demand streams: 180
    lRefExpectedNbOfEvents = 180;
    lRefActualNbOfEvents = 186;

  } else {

    // Create the DemandStream objects, and insert them within the BOM tree
    const TRADEMGEN::DemandFilePath lDemandFilePath (iDemandInputFilename);
    trademgenService.parseAndLoad (lDemandFilePath);

    lNbOfEventsMap.insert (NbOfEventsByDemandStreamMap_T::
                           value_type ("SIN-HND 2010-Feb-08 Y",
                                       NbOfEventsPair_T (1, 10)));
    lNbOfEventsMap.insert (NbOfEventsByDemandStreamMap_T::
                           value_type ("SIN-HND 2010-Feb-09 Y",
                                       NbOfEventsPair_T (1, 10)));
    lNbOfEventsMap.insert (NbOfEventsByDemandStreamMap_T::
                           value_type ("SIN-BKK 2010-Feb-08 Y",
                                       NbOfEventsPair_T (1, 10)));
    lNbOfEventsMap.insert (NbOfEventsByDemandStreamMap_T::
                           value_type ("SIN-BKK 2010-Feb-09 Y",
                                       NbOfEventsPair_T (1, 10)));

    // Total number of events, for the 4 demand streams: 40
    lRefExpectedNbOfEvents = 40;
    lRefActualNbOfEvents = 40;
  }
  
  // Retrieve the expected (mean value of the) number of events to be
  // generated
  const stdair::Count_T& lExpectedNbOfEventsToBeGenerated =
    trademgenService.getExpectedTotalNumberOfRequestsToBeGenerated();

  BOOST_CHECK_EQUAL (lRefExpectedNbOfEvents,
                     std::floor (lExpectedNbOfEventsToBeGenerated));
  
  BOOST_CHECK_MESSAGE (lRefExpectedNbOfEvents ==
                       std::floor (lExpectedNbOfEventsToBeGenerated),
                       "Expected total number of requests to be generated: "
                       << lExpectedNbOfEventsToBeGenerated
                       << " (=> "
                       << std::floor (lExpectedNbOfEventsToBeGenerated)
                       << "). Reference value: " << lRefExpectedNbOfEvents);

  /**
   * Initialisation step.
   *
   * Generate the first event for each demand stream.
   */
  const stdair::Count_T& lActualNbOfEventsToBeGenerated =
    trademgenService.generateFirstRequests(iDemandGenerationMethod);

  // DEBUG
  STDAIR_LOG_DEBUG ("Expected number of events: "
                    << lExpectedNbOfEventsToBeGenerated << ", actual: "
                    << lActualNbOfEventsToBeGenerated);
  
  // Total number of events, for all the demand streams:
  BOOST_CHECK_GE (lRefActualNbOfEvents, lActualNbOfEventsToBeGenerated - 10.0);
  BOOST_CHECK_LE (lRefActualNbOfEvents, lActualNbOfEventsToBeGenerated + 10.0);
  
  BOOST_CHECK_MESSAGE (lRefActualNbOfEvents<=lActualNbOfEventsToBeGenerated+10.0
                       &&lRefActualNbOfEvents>=lActualNbOfEventsToBeGenerated-10.0,
                       "Actual total number of requests to be generated: "
                       << lExpectedNbOfEventsToBeGenerated
                       << " (=> "
                       << std::floor (lExpectedNbOfEventsToBeGenerated)
                       << "). Reference value: " << lRefActualNbOfEvents);

  /** Is the queue empty? */
  const bool isQueueDone = trademgenService.isQueueDone();
  BOOST_REQUIRE_MESSAGE (isQueueDone == false,
                         "The event queue should not be empty.");

  /**
     Main loop.
     <ul>
     <li>Pop a request and get its associated type/demand stream.</li>
     <li>Generate the next request for the same type/demand stream.</li>
     </ul>
  */
  stdair::Count_T idx = 1;
  while (trademgenService.isQueueDone() == false) {

    // Get the next event from the event queue
    stdair::EventStruct lEventStruct;
    stdair::ProgressStatusSet lPPS = trademgenService.popEvent (lEventStruct);

    // DEBUG
    STDAIR_LOG_DEBUG ("Poped event: '" << lEventStruct.describe() << "'.");
      
    // Extract the corresponding demand/booking request
    const stdair::BookingRequestStruct& lPoppedRequest =
      lEventStruct.getBookingRequest();
    
    // DEBUG
    STDAIR_LOG_DEBUG ("Poped booking request: '"
                      << lPoppedRequest.describe() << "'.");
    
    // Retrieve the corresponding demand stream
    const stdair::DemandGeneratorKey_T& lDemandStreamKey =
      lPoppedRequest.getDemandGeneratorKey();

    // Check that the number of booking requests to be generated are correct
    const NbOfEventsByDemandStreamMap_T::iterator itNbOfEventsMap =
      lNbOfEventsMap.find (lDemandStreamKey);
    BOOST_REQUIRE_MESSAGE (itNbOfEventsMap != lNbOfEventsMap.end(),
                           "The demand stream key '" << lDemandStreamKey
                           << "' is not expected in that test");

    /**
       For that demand stream, retrieve:
        <ul>
        <li>The current number of events</li>
        <li>The expected total number of events to be generated. That
        number is just hard coded for that test (it does not correspond
        to an automatically generated number)</li>
        </ul>
    */
    const NbOfEventsPair_T& lNbOfEventsPair = itNbOfEventsMap->second;
    stdair::Count_T lCurrentNbOfEvents = lNbOfEventsPair.first;
    const stdair::Count_T& lExpectedTotalNbOfEvents = lNbOfEventsPair.second;

    // Assess whether more events should be generated for that demand stream
    const bool stillHavingRequestsToBeGenerated = trademgenService.
      stillHavingRequestsToBeGenerated (lDemandStreamKey, lPPS,
                                        iDemandGenerationMethod);

    /**
       The first time an event is popped from the queue for that demand stream,
       check that the actual total number of requests to be generated (as
       calculated by the demand stream itself during the initialisation
       step), is equal to the expected number.
    */
    if (lCurrentNbOfEvents == 1) {
      /**
         Retrieve, from the demand stream, the total number of events
         to be generated, so that that number can be compared to the
         expected one.
      */
      const stdair::ProgressStatus& lDemandStreamProgressStatus =
        lPPS.getSpecificGeneratorStatus();
      const stdair::Count_T& lNbOfRequests =
        lDemandStreamProgressStatus.getExpectedNb();

      BOOST_CHECK_EQUAL (lNbOfRequests, lExpectedTotalNbOfEvents);
      BOOST_CHECK_MESSAGE (lNbOfRequests == lExpectedTotalNbOfEvents,
                           "[" << lDemandStreamKey
                           << "] Total number of requests to be generated: "
                           << lNbOfRequests << "). Expected value: "
                           << lExpectedTotalNbOfEvents);
    }

    // DEBUG
    STDAIR_LOG_DEBUG ("=> [" << lDemandStreamKey << "][" << lCurrentNbOfEvents
                      << "/" << lExpectedTotalNbOfEvents
                      << "] is now processed. "
                      << "Still generate events for that demand stream? "
                      << stillHavingRequestsToBeGenerated);

    // If there are still events to be generated for that demand stream,
    // generate and add them to the event queue
    if (stillHavingRequestsToBeGenerated == true) {
      const stdair::BookingRequestPtr_T lNextRequest_ptr =
        trademgenService.generateNextRequest (lDemandStreamKey,
                                              iDemandGenerationMethod);
      assert (lNextRequest_ptr != NULL);

      /**
         Sanity check
         <br>The date-time of the next event must be greater than
         the date-time of the current event.
      */
      const stdair::Duration_T lDuration =
        lNextRequest_ptr->getRequestDateTime()
        - lPoppedRequest.getRequestDateTime();
      BOOST_REQUIRE_GT (lDuration.total_milliseconds(), 0);
      BOOST_REQUIRE_MESSAGE (lDuration.total_milliseconds() > 0,
                             "[" << lDemandStreamKey
                             << "] The date-time of the generated event ("
                             << lNextRequest_ptr->getRequestDateTime()
                             << ") is lower than the date-time "
                             << "of the current event ("
                             << lPoppedRequest.getRequestDateTime() << ")");
      
      // DEBUG
      STDAIR_LOG_DEBUG ("[" << lDemandStreamKey << "][" << lCurrentNbOfEvents
                        << "/" << lExpectedTotalNbOfEvents
                        << "] Added request: '" << lNextRequest_ptr->describe()
                        << "'. Is queue done? "
                        << trademgenService.isQueueDone());

      // Keep, within the dedicated map, the current counters of events updated.
      ++lCurrentNbOfEvents;
      itNbOfEventsMap->second = NbOfEventsPair_T (lCurrentNbOfEvents,
                                                  lExpectedTotalNbOfEvents);
    }
    
    // Iterate
    ++idx;
  }
  // Compensate for the last iteration
  --idx;

  if (iDemandGenerationMethod == stdair::DemandGenerationMethod::STA_ORD) {
    //
    BOOST_CHECK_GE (idx, lRefActualNbOfEvents - 10.0);
    BOOST_CHECK_LE (idx, lRefActualNbOfEvents + 10.0);
    BOOST_CHECK_MESSAGE (idx >= lRefActualNbOfEvents - 10.0
                         && idx <= lRefActualNbOfEvents + 10.0,
                         "The total actual number of events is "
                         << lRefActualNbOfEvents << ", but " << idx
                         << " events have been generated");
  }
  
  /** Reset the context of the demand streams for another demand generation
      without having to reparse the demand input file. */
  trademgenService.reset();

  // DEBUG
  STDAIR_LOG_DEBUG ("End of the simulation");

  // Close the log file
  logOutputFile.close();

}


// /////////////// Main: Unit Test Suite //////////////

// Set the UTF configuration (re-direct the output to a specific file)
BOOST_GLOBAL_FIXTURE (UnitTestConfig);

// Start the test suite
BOOST_AUTO_TEST_SUITE (master_test_suite)

/**
 * Test a simple demand generation
 */
BOOST_AUTO_TEST_CASE (trademgen_simple_simulation_test) {

  // Input file name
  const stdair::Filename_T lInputFilename (STDAIR_SAMPLE_DIR "/demand01.csv");

  // Generate the date time of the requests with the statistic order method.
  const stdair::DemandGenerationMethod lDemandGenerationMethod (stdair::DemandGenerationMethod::STA_ORD);
  
  // State whether the BOM tree should be built-in or parsed from an input file
  const bool isBuiltin = false;
  BOOST_CHECK_NO_THROW (testDemandGenerationHelper(0,
                                                   lInputFilename,
                                                   lDemandGenerationMethod,
                                                   isBuiltin));
  
}

/**
 * Test a simple error case: the demand input file is missing
 */
BOOST_AUTO_TEST_CASE (trademgen_missing_input_file_test) {

  // Input file name
  const stdair::Filename_T lInputFilename (STDAIR_SAMPLE_DIR "/missingFile.csv");

  // Generate the date time of the requests with the statistic order method.
  const stdair::DemandGenerationMethod lDemandGenerationMethod (stdair::DemandGenerationMethod::STA_ORD);
  
  // State whether the BOM tree should be built-in or parsed from an input file
  const bool isBuiltin = false;
  BOOST_CHECK_THROW (testDemandGenerationHelper(1,
                                                lInputFilename,
                                                lDemandGenerationMethod,
                                                isBuiltin),
                     TRADEMGEN::DemandInputFileNotFoundException);
  
}

/**
 * Test a simple demand generation with the default BOM tree
 */
BOOST_AUTO_TEST_CASE (trademgen_default_bom_simulation_test) {
  
  // Generate the date time of the requests with the statistic order method.
  const stdair::DemandGenerationMethod lDemandGenerationMethod (stdair::DemandGenerationMethod::STA_ORD);
  
  // State whether the BOM tree should be built-in or parsed from an input file
  const bool isBuiltin = true;
  BOOST_CHECK_NO_THROW (testDemandGenerationHelper(2,
                                                   " " ,
                                                   lDemandGenerationMethod,
                                                   isBuiltin));
  
}

/**
 * Test a simple demand generation with the default BOM tree
 */
BOOST_AUTO_TEST_CASE (trademgen_poisson_process_test) {
  
  // Generate the date time of the requests with the poisson process.
  const stdair::DemandGenerationMethod lDemandGenerationMethod (stdair::DemandGenerationMethod::POI_PRO);
  
  // State whether the BOM tree should be built-in or parsed from an input file
  const bool isBuiltin = true;
  BOOST_CHECK_NO_THROW (testDemandGenerationHelper(3,
                                                   " " ,
                                                   lDemandGenerationMethod,
                                                   isBuiltin));
  
}

// End the test suite
BOOST_AUTO_TEST_SUITE_END()

/*!
 * \endcode
 */
