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
#include <trademgen/basic/ChannelCode.hpp>
#include <trademgen/bom/DemandStruct.hpp>

namespace TRADEMGEN {

  // ////////////////////////////////////////////////////////////////////
  DemandStruct_T::DemandStruct_T ()
    : _prefDepDate (stdair::DEFAULT_DATE), _prefArrDate (stdair::DEFAULT_DATE),
      _prefCabin (""), _itHours (0), _itMinutes (0), _itSeconds (0),
      _itFFCode (FFCode::NONE) {
  }

  // ////////////////////////////////////////////////////////////////////
  stdair::Date_T DemandStruct_T::getDate() const {
    return stdair::Date_T (_itYear, _itMonth, _itDay);
  }

  // ////////////////////////////////////////////////////////////////////
  stdair::Duration_T DemandStruct_T::getTime() const {
    return boost::posix_time::hours (_itHours)
      + boost::posix_time::minutes (_itMinutes)
      + boost::posix_time::seconds (_itSeconds);
  }

  // ////////////////////////////////////////////////////////////////////
  const stdair::ContinuousFloatDuration_T DemandStruct_T::
  getArrivalPattern () const {
    stdair::ArrivalPatternCumulativeDistribution_T arrivalPatternCumulativeDistribution;
    
    for (DTDProbDist_T::const_iterator it = _dtdProbDist.begin();
         it != _dtdProbDist.end(); ++it) {
      const stdair::DayDuration_T& lDTD = it->first;
      const DTDProbMass_T& lDTDProbMass = it->second;
      
      const stdair::FloatDuration_T lZeroDTDFloat = 0.0;
      stdair::FloatDuration_T lDTDFloat =
        static_cast<stdair::FloatDuration_T> (lDTD);
      lDTDFloat = lZeroDTDFloat - lDTD;

      arrivalPatternCumulativeDistribution.
        insert (stdair::ArrivalPatternCumulativeDistribution_T::
                value_type (lDTDFloat, lDTDProbMass));
    }
    
    return stdair::ContinuousFloatDuration_T (arrivalPatternCumulativeDistribution);
  }

  // ////////////////////////////////////////////////////////////////////
  const stdair::POSProbabilityMass_T DemandStruct_T::
  getPOSProbabilityMass () const {
    stdair::POSProbabilityMassFunction_T lPOSProbabilityMassFuction;

    for (PosProbDist_T::const_iterator it = _posProbDist.begin();
         it != _posProbDist.end(); ++it) {
      const stdair::AirportCode_T& lPOS = it->first;
      const PosProbMass_T& lProbability = it->second;

      lPOSProbabilityMassFuction.
        insert (stdair::POSProbabilityMassFunction_T::
                value_type (lPOS, lProbability));
    }

    return stdair::POSProbabilityMass_T (lPOSProbabilityMassFuction);
  }

  // ////////////////////////////////////////////////////////////////////
  const stdair::ChannelProbabilityMass_T DemandStruct_T::
  getChannelProbabilityMass () const {
    stdair::ChannelProbabilityMassFunction_T lChannelProbabilityMassFuction;

    for (ChannelProbDist_T::const_iterator it = _channelProbDist.begin();
         it != _channelProbDist.end(); ++it) {
      const ChannelCode& lChannelCode = it->first;
      const ChannelCode::EN_ChannelCode& lENChannelCode = lChannelCode.getCode();
      const stdair::ChannelLabel_T lLabel =
        ChannelCode::getLabel (lENChannelCode);
      const stdair::Probability_T& lProbability = it->second;

      lChannelProbabilityMassFuction.
        insert (stdair::ChannelProbabilityMassFunction_T::
                value_type (lLabel, lProbability));
    }

    return stdair::ChannelProbabilityMass_T (lChannelProbabilityMassFuction);
  }

  // ////////////////////////////////////////////////////////////////////
  const stdair::TripTypeProbabilityMass_T DemandStruct_T::
  getTripTypeProbabilityMass () const {
    stdair::TripTypeProbabilityMassFunction_T lTripTypeProbabilityMassFuction;

    for (TripProbDist_T::const_iterator it = _tripProbDist.begin();
         it != _tripProbDist.end(); ++it) {
      const TripCode& lTripCode = it->first;
      const TripCode::EN_TripCode& lENTripCode = lTripCode.getCode();
      const stdair::TripType_T lLabel =
        TripCode::getLabel (lENTripCode);
      const stdair::Probability_T& lProbability = it->second;

      lTripTypeProbabilityMassFuction.
        insert (stdair::TripTypeProbabilityMassFunction_T::
                value_type (lLabel, lProbability));
    }

    return stdair::TripTypeProbabilityMass_T (lTripTypeProbabilityMassFuction);
  }

