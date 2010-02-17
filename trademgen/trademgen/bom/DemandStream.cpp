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
#include <stdair/basic/BasConst_Request.hpp>
#include <stdair/basic/DemandCharacteristics.hpp>
#include <stdair/basic/DemandDistribution.hpp>
#include <stdair/basic/RandomGeneration.hpp>
#include <stdair/basic/RandomGenerationContext.hpp>
#include <stdair/bom/BookingRequestStruct.hpp>
#include <stdair/service/Logger.hpp>
// TraDemGen
#include <trademgen/bom/DemandStream.hpp>

namespace TRADEMGEN {

  // ////////////////////////////////////////////////////////////////////
  DemandStream::
  DemandStream (const stdair::DemandStreamKey_T& iKey,
                const stdair::DemandCharacteristics& iDemandCharacteristics,
                const stdair::DemandDistribution& iDemandDistribution,
                const stdair::RandomSeed_T& iNumberOfRequestsSeed,
                const stdair::RandomSeed_T& iRequestDateTimeSeed,
                const stdair::RandomSeed_T& iDemandCharacteristicsSeed,
                BomStructure_T& ioBomStructure)
    : stdair::DemandStream (iKey, iDemandCharacteristics,
                            iDemandDistribution, iNumberOfRequestsSeed,
                            iRequestDateTimeSeed, iDemandCharacteristicsSeed,
                            ioBomStructure) {
  }

  // ////////////////////////////////////////////////////////////////////
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

  // ////////////////////////////////////////////////////////////////////
  const stdair::DateTime_T DemandStream::
  generateTimeOfRequest (stdair::DemandStream& ioDemandStream) {
    // Assert that there are requests to be generated.
    const stdair::Count_T lNbOfRequestsGeneratedSoFar =
      ioDemandStream.getNumberOfRequestsGeneratedSoFar ();
    const stdair::Count_T lRemainingNumberOfRequestsToBeGenerated =
      ioDemandStream.getTotalNumberOfRequestsToBeGenerated()
      - lNbOfRequestsGeneratedSoFar;
    assert (lRemainingNumberOfRequestsToBeGenerated > 0);

    // Request datetime, determined from departure date and arrival pattern
    // Sequential generation
    const stdair::Probability_T lCumulativeProbabilitySoFar =
      ioDemandStream.getCumulativeProbabilitySoFar ();

    const stdair::Probability_T lVariate =
      ioDemandStream.generateUniform01WithRequestDateTimeRandomGenerator ();

    const stdair::Probability_T lCumulativeProbabilityThisRequest =
      1.0 - (1.0 - lCumulativeProbabilitySoFar)
      * pow(1 - lVariate, 1.0 / static_cast<float> (lRemainingNumberOfRequestsToBeGenerated));

    const stdair::FloatDuration_T lNumberOfDaysBetweenDepartureAndThisRequest =
      ioDemandStream.
      getArrivalPattern().getValue (lCumulativeProbabilityThisRequest);

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
    
    // Preferred departure date
    const stdair::Date_T& lPreferredDepartureDate =
      ioDemandStream.getPreferredDepartureDate ();
    
    const stdair::DateTime_T lDepartureDateTime =
      boost::posix_time::ptime (lPreferredDepartureDate,
                                lHardcodedReferenceDepartureTime);

    const stdair::DateTime_T oDateTimeThisRequest =
      lDepartureDateTime + lDifferenceBetweenDepartureAndThisRequest;
    
    // Update random generation context
    ioDemandStream.
      setCumulativeProbabilitySoFar (lCumulativeProbabilityThisRequest);
    ioDemandStream.incrementGeneratedRequestsCounter ();
    return oDateTimeThisRequest;
  }

  // ////////////////////////////////////////////////////////////////////
  const stdair::DateTime_T DemandStream::generateTimeOfRequest () {
    // Forward the business the the corresponding static function.
    return generateTimeOfRequest (*this);
  }

  // ////////////////////////////////////////////////////////////////////
  const stdair::AirportCode_T DemandStream::
  generatePOS (stdair::DemandStream& ioDemandStream) {
    // Generate a random number between 0 and 1.
    const stdair::Probability_T lVariate =
      ioDemandStream.generateUniform01WithDemandCharacteristicsRandomGenerator();
    
    const stdair::POSProbabilityMass_T& lPOSProbMass =
      ioDemandStream.getPOSProbabilityMass ();

    return lPOSProbMass.getValue (lVariate);
  }

