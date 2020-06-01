// Boost Python
#include <boost/python.hpp>
// STL
#include <cassert>
#include <stdexcept>
#include <fstream>
#include <sstream>
#include <string>
// Boost Accumulators
#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics/stats.hpp>
#include <boost/accumulators/statistics/mean.hpp>
#include <boost/accumulators/statistics/moment.hpp>
#include <boost/accumulators/statistics/min.hpp>
#include <boost/accumulators/statistics/max.hpp>
#include <boost/accumulators/statistics/variance.hpp>
// Boost Timer (progress display)
#if BOOST_VERSION_MACRO >= 107200
#include <boost/timer/progress_display.hpp>
#else // BOOST_VERSION_MACRO >= 107200
#include <boost/progress.hpp>
#endif // BOOST_VERSION_MACRO >= 107200
// StdAir
#include <stdair/stdair_basic_types.hpp>
#include <stdair/basic/ProgressStatusSet.hpp>
#include <stdair/basic/DemandGenerationMethod.hpp>
#include <stdair/bom/EventStruct.hpp>
#include <stdair/bom/BookingRequestStruct.hpp>
//#include <stdair/bom/BomDisplay.hpp>
#include <stdair/service/Logger.hpp>
// TraDemGen
#include <trademgen/TRADEMGEN_Service.hpp>
#include <trademgen/config/trademgen-paths.hpp>

// Aliases for namespaces
namespace ba = boost::accumulators;

// //////// Specific type definitions ///////
typedef unsigned int NbOfRuns_T;

/**
 * Type definition to gather statistics.
 */
typedef ba::accumulator_set<double,
                            ba::stats<ba::tag::min, ba::tag::max,
                                      ba::tag::mean (ba::immediate),
                                      ba::tag::sum,
                                      ba::tag::variance> > stat_acc_type;

namespace TRADEMGEN {

  /**
   * Display the statistics held by the dedicated accumulator.
   */
  void stat_display (std::ostream& oStream, const stat_acc_type& iStatAcc) {

    // Store current formatting flags of the output stream
    std::ios::fmtflags oldFlags = oStream.flags();

    //
    oStream.setf (std::ios::fixed);
  
    //
    oStream << "Statistics for the demand generation runs: " << std::endl;
    oStream << "  minimum   = " << ba::min (iStatAcc) << std::endl;
    oStream << "  mean      = " << ba::mean (iStatAcc) << std::endl;
    oStream << "  maximum   = " << ba::max (iStatAcc) << std::endl;
    oStream << "  count     = " << ba::count (iStatAcc) << std::endl;
    oStream << "  variance  = " << ba::variance (iStatAcc) << std::endl;
  
    // Reset formatting flags of output stream
    oStream.flags (oldFlags);
  }

