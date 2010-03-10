#ifndef __TRADEMGEN_BOM_DEMANDSTREAM_HPP
#define __TRADEMGEN_BOM_DEMANDSTREAM_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <iosfwd>
// Boost
#include <boost/shared_ptr.hpp>
// StdAir
#include <stdair/STDAIR_Types.hpp>
#include <stdair/bom/DemandStream.hpp>
#include <stdair/bom/BookingRequestTypes.hpp>

// Forward declarations.
namespace stdair {
  struct DemandCharacteristics;
  struct DemandDistribution;
  struct RandomGeneration;
  struct RandomGenerationContext;
}

namespace TRADEMGEN {
  
  /** Class modeling a demand stream. */
  class DemandStream  : public stdair::DemandStream {
    friend class TRADEMGEN_ServiceContext;
    friend class DemandManager;
    
  public:
    /** Definition allowing to retrieve the associated BOM structure type. */
    typedef stdair::DemandStreamStructure_T BomStructure_T;

  private:
    // /////////////// Business Methods //////////////
    /** Check whether enough requests have already been generated. */
    static const bool stillHavingRequestsToBeGenerated (const stdair::DemandStream&);
    const bool stillHavingRequestsToBeGenerated () const;

    /** Generate the time of the next request. */
    static const stdair::DateTime_T generateTimeOfRequest(stdair::DemandStream&);
    const stdair::DateTime_T generateTimeOfRequest ();

    /** Generate the POS. */
    static const stdair::AirportCode_T generatePOS (stdair::DemandStream&);
    const stdair::AirportCode_T generatePOS ();

    /** Generate the reservation channel. */
    static const stdair::ChannelLabel_T generateChannel (stdair::DemandStream&);
    const stdair::ChannelLabel_T generateChannel ();

    /** Generate the trip type. */
    static const stdair::TripType_T generateTripType (stdair::DemandStream&);
    const stdair::TripType_T generateTripType ();

    /** Generate the stay duration. */
    static const stdair::DayDuration_T generateStayDuration (stdair::DemandStream&);
    const stdair::DayDuration_T generateStayDuration ();

    /** Generate the frequent flyer type. */
    static const stdair::FrequentFlyer_T generateFrequentFlyer (stdair::DemandStream&);
    const stdair::FrequentFlyer_T generateFrequentFlyer ();

    /** Generate the preferred departure time. */
    static const stdair::Duration_T generatePreferredDepartureTime (stdair::DemandStream&);
    const stdair::Duration_T generatePreferredDepartureTime ();
    
    /** Generate the WTP. */
    static const stdair::WTP_T generateWTP (stdair::DemandStream&);
    const stdair::WTP_T generateWTP ();

    /** Generate the value of time. */
    static const stdair::PriceValue_T generateValueOfTime (stdair::DemandStream&);
    const stdair::PriceValue_T generateValueOfTime ();
    
    /** Generate the next request. */
    static stdair::BookingRequestPtr_T generateNextRequest (stdair::DemandStream&);
    stdair::BookingRequestPtr_T generateNextRequest ();


  private:
    // ////////// Constructors and destructors /////////
    /** Default constructors. */
    DemandStream ();
    DemandStream (const DemandStream&);
    DemandStream (const stdair::DemandStreamKey_T&,
                  const stdair::ArrivalPatternCumulativeDistribution_T&,
                  const stdair::POSProbabilityMassFunction_T&,
                  const stdair::ChannelProbabilityMassFunction_T&,
                  const stdair::TripTypeProbabilityMassFunction_T&,
                  const stdair::StayDurationProbabilityMassFunction_T&,
                  const stdair::FrequentFlyerProbabilityMassFunction_T&,
                  const stdair::PreferredDepartureTimeContinuousDistribution_T&,
                  const stdair::WTPContinuousDistribution_T&,
                  const stdair::ValueOfTimeContinuousDistribution_T&,
                  const stdair::DemandDistribution&,
                  const stdair::RandomSeed_T& iNumberOfRequestsSeed,
                  const stdair::RandomSeed_T& iRequestDateTimeSeed,
                  const stdair::RandomSeed_T& iDemandCharacteristicsSeed,
                  BomStructure_T&);
    /** Destructor. */
    ~DemandStream ();

  };

}
#endif // __TRADEMGEN_BOM_DEMANDSTREAM_HPP
