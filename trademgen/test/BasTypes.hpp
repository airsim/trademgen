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

namespace TRADEMGEN {

  /** See for the random generation, so that it can be reproductible. */
  typedef unsigned long int RandomSeed_T;
  
  /** Duration (e.g., between two events) */
  typedef double Duration_T;
  
  /** Event time (time at which an event occurs) */
  typedef double EventTime_T;
  
  /** Event (here, the time of the event) */
  typedef EventTime_T Event_T;
  
  /** List of events */
  typedef std::list<Event_T> EventList_T;

  /** Rate of event arrivals */
  typedef double Rate_T;
  
  /** Key */
  typedef std::string Key_T;

  /** Random number generator. */
  typedef boost::minstd_rand BaseGenerator_T;
}
#endif // __TRADEMGEN_BAS_BASTYPES_HPP

