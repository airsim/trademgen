#ifndef _TRADEMGEN_CATEGORICALATTRIBUTE_HPP
#define _TRADEMGEN_CATEGORICALATTRIBUTE_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <map>
#include <iosfwd>
// TraDemGen
#include <BasTypes.hpp>

namespace TRADEMGEN {

  /** Class modeling the distribution of values that can be taken by a
      categorical attribute. */
  template <class T>
  class CategoricalAttribute {
  public:
    // ///////////// Getters ///////////
    /** Get the probability mass function. */
    const std::map<T,Probability_T>& getProbabilityMassFunction() const {
      return _probability_mass_function;
    }
  
    /** Get the inverse cumulative distribution. */
    const std::map<Probability_T, T>& getInverseCumulativeDistribution () const {
      return _inverse_cumulative_distribution;
    }

    // ///////////// Setters ///////////
    /** Set the probability mass function */
    void setProbabilityMassFunction (const std::map<T, Probability_T>&);

  public:
    // /////////////// Business Methods //////////
    /** Get value from inverse cumulative distribution. */
    T getValue(Probability_T);
    
  public:
    // ////////////// Display Support Methods //////////
    /** Display inverse cumulative distribution */
    void displayInverseCumulativeDistribution (std::ostream&) const;
    
  public:
    // ////////// Constructors and destructors /////////
    /** Constructor by default */
    CategoricalAttribute (const std::map<T, Probability_T>&);

    /** Destructor */
    virtual ~CategoricalAttribute ();

  private:
    /** Default constructors. */
    CategoricalAttribute ();
    CategoricalAttribute (const CategoricalAttribute&);

    /** Determine inverse cumulative distribution from probability mass function (initialisation). */
    void determineInverseCumulativeDistributionFromProbabilityMassFunction ();
  
  private:
    // ////////// Attributes //////////
    
    /** Probability mass function */
    std::map<T, Probability_T> _probability_mass_function;
    
    /** Inverse cumulative distribution */
    std::map<Probability_T, T> _inverse_cumulative_distribution;
    
  };

  // //////////////////////////////////////////////////////////////////////
  template <class T>
  CategoricalAttribute<T>::CategoricalAttribute (const std::map<T, Probability_T>& iProbabilityMassFunction)
    : _probability_mass_function (iProbabilityMassFunction) {
    determineInverseCumulativeDistributionFromProbabilityMassFunction();
  }
  
  // //////////////////////////////////////////////////////////////////////
  template <class T>
  CategoricalAttribute<T>::~CategoricalAttribute () {}
  
  // //////////////////////////////////////////////////////////////////////
  template <class T>
  void CategoricalAttribute<T>::determineInverseCumulativeDistributionFromProbabilityMassFunction () {
    Probability_T cumulative_probability_so_far = 0.0;
    for (typename std::map<T, Probability_T>::const_iterator itProbabilityMassFunction = _probability_mass_function.begin();
         itProbabilityMassFunction != _probability_mass_function.end();
         ++itProbabilityMassFunction) {
      Probability_T attribute_probability_mass = itProbabilityMassFunction->second;
      if (attribute_probability_mass > 0) {
        T attribute_value = itProbabilityMassFunction->first;
        cumulative_probability_so_far += attribute_probability_mass;
        _inverse_cumulative_distribution[cumulative_probability_so_far] = attribute_value;
      }
    }
  }

  // //////////////////////////////////////////////////////////////////////
  template <class T>
  void CategoricalAttribute<T>::setProbabilityMassFunction (const std::map<T, Probability_T>& iProbabilityMassFunction) {
    _probability_mass_function = iProbabilityMassFunction;
    determineInverseCumulativeDistributionFromProbabilityMassFunction();
  }

  // //////////////////////////////////////////////////////////////////////
  template <class T>
  T CategoricalAttribute<T>::getValue(Probability_T iCumulativeProbability) {
    return _inverse_cumulative_distribution.lower_bound(iCumulativeProbability)->second;
  }
    
  // //////////////////////////////////////////////////////////////////////
  template <class T>
  void CategoricalAttribute<T>::displayInverseCumulativeDistribution (std::ostream& ioStream) const {
    for (typename std::map<Probability_T, T>::const_iterator it = _inverse_cumulative_distribution.begin();
         it != _inverse_cumulative_distribution.end(); ++it) {
      ioStream << "cumulative prob: " << it->first
               << "  value: " << it->second << std::endl;
    }
  }
}
#endif // _TRADEMGEN_CATEGORICALATTRIBUTE_HPP
