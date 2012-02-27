/*!
 * \page batch_airinv_cpp Command-Line Utility to Demonstrate Typical AirInv Usage
 * \code
 */
// STL
#include <cassert>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
// Boost (Extended STL)
#include <boost/program_options.hpp>
#include <boost/tokenizer.hpp>
#include <boost/regex.hpp>
#include <boost/swap.hpp>
#include <boost/algorithm/string/case_conv.hpp>
// GNU Readline Wrapper
#include <stdair/ui/cmdline/SReadline.hpp>
// StdAir
#include <stdair/stdair_basic_types.hpp>
#include <stdair/stdair_json.hpp>
#include <stdair/basic/BasConst_General.hpp>
#include <stdair/basic/ProgressStatusSet.hpp>
#include <stdair/basic/DemandGenerationMethod.hpp>
#include <stdair/bom/EventStruct.hpp>
#include <stdair/bom/BookingRequestStruct.hpp>
#include <stdair/bom/BomDisplay.hpp>
#include <stdair/service/Logger.hpp>
// TraDemGen
#include <trademgen/TRADEMGEN_Service.hpp>
#include <trademgen/config/trademgen-paths.hpp>


// //////// Type definitions ///////
/**
 * List of workds for a query.
 */
typedef std::vector<std::string> WordList_T;

// //////// Specific type definitions ///////
typedef unsigned int NbOfRuns_T;

// //////// Constants //////
/**
 * Default name and location for the log file.
 */
const stdair::Filename_T K_TRADEMGEN_DEFAULT_LOG_FILENAME ("trademgen.log");

/**
 * Default name and location for the (CSV) input file.
 */
const stdair::Filename_T K_TRADEMGEN_DEFAULT_INPUT_FILENAME (STDAIR_SAMPLE_DIR
                                                             "/demand01.csv");

/**
 * Default name and location for the (CSV) output file.
 */
const stdair::Filename_T K_TRADEMGEN_DEFAULT_OUTPUT_FILENAME ("request.csv");

/**
 * Default demand generation method: Poisson Process.
 */
const stdair::DemandGenerationMethod
K_TRADEMGEN_DEFAULT_DEMAND_GENERATION_METHOD =
  stdair::DemandGenerationMethod::POI_PRO;

/**
 * Default demand generation method name: 'P' for Poisson Process.
 */
const char K_TRADEMGEN_DEFAULT_DEMAND_GENERATION_METHOD_CHAR =
  K_TRADEMGEN_DEFAULT_DEMAND_GENERATION_METHOD.getMethodAsChar();

/**
 * Default random generation seed (e.g., 120765987).
 */
const stdair::RandomSeed_T K_TRADEMGEN_DEFAULT_RANDOM_SEED =
  stdair::DEFAULT_RANDOM_SEED;

/**
 * Default number of random draws to be generated (best if over 100).
 */
const NbOfRuns_T K_TRADEMGEN_DEFAULT_RANDOM_DRAWS = 1;

/**
 * Default for the input type. It can be either built-in or provided by an
 * input file. That latter must then be given with the -i option.
 */
const bool K_TRADEMGEN_DEFAULT_BUILT_IN_INPUT = false;

/**
 * Early return status (so that it can be differentiated from an error).
 */
const int K_TRADEMGEN_EARLY_RETURN_STATUS = 99;

// //////////////////////////////////////////////////////////////////////
/**
 * List of strings, representing the tokens as entered by the user on
 * a command-line.
 */
typedef std::vector<std::string> TokenList_T;

/**
 * Enumeration representing the command entered by the user on the command-line.
 */
struct Command_T {
  typedef enum {
    NOP = 0,
    QUIT,
    HELP,
    DISPLAY,
    LIST,
    RESET,
    NEXT,
    GENERATE_BR,
    JSON_LIST,
    LAST_VALUE
  } Type_T;
};

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
  
}// //////////////////////////////////////////////////////////////////////
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
/**
 * Read and parse the command line options.
 */
