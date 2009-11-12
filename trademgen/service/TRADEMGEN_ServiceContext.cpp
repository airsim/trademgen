// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <sstream>
// Forecast
#include <forecast/service/FORECAST_ServiceContext.hpp>

namespace FORECAST {

  // //////////////////////////////////////////////////////////////////////
  FORECAST_ServiceContext::FORECAST_ServiceContext ()
    : _sociSession (NULL) {
  }

  // //////////////////////////////////////////////////////////////////////
  FORECAST_ServiceContext::
  FORECAST_ServiceContext (const std::string& iServiceName)
    : _sociSession (NULL) {
  }

  // //////////////////////////////////////////////////////////////////////
  FORECAST_ServiceContext::~FORECAST_ServiceContext() {
  }
  
  // //////////////////////////////////////////////////////////////////////
  soci::session& FORECAST_ServiceContext::getSociSessionHandler() const {
    assert (_sociSession != NULL);
    return *_sociSession;
  }

  // //////////////////////////////////////////////////////////////////////
  const std::string FORECAST_ServiceContext::shortDisplay() const {
    std::ostringstream oStr;
    oStr << "FORECAST_ServiceContext: " << std::endl
         << std::endl;
    return oStr.str();
  }

  // //////////////////////////////////////////////////////////////////////
  const std::string FORECAST_ServiceContext::display() const {
    std::ostringstream oStr;
    oStr << shortDisplay();
    return oStr.str();
  }

}