  // ////////////////////////////////////////////////////////////////////
  const stdair::StayDurationProbabilityMass_T DemandStruct_T::
  getStayDurationProbabilityMass () const {
    stdair::StayDurationProbabilityMassFunction_T lStayDurationProbabilityMassFuction;

    for (StayProbDist_T::const_iterator it = _stayProbDist.begin();
         it != _stayProbDist.end(); ++it) {
      const stdair::DayDuration_T& lDuration = it->first;
      const stdair::Probability_T& lProbability = it->second;

      lStayDurationProbabilityMassFuction.
        insert (stdair::StayDurationProbabilityMassFunction_T::
                value_type (lDuration, lProbability));
    }

    return stdair::StayDurationProbabilityMass_T (lStayDurationProbabilityMassFuction);
  }

  // ////////////////////////////////////////////////////////////////////
  const stdair::FrequentFlyerProbabilityMass_T DemandStruct_T::
  getFrequentFlyerProbabilityMass () const {
    stdair::FrequentFlyerProbabilityMassFunction_T lFrequentFlyerProbabilityMassFuction;

    for (FFProbDist_T::const_iterator it = _ffProbDist.begin();
         it != _ffProbDist.end(); ++it) {
      const FFCode& lFFCode = it->first;
      const FFCode::EN_FFCode& lENFFCode = lFFCode.getCode();
      const stdair::FrequentFlyer_T lLabel = FFCode::getLabel (lENFFCode);
      const stdair::Probability_T& lProbability = it->second;

      lFrequentFlyerProbabilityMassFuction.
        insert (stdair::FrequentFlyerProbabilityMassFunction_T::
                value_type (lLabel, lProbability));
    }

    return stdair::FrequentFlyerProbabilityMass_T (lFrequentFlyerProbabilityMassFuction);
  }

  // ////////////////////////////////////////////////////////////////////
  const stdair::PreferredDepartureTimeCumulativeDistribution_T DemandStruct_T::
  getPreferredDepartureTimeCumulativeDistribution () const {

    stdair::PreferredDepartureTimeContinuousDistribution_T preferredDepartureTimeContinuousDistribution;

    for (PrefDepTimeProbDist_T::const_iterator it = _prefDepTimeProbDist.begin();
         it != _prefDepTimeProbDist.end(); ++it) {
      const stdair::Duration_T& lTime = it->first;
      const stdair::IntDuration_T lIntDuration = lTime.total_seconds();
      const PrefDepTimeProbMass_T& lPrefDepTimeProbMass = it->second;
      
      preferredDepartureTimeContinuousDistribution.
        insert (stdair::PreferredDepartureTimeContinuousDistribution_T::
                value_type (lIntDuration, lPrefDepTimeProbMass));
    }

    return stdair::PreferredDepartureTimeCumulativeDistribution_T (preferredDepartureTimeContinuousDistribution);
  }

  // ////////////////////////////////////////////////////////////////////
  const stdair::WTPCumulativeDistribution_T DemandStruct_T::
  getWTPCumulativeDistribution () const {

    stdair::WTPContinuousDistribution_T wtpContinuousDistribution;

    for (WTPProbDist_T::const_iterator it = _wtpProbDist.begin();
         it != _wtpProbDist.end(); ++it) {
      const stdair::WTP_T& lWTP = it->first;
      const WTPProbMass_T& lWTPProMass = it->second;

      wtpContinuousDistribution.
        insert (stdair::WTPContinuousDistribution_T::
                value_type (lWTP, lWTPProMass));
    }

    return stdair::WTPCumulativeDistribution_T (wtpContinuousDistribution);
  }

  // ////////////////////////////////////////////////////////////////////
  const stdair::ValueOfTimeCumulativeDistribution_T DemandStruct_T::
  getValueOfTimeCumulativeDistribution () const {

    stdair::ValueOfTimeContinuousDistribution_T valueOfTimeContinuousDistribution;

    for (TimeValueProbDist_T::const_iterator it = _timeValueProbDist.begin();
         it != _timeValueProbDist.end(); ++it) {
      const stdair::PriceValue_T& lValueOfTime = it->first;
      const TimeValueProbMass_T& lValueOfTimeProMass = it->second;

      valueOfTimeContinuousDistribution.
        insert (stdair::ValueOfTimeContinuousDistribution_T::
                value_type (lValueOfTime, lValueOfTimeProMass));
    }

    return stdair::ValueOfTimeCumulativeDistribution_T (valueOfTimeContinuousDistribution);
  }
  
  // ////////////////////////////////////////////////////////////////////
  const std::string DemandStruct_T::describe() const {
    std::ostringstream ostr;
    ostr << _prefDepDate << " -> " << _prefArrDate
         << " " << _origin << "-" << _destination
         << " " << _prefCabin
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
      const stdair::PriceValue_T& lTimeValue = it->first;
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
