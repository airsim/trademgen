#ifndef __TRADEMGEN_BAS_RANDOM_GENERATION_CONTEXT_HPP
#define __TRADEMGEN_BAS_RANDOM_GENERATION_CONTEXT_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <iosfwd>
// STDAIR
#include <stdair/STDAIR_Types.hpp>

namespace TRADEMGEN {

  /** Structure holding the context necessary for demand random generation. */
  struct RandomGenerationContext {
    
    // ////////// Constructors and destructors /////////
    /** Constructor by default */
    RandomGenerationContext ();
    /** Default constructors. */
    RandomGenerationContext (const RandomGenerationContext&);
    /** Destructor */
    ~RandomGenerationContext ();
    
    // /////////////// Business Methods //////////
    /** Increment counter of requests generated so far */
    void incrementGeneratedRequestsCounter ();

    /** Reset the counters. */
    void reset ();

        // ////////// Attributes //////////
    /** Cumulative probability in arrival pattern for last request
        generated so far (needed for sequential generation)*/
    stdair::Probability_T _cumulativeProbabilitySoFar;
    
    /** Number of requests generated so far */
    stdair::Count_T _numberOfRequestsGeneratedSoFar;
    
  };

}
#endif // __STDAIR_BAS_RANDOM_GENERATION_CONTEXT_HPP
