// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// StdAir
#include <stdair/stdair_types.hpp>
#include <stdair/bom/InventoryKey.hpp>

namespace stdair {

  // ////////////////////////////////////////////////////////////////////
  InventoryKey::InventoryKey (const AirlineCode_T& iAirlineCode)
    : _airlineCode (iAirlineCode) {
  }
  // ////////////////////////////////////////////////////////////////////
  InventoryKey::InventoryKey (const InventoryKey& iKey)
    : _airlineCode (iKey._airlineCode) {
  }

  // ////////////////////////////////////////////////////////////////////
  InventoryKey::~InventoryKey () {
  }

  // ////////////////////////////////////////////////////////////////////
  void InventoryKey::toStream (std::ostream& ioOut) const {
    ioOut << "InventoryKey: " << toString() << std::endl;
  }

  // ////////////////////////////////////////////////////////////////////
  void InventoryKey::fromStream (std::istream& ioIn) {
  }

  // ////////////////////////////////////////////////////////////////////
  const std::string InventoryKey::toString() const {
    std::ostringstream oStr;
    oStr << _airlineCode;
    return oStr.str();
  }

}
