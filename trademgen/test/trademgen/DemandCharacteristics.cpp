// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <sstream>
// TraDemGen
#include <test/trademgen/BasTypes.hpp>
#include <test/trademgen/DemandCharacteristics.hpp>
#include <test/trademgen/ContinuousAttribute.hpp>

namespace TRADEMGEN {
  
  // /////////////////////////////////////////////////////
  DemandCharacteristics::DemandCharacteristics ()
    : _origin ("") {
  }
  
  // /////////////////////////////////////////////////////
  DemandCharacteristics::~DemandCharacteristics () {
  }
  
  // /////////////////////////////////////////////////////
  DemandCharacteristics::
  DemandCharacteristics (const DemandCharacteristics& iDemandCharacteristics) {
    _meanNumberOfRequests = iDemandCharacteristics._meanNumberOfRequests;
    _standardDeviationNumberOfRequests =
      iDemandCharacteristics._standardDeviationNumberOfRequests;
    _origin = iDemandCharacteristics._origin;
    _preferredDepartureDate = iDemandCharacteristics._preferredDepartureDate;
    _arrivalPattern = iDemandCharacteristics._arrivalPattern;
  }
  
  // /////////////////////////////////////////////////////
  std::string DemandCharacteristics::display() const {
    std::ostringstream oStr;

    //
    oStr << "****************** Demand characteristics ******************"
             << std::endl;
    oStr << "Mean number of requests .......... : "
             << _meanNumberOfRequests << std::endl;
    oStr << "Std dev of number of requests .... : "
             << _standardDeviationNumberOfRequests << std::endl;
    oStr << "Origin ........................... : "
             << _origin << std::endl;
    oStr << "Preferred departure date ......... : "
             << _preferredDepartureDate << std::endl;
    oStr << "Arrival pattern (days from departure, proportion):"
             << std::endl;

    //
    oStr << _arrivalPattern.displayCumulativeDistribution();
    
    return oStr.str();
  }
    
}

