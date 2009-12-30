// STL
#include <cassert>
#include <ostream>
// TraDemGen
#include "CategoricalAttribute.hpp"

namespace TRADEMGEN {

  // //////////////////////////////////////////////////////////////////////
  CategoricalAttribute::
  CategoricalAttribute (const ProbabilityMassFunction_T& iProbabilityMassFunction)
    : _probability_mass_function (iProbabilityMassFunction) {
    determineInverseCumulativeDistributionFromProbabilityMassFunction();
  }
	
  // //////////////////////////////////////////////////////////////////////
  CategoricalAttribute::~CategoricalAttribute () {}
	
  // //////////////////////////////////////////////////////////////////////
  void CategoricalAttribute::
  determineInverseCumulativeDistributionFromProbabilityMassFunction () {
    Probability_T cumulative_probability_so_far = 0.0;

    for (ProbabilityMassFunction_T::iterator itProbabilityMassFunction =
           _probability_mass_function.begin();
         itProbabilityMassFunction != _probability_mass_function.end();
         ++itProbabilityMassFunction) {
      Probability_T attribute_probability_mass =
        itProbabilityMassFunction->second;
      
      if (attribute_probability_mass > 0) {
          int attribute_value = itProbabilityMassFunction->first;
          cumulative_probability_so_far += attribute_probability_mass;
          
          _inverse_cumulative_distribution[attribute_probability_mass] =
            attribute_value;
        }
    }
  }

  // //////////////////////////////////////////////////////////////////////
  void CategoricalAttribute::
  setProbabilityMassFunction (const ProbabilityMassFunction_T& iProbabilityMassFunction) {
    _probability_mass_function = iProbabilityMassFunction;
    determineInverseCumulativeDistributionFromProbabilityMassFunction();
  }

  // //////////////////////////////////////////////////////////////////////
  int CategoricalAttribute::
  getValue (Probability_T iCumulativeProbability) const {
    return 0;
  }
    
  // //////////////////////////////////////////////////////////////////////
  void CategoricalAttribute::
  displayInverseCumulativeDistribution (std::ostream& ioStream) const {

    for (InverseCumulativeDistribution_T::const_iterator it =
           _inverse_cumulative_distribution.begin();
         it != _inverse_cumulative_distribution.end(); ++it) {
      ioStream << "cumulative prob: " << it->first
               << "  value: " << it->second << std::endl;
    }
  }
}

