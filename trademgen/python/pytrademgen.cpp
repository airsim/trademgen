// STL
#include <cassert>
#include <stdexcept>
#include <fstream>
#include <sstream>
#include <string>
#include <list>
#include <vector>
// Boost String
#include <boost/python.hpp>
// Forecast
#include <forecast/FORECAST_Service.hpp>
// Flood Utilities
#include <Flood/Utilities/Vector.h>
#include <Flood/Utilities/Matrix.h>
#include <Flood/Utilities/InputTargetDataSet.h>
// Flood Network architecture
#include <Flood/MultilayerPerceptron/MultilayerPerceptron.h>
// Flood Objective functional includes
#include <Flood/ObjectiveFunctional/MeanSquaredError.h>
// Flood Training algorithm includes
#include <Flood/TrainingAlgorithm/QuasiNewtonMethod.h>

namespace FORECAST {

  struct Forecaster {
  public:
    /** Wrapper around the search use case. */
    std::string forecast (const std::string& iQuery, const boost::python::list& iTestUnconstrainedDemand, const boost::python::list& iTestDataCollectionPoints) {
      std::ostringstream oStream;

      // Sanity check
      if (_logOutputStream == NULL) {
        oStream << "The log filepath is not valid." << std::endl;
        return oStream.str();
      }
      assert (_logOutputStream != NULL);
      
      try {

        // DEBUG
        *_logOutputStream << "Python search for '" << iQuery << "'"
                          << std::endl;
      
        if (_forecastService == NULL) {
          oStream << "The Forecast service has not been initialised, "
                         << "i.e., the init() method has not been called "
                         << "correctly on the Forecaster object. Please "
                         << "check that all the parameters are not empty and "
                         << "point to actual files.";
          *_logOutputStream << oStream.str();
          return oStream.str();
        }
        assert (_forecastService != NULL);
        
        // Do the forecast
        const double& lForecastOutput =
          _forecastService->calculateForecast (iTestUnconstrainedDemand, iTestDataCollectionPoints);

        //
        oStream << lForecastOutput;
      
        // DEBUG
        *_logOutputStream << "Python search for '" << iQuery
                          << "' returned '" << lForecastOutput << std::endl;

        // DEBUG
        *_logOutputStream << "Forecast output: "
                          << oStream.str() << std::endl;

      } catch (const RootException& eForecastError) {
        *_logOutputStream << "Forecast error: "  << eForecastError.what()
                          << std::endl;
        
      } catch (const std::exception& eStdError) {
        *_logOutputStream << "Error: "  << eStdError.what() << std::endl;
        
      } catch (...) {
        *_logOutputStream << "Unknown error" << std::endl;
      }

      return oStream.str();
    }

  public:
    /** Default constructor. */
    Forecaster() : _forecastService (NULL), _logOutputStream (NULL) {
    }
    
    /** Default copy constructor. */
    Forecaster (const Forecaster& iForecaster)
      : _forecastService (iForecaster._forecastService),
        _logOutputStream (iForecaster._logOutputStream) {
    }

    /** Default constructor. */
    ~Forecaster() {
      _forecastService = NULL;
      _logOutputStream = NULL;
    }
    
    /** Wrapper around the search use case. */
    bool init (const std::string& iLogFilepath,
               const std::string& iDBUser, const std::string& iDBPasswd,
               const std::string& iDBHost, const std::string& iDBPort,
               const std::string& iDBDBName) {
      bool isEverythingOK = true;

      try {
        
        // TODO: use Boost Filesystem to check the filepaths
        if (iLogFilepath.empty() == true) {
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
        
        // Initialise the context
        DBParams lDBParams (iDBUser, iDBPasswd, iDBHost, iDBPort, iDBDBName);
        _forecastService = new FORECAST_Service (*_logOutputStream, lDBParams);
        
        // DEBUG
        *_logOutputStream << "Python wrapper initialised" << std::endl;
        
      } catch (const RootException& eForecastError) {
        *_logOutputStream << "Forecast error: "  << eForecastError.what()
                          << std::endl;
        
      } catch (const std::exception& eStdError) {
        *_logOutputStream << "Error: "  << eStdError.what() << std::endl;
        
      } catch (...) {
        *_logOutputStream << "Unknown error" << std::endl;
      }
      
      return isEverythingOK;
    }

  private:
    /** Handle on the Forecast services (API). */
    FORECAST_Service* _forecastService;
    std::ofstream* _logOutputStream;
  };

}

// /////////////////////////////////////////////////////////////
BOOST_PYTHON_MODULE(libpyforecast) {
  boost::python::class_<FORECAST::Forecaster> ("Forecaster")
    .def ("forecast", &FORECAST::Forecaster::forecast)
    .def ("init", &FORECAST::Forecaster::init);
}
