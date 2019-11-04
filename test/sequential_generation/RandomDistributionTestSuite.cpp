// STL
#include <iostream>
#include <fstream>
// Boost Date-Time
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
// Boost Random
#include <boost/random/linear_congruential.hpp>
#include <boost/random/normal_distribution.hpp>
#include <boost/random/poisson_distribution.hpp>
#include <boost/random/exponential_distribution.hpp>
#include <boost/random/variate_generator.hpp>
// Boost Math
#include <boost/math/distributions/normal.hpp>
#include <boost/math/distributions/weibull.hpp>
// Boost Accumulators
#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics.hpp>
// Boost Empirical distribution function
//#include "../math/boost_math/empirical_distribution/frequency.hpp"
// Boost Unit Test Framework (UTF)
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#define BOOST_TEST_MODULE RandomDistributionTest
#include <boost/test/unit_test.hpp>

// using namespace boost;
namespace ba = boost::accumulators;
namespace boost_utf = boost::unit_test;

// (Boost) Unit Test XML Report
std::ofstream utfReportStream ("RandomDistributionTestSuite_utfresults.xml");

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

// ////////// Type definitions //////////
/** Type definition for a random number generator base (mt19937). */
typedef boost::minstd_rand base_generator_type;

/** Type definition for the random generator seed.
    <br>That seed must be unsigned, otherwise the wrong overload may be
    selected when using mt19937 as the base_generator_type. */
typedef unsigned int random_generator_seed_type;

/** Type definiton for the normal distribution (characteristics). */
typedef boost::normal_distribution<> normal_dist_type;

/** Type definiton for the Poisson distribution (characteristics). */
typedef boost::poisson_distribution<> poisson_dist_type;
    
/** Type definiton for the exponential distribution (characteristics). */
typedef boost::exponential_distribution<> exponential_dist_type;
    
/** Type definition for the normal distribution random generator. */
typedef boost::variate_generator<base_generator_type&, 
                                 normal_dist_type> normal_gen_type;

/** Type definition for the Poisson distribution random generator. */
typedef boost::variate_generator<base_generator_type&, 
                                 poisson_dist_type> poisson_gen_type;

/** Type definition for the exponential distribution random generator. */
typedef boost::variate_generator<base_generator_type&, 
                                 exponential_dist_type> exponential_gen_type;

/** Type definition to gather statistics. */
typedef ba::accumulator_set<double,
                            ba::stats<ba::tag::min, ba::tag::max,
                                      ba::tag::mean (ba::immediate),
                                      ba::tag::sum,
                                      ba::tag::lazy_variance> > stat_acc_type;

class simulation_exception : public std::exception {
public:
  /** Constructor. */
  simulation_exception (const std::string& iWhat) throw() : _what () {}
  ~simulation_exception () throw() {}
  /** Gives the reason of the exception. */
  const char* what() const throw() { return _what.c_str(); }
private:
  std::string _what;
};

// /////////////// Main: Unit Test Suite //////////////

// Set the UTF configuration (re-direct the output to a specific file)
BOOST_GLOBAL_FIXTURE (UnitTestConfig);

// Start the test suite
BOOST_AUTO_TEST_SUITE (master_test_suite)

/**
 * Test a simple simulation
 */
BOOST_AUTO_TEST_CASE (simple_random_distribution_test) {

  // Output log File
  const std::string lLogFilename ("RandomDistributionTestSuite.log");
    
  // Set the log parameters
  std::ofstream logOutputFile;
    
  // Open and clean the log outputfile
  logOutputFile.open (lLogFilename.c_str());
  logOutputFile.clear();
    
  // Final demand (i.e., number of passengers) for the given simulated
  // flight-date
  const double kFinalDemandMean1 = 300.0;
  const double kFinalDemandStandardDeviation1 = 17.32;
  const double kFinalDemandMean2 = 100.0;
  const double kFinalDemandStandardDeviation2 = 17.32;

  // Initialisation of random generators
  /** Base for the random generator. */
  base_generator_type lGenerator (42u);

  // Statistical accumulators for the respective numbers to watch
  stat_acc_type lGaussianVariateAcc1;
  stat_acc_type lGaussianVariateAcc2;

  //
  const unsigned int K = 10;
  
  // Perform K simulations    
  for (unsigned int i=0; i != K; i++) {

    // Perform 1000 draws
    for (unsigned int i=0; i != 1000; i++) {

      // Generate a Gaussian variate, just in order to compare the final
      // distributions (Gaussian and accumulated Poissonian).
      normal_dist_type lNormalDistribution1 (kFinalDemandMean1,
                                             kFinalDemandStandardDeviation1);

      normal_dist_type lNormalDistribution2 (kFinalDemandMean2,
                                             kFinalDemandStandardDeviation2);

      /** Random generator for the normal distribution. */
      normal_gen_type lNormalDistributionGenerator1 (lGenerator,
                                                     lNormalDistribution1);
      normal_gen_type lNormalDistributionGenerator2 (lGenerator,
                                                     lNormalDistribution2);

      const double lGaussianVariate1 = lNormalDistributionGenerator1();
      const double lGaussianVariate2 = lNormalDistributionGenerator2();
      
      // Update the statistics
      lGaussianVariateAcc1 (lGaussianVariate1);
      lGaussianVariateAcc2 (lGaussianVariate2);
    }
  }
    
  // DEBUG
  logOutputFile << "Statistics for random draws 1 (min, max, mean, variance);"
                << std::endl;
  logOutputFile << ba::min (lGaussianVariateAcc1) << ", "
                << ba::max (lGaussianVariateAcc1) << ", "
                << ba::mean (lGaussianVariateAcc1) << ", "
                << ba::variance (lGaussianVariateAcc1) << "; "
                << std::endl;
  logOutputFile << "Statistics for random draws 2 (min, max, mean, variance);"
                << std::endl;
  logOutputFile << ba::min (lGaussianVariateAcc2) << ", "
                << ba::max (lGaussianVariateAcc2) << ", "
                << ba::mean (lGaussianVariateAcc2) << ", "
                << ba::variance (lGaussianVariateAcc2) << "; "
                << std::endl;

  // Close the log file
  logOutputFile.close();
}

// End the test suite
BOOST_AUTO_TEST_SUITE_END()
