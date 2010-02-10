#ifndef __TRADEMGEN_BOM_DEMANDSTREAMTYPES_HPP
#define __TRADEMGEN_BOM_DEMANDSTREAMTYPES_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <map>
// STDAIR
#include <stdair/STDAIR_Types.hpp>

namespace TRADEMGEN {

  // Forward declaration
  struct DemandStream;
  
  /** Define a map of demand streams. */
  typedef std::map<const stdair::DemandStreamKey_T, DemandStream> DemandStreamList_T;
  
}
#endif // __TRADEMGEN_BOM_DEMANDSTREAMTYPES_HPP
