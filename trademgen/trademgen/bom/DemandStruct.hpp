#ifndef __TRADEMGEN_BOM_FLIGHTPERIODSTRUCT_HPP
#define __TRADEMGEN_BOM_FLIGHTPERIODSTRUCT_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <string>
// STDAIR
#include <stdair/STDAIR_Types.hpp>
#include <stdair/bom/StructAbstract.hpp>
// TraDemGen
#include <trademgen/basic/FFCodeTypes.hpp>

namespace TRADEMGEN {

  /** Utility Structure for the parsing of Flight-Period structures. */
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
    stdair::Duration_T _prefDepTime;
    stdair::AirportCode_T _origin;
    stdair::AirportCode_T _destination;
    std::string _cabinCode;
    FFProbDist_T _ffProbDist;
    float _demandMean;
    float _demandStdDev;
    
    /** Staging Date. */
    unsigned int _itYear;
    unsigned int _itMonth;
    unsigned int _itDay;
      
    /** Staging Time. */
    long _itHours;
    long _itMinutes;
    long _itSeconds;

    /** Staging Frequent Flyer code. */
    FFCode::EN_FFCode _itFFCode;
  };

}
#endif // __TRADEMGEN_BOM_DEMANDSTRUCT_HPP
