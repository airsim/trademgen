#ifndef __TRADEMGEN_BAS_STAYDURATIONTYPES_HPP
#define __TRADEMGEN_BAS_STAYDURATIONTYPES_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <map>
// StdAir
#include <stdair/STDAIR_Types.hpp>

namespace TRADEMGEN {

  /** Point-Of-Sale (STAY) probability mass. */
  typedef float StayProbMass_T;
  
  /** Type definition for the distribution of Point-Of-Sale (STAY)
      probability masses. */
  typedef std::map<stdair::DayDuration_T, StayProbMass_T> StayProbDist_T;
  
}
#endif // __TRADEMGEN_BAS_STAYDURATIONTYPES_HPP
