// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <sstream>
// StdAir
#include <stdair/basic/BasConst_General.hpp>
#include <stdair/service/Logger.hpp>
// TRADEMGEN
#include <trademgen/TRADEMGEN_Types.hpp>
#include <trademgen/bom/DemandStruct.hpp>

namespace TRADEMGEN {

  // //////////////////////////////////////////////////////////////////////
  DemandStruct_T::DemandStruct_T ()
    : _prefDepDate (stdair::DEFAULT_DATE), _itSeconds (0) {
  }

  // //////////////////////////////////////////////////////////////////////
  stdair::Date_T DemandStruct_T::getDate() const {
    return stdair::Date_T (_itYear, _itMonth, _itDay);
  }

  // //////////////////////////////////////////////////////////////////////
  stdair::Duration_T DemandStruct_T::getTime() const {
    return boost::posix_time::hours (_itHours)
      + boost::posix_time::minutes (_itMinutes)
      + boost::posix_time::seconds (_itSeconds);
  }
  
  // //////////////////////////////////////////////////////////////////////
  const std::string DemandStruct_T::describe() const {
    std::ostringstream ostr;
    ostr << _prefDepDate << std::endl;

    return ostr.str();
  }

}
