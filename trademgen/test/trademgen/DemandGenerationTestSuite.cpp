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
#include <stdair/basic/DemandDistribution.hpp>
#include <stdair/basic/CategoricalAttribute.hpp>
#include <stdair/basic/ContinuousAttribute.hpp>
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

  /** Origin. */
  const stdair::AirportCode_T lLHR ("LHR");
  /** Destination. */
  const stdair::AirportCode_T lJFK ("JFK");
  /** Preferred departure date. */
  const stdair::Date_T lPreferredDepartureDate (2010, 1, 17);
  /** Passenger type. */
  const stdair::PassengerType lLeisurePax ('L');
  const stdair::PassengerType lBusinessPax ('B');
  /** Demand distribution parameters. */
  const stdair::NbOfRequests_T lDemandMean1 (10.0);
  const stdair::StandardDeviationValue_T lDemandStdDev1 (2.0);
  const stdair::NbOfRequests_T lDemandMean2 (12.0);
  const stdair::StandardDeviationValue_T lDemandStdDev2 (1.0);
  
  // Demand characteristics
  const stdair::DemandStreamKey_T key1 (lLHR, lJFK, lPreferredDepartureDate,
                                        lLeisurePax);
  const stdair::DemandStreamKey_T key2 (lLHR, lJFK, lPreferredDepartureDate,
                                        lBusinessPax);

  stdair::DemandCharacteristics demandCharacteristics1 (key1);
  stdair::DemandCharacteristics demandCharacteristics2 (key2);
    
    
  // Demand distribution
  stdair::DemandDistribution demandDistribution1 (lDemandMean1, lDemandStdDev1);
  stdair::DemandDistribution demandDistribution2 (lDemandMean2, lDemandStdDev2);

  // Arrival pattern
  stdair::ArrivalPatternCumulativeDistribution_T arrivalPatternCumulativeDistribution1;
  arrivalPatternCumulativeDistribution1.
    insert (stdair::ArrivalPatternCumulativeDistribution_T::value_type (-365.0,
                                                                        0) );
  arrivalPatternCumulativeDistribution1.
    insert (stdair::ArrivalPatternCumulativeDistribution_T::value_type (-67.0,
                                                                        0.2) );
  arrivalPatternCumulativeDistribution1.
    insert (stdair::ArrivalPatternCumulativeDistribution_T::value_type (-17.0,
                                                                        0.5) );
  arrivalPatternCumulativeDistribution1.
    insert (stdair::ArrivalPatternCumulativeDistribution_T::value_type (0.0,
                                                                        1.0) );
  
  stdair::ArrivalPatternCumulativeDistribution_T arrivalPatternCumulativeDistribution2;
  arrivalPatternCumulativeDistribution2.
    insert (stdair::ArrivalPatternCumulativeDistribution_T::value_type (-365.0,
                                                                        0) );
  arrivalPatternCumulativeDistribution2.
    insert (stdair::ArrivalPatternCumulativeDistribution_T::value_type (-300.0,
                                                                        0.5) );
  arrivalPatternCumulativeDistribution2.
    insert (stdair::ArrivalPatternCumulativeDistribution_T::value_type (-200.0,
                                                                        0.9) );
  arrivalPatternCumulativeDistribution2.
    insert (stdair::ArrivalPatternCumulativeDistribution_T::value_type (0.0,
                                                                        1.0) );
  

  // When creating the ContinuousAttribute object, the mapping is
  // inverted, i.e., the inverse cumulative distribution can be
  // derived from the cumulative distribution
  const stdair::ContinuousFloatDuration_T arrivalPattern1 (arrivalPatternCumulativeDistribution1);
  demandCharacteristics1.setArrivalPattern (arrivalPattern1);
  const stdair::ContinuousFloatDuration_T arrivalPattern2 (arrivalPatternCumulativeDistribution2);
  demandCharacteristics2.setArrivalPattern (arrivalPattern2);
  
  // Display
  STDAIR_LOG_DEBUG ("Demand 1: " << std::endl
                    << demandCharacteristics1.display()
                    << demandDistribution1.display());
  
  STDAIR_LOG_DEBUG ("Demand 2: " << std::endl
                    << demandCharacteristics2.display()
                    << demandDistribution2.display());

  // Seeds
  stdair::RandomSeed_T seed = 2;
  

  // Initialize the demand stream
  trademgenService.addDemandStream (key1, demandCharacteristics1,
                                    demandDistribution1, seed, seed, seed);
  trademgenService.addDemandStream (key2, demandCharacteristics2,
                                    demandDistribution2, seed, seed, seed);
  
  // Get the total number of requests to be generated
  stdair::Count_T totalNumberOfRequestsToBeGenerated1 =
    trademgenService.getTotalNumberOfRequestsToBeGenerated (key1);
  stdair::Count_T totalNumberOfRequestsToBeGenerated2 =
    trademgenService.getTotalNumberOfRequestsToBeGenerated (key2);

  STDAIR_LOG_DEBUG ("Number of requests to be generated (demand 1): "
                    << totalNumberOfRequestsToBeGenerated1 << std::endl);
  STDAIR_LOG_DEBUG ("Number of requests to be generated (demand 2): "
                    << totalNumberOfRequestsToBeGenerated2 << std::endl);

  // /////////////////////////////////////////////////////
  // Event queue
  stdair::EventQueue lEventQueue = stdair::EventQueue ();
  
  // Initialize by adding one request of each type
  const bool stillHavingRequestsToBeGenerated1 = 
    trademgenService.stillHavingRequestsToBeGenerated (key1);
  if (stillHavingRequestsToBeGenerated1) {
    stdair::BookingRequestPtr_T lRequest1 =
      trademgenService.generateNextRequest (key1);
    assert (lRequest1 != NULL);
    stdair::DateTime_T lRequestDateTime = lRequest1->getRequestDateTime ();
    stdair::EventStruct lEventStruct ("Request", lRequestDateTime, key1,
                                      lRequest1);
    lEventQueue.addEvent (lEventStruct);
  }
  
  const bool stillHavingRequestsToBeGenerated2 = 
    trademgenService.stillHavingRequestsToBeGenerated (key2);
  if (stillHavingRequestsToBeGenerated2) {
    stdair::BookingRequestPtr_T lRequest2 =
      trademgenService.generateNextRequest (key2);
    assert (lRequest2 != NULL);
    stdair::DateTime_T lRequestDateTime = lRequest2->getRequestDateTime ();
    stdair::EventStruct lEventStruct("Request", lRequestDateTime, key2,
                                     lRequest2);
    lEventQueue.addEvent (lEventStruct);
  }
  
  // Pop requests, get type, and generate next request of same type
  int i = 0;
  while (lEventQueue.isQueueDone() == false && i < 20) {
    // DEBUG
    STDAIR_LOG_DEBUG ("Before popping (" << i << ")" );
    STDAIR_LOG_DEBUG ("Queue size: " << lEventQueue.getQueueSize () );
    STDAIR_LOG_DEBUG ("Is queue done? " << lEventQueue.isQueueDone () );
    
    stdair::EventStruct& lEventStruct = lEventQueue.popEvent ();

    // DEBUG
    STDAIR_LOG_DEBUG ("After popping" );
    STDAIR_LOG_DEBUG ("Queue size: " << lEventQueue.getQueueSize ());
    STDAIR_LOG_DEBUG ("Is queue done? " << lEventQueue.isQueueDone ());

    STDAIR_LOG_DEBUG ("Popped request " << i );
    
    const stdair::BookingRequestStruct& lPoppedRequest =
      lEventStruct.getBookingRequest ();
    
    // DEBUG
    STDAIR_LOG_DEBUG (lPoppedRequest.describe());
    
    // Retrieve the corresponding demand stream
    const stdair::DemandStreamKey_T& lDemandStreamKey =
      lEventStruct.getDemandStreamKey ();
    // generate next request
    bool stillHavingRequestsToBeGenerated = 
      trademgenService.stillHavingRequestsToBeGenerated(lDemandStreamKey);
    STDAIR_LOG_DEBUG ("stillHavingRequestsToBeGenerated: " << stillHavingRequestsToBeGenerated );
    if (stillHavingRequestsToBeGenerated) {
      stdair::BookingRequestPtr_T lNextRequest =
        trademgenService.generateNextRequest (lDemandStreamKey);
      assert (lNextRequest != NULL);
      // DEBUG
      STDAIR_LOG_DEBUG ("Added request: " << lNextRequest->describe());
      
      stdair::DateTime_T lNextRequestDateTime =
        lNextRequest->getRequestDateTime ();
      stdair::EventStruct lNextEventStruct ("Request",
                                            lNextRequestDateTime,
                                            lDemandStreamKey,
                                            lNextRequest);
      lEventQueue.eraseLastUsedEvent ();
      lEventQueue.addEvent (lNextEventStruct);
      
      // DEBUG
      STDAIR_LOG_DEBUG ("After adding");
      STDAIR_LOG_DEBUG ("Queue size: " << lEventQueue.getQueueSize ());
      STDAIR_LOG_DEBUG ("Is queue done? " << lEventQueue.isQueueDone ());
    
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

