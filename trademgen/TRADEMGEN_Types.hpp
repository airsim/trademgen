#ifndef __TRADEMGEN_TRADEMGEN_TYPES_HPP
#define __TRADEMGEN_TRADEMGEN_TYPES_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// Boost
#include <boost/shared_ptr.hpp>
// StdAir
#include <stdair/stdair_file.hpp>
// TraDemGen
#include <trademgen/TRADEMGEN_Exceptions.hpp>

namespace TRADEMGEN {

  // Forward declarations
  class TRADEMGEN_Service;


  // //////// Type definitions specific to DSim /////////
  /**
   * (Smart) Pointer on the TraDemGen service handler.
   */
  typedef boost::shared_ptr<TRADEMGEN_Service> TRADEMGEN_ServicePtr_T;
  
  // ///////// Files ///////////
  /**
   * Demand input file.
   */
  class DemandFilePath : public stdair::InputFilePath {
  public:
    /**
     * Constructor.
     */
    explicit DemandFilePath (const stdair::Filename_T& iFilename)
      : stdair::InputFilePath (iFilename) {}
  };
  
}
#endif // __TRADEMGEN_TRADEMGEN_TYPES_HPP

