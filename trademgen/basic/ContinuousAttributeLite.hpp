#ifndef __TRADEMGEN_BAS_CONTINUOUSATTRIBUTELITE_HPP
#define __TRADEMGEN_BAS_CONTINUOUSATTRIBUTELITE_HPP

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
// TraDemGen
#include <trademgen/TRADEMGEN_Exceptions.hpp>
#include <trademgen/basic/DictionaryManager.hpp>

namespace TRADEMGEN {

  /**
   * @brief Class modeling the distribution of values that can be
   * taken by a continuous attribute.
   */
  template <typename T>
  struct ContinuousAttributeLite {
  public:
    // ///////////////////// Type definitions ///////////////////////
    /**
     * Type for the probability mass function.
     */
    typedef std::multimap<T, stdair::Probability_T> ContinuousDistribution_T;
    
  public:
    // ////////////////////// Business Methods ////////////////////
    /**
     * Get value from inverse cumulative distribution.
     */
    const T getValue(const stdair::Probability_T& iCumulativeProbability) const {
      const DictionaryKey_T& lKey =
        DictionaryManager::valueToKey (iCumulativeProbability);

      // Find the first cumulative probablity value greater or equal to lKey.
      unsigned int idx = 0;
      for (; idx < _size; ++idx) {
        if (_cumulativeDistribution[idx] > lKey) {
          break;
        }
      }

      if (idx == 0) {
        return _valueArray[idx];
      }
      if (idx == _size) {
        return _valueArray[idx-1];
      }

      //
      const stdair::Probability_T& lCumulativeCurrentPoint =
        DictionaryManager::keyToValue (_cumulativeDistribution[idx]);
      const T& lValueCurrentPoint = _valueArray[idx];

      //
      const stdair::Probability_T& lCumulativePreviousPoint =
        DictionaryManager::keyToValue (_cumulativeDistribution[idx-1]);
      const T& lValuePreviousPoint = _valueArray[idx-1];

      if (lCumulativePreviousPoint == lCumulativeCurrentPoint) {
        return lValuePreviousPoint;
      }

      T oValue = lValuePreviousPoint + (lValueCurrentPoint - lValuePreviousPoint)
        * (iCumulativeProbability - lCumulativePreviousPoint)
        / (lCumulativeCurrentPoint - lCumulativePreviousPoint);

      return oValue;
    }
    
  public:
    // ////////////// Display Support Methods ////////////////
    /**
     * Display cumulative distribution.
     */
    const std::string displayCumulativeDistribution() const {
      std::ostringstream oStr;
      
      for (unsigned int idx = 0; idx < _size; ++idx) {
        if (idx != 0) {
          oStr << ", ";
        }

        const stdair::Probability_T& lProbability =
          DictionaryManager::keyToValue (_cumulativeDistribution[idx]);
      
        oStr << _valueArray[idx] << ":" << lProbability;
      }
      return oStr.str();
    }


  public:
    // ////////// Constructors and destructors //////////////
    /**
     * Constructor.
     */
    ContinuousAttributeLite (const ContinuousDistribution_T& iValueMap)
      : _size (iValueMap.size()) {
      _cumulativeDistribution = new DictionaryKey_T[_size];
      _valueArray = new T[_size];
      init (iValueMap);
    }
    
    /**
     * Copy constructor.
     */
    ContinuousAttributeLite (const ContinuousAttributeLite& iCAL)
      : _size (iCAL._size),
        _cumulativeDistribution (iCAL._cumulativeDistribution),
        _valueArray (iCAL._valueArray) {
    }

    /**
     * Destructor.
     */
    virtual ~ContinuousAttributeLite() {
      // TODO: Verify that the arrays are correctly cleaned.
      delete[] _cumulativeDistribution; _cumulativeDistribution = NULL;
      delete[] _valueArray; _valueArray = NULL;
    }

  private:
    /**
     * Default constructor.
     */
    ContinuousAttributeLite() : _size(1) {
      assert (false);
    }

    /**
     * Determine inverse cumulative distribution from cumulative
     * distribution (initialisation).
     */
    void init (const ContinuousDistribution_T& iValueMap) {
      unsigned int idx = 0;

      // Browse the map to retrieve the values and cumulative probabilities.
      for (typename ContinuousDistribution_T::const_iterator it =
             iValueMap.begin(); it != iValueMap.end(); ++it, ++idx) {
        
        const T& attributeValue = it->first;
        const DictionaryKey_T& lKey = DictionaryManager::valueToKey (it->second);
        
        // Build the two arrays.
        _cumulativeDistribution[idx] = lKey;
        _valueArray[idx] = attributeValue;
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
     * The corresponding values.
     */
    T* _valueArray;
  };
  
}
#endif // __TRADEMGEN_BAS_CONTINUOUSATTRIBUTELITE_HPP
