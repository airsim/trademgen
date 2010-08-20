#ifndef __TRADEMGEN_BOM_DEMANDSTRUCT_HPP
#define __TRADEMGEN_BOM_DEMANDSTRUCT_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <string>
// StdAir
#include <stdair/STDAIR_Types.hpp>
#include <stdair/basic/StructAbstract.hpp>
// TraDemGen
#include <trademgen/basic/DemandCharacteristicTypes.hpp>
#include <trademgen/basic/StayDurationTypes.hpp>
#include <trademgen/basic/PosCodeTypes.hpp>
#include <trademgen/basic/ChannelCodeTypes.hpp>
#include <trademgen/basic/TripCodeTypes.hpp>
#include <trademgen/basic/FFCodeTypes.hpp>
#include <trademgen/basic/PrefDepTimeTypes.hpp>
#include <trademgen/basic/WTPTypes.hpp>
#include <trademgen/basic/TimeValueTypes.hpp>
#include <trademgen/basic/DTDTypes.hpp>

namespace TRADEMGEN {

  /** Utility Structure for the parsing of Demand structures. */
  struct DemandStruct : public stdair::StructAbstract {
    
    /** Get the date from the staging details. */
    stdair::Date_T getDate() const;

    /** Get the time from the staging details. */
    stdair::Duration_T getTime() const;

    /** Build the arrival pattern. */
    void buildArrivalPattern (ArrivalPatternCumulativeDistribution_T&) const;
    
    /** Build the POS probabilty mass. */
    void buildPOSProbabilityMass (POSProbabilityMassFunction_T&) const;
    
    /** Build the channel probabilty mass. */
    void buildChannelProbabilityMass (ChannelProbabilityMassFunction_T&) const;

    /** Build the trip type probabilty mass. */
    void buildTripTypeProbabilityMass (TripTypeProbabilityMassFunction_T&) const;
    
    /** Build the stay duration probabilty mass. */
    void buildStayDurationProbabilityMass (StayDurationProbabilityMassFunction_T&) const;
    
    /** Build the frequent flyer probabilty mass. */
    void buildFrequentFlyerProbabilityMass (FrequentFlyerProbabilityMassFunction_T&) const;
    
    /** Build the preferred departure time cumulative distribition. */
    void buildPreferredDepartureTimeContinuousDistribution (PreferredDepartureTimeContinuousDistribution_T&) const;
    
    /** Build the WTP cumulative distribition. */
    void buildWTPContinuousDistribution (WTPContinuousDistribution_T&) const;

    /** Build the value of time cumulative distribition. */
    void buildValueOfTimeContinuousDistribution (ValueOfTimeContinuousDistribution_T&) const;
    
    /** Give a description of the structure (for display purposes). */
    const std::string describe() const;

    /** Constructor. */
    DemandStruct ();

    // Attributes
    stdair::Date_T _prefDepDate;
    stdair::Date_T _prefArrDate;
    stdair::AirportCode_T _origin;
    stdair::AirportCode_T _destination;
    stdair::CabinCode_T _prefCabin;
    float _demandMean;
    float _demandStdDev;
    PosProbDist_T _posProbDist;
    ChannelProbDist_T _channelProbDist;
    TripProbDist_T _tripProbDist;
    StayProbDist_T _stayProbDist;
    FFProbDist_T _ffProbDist;
    PrefDepTimeProbDist_T _prefDepTimeProbDist; 
    WTPProbDist_T _wtpProbDist;
    TimeValueProbDist_T _timeValueProbDist;
    DTDProbDist_T _dtdProbDist;
    
    /** Staging Date. */
    unsigned int _itYear;
    unsigned int _itMonth;
    unsigned int _itDay;
      
    /** Staging Time. */
    long _itHours;
    long _itMinutes;
    long _itSeconds;

    /** Staging Point-Of-Sale (POS) code. */
    stdair::AirportCode_T _itPosCode;

    /** Staging channel type code. */
    ChannelCode::EN_ChannelCode _itChannelCode;

    /** Staging trip type code. */
    TripCode::EN_TripCode _itTripCode;

    /** Staging stay duration. */
    stdair::DayDuration_T _itStayDuration;
    
    /** Staging Frequent Flyer code. */
    FFCode::EN_FFCode _itFFCode;

    /** Staging preferred departure time. */
    stdair::Duration_T _itPrefDepTime;

    /** Staging Willingness-To-Pay (WTP). */
    stdair::WTP_T _itWTP;
    
    /** Staging time value. */
    stdair::PriceValue_T _itTimeValue;

    /** Staging DTD (Days-To-Departure). */
    stdair::DayDuration_T _itDTD;
  };

}
#endif // __TRADEMGEN_BOM_DEMANDSTRUCT_HPP
