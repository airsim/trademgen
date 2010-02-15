// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <iosfwd>
#include <ostream>
#include <sstream>
#include <cmath>
// Boost
#include <boost/math/distributions/normal.hpp>
// StdAir
#include <stdair/basic/BasConst_General.hpp>
#include <stdair/basic/DemandCharacteristics.hpp>
#include <stdair/basic/DemandDistribution.hpp>
#include <stdair/basic/RandomGeneration.hpp>
#include <stdair/basic/RandomGenerationContext.hpp>
#include <stdair/bom/BookingRequestStruct.hpp>
#include <stdair/service/Logger.hpp>
// TraDemGen
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
    : stdair::DemandStream (iKey, iDemandCharacteristics,
                            iDemandDistribution, iNumberOfRequestsSeed,
                            iRequestDateTimeSeed, iDemandCharacteristicsSeed) {
  }

  // //////////////////////////////////////////////////////////////////////
  DemandStream::~DemandStream () {
  }

  // ////////////////////////////////////////////////////////////////////
  const bool DemandStream::
  stillHavingRequestsToBeGenerated (const stdair::DemandStream& iDemandStream) {
    // Check whether enough requests have already been generated
    const stdair::Count_T lNbOfRequestsGeneratedSoFar =
      iDemandStream.getNumberOfRequestsGeneratedSoFar();
    const stdair::Count_T lRemainingNumberOfRequestsToBeGenerated =
      iDemandStream.getTotalNumberOfRequestsToBeGenerated ()
      - lNbOfRequestsGeneratedSoFar;
    
    if (lRemainingNumberOfRequestsToBeGenerated <= 0) {
      return false;
    }

    return true;

  }

  // ////////////////////////////////////////////////////////////////////
  const bool DemandStream::stillHavingRequestsToBeGenerated () const {
    // Forward the business the the corresponding static function.
    return stillHavingRequestsToBeGenerated (*this);
  }
  
  // //////////////////////////////////////////////////////////////////////
  stdair::BookingRequestPtr_T DemandStream::
  generateNextRequest (stdair::DemandStream& ioDemandStream) {
    // Assert that there are requests to be generated.
    const stdair::Count_T lNbOfRequestsGeneratedSoFar =
      ioDemandStream.getNumberOfRequestsGeneratedSoFar ();
    const stdair::Count_T lRemainingNumberOfRequestsToBeGenerated =
      ioDemandStream.getTotalNumberOfRequestsToBeGenerated()
      - lNbOfRequestsGeneratedSoFar;
    assert (lRemainingNumberOfRequestsToBeGenerated > 0);

    
    // Origin
    const stdair::AirportCode_T& lOrigin = ioDemandStream.getOrigin ();
    // Destination
    const stdair::AirportCode_T& lDestination = ioDemandStream.getDestination ();
    // Preferred departure date
    const stdair::Date_T& lPreferredDepartureDate =
      ioDemandStream.getPreferredDepartureDate ();
    // Preferred cabin
    const stdair::CabinCode_T& lPreferredCabin =
      ioDemandStream.getPreferredCabin();
    
    // Request datetime, determined from departure date and arrival pattern
    // Sequential generation
    const stdair::Probability_T lCumulativeProbabilitySoFar =
      ioDemandStream.getCumulativeProbabilitySoFar ();

    const stdair::Probability_T lVariate =
      ioDemandStream.generateUniform01WithRequestDateTimeRandomGenerator ();

    const stdair::Probability_T lCumulativeProbabilityThisRequest =
      1.0 - (1.0 - lCumulativeProbabilitySoFar) * pow(1 - lVariate, 1.0 / static_cast<float> (lRemainingNumberOfRequestsToBeGenerated));

    const stdair::FloatDuration_T lNumberOfDaysBetweenDepartureAndThisRequest =
      ioDemandStream.getArrivalPattern().getValue (lCumulativeProbabilityThisRequest);

    // convert the number of days in number of seconds + number of milliseconds
    const stdair::FloatDuration_T lNumberOfSeconds =
      lNumberOfDaysBetweenDepartureAndThisRequest
      * static_cast<float> (stdair::SECONDS_IN_ONE_DAY);

    const stdair::IntDuration_T lIntNumberOfSeconds = floor (lNumberOfSeconds);

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
    ioDemandStream.setCumulativeProbabilitySoFar (lCumulativeProbabilityThisRequest);
    ioDemandStream.incrementGeneratedRequestsCounter ();

    // Create the booking request with a hardcoded party size.
    stdair::BookingRequestPtr_T oBookingRequest_ptr = stdair::BookingRequestPtr_T
      (new stdair::BookingRequestStruct (lOrigin, lDestination,
                                         lPreferredDepartureDate,
                                         lDateTimeThisRequest,
                                         lPreferredCabin, 1));
    return oBookingRequest_ptr;
  }

  // //////////////////////////////////////////////////////////////////////
  stdair::BookingRequestPtr_T DemandStream::generateNextRequest () {
    // Forward the business the the corresponding static function.
    return generateNextRequest (*this);
  }

}
