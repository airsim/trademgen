#ifndef __TRADEMGEN_BAS_TRIPCODETYPES_HPP
#define __TRADEMGEN_BAS_TRIPCODETYPES_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <map>
// TraDemGen
#include <trademgen/basic/TripCode.hpp>

namespace TRADEMGEN {

  /** Trip type probability mass. */
  typedef float TripProbMass_T;
  
  /** Type definition for the distribution of trip type probability masses. */
  typedef std::map<TripCode::EN_TripCode, TripProbMass_T> TripProbDist_T;
  
}
#endif // __TRADEMGEN_BAS_TRIPCODETYPES_HPP
