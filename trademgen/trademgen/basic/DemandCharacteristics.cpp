// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <sstream>
// STDAIR
#include <stdair/STDAIR_Types.hpp>
#include <stdair/basic/BasConst_General.hpp>
// TRADEMGEN
#include <trademgen/basic/DemandCharacteristics.hpp>

namespace TRADEMGEN {
  
  // /////////////////////////////////////////////////////
  DemandCharacteristics::
  DemandCharacteristics (const ArrivalPatternCumulativeDistribution_T& iArrivalPattern,
                         const POSProbabilityMassFunction_T& iPOSProbMass,
                         const ChannelProbabilityMassFunction_T& iChannelProbMass,
                         const TripTypeProbabilityMassFunction_T& iTripTypeProbMass,
                         const StayDurationProbabilityMassFunction_T& iStayDurationProbMass,
                         const FrequentFlyerProbabilityMassFunction_T& iFrequentFlyerProbMass,
                         const PreferredDepartureTimeContinuousDistribution_T& iPreferredDepartureTimeContinuousDistribution,
                         const WTPContinuousDistribution_T& iWTPContinuousDistribution,
                         const ValueOfTimeContinuousDistribution_T& iValueOfTimeContinuousDistribution)
    : _arrivalPattern (iArrivalPattern),
      _posProbabilityMass (iPOSProbMass),
      _channelProbabilityMass (iChannelProbMass),
      _tripTypeProbabilityMass (iTripTypeProbMass),
      _stayDurationProbabilityMass (iStayDurationProbMass),
      _frequentFlyerProbabilityMass (iFrequentFlyerProbMass),
      _preferredDepartureTimeCumulativeDistribution (iPreferredDepartureTimeContinuousDistribution),
      _wtpCumulativeDistribution (iWTPContinuousDistribution),
      _valueOfTimeCumulativeDistribution (iValueOfTimeContinuousDistribution) {
  }
    
  // /////////////////////////////////////////////////////
  DemandCharacteristics::~DemandCharacteristics () {
  }
  
  // /////////////////////////////////////////////////////
  std::string DemandCharacteristics::display() const {
    std::ostringstream oStr;

    //
    oStr << "****************** Demand characteristics ******************"
         << std::endl;
    oStr << "Arrival pattern (days from departure, proportion): ";
    oStr << _arrivalPattern.displayCumulativeDistribution() << std::endl;
    oStr << "POS probability mass (POS, propotion): ";
    oStr << _posProbabilityMass.displayProbabilityMass()
         << std::endl;
    oStr << "Channel probability mass (channel, propotion): ";
    oStr << _channelProbabilityMass.displayProbabilityMass()
         << std::endl;
    oStr << "Trip type probability mass (trip type, propotion): ";
    oStr << _tripTypeProbabilityMass.displayProbabilityMass()
         << std::endl;
    oStr << "Stay duration probability mass (number of days, propotion): ";
    oStr << _stayDurationProbabilityMass.displayProbabilityMass()
         << std::endl;
    oStr << "Frequent flyer probability mass (frequent flyer, propotion): ";
    oStr << _frequentFlyerProbabilityMass.displayProbabilityMass()
         << std::endl;
    oStr << "Preferred departure time cumulative distribution (time, proportion: ";
    oStr << _preferredDepartureTimeCumulativeDistribution.displayCumulativeDistribution() << std::endl;
    oStr << "WTP cumulative distribution (WTP, proportion: ";
    oStr << _wtpCumulativeDistribution.displayCumulativeDistribution()
         << std::endl;
    oStr << "Value of time cumulative distribution (value of time, proportion: ";
    oStr << _valueOfTimeCumulativeDistribution.displayCumulativeDistribution()
         << std::endl;

    
    return oStr.str();
  }
    
}
