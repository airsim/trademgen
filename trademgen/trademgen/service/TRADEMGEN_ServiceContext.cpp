// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <sstream>
// StdAir
#include <stdair/STDAIR_Service.hpp>
// Trademgen
#include <trademgen/service/TRADEMGEN_ServiceContext.hpp>

namespace TRADEMGEN {

  // //////////////////////////////////////////////////////////////////////
  TRADEMGEN_ServiceContext::TRADEMGEN_ServiceContext () {
  }

  // //////////////////////////////////////////////////////////////////////
  TRADEMGEN_ServiceContext::
  TRADEMGEN_ServiceContext (const std::string& iServiceName) {
  }

  // //////////////////////////////////////////////////////////////////////
  TRADEMGEN_ServiceContext::~TRADEMGEN_ServiceContext() {
  }
  
  // //////////////////////////////////////////////////////////////////////
  stdair::STDAIR_Service& TRADEMGEN_ServiceContext::getSTDAIR_Service() const {
    assert (_stdairService != NULL);
    return *_stdairService;
  }
  
  // //////////////////////////////////////////////////////////////////////
  void TRADEMGEN_ServiceContext::
  setSTDAIR_Service (stdair::STDAIR_ServicePtr_T ioSTDAIR_ServicePtr) {
    _stdairService = ioSTDAIR_ServicePtr;
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
