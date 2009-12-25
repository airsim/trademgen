#ifndef __TRADEMGEN_TRADEMGEN_SERVICE_HPP
#define __TRADEMGEN_TRADEMGEN_SERVICE_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <string>
#include <iosfwd>
// Trademgen
#include <trademgen/TRADEMGEN_Types.hpp>
#include <trademgen/DBParams.hpp>

namespace TRADEMGEN {

  // Forward declaration
  class TRADEMGEN_ServiceContext;
  
  /** Trademgen services. */
  class TRADEMGEN_Service {
  public:

    // ////////// Use Cases //////////
    /** Calculate the trademgens. */
    std::string calculateTrademgen ();

    
    // ////////// Constructors and destructors //////////
    /** Constructor.
        @param std::ostream& Output log stream (for instance, std::cout).
        @param const DBParams& The SQL database parameters. */
    TRADEMGEN_Service (std::ostream& ioLogStream, const DBParams&);

    /** Destructor. */
    ~TRADEMGEN_Service();

  private:
    // /////// Construction and Destruction helper methods ///////
    /** Default constructor. */
    TRADEMGEN_Service ();
    /** Default copy constructor. */
    TRADEMGEN_Service (const TRADEMGEN_Service&);

    /** Initialise. */
    void init (std::ostream& ioLogStream, const DBParams&);

    /** Finalise. */
    void finalise ();

    
  private:
    // ///////// Service Context /////////
    /** Trademgen context. */
    TRADEMGEN_ServiceContext* _trademgenServiceContext;
  };

}
#endif // __TRADEMGEN_TRADEMGEN_SERVICE_HPP