  // ////////////////////////////////////////////////////////////////////
  const stdair::AirportCode_T DemandStream::generatePOS () {
    // Forward the business the the corresponding static function.
    return generatePOS (*this);        
  }

  // ////////////////////////////////////////////////////////////////////
  const stdair::ChannelLabel_T DemandStream::
  generateChannel (stdair::DemandStream& ioDemandStream) {
    // Generate a random number between 0 and 1.
    const stdair::Probability_T lVariate =
      ioDemandStream.generateUniform01WithDemandCharacteristicsRandomGenerator();
    
    const stdair::ChannelProbabilityMass_T& lChannelProbMass =
      ioDemandStream.getChannelProbabilityMass ();

    return lChannelProbMass.getValue (lVariate);
  }

  // ////////////////////////////////////////////////////////////////////
  const stdair::ChannelLabel_T DemandStream::generateChannel () {
    // Forward the business the the corresponding static function.
    return generateChannel (*this);        
  }

  // ////////////////////////////////////////////////////////////////////
  const stdair::TripType_T DemandStream::
  generateTripType (stdair::DemandStream& ioDemandStream) {
    // Generate a random number between 0 and 1.
    const stdair::Probability_T lVariate =
      ioDemandStream.generateUniform01WithDemandCharacteristicsRandomGenerator();
    
    const stdair::TripTypeProbabilityMass_T& lTripTypeProbMass =
      ioDemandStream.getTripTypeProbabilityMass ();

    return lTripTypeProbMass.getValue (lVariate);
  }

  // ////////////////////////////////////////////////////////////////////
  const stdair::TripType_T DemandStream::generateTripType () {
    // Forward the business the the corresponding static function.
    return generateTripType (*this);        
  }

  // ////////////////////////////////////////////////////////////////////
  const stdair::DayDuration_T DemandStream::
  generateStayDuration (stdair::DemandStream& ioDemandStream) {
    // Generate a random number between 0 and 1.
    const stdair::Probability_T lVariate =
      ioDemandStream.generateUniform01WithDemandCharacteristicsRandomGenerator();
    
    const stdair::StayDurationProbabilityMass_T& lStayDurationProbMass =
      ioDemandStream.getStayDurationProbabilityMass ();

    return lStayDurationProbMass.getValue (lVariate);
  }

  // ////////////////////////////////////////////////////////////////////
  const stdair::DayDuration_T DemandStream::generateStayDuration () {
    // Forward the business the the corresponding static function.
    return generateStayDuration (*this);        
  }
  
  // ////////////////////////////////////////////////////////////////////
  const stdair::FrequentFlyer_T DemandStream::
  generateFrequentFlyer (stdair::DemandStream& ioDemandStream) {
    // Generate a random number between 0 and 1.
    const stdair::Probability_T lVariate =
      ioDemandStream.generateUniform01WithDemandCharacteristicsRandomGenerator();
    
    const stdair::FrequentFlyerProbabilityMass_T& lFrequentFlyerProbMass =
      ioDemandStream.getFrequentFlyerProbabilityMass ();

    return lFrequentFlyerProbMass.getValue (lVariate);
  }

  // ////////////////////////////////////////////////////////////////////
  const stdair::FrequentFlyer_T DemandStream::generateFrequentFlyer () {
    // Forward the business the the corresponding static function.
    return generateFrequentFlyer (*this);        
  }

  // ////////////////////////////////////////////////////////////////////
  const stdair::Duration_T DemandStream::
  generatePreferredDepartureTime (stdair::DemandStream& ioDemandStream) {
    // Generate a random number between 0 and 1.
    const stdair::Probability_T lVariate =
      ioDemandStream.generateUniform01WithDemandCharacteristicsRandomGenerator();
    
    const stdair::PreferredDepartureTimeCumulativeDistribution_T& lPreferredDepartureTimeCumulativeDistribution =
      ioDemandStream.getPreferredDepartureTimeCumulativeDistribution ();
    
    const stdair::IntDuration_T lNbOfSeconds =
      lPreferredDepartureTimeCumulativeDistribution.getValue (lVariate);

    const stdair::Duration_T oTime = boost::posix_time::seconds (lNbOfSeconds);

    return oTime;
  }