int readConfiguration (int argc, char* argv[], bool& ioIsBuiltin,
                       stdair::RandomSeed_T& ioRandomSeed,
                       NbOfRuns_T& ioRandomRuns,
                       stdair::Filename_T& ioInputFilename,
                       stdair::Filename_T& ioOutputFilename,
                       stdair::Filename_T& ioLogFilename,
                       stdair::DemandGenerationMethod& ioDemandGenerationMethod) {

  // Demand generation method as a single char (e.g., 'P' or 'S').
  char lDemandGenerationMethodChar;

  // Default for the built-in input
  ioIsBuiltin = K_TRADEMGEN_DEFAULT_BUILT_IN_INPUT;

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
    ("builtin,b",
     "The sample BOM tree can be either built-in or parsed from an input file. That latter must then be given with the -i/--input option")
    ("seed,s",
     boost::program_options::value<stdair::RandomSeed_T>(&ioRandomSeed)->default_value(K_TRADEMGEN_DEFAULT_RANDOM_SEED),
     "Seed for the random generation")
    ("draws,d",
     boost::program_options::value<NbOfRuns_T>(&ioRandomRuns)->default_value(K_TRADEMGEN_DEFAULT_RANDOM_DRAWS), 
     "Number of runs for the demand generations")
    ("demandgeneration,G",
     boost::program_options::value< char >(&lDemandGenerationMethodChar)->default_value(K_TRADEMGEN_DEFAULT_DEMAND_GENERATION_METHOD_CHAR),
     "Method used to generate the demand (i.e., the booking requests): Poisson Process (P) or Order Statistics (S)")
    ("input,i",
     boost::program_options::value< std::string >(&ioInputFilename)->default_value(K_TRADEMGEN_DEFAULT_INPUT_FILENAME),
     "(CSV) input file for the demand distributions")
    ("output,o",
     boost::program_options::value< std::string >(&ioOutputFilename)->default_value(K_TRADEMGEN_DEFAULT_OUTPUT_FILENAME),
     "(CSV) output file for the generated requests")
    ("log,l",
     boost::program_options::value< std::string >(&ioLogFilename)->default_value(K_TRADEMGEN_DEFAULT_LOG_FILENAME),
     "Filepath for the logs")
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

  if (vm.count ("builtin")) {
    ioIsBuiltin = true;
  }
  const std::string isBuiltinStr = (ioIsBuiltin == true)?"yes":"no";
  std::cout << "The BOM should be built-in? " << isBuiltinStr << std::endl;

  if (ioIsBuiltin == false) {

    // The BOM tree should be built from parsing a demand input file
    if (vm.count ("input")) {
      ioInputFilename = vm["input"].as< std::string >();
      std::cout << "Input filename is: " << ioInputFilename << std::endl;

    } else {
      // The built-in option is not selected. However, no demand input file
      // is specified
      std::cerr << "Either one among the -b/--builtin and -i/--input "
                << "options must be specified" << std::endl;
    }
  }

  if (vm.count ("output")) {
    ioOutputFilename = vm["output"].as< std::string >();
    std::cout << "Output filename is: " << ioOutputFilename << std::endl;
  }

  if (vm.count ("log")) {
    ioLogFilename = vm["log"].as< std::string >();
    std::cout << "Log filename is: " << ioLogFilename << std::endl;
  }

  if (vm.count ("demandgeneration")) {
    ioDemandGenerationMethod =
      stdair::DemandGenerationMethod (lDemandGenerationMethodChar);
    std::cout << "Date-time request generation method is: "
              << ioDemandGenerationMethod.describe() << std::endl;
  }

  //
  std::cout << "The random generation seed is: " << ioRandomSeed << std::endl;

  //
  std::cout << "The number of runs is: " << ioRandomRuns << std::endl;
  
  return 0;
}

// //////////////////////////////////////////////////////////////////
void initReadline (swift::SReadline& ioInputReader) {

  // Prepare the list of my own completers
  std::vector<std::string> Completers;

  // The following is supported:
  // - "identifiers"
  // - special identifier %file - means to perform a file name completion
  Completers.push_back ("help");
  Completers.push_back ("display");
  Completers.push_back ("list");
  Completers.push_back ("reset");
  Completers.push_back ("generate_br");
  Completers.push_back ("next");
  Completers.push_back ("json_list");
  Completers.push_back ("quit");

  // Now register the completers.
  // Actually it is possible to re-register another set at any time
  ioInputReader.RegisterCompletions (Completers);
}

// //////////////////////////////////////////////////////////////////
Command_T::Type_T extractCommand (TokenList_T& ioTokenList) {
  Command_T::Type_T oCommandType = Command_T::LAST_VALUE;

  // Interpret the user input
  if (ioTokenList.empty() == false) {
    TokenList_T::iterator itTok = ioTokenList.begin();
    std::string lCommand (*itTok);
    boost::algorithm::to_lower (lCommand);
    
    if (lCommand == "help") {
      oCommandType = Command_T::HELP;

    } else if (lCommand == "display") {
      oCommandType = Command_T::DISPLAY;

    } else if (lCommand == "list") {
      oCommandType = Command_T::LIST;

    } else if (lCommand == "reset") {
      oCommandType = Command_T::RESET;

    } else if (lCommand == "next") {
      oCommandType = Command_T::NEXT;

    } else if (lCommand == "generate_br") {
      oCommandType = Command_T::GENERATE_BR;

    } else if (lCommand == "json_list") {
      oCommandType = Command_T::JSON_LIST;

    } else if (lCommand == "quit") {
      oCommandType = Command_T::QUIT;
    }

    // Remove the first token (the command), as the corresponding information
    // has been extracted in the form of the returned command type enumeration
    ioTokenList.erase (itTok);

  } else {
    oCommandType = Command_T::NOP;
  }

  return oCommandType;
}

