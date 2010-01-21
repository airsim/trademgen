#ifndef __TRADEMGEN_TRADEMGEN_SERVICE_HPP
#define __TRADEMGEN_TRADEMGEN_SERVICE_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// StdAir
#include <stdair/STDAIR_Types.hpp>
#include <stdair/basic/BasLogParams.hpp>
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
        <br>The init() method is called; see the corresponding documentation
        for more details.
        <br>Moreover, a reference on an output stream is given, so
        that log outputs can be directed onto that stream.       
        @param const stdair::BasLogParams& Parameters for the output log stream.
        @param const DBParams& The SQL database parameters. */
    TRADEMGEN_Service (const stdair::BasLogParams&, const DBParams&);

    /** Constructor.
        <br>The init() method is called; see the corresponding documentation
        for more details.
        <br>Moreover, as no reference on any output stream is given,
        it is assumed that the StdAir log service has already been
        initialised with the proper log output stream by some other
        methods in the calling chain (for instance, when the AIRINV_Service
        is itself being initialised by another library service such as
        DSIM_Service).
        @param const DBParams& The SQL database parameters. */
    TRADEMGEN_Service (const DBParams&);

    /** Destructor. */
    ~TRADEMGEN_Service();

  private:
    // /////// Construction and Destruction helper methods ///////
    /** Default constructor. */
    TRADEMGEN_Service ();
    /** Default copy constructor. */
    TRADEMGEN_Service (const TRADEMGEN_Service&);

    /** Initialise the log. */
    void logInit (const stdair::BasLogParams&);

    /** Initialise.
        @param const DBParams& The SQL database parameters. */
    void init (const DBParams&);

    /** Finalise. */
    void finalise ();

    
  private:
    // ///////// Service Context /////////
    /** Trademgen context. */
    TRADEMGEN_ServiceContext* _trademgenServiceContext;
  };

}
#endif // __TRADEMGEN_TRADEMGEN_SERVICE_HPP
