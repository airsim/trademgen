// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <sstream>
// Boost
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/posix_time/ptime.hpp>
// FORECAST
#include <forecast/basic/BasConst_FORECAST_Service.hpp>
#include <forecast/basic/BasChronometer.hpp>
#include <forecast/command/SociSessionManager.hpp>
#include <forecast/command/DBManager.hpp>
#include <forecast/factory/FacForecastServiceContext.hpp>
#include <forecast/service/FORECAST_ServiceContext.hpp>
#include <forecast/service/Logger.hpp>
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

  // //////////////////////////////////////////////////////////////////////
  FORECAST_Service::
  FORECAST_Service (std::ostream& ioLogStream, const DBParams& iDBParams)
    : _forecastServiceContext (NULL) {
    init (ioLogStream, iDBParams);
  }

  // //////////////////////////////////////////////////////////////////////
  FORECAST_Service::FORECAST_Service ()
    : _forecastServiceContext (NULL) {
    assert (false);
  }

  // //////////////////////////////////////////////////////////////////////
  FORECAST_Service::FORECAST_Service (const FORECAST_Service& iService) {
    assert (false);
  }

  // //////////////////////////////////////////////////////////////////////
  FORECAST_Service::~FORECAST_Service () {
    // Delete/Clean all the objects from memory
    finalise();
  }

  // //////////////////////////////////////////////////////////////////////
  void logInit (const LOG::EN_LogLevel iLogLevel,
                std::ostream& ioLogOutputFile) {
    Logger::instance().setLogParameters (iLogLevel, ioLogOutputFile);
  }

  // //////////////////////////////////////////////////////////////////////
  void FORECAST_Service::init (std::ostream& ioLogStream,
                               const DBParams& iDBParams) {
    // Set the log file
    logInit (LOG::DEBUG, ioLogStream);

    // Initialise the context
    FORECAST_ServiceContext& lFORECAST_ServiceContext = 
      FacForecastServiceContext::instance().create ("DEBUG_TO_BE_REMOVED");
    _forecastServiceContext = &lFORECAST_ServiceContext;

    // Initialise the SOCI Session
    soci::session* lSociSession_ptr = lFORECAST_ServiceContext.getSociSession();
    SociSessionManager::init (lSociSession_ptr, iDBParams);
    assert (lSociSession_ptr != NULL);
    lFORECAST_ServiceContext.setSociSession (*lSociSession_ptr);

    // Instanciate an empty World object
    // World& lWorld = FacWorld::instance().create();
    // lFORECAST_ServiceContext.setWorld (lWorld);
  }
  
  // //////////////////////////////////////////////////////////////////////
  void FORECAST_Service::finalise () {
    assert (_forecastServiceContext != NULL);

    // Terminate the SOCI Session
    soci::session* lSociSession_ptr = _forecastServiceContext->getSociSession();
    SociSessionManager::finalise (lSociSession_ptr);
  }

  // //////////////////////////////////////////////////////////////////////
  double FORECAST_Service::calculateForecast (const boost::python::list& iDemandList, const boost::python::list& iDCPList ) {

    if (_forecastServiceContext == NULL) {
      throw NonInitialisedServiceException();
    }
    assert (_forecastServiceContext != NULL);
    FORECAST_ServiceContext& lFORECAST_ServiceContext= *_forecastServiceContext;

    // Get the date-time for the present time
    boost::posix_time::ptime lNowDateTime =
      boost::posix_time::second_clock::local_time();
    boost::gregorian::date lNowDate = lNowDateTime.date();

    // DEBUG
    FORECAST_LOG_DEBUG (std::endl
                        << "==================================================="
                        << std::endl
                        << lNowDateTime);

    double oOutput;
        
    try {
      
      // Retrieve the SOCI Session
      soci::session& lSociSession =
        lFORECAST_ServiceContext.getSociSessionHandler();
      
      // Delegate the query execution to the dedicated command
      BasChronometer lForecastChronometer;
      lForecastChronometer.start();

      //
      
      Flood::InputTargetDataSet lDataSet;
      unsigned int lNbOfPoints = boost::python::extract<int>(iDCPList.attr("__len__")());
      
      lDataSet.setInputTargetDataSet(lNbOfPoints, 1, 1);     

      for( unsigned int i=0; i<lNbOfPoints; i++){
        Flood::Vector<double> lSample;
        lSample.setSize(2);
        lSample[0] = boost::python::extract<double>(iDCPList[i]);
  	lSample[1] = boost::python::extract<double>(iDemandList[i]);
        lDataSet.setSample(i, lSample);
      }

      // Get all information
      Flood::Vector< Flood::Vector<std::string> > lInformation = lDataSet.getAllInformation();

      // Calculate all statistics
      Flood::Vector< Flood::Vector<double> > lStatistics = lDataSet.calculateAllStatistics();
      Flood::Matrix<double> lMeanAndStandardDeviationOfInputData = lDataSet.calculateMeanAndStandardDeviationOfInputData();
      Flood::Matrix<double> lMeanAndStandardDeviationOfTargetData = lDataSet.calculateMeanAndStandardDeviationOfTargetData();


      // Preprocess training data for zero mean and unity standard deviation.
      lDataSet.preprocessMeanAndStandardDeviation();

      // Construct a multilayer perceptron object with 1 input, 3 sigmoid neurons
      //in the hidden layer and 1 linear output neuron.
      Flood::MultilayerPerceptron lMultiLayerPerceptron(1, 3, 1);

      lMultiLayerPerceptron.initFreeParametersNormal(0.0,1.0);

      // Construct a mean squared error object.
      Flood::MeanSquaredError lMeanSquaredError(&lMultiLayerPerceptron, &lDataSet);

      // Construct a quasi-Newton method object.
      Flood::QuasiNewtonMethod lQuasiNewtonMethod(&lMeanSquaredError);

      // Set the stopping criteria for the quasi-Newton method training algorithm
      lQuasiNewtonMethod.setEvaluationGoal(0.0);
      lQuasiNewtonMethod.setGradientNormGoal(0.0);
      lQuasiNewtonMethod.setMaximumTime(1000.0);
      lQuasiNewtonMethod.setMaximumNumberOfEpochs(1000);
      lQuasiNewtonMethod.setDisplay(false);

      // Train the neural network
      lQuasiNewtonMethod.train();

      // Set all information
      lMultiLayerPerceptron.setAllInformation(lInformation);

      // Set all statistics
      lMultiLayerPerceptron.setAllStatistics(lStatistics);

      // Save training history
      lDataSet.postprocessMeanAndStandardDeviation(lMeanAndStandardDeviationOfInputData, lMeanAndStandardDeviationOfTargetData);
      lMultiLayerPerceptron.setPreAndPostProcessingMethod(Flood::MultilayerPerceptron::MeanAndStandardDeviation);

      // Forecast the remaining demand for the Day Of Departure.
      Flood::Vector<double> lInput(1, 0.0);
      
      lInput[0] = 0;
      oOutput = lMultiLayerPerceptron.calculateOutput(lInput)[0];

      const double lForecastMeasure =
        lForecastChronometer.elapsed();

      // DEBUG
      FORECAST_LOG_DEBUG ("Match query on Xapian database (index): "
                          << lForecastMeasure);
      
    } catch (const std::exception& error) {
      FORECAST_LOG_ERROR ("Exception: "  << error.what());
      throw ForecastCalculationException();
    }
  
    return oOutput;
  }

}
