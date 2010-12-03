// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <sstream>
// StdAir
#include <stdair/bom/LegDateKey.hpp>

namespace stdair {

  // ////////////////////////////////////////////////////////////////////
  LegDateKey::LegDateKey (const AirportCode_T& iBoardingPoint) 
    : _boardingPoint (iBoardingPoint) {
  }
  // ////////////////////////////////////////////////////////////////////
  LegDateKey::LegDateKey (const LegDateKey& iKey)
    : _boardingPoint (iKey._boardingPoint) {
  }

  // ////////////////////////////////////////////////////////////////////
  LegDateKey::~LegDateKey () {
  }

  // ////////////////////////////////////////////////////////////////////
  void LegDateKey::toStream (std::ostream& ioOut) const {
    ioOut << "LegDateKey: " << toString() << std::endl;
  }

  // ////////////////////////////////////////////////////////////////////
  void LegDateKey::fromStream (std::istream& ioIn) {
  }

  // ////////////////////////////////////////////////////////////////////
  const std::string LegDateKey::toString() const {
    std::ostringstream oStr;
    oStr << _boardingPoint;
    return oStr.str();
  }

}
