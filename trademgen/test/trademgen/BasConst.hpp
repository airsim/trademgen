#ifndef __TRADEMGEN_BAS_BASCONST_HPP
#define __TRADEMGEN_BAS_BASCONST_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <string>
#include <list>
// Boost Random
#include <boost/random/linear_congruential.hpp>
// Boost Datetime
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

namespace TRADEMGEN {

  /** Number of seconds in one day */
  extern const Count_T SECONDS_IN_ONE_DAY = 86000;
  
  /** Number of milliseconds in one second */
  extern const Count_T MILLISECONDS_IN_ONE_SECOND = 1000;
  
}
#endif // __TRADEMGEN_BAS_BASCONST_HPP

