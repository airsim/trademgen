// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <iosfwd>
// TraDemGen
#include <BasTypes.hpp>
#include <DemandCharacteristics.hpp>
#include <ContinuousAttribute.hpp>

namespace TRADEMGEN {
  
  // /////////////////////////////////////////////////////
  DemandCharacteristics::DemandCharacteristics ()
    : _origin ("") {
  }
  
  // /////////////////////////////////////////////////////
  DemandCharacteristics::~DemandCharacteristics () {
  }
  
  // /////////////////////////////////////////////////////
  DemandCharacteristics::DemandCharacteristics (const DemandCharacteristics& iDemandCharacteristics) {
    _meanNumberOfRequests = iDemandCharacteristics._meanNumberOfRequests;
    _standardDeviationNumberOfRequests = iDemandCharacteristics._standardDeviationNumberOfRequests;
    _origin = iDemandCharacteristics._origin;
    _preferredDepartureDate = iDemandCharacteristics._preferredDepartureDate;
    _arrivalPattern = iDemandCharacteristics._arrivalPattern;
  }
  
  // /////////////////////////////////////////////////////
  void DemandCharacteristics::display (std::ostream& ioStream) const {
    ioStream << "****************** Demand characteristics ******************" << std::endl;
    ioStream << "Mean number of requests .......... : " << _meanNumberOfRequests << std::endl;
    ioStream << "Std dev of number of requests .... : " << _standardDeviationNumberOfRequests << std::endl;
    ioStream << "Origin ........................... : " << _origin << std::endl;
    ioStream << "Preferred departure date ......... : " << _preferredDepartureDate << std::endl;
    ioStream << "Arrival pattern (days from departure, proportion):" << std::endl;
    _arrivalPattern.displayCumulativeDistribution (ioStream);
  }
    
}

