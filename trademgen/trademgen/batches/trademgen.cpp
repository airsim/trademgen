// STL
#include <cassert>
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <string>
// Boost (Extended STL)
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/tokenizer.hpp>
#include <boost/program_options.hpp>
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
#include <trademgen/config/trademgen-paths.hpp>


// //////// Type definitions ///////
typedef std::vector<std::string> WordList_T;


// //////// Constants //////
/** Default name and location for the log file. */
const std::string K_TRADEMGEN_DEFAULT_LOG_FILENAME ("trademgen.log");

/** Default name and location for the (CSV) input file. */
const std::string K_TRADEMGEN_DEFAULT_INPUT_FILENAME ("../../test/samples/demand01.csv");

/** Default query string. */
const std::string K_TRADEMGEN_DEFAULT_QUERY_STRING ("my good old query");


// //////////////////////////////////////////////////////////////////////
void tokeniseStringIntoWordList (const std::string& iPhrase,
                                 WordList_T& ioWordList) {
  // Empty the word list
  ioWordList.clear();
  
  // Boost Tokeniser
  typedef boost::tokenizer<boost::char_separator<char> > Tokeniser_T;
  
  // Define the separators
  const boost::char_separator<char> lSepatorList(" .,;:|+-*/_=!@#$%`~^&(){}[]?'<>\"");
  
  // Initialise the phrase to be tokenised
  Tokeniser_T lTokens (iPhrase, lSepatorList);
  for (Tokeniser_T::const_iterator tok_iter = lTokens.begin();
       tok_iter != lTokens.end(); ++tok_iter) {
    const std::string& lTerm = *tok_iter;
    ioWordList.push_back (lTerm);
  }
  
}

// //////////////////////////////////////////////////////////////////////
std::string createStringFromWordList (const WordList_T& iWordList) {
  std::ostringstream oStr;

  unsigned short idx = iWordList.size();
  for (WordList_T::const_iterator itWord = iWordList.begin();
       itWord != iWordList.end(); ++itWord, --idx) {
    const std::string& lWord = *itWord;
    oStr << lWord;
    if (idx > 1) {
      oStr << " ";
    }
  }
  
  return oStr.str();
}


// ///////// Parsing of Options & Configuration /////////
// A helper function to simplify the main part.
template<class T> std::ostream& operator<< (std::ostream& os,
                                            const std::vector<T>& v) {
  std::copy (v.begin(), v.end(), std::ostream_iterator<T> (std::cout, " ")); 
  return os;
}

/** Early return status (so that it can be differentiated from an error). */
const int K_TRADEMGEN_EARLY_RETURN_STATUS = 99;

