#ifndef __TRADEMGEN_BOM_DEMANDSTREAM_HPP
#define __TRADEMGEN_BOM_DEMANDSTREAM_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// Boost
#include <boost/shared_ptr.hpp>
// STL
#include <iosfwd>
// STDAIR
#include <stdair/STDAIR_Types.hpp>
#include <stdair/basic/DemandCharacteristics.hpp>
#include <stdair/basic/DemandDistribution.hpp>
#include <stdair/basic/RandomGeneration.hpp>
#include <stdair/basic/RandomGenerationContext.hpp>
#include <stdair/bom/StructAbstract.hpp>
#include <stdair/bom/BookingRequestTypes.hpp>

namespace TRADEMGEN {  
  /** Class modeling a demand stream. */
  struct DemandStream  : public stdair::StructAbstract {
    
  public:
    // ///////////// Getters ///////////
    /** Get the key */
    const stdair::DemandStreamKey_T& getKey () const {
      return _key;
    }
    
    /** Get the total number of requests to be generated. */
    const stdair::Count_T& getTotalNumberOfRequestsToBeGenerated() const {
      return _totalNumberOfRequestsToBeGenerated;
    }

  public:
    // ///////////// Setters ///////////
    /** Set the demand characteristics */
    void setKey (const stdair::DemandStreamKey_T& iKey) {
      _key = iKey;
    }

  public:
    /** Give a description of the structure (for display purposes). */
    const std::string describe() const { return ""; }
    
  public:
    // /////////////// Business Methods //////////////
    /** Check whether enough requests have already been generated. */
    const bool stillHavingRequestsToBeGenerated () const;
    
    /** Generate the next request. */
    stdair::BookingRequestPtr_T generateNext ();

  public:
    // ////////// Constructors and destructors /////////
    /** Constructor by default */
    DemandStream (const stdair::DemandStreamKey_T&,
                  const stdair::DemandCharacteristics&,
                  const stdair::DemandDistribution&, const stdair::RandomSeed_T&,
                  const stdair::RandomSeed_T&, const stdair::RandomSeed_T&);
    /** Destructor */
    virtual ~DemandStream ();

    /** Default constructors. */
    DemandStream ();
    DemandStream (const DemandStream&);

    /** Initialization. */
    void init();
  
  private:
    // ////////// Attributes //////////
    
    /** Key */
    stdair::DemandStreamKey_T _key;
    
    /** Demand characteristics */
    stdair::DemandCharacteristics _demandCharacteristics;

    /** Demand distribution */
    stdair::DemandDistribution _demandDistribution;
    
    /** Total number of requests to be generated*/
    stdair::Count_T _totalNumberOfRequestsToBeGenerated;

    /** Random generation context */
    stdair::RandomGenerationContext _randomGenerationContext;
  
    /** Random generator for number of requests */
    stdair::RandomGeneration _numberOfRequestsRandomGenerator;
    
    /** Random generator for request datetime */
    stdair::RandomGeneration _requestDateTimeRandomGenerator;
    
    /** Random generator for demand characteristics */
    stdair::RandomGeneration _demandCharacteristicsRandomGenerator;
    
  };

}
#endif // __TRADEMGEN_BOM_DEMANDSTREAM_HPP
