#ifndef __TRADEMGEN_BAS_CATEGORICALATTRIBUTELITE_HPP
#define __TRADEMGEN_BAS_CATEGORICALATTRIBUTELITE_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <iosfwd>
#include <string>
#include <map>
// StdAir
#include <stdair/stdair_basic_types.hpp>
#include <stdair/service/Logger.hpp>
// TraDemGen
#include <trademgen/TRADEMGEN_Exceptions.hpp>
#include <trademgen/basic/DictionaryManager.hpp>

namespace TRADEMGEN {

  /**
   * @brief Class modeling the distribution of values that can be
   * taken by a categorical attribute.
   */
  template <typename T>
  struct CategoricalAttributeLite {
    public:
    // ///////////// Type definitions //////////////
    /**
     * Type for the probability mass function.
     */
    typedef std::map<T, stdair::Probability_T> ProbabilityMassFunction_T;
    

  public:
    // /////////////// Business Methods //////////
    /**
     * Get value from inverse cumulative distribution.
     */
    const T& getValue (const stdair::Probability_T& iCumulativeProbability) const {
      const DictionaryKey_T& lKey =
        DictionaryManager::valueToKey (iCumulativeProbability);

      for (unsigned int idx = 0; idx < _size; ++idx) {
        if (_cumulativeDistribution[idx] >= lKey) {
          const T& oValue = _valueArray[idx];
          return oValue;
        }
      }

      std::ostringstream oStr;
      oStr << "The following cumulative probability is out of range: "
           << iCumulativeProbability << displayProbabilityMass();
      throw IndexOutOfRangeException (oStr.str());
    }

    /**
     * Check if a value belongs to the value list.
     */
    bool checkValue (const T& iValue) const {
      for (unsigned int idx = 0; idx < _size; ++idx) {
        if (_valueArray[idx] == iValue) {
          return true;
        }
      }
      return false;
    }
    

  public:
    // ////////////// Display Support Methods //////////
    /**
     * Display probability mass function.
     */
    const std::string displayProbabilityMass() const {
      std::ostringstream oStr;

      for (unsigned int idx = 0; idx < _size; ++idx) {
        if (idx != 0) {
          oStr << ", ";
        }
        oStr << _valueArray[idx] << ":"
             << DictionaryManager::keyToValue (_cumulativeDistribution[idx]);
      }
      return oStr.str();
    }
        

  public:
    // ////////// Constructors and destructors /////////
    /**
     * Main constructor.
     */
    CategoricalAttributeLite (const ProbabilityMassFunction_T& iValueMap)
      : _size (iValueMap.size()) {
      _cumulativeDistribution = new DictionaryKey_T[_size];
      _valueArray = new T[_size];
      init (iValueMap);
    }

    /**
     * Copy constructor.
     */
    CategoricalAttributeLite (const CategoricalAttributeLite& iCAL)
      : _size (iCAL._size),
        _cumulativeDistribution (iCAL._cumulativeDistribution),
        _valueArray (iCAL._valueArray) {
    }

    /**
     * Destructor.
     */
    virtual ~CategoricalAttributeLite() {
      // TODO: Verify that the arrays are correctly cleaned.
      delete[] _cumulativeDistribution; _cumulativeDistribution = NULL;
      delete[] _valueArray; _valueArray = NULL;
    }


  private:
    /**
     * Default constructor.
     */
    CategoricalAttributeLite() : _size(1) {
      assert (false);
    }


    /**
     * Initialise the two arrays from the given map.
     */
    void init (const ProbabilityMassFunction_T& iValueMap) {
      
      stdair::Probability_T cumulative_probability_so_far = 0.0;
      unsigned int idx = 0;

      // Browse the map to retrieve the values and to build the
      // cumulative probabilities.
      for (typename ProbabilityMassFunction_T::const_iterator
             itProbabilityMassFunction = iValueMap.begin();
           itProbabilityMassFunction != iValueMap.end();
           ++itProbabilityMassFunction, ++idx) {
        
        stdair::Probability_T attribute_probability_mass =
          itProbabilityMassFunction->second;

        if (attribute_probability_mass > 0) {
          const T& attribute_value = itProbabilityMassFunction->first;
          cumulative_probability_so_far += attribute_probability_mass;
          const DictionaryKey_T& lKey =
            DictionaryManager::valueToKey (cumulative_probability_so_far);

          // Build the two arrays.
          _cumulativeDistribution[idx] = lKey;
          _valueArray[idx] = attribute_value;
        }
      }
    }
  
  private:
    // ////////// Attributes //////////
    /**
     * Size of the two arrays.
     */
    const unsigned int _size;
    
    /**
     * Cumulative dictionary-coded distribution.
     */
    DictionaryKey_T* _cumulativeDistribution;

    /**
       The corresponding values.
    */
    T* _valueArray;
  };
}
#endif // __TRADEMGEN_BAS_CATEGORICALATTRIBUTELITE_HPP