  /**
   * @brief Wrapper structure around the C++ API, so as to expose a Python API.
   */
  struct Trademgener {
  public:
    /**
     * Wrapper around the travel demand generation use case.
     */
    std::string
    trademgen (const NbOfRuns_T& iNbOfRuns,
               const std::string& iDemandGenerationMethodString) {
      std::ostringstream oStream;

      // Convert the input string into a demand generation method enumeration
      const stdair::DemandGenerationMethod
        iDemandGenerationMethod (iDemandGenerationMethodString);

      // Sanity check
      if (_logOutputStream == NULL) {
        oStream << "The log filepath is not valid." << std::endl;
        return oStream.str();
      }
      assert (_logOutputStream != NULL);
      
      try {

        // DEBUG
        *_logOutputStream << "Demand generation for " << iNbOfRuns << " runs, "
                          << "with the following method: "
                          << iDemandGenerationMethod << std::endl;
      
        if (_trademgenService == NULL) {
          oStream << "The TraDemGen service has not been initialised, "
                  << "i.e., the init() method has not been called "
                  << "correctly on the Trademgener object. Please "
                  << "check that all the parameters are not empty and "
                  << "point to actual files.";
          *_logOutputStream << oStream.str();
          return oStream.str();
        }
        assert (_trademgenService != NULL);

        // Initialise the statistics collector/accumulator
        stat_acc_type lStatAccumulator;

        // Retrieve the expected (mean value of the) number of events to be
        // generated
        const stdair::Count_T& lExpectedNbOfEventsToBeGenerated =
          _trademgenService->getExpectedTotalNumberOfRequestsToBeGenerated();

        // Initialise the (Boost) progress display object
#if BOOST_VERSION_MACRO >= 107200
        boost::timer::progress_display
#else  // BOOST_VERSION_MACRO >= 107200
        boost::progress_display
#endif // BOOST_VERSION_MACRO >= 107200
          lProgressDisplay (lExpectedNbOfEventsToBeGenerated * iNbOfRuns);

        for (NbOfRuns_T runIdx = 1; runIdx <= iNbOfRuns; ++runIdx) {
          // /////////////////////////////////////////////////////
          *_logOutputStream << "Run number: " << runIdx << std::endl;

          /**
             Initialisation step.
             <br>Generate the first event for each demand stream.
          */
          const stdair::Count_T& lActualNbOfEventsToBeGenerated =
            _trademgenService->generateFirstRequests (iDemandGenerationMethod);

          // DEBUG
          *_logOutputStream << "[" << runIdx << "] Expected: "
                            << lExpectedNbOfEventsToBeGenerated << ", actual: "
                            << lActualNbOfEventsToBeGenerated << std::endl;

          /**
             Main loop.
             <ul>
             <li>Pop a request and get its associated type/demand stream.</li>
             <li>Generate the next request for the same type/demand stream.</li>
             </ul>
          */
          while (_trademgenService->isQueueDone() == false) {

            // Extract the next event from the event queue
            stdair::EventStruct lEventStruct;
            stdair::ProgressStatusSet lProgressStatusSet =
              _trademgenService->popEvent (lEventStruct);

            // DEBUG
            // STDAIR_LOG_DEBUG ("[" << runIdx << "] Poped event: '"
            //                   << lEventStruct.describe() << "'.");

            // Extract the corresponding demand/booking request
            const stdair::BookingRequestStruct& lPoppedRequest =
              lEventStruct.getBookingRequest();

            // DEBUG
            *_logOutputStream << "[" << runIdx << "] Poped booking request: '"
                              << lPoppedRequest.describe() << "'." << std::endl;

            // Dump the request into the dedicated CSV file
            // stdair::BomDisplay::csvDisplay (output, lPoppedRequest);

            // Retrieve the corresponding demand stream key
            const stdair::DemandGeneratorKey_T& lDemandStreamKey =
              lPoppedRequest.getDemandGeneratorKey();

            // Assess whether more events should be generated for that
            // demand stream
            const bool stillHavingRequestsToBeGenerated = _trademgenService->
              stillHavingRequestsToBeGenerated (lDemandStreamKey,
                                                lProgressStatusSet,
                                                iDemandGenerationMethod);

            // DEBUG
            *_logOutputStream << lProgressStatusSet.describe() << std::endl;
            *_logOutputStream << "=> [" << lDemandStreamKey
                              << "] is now processed. Still generate events "
                              << "for that demand stream? "
                              << stillHavingRequestsToBeGenerated << std::endl;
    
            // If there are still events to be generated for that demand
            // stream, generate and add them to the event queue
            if (stillHavingRequestsToBeGenerated == true) {

              stdair::BookingRequestPtr_T lNextRequest_ptr =
                _trademgenService->generateNextRequest(lDemandStreamKey,
                                                       iDemandGenerationMethod);
        
              assert (lNextRequest_ptr != NULL);

              // Sanity check
              const stdair::Duration_T lDuration =
                lNextRequest_ptr->getRequestDateTime()
                - lPoppedRequest.getRequestDateTime();
              if (lDuration.total_milliseconds() < 0) {
                *_logOutputStream << "[" << lDemandStreamKey
                                  << "] The date-time of the generated event ("
                                  << lNextRequest_ptr->getRequestDateTime()
                                  << ") is lower than the date-time "
                                  << "of the current event ("
                                  << lPoppedRequest.getRequestDateTime()
                                  << ")" << std::endl;
                assert (false);
              }

              // DEBUG
              *_logOutputStream << "[" << lDemandStreamKey
                                << "] Added request: '"
                                << lNextRequest_ptr->describe()
                                << "'. Is queue done? "
                                << _trademgenService->isQueueDone()
                                << std::endl;
            }
            // DEBUG
            *_logOutputStream << std::endl;
      
            // Update the progress display
            ++lProgressDisplay;
          }

          // Add the number of events to the statistics accumulator
          lStatAccumulator (lActualNbOfEventsToBeGenerated);
    
          // Reset the service (including the event queue) for the next run
          _trademgenService->reset();
        }

        // DEBUG
        *_logOutputStream << "End of the demand generation. Following are some "
                          << "statistics for the " << iNbOfRuns << " runs."
                          << std::endl;
        std::ostringstream oStatStr;
        stat_display (oStatStr, lStatAccumulator);
        *_logOutputStream << oStatStr.str() << std::endl;

        // DEBUG
        const std::string& lBOMStr = _trademgenService->csvDisplay();
        *_logOutputStream << lBOMStr << std::endl;

        // DEBUG
        *_logOutputStream << "TraDemGen output: "
                          << oStream.str() << std::endl;

      } catch (const stdair::RootException& eTrademgenError) {
        oStream << "TraDemGen error: "  << eTrademgenError.what() << std::endl;
        
      } catch (const std::exception& eStdError) {
        oStream << "Error: "  << eStdError.what() << std::endl;
        
      } catch (...) {
        oStream << "Unknown error" << std::endl;
      }

      //
      oStream << "TraDemGen has completed the generation of the booking "
              << "requests. See the log file for more details." << std::endl;

      return oStream.str();
    }

