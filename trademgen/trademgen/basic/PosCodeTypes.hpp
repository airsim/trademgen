#ifndef __TRADEMGEN_BAS_POSCODETYPES_HPP
#define __TRADEMGEN_BAS_POSCODETYPES_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <map>
// StdAir
#include <stdair/STDAIR_Types.hpp>

namespace TRADEMGEN {

  /** Point-Of-Sale (POS) probability mass. */
  typedef float PosProbMass_T;
  
  /** Type definition for the distribution of Point-Of-Sale (POS)
      probability masses. */
  typedef std::map<stdair::AirportCode_T, PosProbMass_T> PosProbDist_T;
  
}
#endif // __TRADEMGEN_BAS_POSCODETYPES_HPP
