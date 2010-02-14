#ifndef __TRADEMGEN_BAS_DTDTYPES_HPP
#define __TRADEMGEN_BAS_DTDTYPES_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <map>
// StdAir
#include <stdair/STDAIR_Types.hpp>

namespace TRADEMGEN {

  /** Arrival pattern (as expressed in DTD) probability distribution
      cumulative point. */
  typedef float DTDProbMass_T;
  
  /** Type definition for the continuous probability distribution of
      arrival pattern (as expressed in DTD). */
  typedef std::map<stdair::DayDuration_T, DTDProbMass_T> DTDProbDist_T;
  
}
#endif // __TRADEMGEN_BAS_DTDTYPES_HPP
