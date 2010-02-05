#ifndef _TRADEMGEN_DEMAND_STREAM_HPP
#define _TRADEMGEN_DEMAND_STREAM_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <iosfwd>
// TraDemGen
#include <BasTypes.hpp>
#include <DemandCharacteristics.hpp>
#include <RandomGeneration.hpp>
#include <RandomGenerationContext.hpp>
#include <Request.hpp>

namespace TRADEMGEN {
  
  /** Class modeling a demand stream. */
  class DemandStream {
  public:
    // ///////////// Getters ///////////
    /** Get the key */
    const Key_T& getKey () const {
      return _key;
    }
    
    /** Get the total number of requests to be generated. */
    const Count_T& getTotalNumberOfRequestsToBeGenerated() const {
      return _totalNumberOfRequestsToBeGenerated;
    }
  
    // ///////////// Setters ///////////
    /** Set the demand characteristics */
    void setKey (const Key_T& iKey) {
      _key = iKey;
    }

  public:
    // /////////////// Business Methods //////////
    /** Generate the next request. */
    bool generateNext(Request&);

  public:
    // ////////// Constructors and destructors /////////
    /** Constructor by default */
    DemandStream (const Key_T&, const DemandCharacteristics&, const RandomSeed_T&, const RandomSeed_T&, const RandomSeed_T&);

    /** Destructor */
    virtual ~DemandStream ();

  private:
    /** Default constructors. */
    DemandStream ();
    DemandStream (const DemandStream&);

    /** Initialization. */
    void init();
  
  private:
    // ////////// Attributes //////////
    
    /** Key */
    Key_T _key;
    
    /** Demand characteristics */
    DemandCharacteristics _demandCharacteristics;
    
    /** Total number of requests to be generated*/
    Count_T _totalNumberOfRequestsToBeGenerated;

    /** Random generation context */
    RandomGenerationContext _randomGenerationContext;
  
    /** Random generator for number of requests */
    RandomGeneration _numberOfRequestsRandomGenerator;
    
    /** Random generator for request datetime */
    RandomGeneration _requestDatetimeRandomGenerator;
    
    /** Random generator for demand characteristics */
    RandomGeneration _demandCharacteristicsRandomGenerator;
    
  };

}
#endif // _TRADEMGEN_DEMAND_STREAM_HPP
