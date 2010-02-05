// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <iosfwd>
// Boost Datetime
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
// TraDemGen
#include <Request.hpp>
#include <BasTypes.hpp>

namespace TRADEMGEN {

  // //////////////////////////////////////////////////////////////////////
  Request::Request ()
    : _requestDatetime (boost::posix_time::ptime(
                                  boost::gregorian::date (2000,1,1),
                                  boost::posix_time::time_duration (0,0,0)
                                  )),
      _origin ("") //,
  //  _tripType (""), _willingnessToPay (0.0) {
  {
  }

  // //////////////////////////////////////////////////////////////////////
  Request::~Request () {
  }

  // //////////////////////////////////////////////////////////////////////
  void Request::display (std::ostream& ioStream) const {
    ioStream << "****************** Request ******************" << std::endl;
    ioStream << "Demand stream key .... : " << _demandStreamKey << std::endl;
    ioStream << "Request datetime ..... : " << _requestDatetime << std::endl; // how can we avoid such harcoding?
    ioStream << "Origin ............... : " << _origin << std::endl;
  }
  
}

