// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <iosfwd>
// TraDemGen
#include <BasTypes.hpp>
#include <RandomGenerationContext.hpp>

namespace TRADEMGEN {

  // //////////////////////////////////////////////////////////////////////
  RandomGenerationContext::RandomGenerationContext ()
    : _cumulativeProbabilitySoFar (0.0),
      _numberOfRequestsGeneratedSoFar (0) {
  }
  
  // //////////////////////////////////////////////////////////////////////
  RandomGenerationContext::~RandomGenerationContext () {
  }
    
  // //////////////////////////////////////////////////////////////////////
  void RandomGenerationContext::incrementGeneratedRequestsCounter () {
    _numberOfRequestsGeneratedSoFar++;
  }

}
