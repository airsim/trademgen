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
// Trademgen
#include <trademgen/TRADEMGEN_Service.hpp>
#include <trademgen/config/trademgen-paths.hpp>


// //////// Type definitions ///////
typedef std::vector<std::string> WordList_T;


// //////// Constants //////
/** Default name and location for the log file. */
const std::string K_TRADEMGEN_DEFAULT_LOG_FILENAME ("trademgen_with_db.log");

/** Default name and location for the (CSV) input file. */
const std::string K_TRADEMGEN_DEFAULT_INPUT_FILENAME ("../../test/samples/demand01.csv");

/** Default query string. */
const std::string K_TRADEMGEN_DEFAULT_QUERY_STRING ("my good old query");

/** Default parameters for the database connection. */
const std::string K_TRADEMGEN_DEFAULT_DB_USER ("dsim");
const std::string K_TRADEMGEN_DEFAULT_DB_PASSWD ("dsim");
const std::string K_TRADEMGEN_DEFAULT_DB_DBNAME ("dsim");
const std::string K_TRADEMGEN_DEFAULT_DB_HOST ("localhost");
const std::string K_TRADEMGEN_DEFAULT_DB_PORT ("3306");


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
                       std::string& ioLogFilename,
                       std::string& ioDBUser, std::string& ioDBPasswd,
                       std::string& ioDBHost, std::string& ioDBPort,
                       std::string& ioDBDBName) {

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
    ("user,u",
     boost::program_options::value< std::string >(&ioDBUser)->default_value(K_TRADEMGEN_DEFAULT_DB_USER),
     "SQL database hostname (e.g., dsim)")
    ("passwd,p",
     boost::program_options::value< std::string >(&ioDBPasswd)->default_value(K_TRADEMGEN_DEFAULT_DB_PASSWD),
     "SQL database hostname (e.g., dsim)")
    ("host,H",
     boost::program_options::value< std::string >(&ioDBHost)->default_value(K_TRADEMGEN_DEFAULT_DB_HOST),
     "SQL database hostname (e.g., localhost)")
    ("port,P",
     boost::program_options::value< std::string >(&ioDBPort)->default_value(K_TRADEMGEN_DEFAULT_DB_PORT),
     "SQL database port (e.g., 3306)")
    ("dbname,m",
     boost::program_options::value< std::string >(&ioDBDBName)->default_value(K_TRADEMGEN_DEFAULT_DB_DBNAME),
     "SQL database name (e.g., dsim)")
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

  std::ifstream ifs ("trademgen_with_db.cfg");
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

  if (vm.count ("user")) {
    ioDBUser = vm["user"].as< std::string >();
    std::cout << "SQL database user name is: " << ioDBUser << std::endl;
  }

  if (vm.count ("passwd")) {
    ioDBPasswd = vm["passwd"].as< std::string >();
    // std::cout << "SQL database user password is: " << ioDBPasswd << std::endl;
  }

  if (vm.count ("host")) {
    ioDBHost = vm["host"].as< std::string >();
    std::cout << "SQL database host name is: " << ioDBHost << std::endl;
  }

  if (vm.count ("port")) {
    ioDBPort = vm["port"].as< std::string >();
    std::cout << "SQL database port number is: " << ioDBPort << std::endl;
  }

  if (vm.count ("dbname")) {
    ioDBDBName = vm["dbname"].as< std::string >();
    std::cout << "SQL database name is: " << ioDBDBName << std::endl;
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

    // SQL database parameters
    std::string lDBUser;
    std::string lDBPasswd;
    std::string lDBHost;
    std::string lDBPort;
    std::string lDBDBName;
                       
    // Airline code
    stdair::AirlineCode_T lAirlineCode ("BA");
    
    // Call the command-line option parser
    const int lOptionParserStatus = 
      readConfiguration (argc, argv, lQuery, lInputFilename, lLogFilename,
                         lDBUser, lDBPasswd, lDBHost, lDBPort, lDBDBName);

    if (lOptionParserStatus == K_TRADEMGEN_EARLY_RETURN_STATUS) {
      return 0;
    }
    
    // Set the database parameters
    stdair::BasDBParams lDBParams (lDBUser, lDBPasswd, lDBHost, lDBPort,
                                   lDBDBName);
    
    // Set the log parameters
    std::ofstream logOutputFile;
    // open and clean the log outputfile
    logOutputFile.open (lLogFilename.c_str());
    logOutputFile.clear();

    // Initialise the TraDemGen service object
    const stdair::BasLogParams lLogParams (stdair::LOG::DEBUG, logOutputFile);
    TRADEMGEN::TRADEMGEN_Service trademgenService (lLogParams, lDBParams,
                                                   lInputFilename);

    // Query the database
    trademgenService.displayAirlineListFromDB();

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