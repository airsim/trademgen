#ifndef __TRADEMGEN_BAS_TIMEVALUETYPES_HPP
#define __TRADEMGEN_BAS_TIMEVALUETYPES_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <map>
// StdAir
#include <stdair/STDAIR_Types.hpp>

namespace TRADEMGEN {

  /** Time value probability distribution cumulative point. */
  typedef float TimeValueProbMass_T;
  
  /** Type definition for the continuous probability distribution of
      time value. */
  typedef std::map<stdair::PriceValue_T,
                   TimeValueProbMass_T> TimeValueProbDist_T;
  
}
#endif // __TRADEMGEN_BAS_TIMEVALUETYPES_HPP
