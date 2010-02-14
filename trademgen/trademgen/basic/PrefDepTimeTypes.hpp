#ifndef __TRADEMGEN_BAS_PREFDEPTIMETYPES_HPP
#define __TRADEMGEN_BAS_PREFDEPTIMETYPES_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <map>
// StdAir
#include <stdair/STDAIR_Types.hpp>

namespace TRADEMGEN {

  /** Preferred departure time probability distribution cumulative point. */
  typedef float PrefDepTimeProbMass_T;
  
  /** Type definition for the continuous probability distribution of
      preferred departure time. */
  typedef std::map<stdair::Duration_T,
                   PrefDepTimeProbMass_T> PrefDepTimeProbDist_T;
  
}
#endif // __TRADEMGEN_BAS_PREFDEPTIMETYPES_HPP
