// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <iosfwd>
#include <ostream>
#include <sstream>
// Boost
#include <boost/math/distributions/normal.hpp>
// Math
#include <math.h>
// TraDemGen
#include <BasTypes.hpp>
#include <BasConst.hpp>
#include <DemandStream.hpp>
#include <DemandCharacteristics.hpp>
#include <RandomGeneration.hpp>
#include <RandomGenerationContext.hpp>
#include <Request.hpp>


namespace TRADEMGEN {

  // //////////////////////////////////////////////////////////////////////
  DemandStream::DemandStream (const Key_T& iKey,
                              const DemandCharacteristics& iDemandCharacteristics,
                              const RandomSeed_T& iNumberOfRequestsSeed,
                              const RandomSeed_T& iRequestDatetimeSeed,
                              const RandomSeed_T& iDemandCharacteristicsSeed)
    : _key (iKey),
      _demandCharacteristics (iDemandCharacteristics),
      _totalNumberOfRequestsToBeGenerated (0),
      _numberOfRequestsRandomGenerator (iNumberOfRequestsSeed),
      _requestDatetimeRandomGenerator (iRequestDatetimeSeed),
      _demandCharacteristicsRandomGenerator (iDemandCharacteristicsSeed) {
    init();
  }

  // //////////////////////////////////////////////////////////////////////
  DemandStream::~DemandStream () {
  }

  // //////////////////////////////////////////////////////////////////////
  void DemandStream::init() {
    // Generate the number of requests
    const RealNumber_T lMu = _demandCharacteristics.getMeanNumberOfRequests ();
    const RealNumber_T lSigma = _demandCharacteristics.getStandardDeviationNumberOfRequests ();
    const RealNumber_T lRealNumberOfRequestsToBeGenerated = _numberOfRequestsRandomGenerator.generateNormal (lMu, lSigma);
    Count_T lIntegerNumberOfRequestsToBeGenerated = 0;
    if (lRealNumberOfRequestsToBeGenerated < 0.5) {
    	lIntegerNumberOfRequestsToBeGenerated = 0;
    } else {
    	lIntegerNumberOfRequestsToBeGenerated = static_cast<Count_T> (lRealNumberOfRequestsToBeGenerated + 0.5);
    }
    _totalNumberOfRequestsToBeGenerated = lIntegerNumberOfRequestsToBeGenerated;
  }
  
  // //////////////////////////////////////////////////////////////////////
  bool DemandStream::generateNext(Request& ioRequest) {
    // Check whether enough requests have already been generated
    const Count_T lRemainingNumberOfRequestsToBeGenerated = _totalNumberOfRequestsToBeGenerated - _randomGenerationContext.getNumberOfRequestsGeneratedSoFar ();
    if (lRemainingNumberOfRequestsToBeGenerated <= 0) {
      return false;
    }
    
    // Key
    ioRequest.setDemandStreamKey (_key);
    
    // Origin
    ioRequest.setOrigin (_demandCharacteristics.getOrigin ());
    
    // Preferred departure date
    const Date_T lPreferredDepartureDate = _demandCharacteristics.getPreferredDepartureDate ();
    ioRequest.setPreferredDepartureDate (lPreferredDepartureDate);
    
    // Request datetime, determined from departure date and arrival pattern
    // Sequential generation
    const Probability_T lCumulativeProbabilitySoFar = _randomGenerationContext.getCumulativeProbabilitySoFar ();
    const Probability_T lVariate = _requestDatetimeRandomGenerator.generateUniform01 ();
    const Probability_T lCumulativeProbabilityThisRequest = 1.0 - (1.0 - lCumulativeProbabilitySoFar) * pow(1 - lVariate, 1.0 / static_cast<float> (lRemainingNumberOfRequestsToBeGenerated));
    const FloatDuration_T lNumberOfDaysBetweenDepartureAndThisRequest = _demandCharacteristics.getArrivalPattern().getValue (lCumulativeProbabilityThisRequest);
    // convert the number of days in number of seconds + number of milliseconds
    const FloatDuration_T lNumberOfSeconds = lNumberOfDaysBetweenDepartureAndThisRequest*static_cast<float> (SECONDS_IN_ONE_DAY);
    const IntDuration_T lIntNumberOfSeconds = floor(lNumberOfSeconds);
    const FloatDuration_T lNumberOfMilliseconds = (lNumberOfSeconds - lIntNumberOfSeconds) * static_cast<float> (MILLISECONDS_IN_ONE_SECOND);
    const IntDuration_T lIntNumberOfMilliseconds = floor(lNumberOfMilliseconds) + 1; // +1 is a trick to ensure that the next event is strictly later than the current one
    const TimeDuration_T lDifferenceBetweenDepartureAndThisRequest = boost::posix_time::seconds(lIntNumberOfSeconds) + boost::posix_time::millisec(lIntNumberOfMilliseconds);
    
    const Time_T lHardcodedReferenceDepartureTime = boost::posix_time::hours(8);
    const Datetime_T lDepartureDatetime = boost::posix_time::ptime(lPreferredDepartureDate, lHardcodedReferenceDepartureTime);
    
    const Datetime_T lDatetimeThisRequest = lDepartureDatetime + lDifferenceBetweenDepartureAndThisRequest;
    ioRequest.setRequestDatetime (lDatetimeThisRequest);
    
    // Update random generation context
    _randomGenerationContext.setCumulativeProbabilitySoFar (lCumulativeProbabilityThisRequest);
    _randomGenerationContext.incrementGeneratedRequestsCounter ();
   
    return true;
    
  }

}
