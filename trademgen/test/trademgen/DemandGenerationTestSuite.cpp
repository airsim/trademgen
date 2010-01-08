// STL
#include <sstream>
#include <fstream>
#include <string>
// CPPUNIT
#include <extracppunit/CppUnitCore.hpp>
// TraDemGen
#include <trademgen/TRADEMGEN_Service.hpp>
#include <trademgen/TRADEMGEN_Types.hpp>
// TraDemGen Test Suite
#include <test/trademgen/EventStream.hpp>
#include <test/trademgen/CategoricalAttribute.hpp>
#include <test/trademgen/DemandGenerationTestSuite.hpp>

// //////////////////////////////////////////////////////////////////////
// Test is based on ...
// //////////////////////////////////////////////////////////////////////

// //////////////////////////////////////////////////////////////////////
void DemandGenerationTestSuite::simpleEventGenerationHelper() {

  // Input: seed, rate
  const unsigned long int seed = 2;
  
  // create event stream
  TRADEMGEN::EventStream lEventStream (seed);
  lEventStream.setKey ("seedKey01");
  lEventStream.setRate (2.0);
	
  // DEBUG
  // Get rate
  // const double r = lEventStream.getRate();
  std::cout << "Seed: " << seed << std::endl << std::endl;
	
  // Create instances
  for (int i = 0; i < 10; ++i) {
    lEventStream.generateNext();
  }

  // DEBUG
  // Display events
  lEventStream.displayAllEvents (std::cout);
	
	
  // /////////////////////////////////////////////////////
  // attributes
  std::map<int, float> M;
  M[1] = 0.1;
  M[17] = 0.7;
  M[77] = 0.2;
  TRADEMGEN::CategoricalAttribute C (M);
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

