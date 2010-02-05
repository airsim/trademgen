// STL
#include <sstream>
#include <fstream>
#include <string>
// CPPUNIT
#include <extracppunit/CppUnitCore.hpp>
// TraDemGen
#include <test/trademgen/BasTypes.hpp>
#include <test/trademgen/DemandCharacteristics.hpp>
#include <test/trademgen/DemandStream.hpp>
#include <test/trademgen/CategoricalAttribute.hpp>
#include <test/trademgen/ContinuousAttribute.hpp>
#include <test/trademgen/EventStruct.hpp>
#include <test/trademgen/EventQueue.hpp>
//#include <trademgen/TRADEMGEN_Service.hpp>
//#include <trademgen/TRADEMGEN_Types.hpp>
// TraDemGen Test Suite
#include <test/trademgen/DemandGenerationTestSuite.hpp>

// //////////////////////////////////////////////////////////////////////
// Test is based on ...
// //////////////////////////////////////////////////////////////////////

// //////////////////////////////////////////////////////////////////////
void DemandGenerationTestSuite::simpleEventGenerationHelper() {

  // Demand characteristics
  TRADEMGEN::DemandCharacteristics demand1;
  TRADEMGEN::DemandCharacteristics demand2;

  // distribution of number of requests
  demand1.setMeanNumberOfRequests (10.0);
  demand1.setStandardDeviationNumberOfRequests (2.0);
  demand2.setMeanNumberOfRequests (12.0);
  demand2.setStandardDeviationNumberOfRequests (1.0);

  // origin
  demand1.setOrigin ("CDG");
  demand2.setOrigin ("FRA");
  // preferred departure date
  demand1.setPreferredDepartureDate (boost::gregorian::date (2010,1,1));
  demand2.setPreferredDepartureDate (boost::gregorian::date (2010,2,1));
  
  // arrival pattern
  std::multimap<TRADEMGEN::FloatDuration_T, TRADEMGEN::Probability_T> arrivalPatternCumulativeDistribution1;
  arrivalPatternCumulativeDistribution1.insert ( std::pair<TRADEMGEN::FloatDuration_T, TRADEMGEN::Probability_T> (-365.0, 0) );
  arrivalPatternCumulativeDistribution1.insert ( std::pair<TRADEMGEN::FloatDuration_T, TRADEMGEN::Probability_T> (-67.0, 0.2) );
  arrivalPatternCumulativeDistribution1.insert ( std::pair<TRADEMGEN::FloatDuration_T, TRADEMGEN::Probability_T> (-17.0, 0.5) );
  arrivalPatternCumulativeDistribution1.insert ( std::pair<TRADEMGEN::FloatDuration_T, TRADEMGEN::Probability_T> (0.0, 1.0) );

  std::multimap<TRADEMGEN::FloatDuration_T, TRADEMGEN::Probability_T> arrivalPatternCumulativeDistribution2;
  arrivalPatternCumulativeDistribution2.insert ( std::pair<TRADEMGEN::FloatDuration_T, TRADEMGEN::Probability_T> (-365.0, 0) );
  arrivalPatternCumulativeDistribution2.insert ( std::pair<TRADEMGEN::FloatDuration_T, TRADEMGEN::Probability_T> (-300.0, 0.5) );
  arrivalPatternCumulativeDistribution2.insert ( std::pair<TRADEMGEN::FloatDuration_T, TRADEMGEN::Probability_T> (-200.0, 0.9) );
  arrivalPatternCumulativeDistribution2.insert ( std::pair<TRADEMGEN::FloatDuration_T, TRADEMGEN::Probability_T> (0.0, 1.0) );

  // When creating the ContinuousAttribute object, the mapping is inverted, i.e.,
  // the inverse cumulative distribution can be derived from the cumulative
  // distribution
  const TRADEMGEN::ContinuousAttribute<TRADEMGEN::FloatDuration_T> arrivalPattern1 (arrivalPatternCumulativeDistribution1);
  demand1.setArrivalPattern (arrivalPattern1);
  const TRADEMGEN::ContinuousAttribute<TRADEMGEN::FloatDuration_T> arrivalPattern2 (arrivalPatternCumulativeDistribution2);
  demand2.setArrivalPattern (arrivalPattern2);
  

  // display
  std::cout << "Demand 1" << std::endl;
  demand1.display (std::cout);
  std::cout << std::endl << std::endl;

  std::cout << "Demand 2" << std::endl;
  demand2.display (std::cout);
  std::cout << std::endl << std::endl;

  // Seeds
  TRADEMGEN::RandomSeed_T seed = 2;
  
  // Key
  TRADEMGEN::Key_T key1 = 1;
  TRADEMGEN::Key_T key2 = 2;
  

  // Initialize the demand stream
  TRADEMGEN::DemandStream demandStream1 (key1, demand1, seed, seed, seed);
  TRADEMGEN::DemandStream demandStream2 (key2, demand2, seed, seed, seed);
  
  // map of demand streams
  std::map<TRADEMGEN::Key_T, TRADEMGEN::DemandStream *> lDemandStreamMap;
  lDemandStreamMap.insert(std::pair<TRADEMGEN::Key_T, TRADEMGEN::DemandStream *> (key1, &demandStream1));
  lDemandStreamMap.insert(std::pair<TRADEMGEN::Key_T, TRADEMGEN::DemandStream *> (key2, &demandStream2));
  
  // Get the total number of requests to be generated
  TRADEMGEN::Count_T totalNumberOfRequestsToBeGenerated1 = demandStream1.getTotalNumberOfRequestsToBeGenerated ();
  TRADEMGEN::Count_T totalNumberOfRequestsToBeGenerated2 = demandStream2.getTotalNumberOfRequestsToBeGenerated ();

  std::cout << "Number of requests to be generated (demand 1): "
            << totalNumberOfRequestsToBeGenerated1 << std::endl << std::endl;
  std::cout << "Number of requests to be generated (demand 2): "
            << totalNumberOfRequestsToBeGenerated2 << std::endl << std::endl;

  // /////////////////////////////////////////////////////
  // Event queue
  TRADEMGEN::Datetime_T lInitialDatetime = boost::posix_time::ptime(
                                  boost::gregorian::date (2009,1,1),
                                  boost::posix_time::time_duration (0,0,0)
                                  );
  TRADEMGEN::EventQueue lEventQueue (lInitialDatetime);
  
  // Initialize by adding one request of each type
  TRADEMGEN::Request * lRequest1 = new TRADEMGEN::Request ();
  bool flag_generated1 = demandStream1.generateNext (*lRequest1);
  if (flag_generated1) {
    TRADEMGEN::Datetime_T lRequestDatetime = lRequest1->getRequestDatetime ();
    TRADEMGEN::EventStruct lEventStruct ("Request", lRequestDatetime, lRequest1);
    lEventQueue.addEvent (lEventStruct);
  }
  TRADEMGEN::Request * lRequest2 = new TRADEMGEN::Request ();
  bool flag_generated2 = demandStream2.generateNext (*lRequest2);
  if (flag_generated2) {
    TRADEMGEN::Datetime_T lRequestDatetime = lRequest2->getRequestDatetime ();
    TRADEMGEN::EventStruct lEventStruct ("Request", lRequestDatetime, lRequest2);
    lEventQueue.addEvent (lEventStruct);
  }
  
  // Pop requests, get type, and generate next request of same type
  int i = 0;
  while (!lEventQueue.isQueueDone () && i < 20) {
    std::cout << "Before popping (" << i << ")" << std::endl;
    std::cout << "Queue size: " << lEventQueue.getQueueSize () << std::endl;
    std::cout << "Position of current: " << lEventQueue.getPositionOfCurrent () << std::endl;
    std::cout << "Is queue done? " << lEventQueue.isQueueDone () << std::endl;
    TRADEMGEN::EventStruct * lEventStruct_ptr = lEventQueue.popEvent ();
    std::cout << "After popping" << std::endl;
    std::cout << "Queue size: " << lEventQueue.getQueueSize () << std::endl;
    std::cout << "Position of current: " << lEventQueue.getPositionOfCurrent () << std::endl;
    std::cout << "Is queue done? " << lEventQueue.isQueueDone () << std::endl;
    if (lEventStruct_ptr != NULL) {
      std::cout << "Popped request " << i << std::endl;
      const TRADEMGEN::Request * lPoppedRequest = lEventStruct_ptr->getPointerToRequestEvent ();
      lPoppedRequest->display (std::cout);
      // get key to determine demand stream
      const TRADEMGEN::Key_T lKey = lPoppedRequest->getDemandStreamKey ();
      TRADEMGEN::DemandStream * lDemandStream = lDemandStreamMap[lKey];
      // delete popped request
      delete lPoppedRequest; lPoppedRequest=NULL;
      // generate next request
      TRADEMGEN::Request * lNextRequest = new TRADEMGEN::Request ();
      bool flag_generated = lDemandStream->generateNext (*lNextRequest);
      std::cout << "flag_generated: " << flag_generated << std::endl;
      if (flag_generated) {
        std::cout << "Added request" << std::endl;
        lNextRequest->display (std::cout);
        TRADEMGEN::Datetime_T lNextRequestDatetime = lNextRequest->getRequestDatetime ();
        TRADEMGEN::EventStruct lNextEventStruct ("Request", lNextRequestDatetime, lNextRequest);
        lEventQueue.addEvent (lNextEventStruct);
        std::cout << "After adding" << std::endl;
        std::cout << "Queue size: " << lEventQueue.getQueueSize () << std::endl;
        std::cout << "Position of current: " << lEventQueue.getPositionOfCurrent () << std::endl;
        std::cout << "Is queue done? " << lEventQueue.isQueueDone () << std::endl;
      }
    }
    std::cout << std::endl << std::endl;
    // iterate
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

