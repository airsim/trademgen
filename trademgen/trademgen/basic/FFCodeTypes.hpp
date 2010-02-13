#ifndef __TRADEMGEN_BAS_FFCODETYPES_HPP
#define __TRADEMGEN_BAS_FFCODETYPES_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <map>
// TraDemGen
#include <trademgen/basic/FFCode.hpp>

namespace TRADEMGEN {

  /** Frequent flyer probability mass. */
  typedef float FFProbMass_T;
  
  /** Type definition for the distribution of Frequent Flyer
      probability masses. */
  typedef std::map<FFCode::EN_FFCode, FFProbMass_T> FFProbDist_T;
  
}
#endif // __TRADEMGEN_BAS_FFCODETYPES_HPP
