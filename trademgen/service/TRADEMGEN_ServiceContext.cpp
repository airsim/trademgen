// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <sstream>
// Trademgen
#include <trademgen/service/TRADEMGEN_ServiceContext.hpp>

namespace TRADEMGEN {

  // //////////////////////////////////////////////////////////////////////
  TRADEMGEN_ServiceContext::TRADEMGEN_ServiceContext ()
    : _sociSession (NULL) {
  }

  // //////////////////////////////////////////////////////////////////////
  TRADEMGEN_ServiceContext::
  TRADEMGEN_ServiceContext (const std::string& iServiceName)
    : _sociSession (NULL) {
  }

  // //////////////////////////////////////////////////////////////////////
  TRADEMGEN_ServiceContext::~TRADEMGEN_ServiceContext() {
  }
  
  // //////////////////////////////////////////////////////////////////////
  soci::session& TRADEMGEN_ServiceContext::getSociSessionHandler() const {
    assert (_sociSession != NULL);
    return *_sociSession;
  }

  // //////////////////////////////////////////////////////////////////////
  const std::string TRADEMGEN_ServiceContext::shortDisplay() const {
    std::ostringstream oStr;
    oStr << "TRADEMGEN_ServiceContext: " << std::endl
         << std::endl;
    return oStr.str();
  }

  // //////////////////////////////////////////////////////////////////////
  const std::string TRADEMGEN_ServiceContext::display() const {
    std::ostringstream oStr;
    oStr << shortDisplay();
    return oStr.str();
  }

}
