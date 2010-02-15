// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <sstream>
// StdAir
#include <stdair/basic/BasConst_General.hpp>
#include <stdair/service/Logger.hpp>
// TRADEMGEN
#include <trademgen/TRADEMGEN_Types.hpp>
#include <trademgen/bom/DemandStruct.hpp>

namespace TRADEMGEN {

  // //////////////////////////////////////////////////////////////////////
  DemandStruct_T::DemandStruct_T ()
    : _prefDepDate (stdair::DEFAULT_DATE), _prefArrDate (stdair::DEFAULT_DATE),
      _paxType (stdair::PassengerType::LEISURE),
      _itHours (0), _itMinutes (0), _itSeconds (0),
      _itFFCode (FFCode::NONE) {
  }

  // //////////////////////////////////////////////////////////////////////
  stdair::Date_T DemandStruct_T::getDate() const {
    return stdair::Date_T (_itYear, _itMonth, _itDay);
  }

  // //////////////////////////////////////////////////////////////////////
  stdair::Duration_T DemandStruct_T::getTime() const {
    return boost::posix_time::hours (_itHours)
      + boost::posix_time::minutes (_itMinutes)
      + boost::posix_time::seconds (_itSeconds);
  }
  
  // //////////////////////////////////////////////////////////////////////
  const std::string DemandStruct_T::describe() const {
    std::ostringstream ostr;
    ostr << _prefDepDate << " -> " << _prefArrDate
         << " " << _origin << "-" << _destination
         << " " << _paxType
         << ", N(" << _demandMean << ", " << _demandStdDev << "); ";

    unsigned short idx = 0;
    for (PosProbDist_T::const_iterator it = _posProbDist.begin();
         it != _posProbDist.end(); ++it, ++idx) {
      const stdair::AirportCode_T& lPosCode = it->first;
      const PosProbMass_T& lPosProbMass = it->second;
      if (idx != 0) {
        ostr << ", ";
      }
      ostr << lPosCode << ":" << lPosProbMass;
    }
    ostr << "; ";
    
    idx = 0;
    for (ChannelProbDist_T::const_iterator it = _channelProbDist.begin();
         it != _channelProbDist.end(); ++it, ++idx) {
      const ChannelCode::EN_ChannelCode lChannelCode = it->first;
      const ChannelProbMass_T& lChannelProbMass = it->second;
      if (idx != 0) {
        ostr << ", ";
      }
      ostr << ChannelCode::getCodeLabel(lChannelCode) << ":" << lChannelProbMass;
    }
    ostr << "; ";
    
    idx = 0;
    for (TripProbDist_T::const_iterator it = _tripProbDist.begin();
         it != _tripProbDist.end(); ++it, ++idx) {
      const TripCode::EN_TripCode lTripCode = it->first;
      const TripProbMass_T& lTripProbMass = it->second;
      if (idx != 0) {
        ostr << ", ";
      }
      ostr << TripCode::getCodeLabel(lTripCode) << ":" << lTripProbMass;
    }
    ostr << "; ";
    
    idx = 0;
    for (StayProbDist_T::const_iterator it = _stayProbDist.begin();
         it != _stayProbDist.end(); ++it, ++idx) {
      const stdair::DayDuration_T& lStayDuration = it->first;
      const StayProbMass_T& lStayProbMass = it->second;
      if (idx != 0) {
        ostr << ", ";
      }
      ostr << lStayDuration << ":" << lStayProbMass;
    }
    ostr << "; ";
    
    idx = 0;
    for (FFProbDist_T::const_iterator it = _ffProbDist.begin();
         it != _ffProbDist.end(); ++it, ++idx) {
      const FFCode::EN_FFCode lFFCode = it->first;
      const FFProbMass_T& lFFProbMass = it->second;
      if (idx != 0) {
        ostr << ", ";
      }
      ostr << FFCode::getCodeLabel(lFFCode) << ":" << lFFProbMass;
    }
    ostr << "; ";
    
    idx = 0;
    for (PrefDepTimeProbDist_T::const_iterator it = _prefDepTimeProbDist.begin();
         it != _prefDepTimeProbDist.end(); ++it, ++idx) {
      const stdair::Duration_T& lPrefDepTime = it->first;
      const PrefDepTimeProbMass_T& lPrefDepTimeProbMass = it->second;
      if (idx != 0) {
        ostr << ", ";
      }
      ostr << lPrefDepTime << ":" << lPrefDepTimeProbMass;
    }
    ostr << "; ";
    
    idx = 0;
    for (WTPProbDist_T::const_iterator it = _wtpProbDist.begin();
         it != _wtpProbDist.end(); ++it, ++idx) {
      const stdair::WTP_T& lWTP = it->first;
      const WTPProbMass_T& lWTPProbMass = it->second;
      if (idx != 0) {
        ostr << ", ";
      }
      ostr << lWTP << ":" << lWTPProbMass;
    }
    ostr << "; ";
    
    idx = 0;
    for (TimeValueProbDist_T::const_iterator it = _timeValueProbDist.begin();
         it != _timeValueProbDist.end(); ++it, ++idx) {
      const stdair::Duration_T& lTimeValue = it->first;
      const TimeValueProbMass_T& lTimeValueProbMass = it->second;
      if (idx != 0) {
        ostr << ", ";
      }
      ostr << lTimeValue << ":" << lTimeValueProbMass;
    }
    ostr << "; ";
    
    idx = 0;
    for (DTDProbDist_T::const_iterator it = _dtdProbDist.begin();
         it != _dtdProbDist.end(); ++it, ++idx) {
      const stdair::DayDuration_T& lDTD = it->first;
      const DTDProbMass_T& lDTDProbMass = it->second;
      if (idx != 0) {
        ostr << ", ";
      }
      ostr << lDTD << ":" << lDTDProbMass;
    }
    ostr << "; ";
    
    return ostr.str();
  }

}
