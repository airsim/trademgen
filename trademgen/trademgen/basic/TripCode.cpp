// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <sstream>
// StdAir
#include <stdair/service/Logger.hpp>
// TraDemGen
#include <trademgen/TRADEMGEN_Types.hpp>
#include <trademgen/basic/TripCode.hpp>

namespace TRADEMGEN {
  
  // //////////////////////////////////////////////////////////////////////
  const std::string TripCode::_labels[LAST_VALUE] =
    { "Outbound of Roundtrip", "Inbound of Roundtrip", "One-way"};

  const std::string TripCode::_codeLabels[LAST_VALUE] = { "RO", "RI", "OW" };


  // //////////////////////////////////////////////////////////////////////
  TripCode::TripCode (const EN_TripCode& iTripCode)
    : _code (iTripCode) {
  }

  // //////////////////////////////////////////////////////////////////////
  TripCode::TripCode (const std::string& iCode) {
    _code = LAST_VALUE;
    
    if (iCode == "RO") {
      _code = OUTBOUND;

    } else if (iCode == "RI") {
      _code = INBOUND;

    } else if (iCode == "OW") {
      _code = ONEWAY;

    }

    if (_code == LAST_VALUE) {
      const std::string& lLabels = describeLabels();
      STDAIR_LOG_ERROR ("The trip code '" << iCode
                        << "' is not known. Known trip codes: " << lLabels);
      throw CodeConversionException();
    }
  }
  
  // //////////////////////////////////////////////////////////////////////
  const std::string& TripCode::getLabel (const EN_TripCode& iCode) {
    return _labels[iCode];
  }
  
  // //////////////////////////////////////////////////////////////////////
  const std::string& TripCode::getCodeLabel (const EN_TripCode& iCode) {
    return _codeLabels[iCode];
  }

  // //////////////////////////////////////////////////////////////////////
  std::string TripCode::describeLabels() {
    std::ostringstream ostr;
    for (unsigned short idx = 0; idx != LAST_VALUE; ++idx) {
      if (idx != 0) {
        ostr << ", ";
      }
      ostr << _labels[idx];
    }
    return ostr.str();
  }

  // //////////////////////////////////////////////////////////////////////
  TripCode::EN_TripCode TripCode::getCode() const {
    return _code;
  }
  
  // //////////////////////////////////////////////////////////////////////
  const std::string TripCode::describe() const {
    std::ostringstream ostr;
    ostr << _labels[_code];
    return ostr.str();
  }

}