  public:
    /** Default constructor. */
    Trademgener() : _trademgenService (NULL), _logOutputStream (NULL) {
    }
    
    /** Default copy constructor. */
    Trademgener (const Trademgener& iTrademgener)
      : _trademgenService (iTrademgener._trademgenService),
        _logOutputStream (iTrademgener._logOutputStream) {
    }

    /** Default constructor. */
    ~Trademgener() {
      _trademgenService = NULL;
      _logOutputStream = NULL;
    }
    
    /**
     * Wrapper around the search use case.
     */
    bool init (const std::string& iLogFilepath,
               const stdair::RandomSeed_T& iRandomSeed, const bool isBuiltin,
               const stdair::Filename_T& iDemandInputFilename) {
      bool isEverythingOK = true;

      try {
        
        // Check that the file path given as input corresponds to an actual file
        const bool isWriteable = (iLogFilepath.empty() == false);
        // stdair::BasFileMgr::isWriteable (iLogFilepath);
        if (isWriteable == false) {
          isEverythingOK = false;
          return isEverythingOK;
        }
        
        // Set the log parameters
        _logOutputStream = new std::ofstream;
        assert (_logOutputStream != NULL);
        
        // Open and clean the log outputfile
        _logOutputStream->open (iLogFilepath.c_str());
        _logOutputStream->clear();
        
        // DEBUG
        *_logOutputStream << "Python wrapper initialisation" << std::endl;
        const stdair::BasLogParams lLogParams (stdair::LOG::DEBUG,
                                               *_logOutputStream);
        
        // Initialise the context
        _trademgenService = new TRADEMGEN_Service (lLogParams, iRandomSeed);
        assert (_trademgenService != NULL);

        // Check wether or not a (CSV) input file should be read
        if (isBuiltin == true) {
          // Create a sample DemandStream object, and insert it within
          // the BOM tree
          _trademgenService->buildSampleBom();

        } else {
          // Create the DemandStream objects, and insert them within
          // the BOM tree
          const DemandFilePath lDemandFilePath (iDemandInputFilename);
          _trademgenService->parseAndLoad (lDemandFilePath);
        }

        // DEBUG
        *_logOutputStream << "Python wrapper initialised" << std::endl;
        
      } catch (const stdair::RootException& eTrademgenError) {
        *_logOutputStream << "Trademgen error: "  << eTrademgenError.what()
                          << std::endl;
        
      } catch (const std::exception& eStdError) {
        *_logOutputStream << "Error: "  << eStdError.what() << std::endl;
        
      } catch (...) {
        *_logOutputStream << "Unknown error" << std::endl;
      }
      
      return isEverythingOK;
    }

  private:
    /** Handle on the Trademgen services (API). */
    TRADEMGEN_Service* _trademgenService;
    std::ofstream* _logOutputStream;
  };

}

// /////////////////////////////////////////////////////////////
BOOST_PYTHON_MODULE(pytrademgen) {
  boost::python::class_<TRADEMGEN::Trademgener> ("Trademgener")
    .def ("trademgen", &TRADEMGEN::Trademgener::trademgen)
    .def ("init", &TRADEMGEN::Trademgener::init);
}