// /////////////////////////////////////////////////////////
std::string toString (const TokenList_T& iTokenList) {
  std::ostringstream oStr;

  // Re-create the string with all the tokens, trimmed by read-line
  unsigned short idx = 0;
  for (TokenList_T::const_iterator itTok = iTokenList.begin();
       itTok != iTokenList.end(); ++itTok, ++idx) {
    if (idx != 0) {
      oStr << " ";
    }
    oStr << *itTok;
  }

  return oStr.str();
}

// ///////// M A I N ////////////
int main (int argc, char* argv[]) {

  // Readline history
  const unsigned int lHistorySize (100);
  const std::string lHistoryFilename ("trademgen.hist");
  const std::string lHistoryBackupFilename ("trademgen.hist.bak");

  // Default parameters for the interactive session
  stdair::EventStruct lCurrentInteractiveEventStruct; 
  stdair::DateTime_T lCurrentInteractiveDateTime; 
  stdair::EventType::EN_EventType lCurrentInteractiveEventType;

  // State whether the BOM tree should be built-in or parsed from an input file
  bool isBuiltin;

  // Random generation seed
  stdair::RandomSeed_T lRandomSeed;

  // Number of random draws to be generated (best if greater than 100)
  NbOfRuns_T lNbOfRuns;
    
  // Input file name
  stdair::Filename_T lInputFilename;

  // Output file name
  stdair::Filename_T lOutputFilename;

  // Output log File
  stdair::Filename_T lLogFilename;
  
  // Demand generation method.
  stdair::DemandGenerationMethod
    lDemandGenerationMethod (K_TRADEMGEN_DEFAULT_DEMAND_GENERATION_METHOD);

  // Call the command-line option parser
  const int lOptionParserStatus = 
    readConfiguration (argc, argv, isBuiltin, lRandomSeed, lNbOfRuns,
                       lInputFilename, lOutputFilename, lLogFilename,
                       lDemandGenerationMethod);

  if (lOptionParserStatus == K_TRADEMGEN_EARLY_RETURN_STATUS) {
    return 0;
  }

  // Set the log parameters
  std::ofstream logOutputFile;
  // Open and clean the log outputfile
  logOutputFile.open (lLogFilename.c_str());
  logOutputFile.clear();

  // Set up the log parameters
  const stdair::BasLogParams lLogParams (stdair::LOG::DEBUG, logOutputFile);

  // Initialise the TraDemGen service object
  TRADEMGEN::TRADEMGEN_Service trademgenService (lLogParams, lRandomSeed);

  // Check wether or not a (CSV) input file should be read
  if (isBuiltin == true) {
    // Create a sample DemandStream object, and insert it within the BOM tree
    trademgenService.buildSampleBom();

  } else {
    // Create the DemandStream objects, and insert them within the BOM tree
    trademgenService.parseAndLoad (lInputFilename);
  }

  // Retrieve the expected (mean value of the) number of events to be
  // generated
  const stdair::Count_T& lExpectedNbOfEventsToBeGenerated =
    trademgenService.getExpectedTotalNumberOfRequestsToBeGenerated();

  // Generate the first event for each demand stream.
  const stdair::Count_T& lActualNbOfEventsToBeGenerated =
    trademgenService.generateFirstRequests (lDemandGenerationMethod);
  
  // DEBUG
  std::ostringstream oStatusStream;
  oStatusStream << "Expected: " << lExpectedNbOfEventsToBeGenerated
                << ", actual: " << lActualNbOfEventsToBeGenerated;
  std::cout << oStatusStream.str() << std::endl;
  STDAIR_LOG_DEBUG (oStatusStream.str());

  // Pop out the first event from the queue.
  trademgenService.popEvent(lCurrentInteractiveEventStruct);

  // DEBUG
  STDAIR_LOG_DEBUG ("====================================================");
  STDAIR_LOG_DEBUG ("=       Beginning of the interactive session       =");
  STDAIR_LOG_DEBUG ("====================================================");

  // Initialise the GNU readline wrapper
  swift::SReadline lReader (lHistoryFilename, lHistorySize);
  initReadline (lReader);

  // Now we can ask user for a line
  std::string lUserInput;
  bool EndOfInput (false);
  Command_T::Type_T lCommandType (Command_T::NOP);
  
  while (lCommandType != Command_T::QUIT && EndOfInput == false) {

    // Update the interactive parameters which have not been updated yet
    lCurrentInteractiveDateTime = lCurrentInteractiveEventStruct.getEventTime ();
    lCurrentInteractiveEventType = lCurrentInteractiveEventStruct.getEventType ();
    
    // Prompt
    std::ostringstream oPromptStr;
    oPromptStr << "trademgen "
               << stdair::EventType::getTypeLabelAsString(lCurrentInteractiveEventType)
	       << " / " << lCurrentInteractiveDateTime << "> " ;
    // Call read-line, which will fill the list of tokens
    TokenList_T lTokenListByReadline;
    lUserInput = lReader.GetLine (oPromptStr.str(), lTokenListByReadline,
                                  EndOfInput);

    // The history can be saved to an arbitrary file at any time
    lReader.SaveHistory (lHistoryBackupFilename);

    // The end-of-input typically corresponds to a CTRL-D typed by the user
    if (EndOfInput) {
      std::cout << std::endl;
      break;
    }

    // Interpret the user input
    lCommandType = extractCommand (lTokenListByReadline);

    switch (lCommandType) {

      // ////////////////////////////// Help ////////////////////////
    case Command_T::HELP: {
      std::cout << std::endl;
      std::cout << "Commands: " << std::endl;
      std::cout << " help" << "\t\t" << "Display this help" << std::endl;
      std::cout << " quit" << "\t\t" << "Quit the application" << std::endl;
      std::cout << " display" << "\t"
                << "Display the current event" << std::endl;
      std::cout << " list" << "\t\t" << "List events in the queue" << std::endl;
      std::cout << " reset" << "\t\t" << "Reset the service (including the "
                << "event queue) and generate the first event for each demand "
                << "stream" << std::endl;
      std::cout << " generate_br" << "   " << " If there are still "
                << "events to be generated for the current demand stream (i.e. the "
                << "stream of the current event),\n\t\tgenerate the next event "
                << "and add it to the event queue" << std::endl;
      std::cout << " next" << "\t\t"
                << "Play the current event and pop the next one from the queue"
                << std::endl;
      std::cout << " \nDebug Commands" << std::endl;
      std::cout << " json_list" << "\t"
                << "List events in the queue in a JSON format"
                << std::endl;
      std::cout << std::endl;
      break;
    }
 
      // ////////////////////////////// Quit ////////////////////////
    case Command_T::QUIT: {
      break;
    }

      // ////////////////////////////// Display ////////////////////////
    case Command_T::DISPLAY: {
      //
      std::cout << "Display" << std::endl;     

      // DEBUG 
      std::ostringstream oEventStr;
      oEventStr << lCurrentInteractiveEventStruct.describe(); 
      std::cout << oEventStr.str() << std::endl;
      STDAIR_LOG_DEBUG (oEventStr.str());

      //
      break;
    }

      // ////////////////////////////// List /////////////////////////
    case Command_T::LIST: {
      //
      std::cout << "List" << std::endl;   

      std::ostringstream oEventListStr;
      oEventListStr << trademgenService.list ();	
      std::cout << oEventListStr.str() << std::endl;   
      STDAIR_LOG_DEBUG (oEventListStr.str());

      //
      break;
    }

      // ////////////////////////////// Reset /////////////////////////
    case Command_T::RESET: {

      std::cout << "Reset" << std::endl;

      // Reset the service (including the event queue) for the next run
      trademgenService.reset();
      
      // Retrieve the expected (mean value of the) number of events to be
      // generated
      const stdair::Count_T& lExpectedNbOfEventsToBeGenerated =
        trademgenService.getExpectedTotalNumberOfRequestsToBeGenerated();
      
      // Generate the first event for each demand stream.
      const stdair::Count_T& lActualNbOfEventsToBeGenerated =
        trademgenService.generateFirstRequests (lDemandGenerationMethod);
  
      // DEBUG
      std::ostringstream oStatusStream;
      oStatusStream << "Expected: " << lExpectedNbOfEventsToBeGenerated
                    << ", actual: " << lActualNbOfEventsToBeGenerated;
      std::cout << oStatusStream.str() << std::endl;
      STDAIR_LOG_DEBUG (oStatusStream.str());
      
      // Pop out the first event from the queue.
      trademgenService.popEvent(lCurrentInteractiveEventStruct);
        
      break;
    }

      // ////////////////////////////// Generate requests ////////////////////////
    case Command_T::GENERATE_BR: {

      std::cout << "Generate next request" << std::endl;

      // Extract the corresponding demand/booking request
      const stdair::BookingRequestStruct& lPoppedRequest =
        lCurrentInteractiveEventStruct.getBookingRequest();

      // Retrieve the corresponding demand stream key
      const stdair::DemandGeneratorKey_T& lDemandStreamKey =
        lPoppedRequest.getDemandGeneratorKey();

      stdair::ProgressStatusSet lProgressStatusSet (stdair::EventType::BKG_REQ);
      const bool stillHavingRequestsToBeGenerated =
        trademgenService.stillHavingRequestsToBeGenerated (lDemandStreamKey,
                                                           lProgressStatusSet,
                                                           lDemandGenerationMethod);
      if (stillHavingRequestsToBeGenerated == false) {
        // DEBUG 
        std::ostringstream oNoMoreEventToGenerateStr;
        oNoMoreEventToGenerateStr << "No more events to generate for the demand "
                                  << "stream: '" << lDemandStreamKey << "'.";
        std::cout << oNoMoreEventToGenerateStr.str() << std::endl;
        STDAIR_LOG_DEBUG (oNoMoreEventToGenerateStr.str());
        break;
      }
      assert (stillHavingRequestsToBeGenerated == true);
      
      trademgenService.generateNextRequest (lDemandStreamKey, lDemandGenerationMethod);
      
      // DEBUG 
      std::ostringstream oOneMoreEventGeneratedStr;
      oOneMoreEventGeneratedStr << "One more event have been generated for the demand "
                                << "stream: '" << lDemandStreamKey << "'.";
      std::cout << oOneMoreEventGeneratedStr.str() << std::endl;
      STDAIR_LOG_DEBUG (oOneMoreEventGeneratedStr.str());

      break;
    }

      // ////////////////////////////// Next ////////////////////////
    case Command_T::NEXT: {
      //
      std::cout << "Next" << std::endl;  

      if (trademgenService.isQueueDone() == true) { 

	// DEBUG 
	std::ostringstream oEmptyQueueStr;
	oEmptyQueueStr << "The event queue is empty: no event can be popped out.";
	std::cout << oEmptyQueueStr.str() << std::endl;   
	STDAIR_LOG_DEBUG (oEmptyQueueStr.str());

	//
	break;

      }

      // Get the next event from the event queue
      trademgenService.popEvent (lCurrentInteractiveEventStruct);

      // DEBUG 
      std::ostringstream oEventStr;
      oEventStr << "Poped event: '" 
		<< lCurrentInteractiveEventStruct.describe() << "'.";
      std::cout << oEventStr.str() << std::endl;
      STDAIR_LOG_DEBUG (oEventStr.str());

      //
      break;
    }     
      // ////////////////////////////// JSon Event List ////////////////////////

    case Command_T::JSON_LIST: {
      //
      std::cout << "JSON Event List" << std::endl;

      std::ostringstream lMyCommandJSONstream;
      lMyCommandJSONstream << "{\"event_list\":"
                           << "{ \"event_type\":\"" << "all"
			   << "\"}}";

      // Delegate the call to the dedicated service
      const stdair::JSONString lJSONCommandString (lMyCommandJSONstream.str());
      const std::string& lCSVEventListDump =
        trademgenService.jsonHandler (lJSONCommandString);

      // DEBUG: Display the events queue JSON string
      std::cout << lCSVEventListDump << std::endl;
      STDAIR_LOG_DEBUG (lCSVEventListDump);

      break;
    } 

      // /////////////////////////// Default / No value ///////////////////////
    case Command_T::NOP: {
      break;
    }
 
    case Command_T::LAST_VALUE:
    default: {
      // DEBUG
      std::ostringstream oStr;
      oStr << "That command is not yet understood: '" << lUserInput
           << "' => " << lTokenListByReadline;
      STDAIR_LOG_DEBUG (oStr.str());
      std::cout << oStr.str() << std::endl;
    }
    }
  }

  // DEBUG
  STDAIR_LOG_DEBUG ("End of the session. Exiting.");
  std::cout << "End of the session. Exiting." << std::endl;

  // Close the Log outputFile
  logOutputFile.close();

  /*
    Note: as that program is not intended to be run on a server in
    production, it is better not to catch the exceptions. When it
    happens (that an exception is throwned), that way we get the
    call stack.
  */

  return 0;	
}
