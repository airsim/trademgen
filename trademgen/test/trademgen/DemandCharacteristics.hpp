#ifndef _TRADEMGEN_DEMAND_CHARACTERISTICS_HPP
#define _TRADEMGEN_DEMAND_CHARACTERISTICS_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <string>
// TraDemGen
#include <test/trademgen/BasTypes.hpp>
#include <test/trademgen/ContinuousAttribute.hpp>

namespace TRADEMGEN {
  
  /** Forward declarations. */
  //class CategoricalAttributes;
  //class ContinuousAttributes;

  /** Class modeling the characteristics of a demand type. */
  class DemandCharacteristics {
  public:
    // ///////////// Getters ///////////
    
    /** Get the mean number of requests. */
    const RealNumber_T& getMeanNumberOfRequests() const {
      return _meanNumberOfRequests;
    }
    
    /** Get the standard deviation of number of requests. */
    const RealNumber_T& getStandardDeviationNumberOfRequests() const {
      return _standardDeviationNumberOfRequests;
    }
    
    /** Get the origin. */
    const LocationCode_T& getOrigin() const {
      return _origin;
    }
    
    /** Get the preferred departure date. */
    const Date_T& getPreferredDepartureDate() const {
      return _preferredDepartureDate;
    }
    
    /** Get the arrival pattern. */
    const ContinuousAttribute<FloatDuration_T>& getArrivalPattern() const {
      return _arrivalPattern;
    }
  
    // ///////////// Setters ///////////
    
    /** Set the mean number of requests. */
    void setMeanNumberOfRequests (const RealNumber_T& iMean) {
      _meanNumberOfRequests = iMean;
    }
    
    /** Set the standard deviation of number of requests. */
    void setStandardDeviationNumberOfRequests (const RealNumber_T& iStandardDeviation) {
      _standardDeviationNumberOfRequests = iStandardDeviation;
    }
    
    /** Set the origin. */
    void setOrigin (const LocationCode_T& iOrigin) {
      _origin = iOrigin;
    }
    
    /** Set the preferred departure date. */
    void setPreferredDepartureDate (const Date_T& iPreferredDepartureDate) {
      _preferredDepartureDate = iPreferredDepartureDate;
    }
    
    /** Set the arrival pattern. */
    void setArrivalPattern (const ContinuousAttribute<FloatDuration_T>& iArrivalPattern) {
      _arrivalPattern = iArrivalPattern;
    }

  public:
    // ////////////// Display Support Methods //////////
    /** Display demand characteristics */
    std::string display() const;

    
  public:
    // ////////// Constructors and destructors /////////
    /** Constructor by default */
    DemandCharacteristics ();
    
    /** Copy constructor */
    DemandCharacteristics (const DemandCharacteristics&); // Should be private, but well...

    /** Destructor */
    virtual ~DemandCharacteristics ();
  
  
  private:
    // ////////// Attributes //////////
    
    /** Mean number of requests */
    RealNumber_T _meanNumberOfRequests;
    
    /** Standard deviation of number of requests */
    RealNumber_T _standardDeviationNumberOfRequests;
    
    /** Origin */
    LocationCode_T _origin;
    
    /** Preferred departure date */
    Date_T _preferredDepartureDate;
    
    /** Arrival pattern (cumulative distribution of timing of arrival of requests (negative number of days between departure date and request date)*/
    ContinuousAttribute<FloatDuration_T> _arrivalPattern;
    
    /** Trip type probability mass */
    //CategoricalAttribute<TripType_T> _tripTypeProbabilityMass;
    
    /** Willingness-to-pay cumulative distribution */
    //ContinuousAttribute<MonetaryValue_T> _willingnessToPayCumulativeDistribution;
    
  };

}
#endif // _TRADEMGEN_DEMAND_CHARACTERISTICS_HPP
