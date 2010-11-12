// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// TRADEMGEN
#include <trademgen/bom/DemandStreamKey.hpp>

namespace TRADEMGEN {

  // ////////////////////////////////////////////////////////////////////
  DemandStreamKey::DemandStreamKey () {
  }
  
  // ////////////////////////////////////////////////////////////////////
  DemandStreamKey::DemandStreamKey(const stdair::AirportCode_T& iOrigin,
                                   const stdair::AirportCode_T& iDestination,
                                   const stdair::Date_T& iPreferredDepartureDate,
                                   const stdair::CabinCode_T& iPreferredCabin)
    : _origin (iOrigin), _destination (iDestination),
      _preferredDepartureDate (iPreferredDepartureDate),
      _preferredCabin (iPreferredCabin) {
  }

  // ////////////////////////////////////////////////////////////////////
  DemandStreamKey::DemandStreamKey (const DemandStreamKey& iKey)
    : _origin (iKey._origin), _destination (iKey._destination),
      _preferredDepartureDate (iKey._preferredDepartureDate),
      _preferredCabin (iKey._preferredCabin) {
  }

  // ////////////////////////////////////////////////////////////////////
  DemandStreamKey::~DemandStreamKey () {
  }

  // ////////////////////////////////////////////////////////////////////
  void DemandStreamKey::toStream (std::ostream& ioOut) const {
    ioOut << "DemandStreamKey: " << toString() << std::endl;
  }

  // ////////////////////////////////////////////////////////////////////
  void DemandStreamKey::fromStream (std::istream& ioIn) {
  }

  // ////////////////////////////////////////////////////////////////////
  const std::string DemandStreamKey::toString() const {
    std::ostringstream oStr;
    oStr << _origin << "-" << _destination << " " << _preferredDepartureDate
         << " " << _preferredCabin;
    return oStr.str();
  }

}
