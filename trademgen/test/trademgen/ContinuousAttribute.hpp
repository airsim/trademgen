#ifndef _TRADEMGEN_CONTINUOUSATTRIBUTE_HPP
#define _TRADEMGEN_CONTINUOUSATTRIBUTE_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <string>
#include <map>
// TraDemGen
#include <test/trademgen/BasTypes.hpp>

namespace TRADEMGEN {

  /** Class modeling the distribution of values that can be taken by a
      continuous attribute. */
  template <class T>
  class ContinuousAttribute {
  public:
    // ///////////// Getters ///////////
    /** Get the cumulative distribution. */
    const std::multimap<T,Probability_T>& getCumulativeDistribution() const {
      return _cumulativeDistribution;
    }
  
    /** Get the inverse cumulative distribution. */
    const std::multimap<Probability_T, T>&
    getInverseCumulativeDistribution () const {
      return _inverseCumulativeDistribution;
    }

    // ///////////// Setters ///////////
    /** Set the cumulative distribution */
    void setCumulativeDistribution (const std::multimap<T, Probability_T>&);

  public:
    // /////////////// Business Methods //////////
    /** Get value from inverse cumulative distribution. */
    const T getValue(const Probability_T&) const;
    
  public:
    // ////////////// Display Support Methods //////////
    /** Display cumulative distribution */
    std::string displayCumulativeDistribution() const;
    
    /** Display inverse cumulative distribution */
    std::string displayInverseCumulativeDistribution() const;
    
  public:
    // ////////// Constructors and destructors /////////
    /** Constructor by default */
    ContinuousAttribute ();
    
    /** Constructor */
    ContinuousAttribute (const std::multimap<T, Probability_T>&);
    
    /** Copy constructor */
    ContinuousAttribute (const ContinuousAttribute&);

    /** Destructor */
    virtual ~ContinuousAttribute ();

    /** Determine inverse cumulative distribution from cumulative
        distribution (initialisation). */
    void determineInverseCumulativeDistributionFromCumulativeDistribution ();
  
  private:
    // ////////// Attributes //////////
    
    /** Cumulative distribution */
    std::multimap<T, Probability_T> _cumulativeDistribution;
    
    /** Inverse cumulative distribution */
    std::multimap<Probability_T, T> _inverseCumulativeDistribution;
    
  };

  // //////////////////////////////////////////////////////////////////////
  template <class T>
  ContinuousAttribute<T>::ContinuousAttribute () {
  }
   
  // //////////////////////////////////////////////////////////////////////
  template <class T>
  ContinuousAttribute<T>::
  ContinuousAttribute (const std::multimap<T, Probability_T>& iCumulativeDistribution)
    : _cumulativeDistribution (iCumulativeDistribution) {
    determineInverseCumulativeDistributionFromCumulativeDistribution();
  }
  
  // //////////////////////////////////////////////////////////////////////
  template <class T>
  ContinuousAttribute<T>::
  ContinuousAttribute (const ContinuousAttribute& iContinuousAttribute)
    : _cumulativeDistribution (iContinuousAttribute._cumulativeDistribution),
      _inverseCumulativeDistribution (iContinuousAttribute._inverseCumulativeDistribution) {
  }
  
  // //////////////////////////////////////////////////////////////////////
  template <class T>
  ContinuousAttribute<T>::~ContinuousAttribute () {}
  
  // //////////////////////////////////////////////////////////////////////
  template <class T>
  void ContinuousAttribute<T>::
  determineInverseCumulativeDistributionFromCumulativeDistribution () {
    for (typename std::multimap<T, Probability_T>::iterator itCumulativeDistribution =
           _cumulativeDistribution.begin();
         itCumulativeDistribution != _cumulativeDistribution.end();
         ++itCumulativeDistribution) {
      _inverseCumulativeDistribution.
        insert ( std::pair<float, float> (itCumulativeDistribution->second,
                                          itCumulativeDistribution->first) );
    }
  }

  // //////////////////////////////////////////////////////////////////////
  template <class T>
  void ContinuousAttribute<T>::
  setCumulativeDistribution (const std::multimap<T, Probability_T>& iCumulativeDistribution) {
    _cumulativeDistribution = iCumulativeDistribution;
    determineInverseCumulativeDistributionFromCumulativeDistribution();
  }

  // //////////////////////////////////////////////////////////////////////
  template <class T>
  const T ContinuousAttribute<T>::
  getValue (const Probability_T& iCumulativeProbability) const {
    typename std::multimap<Probability_T, T>::const_iterator it =
      _inverseCumulativeDistribution.lower_bound (iCumulativeProbability);
    
    Probability_T cumulativeProbabilityNextPoint = it->first;
    T valueNextPoint = it->second;
    
    if (it == _inverseCumulativeDistribution.begin()) {
      std::cout << "hello" << std::endl;
      return valueNextPoint;
    }
    --it;
    
    Probability_T cumulativeProbabilityPreviousPoint = it->first;
    T valuePreviousPoint = it->second;
    if (cumulativeProbabilityNextPoint == cumulativeProbabilityPreviousPoint) {
      //std::cout << "hlelo2" << std::endl;
      return valuePreviousPoint;
    }

    //std::cout << "cumulativeProbabilityPreviousPoint: " << cumulativeProbabilityPreviousPoint << std::endl;
    //std::cout << "cumulativeProbabilityNextPoint: " << cumulativeProbabilityNextPoint << std::endl;
    //std::cout << "valuePreviousPoint: " << valuePreviousPoint << std::endl;
    //std::cout << "valueNextPoint: " << valueNextPoint << std::endl;
    
    return valuePreviousPoint + (valueNextPoint - valuePreviousPoint)
      * (iCumulativeProbability - cumulativeProbabilityPreviousPoint)
      / (cumulativeProbabilityNextPoint - cumulativeProbabilityPreviousPoint);
  }
    
  // //////////////////////////////////////////////////////////////////////
  template <class T>
  std::string ContinuousAttribute<T>::displayCumulativeDistribution() const {
    std::ostringstream oStr;
    for (typename std::multimap<T, Probability_T>::const_iterator it =
           _cumulativeDistribution.begin();
         it != _cumulativeDistribution.end(); ++it) {
      oStr << "value: " << it->first
           << "  cumulative probability: " << it->second << std::endl;
    }
    return oStr.str();
  }
  
  // //////////////////////////////////////////////////////////////////////
  template <class T>
  std::string ContinuousAttribute<T>::
  displayInverseCumulativeDistribution () const {
    std::ostringstream oStr;
    for (typename std::multimap<Probability_T, T>::const_iterator it =
           _inverseCumulativeDistribution.begin();
         it != _inverseCumulativeDistribution.end(); ++it) {
      oStr << "cumulative prob: " << it->first
           << "  value: " << it->second << std::endl;
    }
    return oStr.str();
  }
  
}
#endif // _TRADEMGEN_CONTINUOUSATTRIBUTE_HPP
