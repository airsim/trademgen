// STL
#include <sstream>
#include <fstream>
#include <string>
// CPPUNIT
#include <extracppunit/CppUnitCore.hpp>
// StdAir
#include <stdair/STDAIR_Types.hpp>
#include <stdair/STDAIR_Service.hpp>
#include <stdair/basic/DemandCharacteristics.hpp>
#include <stdair/basic/CategoricalAttribute.hpp>
#include <stdair/basic/ContinuousAttribute.hpp>
#include <stdair/bom/DemandStream.hpp>
#include <stdair/bom/EventStruct.hpp>
#include <stdair/bom/EventQueue.hpp>
#include <stdair/bom/BookingRequestStruct.hpp>
#include <stdair/service/Logger.hpp>
// Trademgen
#include <trademgen/TRADEMGEN_Service.hpp>
// TraDemGen Test Suite
#include <test/trademgen/DemandGenerationTestSuite.hpp>

// //////////////////////////////////////////////////////////////////////
// Test is based on ...
// //////////////////////////////////////////////////////////////////////

// //////////////////////////////////////////////////////////////////////
void DemandGenerationTestSuite::simpleEventGenerationHelper() {

  // Input file name
  stdair::Filename_T lInputFilename ("../samples/demand01.csv");

  // Output log File
  std::string lLogFilename ("DemandGenerationTestSuite.log");
  
  // Airline code
  stdair::AirlineCode_T lAirlineCode ("BA");
    
  // Set the log parameters
  std::ofstream logOutputFile;
  // open and clean the log outputfile
  logOutputFile.open (lLogFilename.c_str());
  logOutputFile.clear();
  
  // Initialise the TraDemGen service object
  const stdair::BasLogParams lLogParams (stdair::LOG::DEBUG, logOutputFile);
  TRADEMGEN::TRADEMGEN_Service trademgenService (lLogParams, lInputFilename);

  // ////////////////////////////////////

  // Demand characteristics
  stdair::DemandCharacteristics demandCharacteristics1;
  stdair::DemandCharacteristics demandCharacteristics2;
  // Demand distribution
  stdair::DemandDistribution demandDistribution1;
  stdair::DemandDistribution demandDistribution2;

  // distribution of number of requests
  demandDistribution1.setMeanNumberOfRequests (10.0);
  demandDistribution1.setStandardDeviationNumberOfRequests (2.0);
  demandDistribution2.setMeanNumberOfRequests (12.0);
  demandDistribution2.setStandardDeviationNumberOfRequests (1.0);

  // origin
  demandCharacteristics1.setOrigin ("CDG");
  demandCharacteristics2.setOrigin ("FRA");
  // preferred departure date
  demandCharacteristics1.setPreferredDepartureDate (boost::gregorian::date (2010,1,1));
  demandCharacteristics2.setPreferredDepartureDate (boost::gregorian::date (2010,2,1));
  
  // arrival pattern
  std::multimap<stdair::FloatDuration_T, stdair::Probability_T> arrivalPatternCumulativeDistribution1;
  arrivalPatternCumulativeDistribution1.
    insert ( std::pair<stdair::FloatDuration_T, stdair::Probability_T> (-365.0, 0) );
  arrivalPatternCumulativeDistribution1.
    insert ( std::pair<stdair::FloatDuration_T, stdair::Probability_T> (-67.0, 0.2) );
  arrivalPatternCumulativeDistribution1.
    insert ( std::pair<stdair::FloatDuration_T, stdair::Probability_T> (-17.0, 0.5) );
  arrivalPatternCumulativeDistribution1.
    insert ( std::pair<stdair::FloatDuration_T, stdair::Probability_T> (0.0, 1.0) );

  std::multimap<stdair::FloatDuration_T, stdair::Probability_T> arrivalPatternCumulativeDistribution2;
  arrivalPatternCumulativeDistribution2.
    insert ( std::pair<stdair::FloatDuration_T, stdair::Probability_T> (-365.0, 0) );
  arrivalPatternCumulativeDistribution2.
    insert ( std::pair<stdair::FloatDuration_T, stdair::Probability_T> (-300.0, 0.5) );
  arrivalPatternCumulativeDistribution2.
    insert ( std::pair<stdair::FloatDuration_T, stdair::Probability_T> (-200.0, 0.9) );
  arrivalPatternCumulativeDistribution2.
    insert ( std::pair<stdair::FloatDuration_T, stdair::Probability_T> (0.0, 1.0) );

  // When creating the ContinuousAttribute object, the mapping is
  // inverted, i.e., the inverse cumulative distribution can be
  // derived from the cumulative distribution
  const stdair::ContinuousAttribute<stdair::FloatDuration_T> arrivalPattern1 (arrivalPatternCumulativeDistribution1);
  demandCharacteristics1.setArrivalPattern (arrivalPattern1);
  const stdair::ContinuousAttribute<stdair::FloatDuration_T> arrivalPattern2 (arrivalPatternCumulativeDistribution2);
  demandCharacteristics2.setArrivalPattern (arrivalPattern2);
  

  // Display
  STDAIR_LOG_DEBUG ("Demand 1: " << demandCharacteristics1.display()
                    << demandDistribution1.display()
                    << std::endl << std::endl);

  STDAIR_LOG_DEBUG ("Demand 2: " << demandCharacteristics2.display()
                    << demandDistribution2.display()
                    << std::endl << std::endl);

  // Seeds
  stdair::RandomSeed_T seed = 2;
  
  // Key
  stdair::DemandStreamKey_T key1 = 1;
  stdair::DemandStreamKey_T key2 = 2;
  

  // Initialize the demand stream
  stdair::DemandStream demandStream1 (key1, demandCharacteristics1,
                                      demandDistribution1, seed, seed, seed);
  stdair::DemandStream demandStream2 (key2, demandCharacteristics2,
                                      demandDistribution2, seed, seed, seed);
  
  // map of demand streams
  std::map<stdair::DemandStreamKey_T, stdair::DemandStream *> lDemandStreamMap;
  lDemandStreamMap.insert(std::pair<stdair::DemandStreamKey_T, stdair::DemandStream *> (key1, &demandStream1));
  lDemandStreamMap.insert(std::pair<stdair::DemandStreamKey_T, stdair::DemandStream *> (key2, &demandStream2));
  
  // Get the total number of requests to be generated
  stdair::Count_T totalNumberOfRequestsToBeGenerated1 =
    demandStream1.getTotalNumberOfRequestsToBeGenerated ();
  stdair::Count_T totalNumberOfRequestsToBeGenerated2 =
    demandStream2.getTotalNumberOfRequestsToBeGenerated ();

  STDAIR_LOG_DEBUG ("Number of requests to be generated (demand 1): "
                    << totalNumberOfRequestsToBeGenerated1 << std::endl);
  STDAIR_LOG_DEBUG ("Number of requests to be generated (demand 2): "
                    << totalNumberOfRequestsToBeGenerated2 << std::endl);

  // /////////////////////////////////////////////////////
  // Event queue
  stdair::DateTime_T lInitialDateTime =
    boost::posix_time::ptime (boost::gregorian::date (2009,1,1),
                              boost::posix_time::time_duration (0,0,0));
  stdair::EventQueue lEventQueue (lInitialDateTime);
  
  // Initialize by adding one request of each type
  stdair::BookingRequestStruct* lRequest1 = new stdair::BookingRequestStruct();
  
  bool flag_generated1 = demandStream1.generateNext (*lRequest1);
  if (flag_generated1) {
    stdair::DateTime_T lRequestDateTime = lRequest1->getRequestDateTime ();
    stdair::EventStruct lEventStruct("Request", lRequestDateTime, demandStream1,
                                     lRequest1);
    lEventQueue.addEvent (lEventStruct);
  }
  
  stdair::BookingRequestStruct* lRequest2 = new stdair::BookingRequestStruct ();
  
  bool flag_generated2 = demandStream2.generateNext (*lRequest2);
  if (flag_generated2) {
    stdair::DateTime_T lRequestDateTime = lRequest2->getRequestDateTime ();
    stdair::EventStruct lEventStruct("Request", lRequestDateTime, demandStream2,
                                     lRequest2);
    lEventQueue.addEvent (lEventStruct);
  }
  
  // Pop requests, get type, and generate next request of same type
  int i = 0;
  while (lEventQueue.isQueueDone() == false && i < 20) {
    // DEBUG
    STDAIR_LOG_DEBUG ("Before popping (" << i << ")" );
    STDAIR_LOG_DEBUG ("Queue size: " << lEventQueue.getQueueSize () );
    STDAIR_LOG_DEBUG ("Position of current: "
                      << lEventQueue.getPositionOfCurrent () );
    STDAIR_LOG_DEBUG ("Is queue done? " << lEventQueue.isQueueDone () );
    
    stdair::EventStruct* lEventStruct_ptr = lEventQueue.popEvent ();

    // DEBUG
    STDAIR_LOG_DEBUG ("After popping" );
    STDAIR_LOG_DEBUG ("Queue size: " << lEventQueue.getQueueSize ());
    STDAIR_LOG_DEBUG ("Position of current: "
                      << lEventQueue.getPositionOfCurrent ());
    STDAIR_LOG_DEBUG ("Is queue done? " << lEventQueue.isQueueDone ());

    if (lEventStruct_ptr != NULL) {
      STDAIR_LOG_DEBUG ("Popped request " << i );

      const stdair::BookingRequestStruct* lPoppedRequest =
        lEventStruct_ptr->getPointerToRequestEvent ();
      assert (lPoppedRequest != NULL);

      // DEBUG
      STDAIR_LOG_DEBUG (lPoppedRequest->describe());

      // Retrieve the corresponding demand stream
      stdair::DemandStream& lDemandStream = lEventStruct_ptr->getDemandStream();
      // delete popped request
      delete lPoppedRequest; lPoppedRequest=NULL;
      // generate next request
      stdair::BookingRequestStruct* lNextRequest = new stdair::BookingRequestStruct ();
      bool flag_generated = lDemandStream.generateNext (*lNextRequest);
      STDAIR_LOG_DEBUG ("flag_generated: " << flag_generated );
      if (flag_generated) {
        // DEBUG
        STDAIR_LOG_DEBUG ("Added request: " << lNextRequest->describe());
        
        stdair::DateTime_T lNextRequestDateTime =
          lNextRequest->getRequestDateTime ();
        stdair::EventStruct lNextEventStruct ("Request",
                                              lNextRequestDateTime,
                                              lDemandStream,
                                              lNextRequest);
        lEventQueue.addEvent (lNextEventStruct);

        // DEBUG
        STDAIR_LOG_DEBUG ("After adding");
        STDAIR_LOG_DEBUG ("Queue size: " << lEventQueue.getQueueSize ());
        STDAIR_LOG_DEBUG ("Position of current: "
                          << lEventQueue.getPositionOfCurrent ());
        STDAIR_LOG_DEBUG ("Is queue done? " << lEventQueue.isQueueDone ());
      }
    }

    // DEBUG
    STDAIR_LOG_DEBUG (std::endl);
    
    // Iterate
    ++i;
  }
}

// //////////////////////////////////////////////////////////////////////
void DemandGenerationTestSuite::simpleEventGeneration () {
  // TODO: Check that the generated events follow the law given as input
  CPPUNIT_ASSERT_NO_THROW ( simpleEventGenerationHelper(););
}

// //////////////////////////////////////////////////////////////////////
// void DemandGenerationTestSuite::errorCase () {
//  CPPUNIT_ASSERT (false);
// }

// //////////////////////////////////////////////////////////////////////
DemandGenerationTestSuite::DemandGenerationTestSuite () {
  _describeKey << "Running test on RMOL Optimisation function";  
}

// /////////////// M A I N /////////////////
CPPUNIT_MAIN()

