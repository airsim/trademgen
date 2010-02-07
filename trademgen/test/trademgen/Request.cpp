// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <iosfwd>
// Boost Datetime
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
// TraDemGen
#include <test/trademgen/BasTypes.hpp>
#include <test/trademgen/Request.hpp>

namespace TRADEMGEN {

  // //////////////////////////////////////////////////////////////////////
  Request::Request ()
    : _requestDatetime (boost::posix_time::ptime (boost::gregorian::date (2000,1,1),
                                                  boost::posix_time::time_duration (0,0,0))),
      _origin ("") //,
  //  _tripType (""), _willingnessToPay (0.0) {
  {
  }

  // //////////////////////////////////////////////////////////////////////
  Request::~Request () {
  }

  // //////////////////////////////////////////////////////////////////////
  std::string Request::display() const {
    std::ostringstream oStr;
    
    oStr << "****************** Request ******************" << std::endl;
    oStr << "Demand stream key .... : " << _demandStreamKey << std::endl;
    oStr << "Request datetime ..... : " << _requestDatetime << std::endl; // how can we avoid such harcoding?
    oStr << "Origin ............... : " << _origin << std::endl;

    return oStr.str();
  }
  
}

