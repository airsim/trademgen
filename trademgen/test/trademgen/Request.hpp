#ifndef _TRADEMGEN_REQUEST_HPP
#define _TRADEMGEN_REQUEST_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <string>
// TraDemGen
#include <test/trademgen/BasTypes.hpp>

namespace TRADEMGEN {

  /** Structure modeling a (generated) request. */
  struct Request {
  public:
    // ///////////// Getters ///////////
    /** Get the demand stream key */
    const Key_T& getDemandStreamKey() const {
      return _demandStreamKey;
    }
    
    /** Get the request datetime. */
    const Datetime_T& getRequestDatetime() const {
      return _requestDatetime;
    }
  
    // ///////////// Setters ///////////
    /** Set the demand stream key */
    void setDemandStreamKey (const Key_T& iKey) {
      _demandStreamKey = iKey;
    }
    
    /** Set the request datetime */
    void setRequestDatetime (const Datetime_T& iRequestDatetime) {
      _requestDatetime = iRequestDatetime;
    }
    
    /** Set the origin */
    void setOrigin (const LocationCode_T& iOrigin) {
      _origin = iOrigin;
    }
    
    /** Set the preferred departure date */
    void setPreferredDepartureDate (const Date_T& iPreferredDepartureDate) {
      _preferredDepartureDate = iPreferredDepartureDate;
    }
    
    
    /** Set the trip type */
    //void setTripType (const TripType_T& iTripType) {
    //  _tripType = iTripType;
    //}

    /** Set the willingness-to-pay. */
    //void setWillingnessToPay (const MonetaryValue_T& iWTP) {
    //  _willingnessToPay = iWTP;
    //}

  public:
    // ////////////// Display Request //////////
    /** Display request */
    std::string display() const;
    
  public:
    // ////////// Constructors and destructors /////////
    /** Constructor by default */
    Request ();

    /** Destructor */
    virtual ~Request ();

  private:
    /** Default constructors. */
    Request (const Request&);
  
  private:
    // ////////// Attributes //////////
    /** Demand stream key */
    Key_T _demandStreamKey;
    
    /** Request datetime */
    Datetime_T _requestDatetime;
    
    /** Preferred departure date */
    Date_T _preferredDepartureDate;
    
    /** Origin */
    LocationCode_T _origin;
    
    /** Trip type */
    //TripType_T _tripType;
    
    /** Willingness-to-pay */
    //MonetaryValue_T _willingnessToPay;
    
    
  };

}
#endif // _TRADEMGEN_REQUEST_HPP
