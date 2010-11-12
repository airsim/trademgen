#ifndef __TRADEMGEN_BAS_DEMANDCHARACTERISTICTYPES_HPP
#define __TRADEMGEN_BAS_DEMANDCHARACTERISTICTYPES_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STDAIR
#include <stdair/STDAIR_Types.hpp>
// TRADEMGEN
#include <trademgen/basic/ContinuousAttributeLite.hpp>
#include <trademgen/basic/CategoricalAttributeLite.hpp>

namespace TRADEMGEN {

  /** Type definition for the continuous distribition of the duration
      (as a float number). */
  typedef ContinuousAttributeLite<stdair::FloatDuration_T> CumulativeFloatDuration_T;
   
  /** Type definition for the arrival pattern cumulative distribution. */
  typedef CumulativeFloatDuration_T::ContinuousDistribution_T ArrivalPatternCumulativeDistribution_T;

  /** Define the point-of-sale probablity mass. */
  typedef CategoricalAttributeLite<stdair::AirportCode_T> POSProbabilityMass_T;

  /** Define the probability mass function type of point-of-sale. */
  typedef POSProbabilityMass_T::ProbabilityMassFunction_T POSProbabilityMassFunction_T;
  
  /** Define the booking channel probablity mass. */
  typedef CategoricalAttributeLite<stdair::ChannelLabel_T> ChannelProbabilityMass_T;

  /** Define the probability mass function type of booking channel. */
  typedef ChannelProbabilityMass_T::ProbabilityMassFunction_T ChannelProbabilityMassFunction_T;

  /** Define the trip type probablity mass. */
  typedef CategoricalAttributeLite<stdair::TripType_T> TripTypeProbabilityMass_T;

  /** Define the probability mass function type of trip type. */
  typedef TripTypeProbabilityMass_T::ProbabilityMassFunction_T TripTypeProbabilityMassFunction_T;
  
  /** Define the stay duration probablity mass. */
  typedef CategoricalAttributeLite<stdair::DayDuration_T> StayDurationProbabilityMass_T;

  /** Define the probability mass function type of stay duration. */
  typedef StayDurationProbabilityMass_T::ProbabilityMassFunction_T StayDurationProbabilityMassFunction_T;

  /** Define the frequent flyer probablity mass. */
  typedef CategoricalAttributeLite<stdair::FrequentFlyer_T> FrequentFlyerProbabilityMass_T;

  /** Define the probability mass function type of frequent flyer. */
  typedef FrequentFlyerProbabilityMass_T::ProbabilityMassFunction_T FrequentFlyerProbabilityMassFunction_T;

  /** Define the preferred departure time cumulative distribution. */
  typedef ContinuousAttributeLite<stdair::IntDuration_T> PreferredDepartureTimeCumulativeDistribution_T;
  typedef PreferredDepartureTimeCumulativeDistribution_T::ContinuousDistribution_T PreferredDepartureTimeContinuousDistribution_T;

  /** Define the value of time cumulative distribution. */
  typedef ContinuousAttributeLite<stdair::PriceValue_T> ValueOfTimeCumulativeDistribution_T;
  typedef ValueOfTimeCumulativeDistribution_T::ContinuousDistribution_T ValueOfTimeContinuousDistribution_T;

  /** Define the continuous distribution for real number. */
  typedef ContinuousAttributeLite<stdair::RealNumber_T> CumulativeDistribution_T;
  typedef CumulativeDistribution_T::ContinuousDistribution_T ContinuousDistribution_T;
}
#endif // __TRADEMGEN_BAS_DEMANDCHARACTERISTICTYPES_HPP