/** Read and parse the command line options. */
int readConfiguration (int argc, char* argv[], 
                       std::string& ioQueryString,
                       stdair::Filename_T& ioInputFilename,
                       std::string& ioLogFilename) {

  // Initialise the travel query string, if that one is empty
  if (ioQueryString.empty() == true) {
    ioQueryString = K_TRADEMGEN_DEFAULT_QUERY_STRING;
  }
  
  // Transform the query string into a list of words (STL strings)
  WordList_T lWordList;
  tokeniseStringIntoWordList (ioQueryString, lWordList);

  // Declare a group of options that will be allowed only on command line
  boost::program_options::options_description generic ("Generic options");
  generic.add_options()
    ("prefix", "print installation prefix")
    ("version,v", "print version string")
    ("help,h", "produce help message");
    
  // Declare a group of options that will be allowed both on command
  // line and in config file
  boost::program_options::options_description config ("Configuration");
  config.add_options()
    ("input,i",
     boost::program_options::value< std::string >(&ioInputFilename)->default_value(K_TRADEMGEN_DEFAULT_INPUT_FILENAME),
     "(CVS) input file for the demand distributions")
    ("log,l",
     boost::program_options::value< std::string >(&ioLogFilename)->default_value(K_TRADEMGEN_DEFAULT_LOG_FILENAME),
     "Filepath for the logs")
    ("query,q",
     boost::program_options::value< WordList_T >(&lWordList)->multitoken(),
     "Query word list")
    ;

  // Hidden options, will be allowed both on command line and
  // in config file, but will not be shown to the user.
  boost::program_options::options_description hidden ("Hidden options");
  hidden.add_options()
    ("copyright",
     boost::program_options::value< std::vector<std::string> >(),
     "Show the copyright (license)");
        
  boost::program_options::options_description cmdline_options;
  cmdline_options.add(generic).add(config).add(hidden);

  boost::program_options::options_description config_file_options;
  config_file_options.add(config).add(hidden);

  boost::program_options::options_description visible ("Allowed options");
  visible.add(generic).add(config);
        
  boost::program_options::positional_options_description p;
  p.add ("copyright", -1);
        
  boost::program_options::variables_map vm;
  boost::program_options::
    store (boost::program_options::command_line_parser (argc, argv).
           options (cmdline_options).positional(p).run(), vm);

  std::ifstream ifs ("trademgen.cfg");
  boost::program_options::store (parse_config_file (ifs, config_file_options),
                                 vm);
  boost::program_options::notify (vm);
    
  if (vm.count ("help")) {
    std::cout << visible << std::endl;
    return K_TRADEMGEN_EARLY_RETURN_STATUS;
  }

  if (vm.count ("version")) {
    std::cout << PACKAGE_NAME << ", version " << PACKAGE_VERSION << std::endl;
    return K_TRADEMGEN_EARLY_RETURN_STATUS;
  }

  if (vm.count ("prefix")) {
    std::cout << "Installation prefix: " << PREFIXDIR << std::endl;
    return K_TRADEMGEN_EARLY_RETURN_STATUS;
  }

  if (vm.count ("input")) {
    ioInputFilename = vm["input"].as< std::string >();
    std::cout << "Input filename is: " << ioInputFilename << std::endl;
  }

  if (vm.count ("log")) {
    ioLogFilename = vm["log"].as< std::string >();
    std::cout << "Log filename is: " << ioLogFilename << std::endl;
  }

  ioQueryString = createStringFromWordList (lWordList);
  std::cout << "The query string is: " << ioQueryString << std::endl;
  
  return 0;
}


// /////////////// M A I N /////////////////
int main (int argc, char* argv[]) {

  try {

    // Query
    std::string lQuery;

    // Input file name
    stdair::Filename_T lInputFilename;

    // Output log File
    std::string lLogFilename;

    // Airline code
    stdair::AirlineCode_T lAirlineCode ("BA");
    
    // Call the command-line option parser
    const int lOptionParserStatus = 
      readConfiguration (argc, argv, lQuery, lInputFilename, lLogFilename);

    if (lOptionParserStatus == K_TRADEMGEN_EARLY_RETURN_STATUS) {
      return 0;
    }
    
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
    demandCharacteristics1.setOrigin ("LHR");
    demandCharacteristics2.setOrigin ("LHR");
    // destination
    demandCharacteristics1.setDestination ("JFK");
    demandCharacteristics2.setDestination ("JFK");
    // preferred departure date
    demandCharacteristics1.setPreferredDepartureDate (boost::gregorian::date (2010,1,17));
    demandCharacteristics2.setPreferredDepartureDate (boost::gregorian::date (2010,1,18));
    // Passenger type
    demandCharacteristics1.setPaxType ("L");
    demandCharacteristics2.setPaxType ("B");
  
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
  
    // Close the Log outputFile
    logOutputFile.close();

  } catch (const TRADEMGEN::RootException& otexp) {
    std::cerr << "Standard exception: " << otexp.what() << std::endl;
    return -1;
    
  } catch (const std::exception& stde) {
    std::cerr << "Standard exception: " << stde.what() << std::endl;
    return -1;
    
  } catch (...) {
    return -1;
  }
  
  return 0;
}
