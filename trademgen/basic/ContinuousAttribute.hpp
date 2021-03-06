#ifndef __TRADEMGEN_BAS_CONTINUOUSATTRIBUTE_HPP
#define __TRADEMGEN_BAS_CONTINUOUSATTRIBUTE_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <string>
#include <map>
// StdAir
#include <stdair/stdair_date_time_types.hpp>
#include <stdair/service/Logger.hpp>
// TraDemGen
#include <trademgen/basic/DictionaryManager.hpp>

namespace TRADEMGEN {

  /** Class modeling the distribution of values that can be taken by a
      continuous attribute. */
  template <class T>
  struct ContinuousAttribute {
  public:

    // ///////////// Type definitions //////////////
    /** */
    typedef std::multimap<T, DictionaryKey_T> ContinuousDistribution_T;
    typedef std::multimap<DictionaryKey_T, T> ContinuousInverseDistribution_T;

  private:
    // ///////////// Getters ///////////
    /** Get the cumulative distribution. */
    const ContinuousDistribution_T& getCumulativeDistribution() const {
      return _cumulativeDistribution;
    }
  
    /** Get the inverse cumulative distribution. */
    const ContinuousInverseDistribution_T& getInverseCumulativeDistribution () const {
      return _inverseCumulativeDistribution;
    }

  private:
    // ///////////// Setters ///////////
    /** Set the cumulative distribution */
    void setCumulativeDistribution (const ContinuousDistribution_T& iCumulativeDistribution) {
      _cumulativeDistribution = iCumulativeDistribution;
      determineInverseCumulativeDistributionFromCumulativeDistribution();
    }

  public:
    // /////////////// Business Methods //////////
    /** Get value from inverse cumulative distribution. */
    const T getValue (const stdair::Probability_T& iCumulativeProbability) const{
      const DictionaryKey_T lKey =
        DictionaryManager::valueToKey (iCumulativeProbability);
      typename ContinuousInverseDistribution_T::const_iterator it =
        _inverseCumulativeDistribution.lower_bound (lKey);
    
      stdair::Probability_T cumulativeProbabilityNextPoint =
        DictionaryManager::keyToValue (it->first);
      T valueNextPoint = it->second;
    
      if (it == _inverseCumulativeDistribution.begin()) {
        STDAIR_LOG_DEBUG ("Last element");
        return valueNextPoint;
      }
      --it;
    
      stdair::Probability_T cumulativeProbabilityPreviousPoint =
        DictionaryManager::keyToValue (it->first);
      T valuePreviousPoint = it->second;
      if (cumulativeProbabilityNextPoint == cumulativeProbabilityPreviousPoint) {
        return valuePreviousPoint;
      }

      return valuePreviousPoint + (valueNextPoint - valuePreviousPoint)
        * (iCumulativeProbability - cumulativeProbabilityPreviousPoint)
        / (cumulativeProbabilityNextPoint - cumulativeProbabilityPreviousPoint);
    }
    
  public:
    // ////////////// Display Support Methods //////////
    /** Display cumulative distribution */
    const std::string displayCumulativeDistribution() const {
      std::ostringstream oStr;
      unsigned int idx = 0;
      for (typename ContinuousDistribution_T::const_iterator it =
             _cumulativeDistribution.begin();
           it != _cumulativeDistribution.end(); ++it, ++idx) {
        if (idx != 0) {
          oStr << ", ";
        }
        oStr << it->first << ":"
             << DictionaryManager::keyToValue (it->second);
      }
      return oStr.str();
    }
    
    /** Display inverse cumulative distribution */
    const std::string displayInverseCumulativeDistribution() const {
      std::ostringstream oStr;
      for (typename ContinuousInverseDistribution_T::const_iterator it =
             _inverseCumulativeDistribution.begin();
           it != _inverseCumulativeDistribution.end(); ++it) {
        oStr << "cumulative prob: " << DictionaryManager::keyToValue (it->first)
             << "  value: " << it->second << std::endl;
      }
      return oStr.str();
    }
    
  public:
    // ////////// Constructors and destructors /////////
    /** Constructor by default */
    ContinuousAttribute () { }
    
    /** Constructor */
    ContinuousAttribute (const ContinuousDistribution_T& iCumulativeDistribution)
      : _cumulativeDistribution (iCumulativeDistribution) {
      determineInverseCumulativeDistributionFromCumulativeDistribution();
    }
    
    /** Copy constructor */
    ContinuousAttribute (const ContinuousAttribute& iContinuousAttribute)
      : _cumulativeDistribution (iContinuousAttribute._cumulativeDistribution),
        _inverseCumulativeDistribution (iContinuousAttribute._inverseCumulativeDistribution) {
    }
    
    /** Destructor */
    virtual ~ContinuousAttribute () { }

    /** Determine inverse cumulative distribution from cumulative
        distribution (initialisation). */
    void determineInverseCumulativeDistributionFromCumulativeDistribution () {
      for (typename ContinuousDistribution_T::iterator itCumulativeDistribution =
             _cumulativeDistribution.begin();
           itCumulativeDistribution != _cumulativeDistribution.end();
           ++itCumulativeDistribution) {
        _inverseCumulativeDistribution.
          insert (typename ContinuousInverseDistribution_T::
                  value_type (itCumulativeDistribution->second,
                              itCumulativeDistribution->first));
      }
    }
  
  private:
    // ////////// Attributes //////////
    
    /** Cumulative distribution */
    ContinuousDistribution_T _cumulativeDistribution;
    
    /** Inverse cumulative distribution */
    ContinuousInverseDistribution_T _inverseCumulativeDistribution;
  };
  
}
#endif // __STDAIR_BAS_CONTINUOUSATTRIBUTE_HPP
