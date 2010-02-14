#ifndef __TRADEMGEN_BAS_CHANNELCODETYPES_HPP
#define __TRADEMGEN_BAS_CHANNELCODETYPES_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <map>
// TraDemGen
#include <trademgen/basic/ChannelCode.hpp>

namespace TRADEMGEN {

  /** Channel type probability mass. */
  typedef float ChannelProbMass_T;
  
  /** Type definition for the distribution of channel type probability masses. */
  typedef std::map<ChannelCode::EN_ChannelCode,
                   ChannelProbMass_T> ChannelProbDist_T;
  
}
#endif // __TRADEMGEN_BAS_CHANNELCODETYPES_HPP
