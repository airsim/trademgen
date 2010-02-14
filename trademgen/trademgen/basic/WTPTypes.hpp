#ifndef __TRADEMGEN_BAS_WTPTYPES_HPP
#define __TRADEMGEN_BAS_WTPTYPES_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <map>
// StdAir
#include <stdair/STDAIR_Types.hpp>

namespace TRADEMGEN {

  /** Willingness-To-Pay (WTP) probability distribution cumulative point. */
  typedef float WTPProbMass_T;
  
  /** Type definition for the continuous probability distribution of
      Willingness-To-Pay (WTP). */
  typedef std::map<stdair::WTP_T, WTPProbMass_T> WTPProbDist_T;
  
}
#endif // __TRADEMGEN_BAS_WTPTYPES_HPP
