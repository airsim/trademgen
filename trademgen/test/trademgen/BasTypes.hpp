#ifndef __TRADEMGEN_BAS_BASTYPES_HPP
#define __TRADEMGEN_BAS_BASTYPES_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <string>
#include <list>
// Boost Random
#include <boost/random/linear_congruential.hpp>
// Boost Datetime
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

namespace TRADEMGEN {

  // //////////////////////////////////////////////////////////////////////
  // Random generation
  
  /** Seed for the random generation, so that it can be reproductible. */
  typedef unsigned long int RandomSeed_T;
  
  /** Random number generator. */
  typedef boost::minstd_rand BaseGenerator_T;

  
  // //////////////////////////////////////////////////////////////////////
  // Probability

  /** Probability */
  typedef float Probability_T;
  
  
  // //////////////////////////////////////////////////////////////////////
  // Date / Time
  
  /** Date (date at which an event occurs) */
  typedef boost::gregorian::date Date_T;
  
  /** Time duration */
  typedef boost::posix_time::time_duration TimeDuration_T;
  
  /** Time */
  typedef boost::posix_time::time_duration Time_T;
  
  /** Datetime (datetime at which an event occurs) */
  typedef boost::posix_time::ptime Datetime_T;
  
  /** Time duration in (integer) number of seconds */
  typedef long int IntDuration_T;
  
  /** Duration in (float) number of time units */
  typedef float FloatDuration_T;
  
  // //////////////////////////////////////////////////////////////////////
  // Basic types
  /** Event type */
  typedef std::string EventType_T;
  
  /** Count */
  typedef int Count_T;
  
  /** Location code */
  typedef std::string LocationCode_T;
  
  /** Airport code */
  typedef LocationCode_T AirportCode_T;
  
  /** City code */
  typedef LocationCode_T CityCode_T;
  
  /** Trip type */
  typedef std::string TripType_T;
  
  /** Monetary value */
  typedef double MonetaryValue_T;
  
  /** Real number */
  typedef double RealNumber_T;
  
  // //////////////////////////////////////////////////////////////////////
  // Demand
  typedef int Key_T;
  
  // /** Duration (e.g., between two events) */
  //typedef boost::gregorian:: Duration_T;
  
  // /** List of events */
  //typedef std::list<Event_T> EventList_T;

  
  // /** Event (here, the time of the event) */
  // typedef EventTime_T Event_T;
  
}
#endif // __TRADEMGEN_BAS_BASTYPES_HPP

