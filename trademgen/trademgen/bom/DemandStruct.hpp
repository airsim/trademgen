#ifndef __TRADEMGEN_BOM_DEMANDSTRUCT_HPP
#define __TRADEMGEN_BOM_DEMANDSTRUCT_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <string>
// STDAIR
#include <stdair/STDAIR_Types.hpp>
#include <stdair/basic/PassengerType.hpp>
#include <stdair/bom/StructAbstract.hpp>
// TraDemGen
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
  struct DemandStruct_T : public stdair::StructAbstract {
    
    /** Set the date from the staging details. */
    stdair::Date_T getDate() const;

    /** Set the time from the staging details. */
    stdair::Duration_T getTime() const;
  
    /** Give a description of the structure (for display purposes). */
    const std::string describe() const;

    /** Constructor. */
    DemandStruct_T ();

    // Attributes
    stdair::Date_T _prefDepDate;
    stdair::Date_T _prefArrDate;
    stdair::AirportCode_T _origin;
    stdair::AirportCode_T _destination;
    stdair::PassengerType _paxType;
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
    stdair::Duration_T _itTimeValue;

    /** Staging DTD (Days-To-Departure). */
    stdair::DayDuration_T _itDTD;
  };

}
#endif // __TRADEMGEN_BOM_DEMANDSTRUCT_HPP
