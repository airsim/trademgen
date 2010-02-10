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
// STDAIR
#include <stdair/basic/BasConst_General.hpp>
#include <stdair/basic/DemandCharacteristics.hpp>
#include <stdair/basic/RandomGeneration.hpp>
#include <stdair/basic/RandomGenerationContext.hpp>
#include <stdair/bom/BookingRequestStruct.hpp>
#include <stdair/service/Logger.hpp>
// TRADEMGEN
#include <trademgen/bom/DemandStream.hpp>


namespace TRADEMGEN {

  // //////////////////////////////////////////////////////////////////////
  DemandStream::
  DemandStream (const stdair::DemandStreamKey_T& iKey,
                const stdair::DemandCharacteristics& iDemandCharacteristics,
                const stdair::DemandDistribution& iDemandDistribution,
                const stdair::RandomSeed_T& iNumberOfRequestsSeed,
                const stdair::RandomSeed_T& iRequestDateTimeSeed,
                const stdair::RandomSeed_T& iDemandCharacteristicsSeed)
    : _key (iKey),
      _demandCharacteristics (iDemandCharacteristics),
      _demandDistribution (iDemandDistribution),
      _totalNumberOfRequestsToBeGenerated (0),
      _numberOfRequestsRandomGenerator (iNumberOfRequestsSeed),
      _requestDateTimeRandomGenerator (iRequestDateTimeSeed),
      _demandCharacteristicsRandomGenerator (iDemandCharacteristicsSeed) {
    init();
  }

  // //////////////////////////////////////////////////////////////////////
  DemandStream::DemandStream (const DemandStream& iDemandStream)
    : _key (iDemandStream._key),
      _demandCharacteristics (iDemandStream._demandCharacteristics),
      _demandDistribution (iDemandStream._demandDistribution),
      _totalNumberOfRequestsToBeGenerated (iDemandStream._totalNumberOfRequestsToBeGenerated),
      _randomGenerationContext (iDemandStream._randomGenerationContext),
      _numberOfRequestsRandomGenerator (iDemandStream._numberOfRequestsRandomGenerator),
      _requestDateTimeRandomGenerator (iDemandStream._requestDateTimeRandomGenerator),
      _demandCharacteristicsRandomGenerator (iDemandStream._demandCharacteristicsRandomGenerator) {
  } 

  // //////////////////////////////////////////////////////////////////////
  DemandStream::~DemandStream () {
  }

  // //////////////////////////////////////////////////////////////////////
  void DemandStream::init() {
    // Generate the number of requests
    const stdair::RealNumber_T lMu =
      _demandDistribution.getMeanNumberOfRequests ();
    const stdair::RealNumber_T lSigma =
      _demandDistribution.getStandardDeviationNumberOfRequests ();
    const stdair::RealNumber_T lRealNumberOfRequestsToBeGenerated =
      _numberOfRequestsRandomGenerator.generateNormal (lMu, lSigma);
    stdair::Count_T lIntegerNumberOfRequestsToBeGenerated = 0;
    if (lRealNumberOfRequestsToBeGenerated < 0.5) {
    	lIntegerNumberOfRequestsToBeGenerated = 0;
    } else {
    	lIntegerNumberOfRequestsToBeGenerated =
          static_cast<stdair::Count_T> (lRealNumberOfRequestsToBeGenerated + 0.5);
    }
    _totalNumberOfRequestsToBeGenerated = lIntegerNumberOfRequestsToBeGenerated;
  }

  // ////////////////////////////////////////////////////////////////////
  const bool DemandStream::stillHavingRequestsToBeGenerated () const {
    // Check whether enough requests have already been generated
    const stdair::Count_T lNbOfRequestsGeneratedSoFar =
      _randomGenerationContext.getNumberOfRequestsGeneratedSoFar ();
    const stdair::Count_T lRemainingNumberOfRequestsToBeGenerated =
      _totalNumberOfRequestsToBeGenerated - lNbOfRequestsGeneratedSoFar;

    if (lRemainingNumberOfRequestsToBeGenerated <= 0) {
      return false;
    }

    return true;

  }
  