  // ////////////////////////////////////////////////////////////////////
  const stdair::Duration_T DemandStream::generatePreferredDepartureTime () {
    // Forward the business the the corresponding static function.
    return generatePreferredDepartureTime (*this);        
  }

  // ////////////////////////////////////////////////////////////////////
  const stdair::WTP_T DemandStream::
  generateWTP (stdair::DemandStream& ioDemandStream) {
    // Generate a random number between 0 and 1.
    const stdair::Probability_T lVariate =
      ioDemandStream.generateUniform01WithDemandCharacteristicsRandomGenerator();
    
    const stdair::WTPCumulativeDistribution_T& lWTPCumulativeDistribution =
      ioDemandStream.getWTPCumulativeDistribution ();

    return lWTPCumulativeDistribution.getValue (lVariate);
  }

  // ////////////////////////////////////////////////////////////////////
  const stdair::WTP_T DemandStream::generateWTP () {
    // Forward the business the the corresponding static function.
    return generateWTP (*this);        
  }

  // ////////////////////////////////////////////////////////////////////
  const stdair::PriceValue_T DemandStream::
  generateValueOfTime (stdair::DemandStream& ioDemandStream) {
    // Generate a random number between 0 and 1.
    const stdair::Probability_T lVariate =
      ioDemandStream.generateUniform01WithDemandCharacteristicsRandomGenerator();
    
    const stdair::ValueOfTimeCumulativeDistribution_T& lValueOfTimeCumulativeDistribution =
      ioDemandStream.getValueOfTimeCumulativeDistribution ();

    return lValueOfTimeCumulativeDistribution.getValue (lVariate);
  }

  // ////////////////////////////////////////////////////////////////////
  const stdair::PriceValue_T DemandStream::generateValueOfTime () {
    // Forward the business the the corresponding static function.
    return generateValueOfTime (*this);        
  }
  
  // ////////////////////////////////////////////////////////////////////
  stdair::BookingRequestPtr_T DemandStream::
  generateNextRequest (stdair::DemandStream& ioDemandStream) {
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
    // POS
    const stdair::AirportCode_T lPOS = generatePOS (ioDemandStream);
    // Time of request.    
    const stdair::DateTime_T lDateTimeThisRequest =
      generateTimeOfRequest (ioDemandStream);
    // Booking channel.
    const stdair::ChannelLabel_T lChannelLabel = generateChannel(ioDemandStream);
    // Trip type.
    const stdair::TripType_T lTripType = generateTripType (ioDemandStream);
    // Stay duration.
    const stdair::DayDuration_T lStayDuration =
      generateStayDuration (ioDemandStream);
    // Frequet flyer type.
    const stdair::FrequentFlyer_T lFrequentFlyer =
      generateFrequentFlyer (ioDemandStream);
    // Preferred departure time.
    const stdair::Duration_T lPreferredDepartureTime =
      generatePreferredDepartureTime (ioDemandStream);
    // WTP
    const stdair::WTP_T lWTP = generateWTP (ioDemandStream);
    // Value of time
    const stdair::PriceValue_T lValueOfTime =
      generateValueOfTime (ioDemandStream);
    
    // Create the booking request with a hardcoded party size.
    stdair::BookingRequestPtr_T oBookingRequest_ptr = stdair::BookingRequestPtr_T
      (new stdair::BookingRequestStruct (lOrigin, lDestination, lPOS,
                                         lPreferredDepartureDate,
                                         lDateTimeThisRequest,
                                         lPreferredCabin,
                                         stdair::DEFAULT_PARTY_SIZE,
                                         lChannelLabel, lTripType, lStayDuration,
                                         lFrequentFlyer, lPreferredDepartureTime,
                                         lWTP, lValueOfTime));
    // DEBUG
    STDAIR_LOG_DEBUG ("\n[BKG] " << oBookingRequest_ptr->describe());
    
    return oBookingRequest_ptr;
  }

  // ////////////////////////////////////////////////////////////////////
  stdair::BookingRequestPtr_T DemandStream::generateNextRequest () {
    // Forward the business the the corresponding static function.
    return generateNextRequest (*this);
  }

}
