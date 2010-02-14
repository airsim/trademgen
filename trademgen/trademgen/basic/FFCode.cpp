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
#include <trademgen/basic/FFCode.hpp>

namespace TRADEMGEN {

  // //////////////////////////////////////////////////////////////////////
  const std::string FFCode::_labels[LAST_VALUE] =
        { "Platinum", "Gold", "Silver", "Member", "None" };

  const char FFCode::_codeLabels[LAST_VALUE] = { 'P', 'G', 'S', 'M', 'N' };
  
  
  // //////////////////////////////////////////////////////////////////////
  FFCode::FFCode (const EN_FFCode& iFFCode) : _code (iFFCode) {
  }
  
  // //////////////////////////////////////////////////////////////////////
  FFCode::FFCode (const char iChar) {
    switch (iChar) {
    case 'P': _code = PLATINUM; break;
    case 'G': _code = GOLD; break;
    case 'S': _code = SILVER; break;
    case 'M': _code = MEMBER; break;
    case 'N': _code = NONE; break;
    default: _code = LAST_VALUE; break;
    }
    
    if (_code == LAST_VALUE) {
      const std::string& lLabels = describeLabels();
      STDAIR_LOG_ERROR ("The channel code '" << iChar
                        << "' is not known. Known channel codes: " << lLabels);
      throw CodeConversionException();
    }
  }
  
  // //////////////////////////////////////////////////////////////////////
  const std::string& FFCode::getLabel (const EN_FFCode& iCode) {
    return _labels[iCode];
  }
  
  // //////////////////////////////////////////////////////////////////////
  char FFCode::getCodeLabel (const EN_FFCode& iCode) {
    return _codeLabels[iCode];
  }

  // //////////////////////////////////////////////////////////////////////
  std::string FFCode::describeLabels() {
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
  FFCode::EN_FFCode FFCode::getCode() const {
    return _code;
  }
  
  // //////////////////////////////////////////////////////////////////////
  const std::string FFCode::describe() const {
    std::ostringstream ostr;
    ostr << _labels[_code];
    return ostr.str();
  }

}