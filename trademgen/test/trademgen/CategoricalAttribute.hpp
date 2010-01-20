#ifndef _TRADEMGEN_CATEGORICALATTRIBUTE_HPP
#define _TRADEMGEN_CATEGORICALATTRIBUTE_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <iosfwd>
// TraDemGen
#include "BasTypes.hpp"

namespace TRADEMGEN {

  /** Class modeling the distribution of values that can be taken by a
      categorical attribute. */
  class CategoricalAttribute {
  public:
    // ///////////// Getters ///////////
    /** Get the probability mass function. */
    const ProbabilityMassFunction_T& getProbabilityMassFunction() const {
      return _probability_mass_function;
    }
	
    /** Get the inverse cumulative distribution. */
    const InverseCumulativeDistribution_T& getInverseCumulativeDistribution () const {
      return _inverse_cumulative_distribution;
    }

    // ///////////// Setters ///////////
    /** Set the probability mass function */
    void setProbabilityMassFunction (const ProbabilityMassFunction_T&);

  public:
    // /////////////// Business Methods //////////
    /** Get value from inverse cumulative distribution. */
    int getValue (Probability_T) const;
    
  public:
    // ////////////// Display Support Methods //////////
    /** Display inverse cumulative distribution */
    void displayInverseCumulativeDistribution (std::ostream&) const;
    
  public:
    // ////////// Constructors and destructors /////////
    /** Constructor by default */
    CategoricalAttribute (const ProbabilityMassFunction_T&);

    /** Destructor */
    virtual ~CategoricalAttribute ();

  private:
    /** Default constructors. */
    CategoricalAttribute ();
    CategoricalAttribute (const CategoricalAttribute&);

    /** Determine inverse cumulative distribution from probability
        mass function (initialisation). */
    void determineInverseCumulativeDistributionFromProbabilityMassFunction ();
  
  private:
    // ////////// Attributes //////////
		
    /** Probability mass function */
    ProbabilityMassFunction_T _probability_mass_function;
		
    /** Inverse cumulative distribution */
    InverseCumulativeDistribution_T _inverse_cumulative_distribution;
		
  };

}
#endif // _TRADEMGEN_CATEGORICALATTRIBUTE_HPP