  // //////////////////////////////////////////////////////////////////////
  stdair::BookingRequestPtr_T DemandStream::generateNext () {
    // Assert that there are requests to be generated.
    const stdair::Count_T lNbOfRequestsGeneratedSoFar =
      _randomGenerationContext.getNumberOfRequestsGeneratedSoFar ();
    const stdair::Count_T lRemainingNumberOfRequestsToBeGenerated =
      _totalNumberOfRequestsToBeGenerated - lNbOfRequestsGeneratedSoFar;
    assert (lRemainingNumberOfRequestsToBeGenerated > 0);

    
    // Origin
    const stdair::AirportCode_T& lOrigin = _demandCharacteristics.getOrigin ();
    // Destination
    const stdair::AirportCode_T& lDestination =
      _demandCharacteristics.getDestination ();
    // Preferred departure date
    const stdair::Date_T& lPreferredDepartureDate =
      _demandCharacteristics.getPreferredDepartureDate ();
    // Passenger type.
    const stdair::PassengerType_T& lPassengerType = 
      _demandCharacteristics.getPaxType();
    
    // Request datetime, determined from departure date and arrival pattern
    // Sequential generation
    const stdair::Probability_T lCumulativeProbabilitySoFar =
      _randomGenerationContext.getCumulativeProbabilitySoFar ();

    const stdair::Probability_T lVariate =
      _requestDateTimeRandomGenerator.generateUniform01 ();

    const stdair::Probability_T lCumulativeProbabilityThisRequest =
      1.0 - (1.0 - lCumulativeProbabilitySoFar) * pow(1 - lVariate, 1.0 / static_cast<float> (lRemainingNumberOfRequestsToBeGenerated));

    const stdair::FloatDuration_T lNumberOfDaysBetweenDepartureAndThisRequest =
      _demandCharacteristics.getArrivalPattern().getValue (lCumulativeProbabilityThisRequest);

    // convert the number of days in number of seconds + number of milliseconds
    const stdair::FloatDuration_T lNumberOfSeconds =
      lNumberOfDaysBetweenDepartureAndThisRequest
      * static_cast<float> (stdair::SECONDS_IN_ONE_DAY);

    const stdair::IntDuration_T lIntNumberOfSeconds = floor(lNumberOfSeconds);

    const stdair::FloatDuration_T lNumberOfMilliseconds =
      (lNumberOfSeconds - lIntNumberOfSeconds)
      * static_cast<float> (stdair::MILLISECONDS_IN_ONE_SECOND);

    const stdair::IntDuration_T lIntNumberOfMilliseconds =
      floor(lNumberOfMilliseconds) + 1; // +1 is a trick to ensure that the next
    // event is strictly later than the current one

    const stdair::Duration_T lDifferenceBetweenDepartureAndThisRequest =
      boost::posix_time::seconds(lIntNumberOfSeconds)
      + boost::posix_time::millisec(lIntNumberOfMilliseconds);

    const stdair::Time_T lHardcodedReferenceDepartureTime =
      boost::posix_time::hours(8);
    
    const stdair::DateTime_T lDepartureDateTime =
      boost::posix_time::ptime (lPreferredDepartureDate,
                                lHardcodedReferenceDepartureTime);

    const stdair::DateTime_T lDateTimeThisRequest =
      lDepartureDateTime + lDifferenceBetweenDepartureAndThisRequest;
    
    // Update random generation context
    _randomGenerationContext.setCumulativeProbabilitySoFar (lCumulativeProbabilityThisRequest);
    _randomGenerationContext.incrementGeneratedRequestsCounter ();


    // Create the booking request with a hardcoded party size.
    stdair::BookingRequestPtr_T oBookingRequest_ptr = stdair::BookingRequestPtr_T
      (new stdair::BookingRequestStruct (lOrigin, lDestination,
                                         lPreferredDepartureDate,
                                         lDateTimeThisRequest,
                                         lPassengerType, 1));
    assert (oBookingRequest_ptr != NULL);
    return oBookingRequest_ptr;
  }

}
