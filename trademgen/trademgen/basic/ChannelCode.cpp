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
#include <trademgen/basic/ChannelCode.hpp>

namespace TRADEMGEN {
  
  // //////////////////////////////////////////////////////////////////////
  const std::string ChannelCode::_labels[LAST_VALUE] =
    { "Direct Off-line","Direct On-line","Indirect Off-line","Indirect On-line"};

  const std::string ChannelCode::_codeLabels[LAST_VALUE] =
    { "DF", "DN", "IF", "IN" };

  
  // //////////////////////////////////////////////////////////////////////
  ChannelCode::ChannelCode (const EN_ChannelCode& iChannelCode)
    : _code (iChannelCode) {
  }

  // //////////////////////////////////////////////////////////////////////
  ChannelCode::ChannelCode (const std::string& iCode) {
    _code = LAST_VALUE;
    
    if (iCode == "DF") {
      _code = DIRECT_OFFLINE;

    } else if (iCode == "DN") {
      _code = DIRECT_ONLINE;

    } else if (iCode == "IF") {
      _code = INDIRECT_OFFLINE;

    } else if (iCode == "IN") {
      _code = INDIRECT_ONLINE;
    }

    if (_code == LAST_VALUE) {
      const std::string& lLabels = describeLabels();
      STDAIR_LOG_ERROR ("The channel code '" << iCode
                        << "' is not known. Known channel codes: " << lLabels);
      throw CodeConversionException();
    }
  }
  
  // //////////////////////////////////////////////////////////////////////
  const std::string& ChannelCode::getLabel (const EN_ChannelCode& iCode) {
    return _labels[iCode];
  }
  
  // //////////////////////////////////////////////////////////////////////
  const std::string& ChannelCode::getCodeLabel (const EN_ChannelCode& iCode) {
    return _codeLabels[iCode];
  }

  // //////////////////////////////////////////////////////////////////////
  std::string ChannelCode::describeLabels() {
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
  ChannelCode::EN_ChannelCode ChannelCode::getCode() const {
    return _code;
  }
  
  // //////////////////////////////////////////////////////////////////////
  const std::string ChannelCode::describe() const {
    std::ostringstream ostr;
    ostr << _labels[_code];
    return ostr.str();
  }

}
