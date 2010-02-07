#ifndef __TRADEMGEN_TRADEMGEN_SERVICE_HPP
#define __TRADEMGEN_TRADEMGEN_SERVICE_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// StdAir
#include <stdair/STDAIR_Types.hpp>
// Trademgen
#include <trademgen/TRADEMGEN_Types.hpp>

// Forward declarations.
namespace stdair {
  class AirlineFeatureSet;
  class STDAIR_Service;
  struct BasLogParams;
  struct BasDBParams;
  struct BookingRequestStruct;
}

namespace TRADEMGEN {

  // Forward declaration
  class TRADEMGEN_ServiceContext;

  
  /** Trademgen services. */
  class TRADEMGEN_Service {
  public:
    // ////////////////// Constructors and Destructors //////////////////    
    /** Constructor.
        <br>The init() method is called; see the corresponding documentation
        for more details.
        <br>A reference on an output stream is given, so that log
        outputs can be directed onto that stream.
        <br>Moreover, database connection parameters are given, so that a
        session can be created on the corresponding database.
        @param const stdair::BasLogParams& Parameters for the output log stream.
        @param const stdair::BasDBParams& Parameters for the database access.
        @param const stdair::AirlineFeatureSet& Set of airline features.
        @param const stdair::Filename_T& Filename of the input demand file. */
    TRADEMGEN_Service (const stdair::BasLogParams&, const stdair::BasDBParams&,
                       const stdair::AirlineFeatureSet&,
                       const stdair::Filename_T& iDemandInputFilename);

    /** Constructor.
        <br>The init() method is called; see the corresponding documentation
        for more details.
        <br>Moreover, as no reference on any output stream is given,
        neither any database access parameter is given, it is assumed
        that the StdAir log service has already been initialised with
        the proper log output stream by some other methods in the
        calling chain (for instance, when the TRADEMGEN_Service is
        itself being initialised by another library service such as
        SIMCRS_Service).
        @param const stdair::AirlineFeatureSet& Set of airline features.
        @param const stdair::Filename_T& Filename of the input demand file. */
    TRADEMGEN_Service (stdair::STDAIR_ServicePtr_T,
                       const stdair::Filename_T& iDemandInputFilename);
    
    /** Destructor. */
    ~TRADEMGEN_Service();
    

    // ////////////////// Business support methods //////////////////    
    /** Calculate the trademgens. */
    std::string calculateTrademgen ();

    /** Generate a hardcoded booking request. */
    stdair::BookingRequestStruct generateBookingRequest () const;

    
  private:
    // ////////////////// Constructors and Destructors //////////////////    
    /** Default Constructors, which must not be used. */
    TRADEMGEN_Service ();
    /** Default copy constructor. */
    TRADEMGEN_Service (const TRADEMGEN_Service&);

    /** Initialise the (TRADEMGEN) service context (i.e., the
        TRADEMGEN_ServiceContext object). */
    void initServiceContext ();

    /** Initialise the STDAIR service (including the log service).
        <br>A reference on the root of the BOM tree, namely the BomRoot object,
        is stored within the service context for later use.
        @param const stdair::BasLogParams& Parameters for the output log stream.
        @param const stdair::BasDBParams& Parameters for the database access.
        @param const stdair::AirlineFeatureSet& Set of airline features. */
    void initStdAirService (const stdair::BasLogParams&,
                            const stdair::BasDBParams&,
                            const stdair::AirlineFeatureSet&);
    
    /** Initialise.
        <br>The CSV file, describing the airline schedules for the
        simulator, is parsed and the inventories are generated accordingly.
        @param const stdair::AirlineFeatureSet& Set of airline features.
        @param const stdair::Filename_T& Filename of the input demand file. */
    void init (const stdair::Filename_T& iDemandInputFilename);
    
    /** Finalise. */
    void finalise ();

    
  private:
    // ///////// Service Context /////////
    /** Trademgen context. */
    TRADEMGEN_ServiceContext* _trademgenServiceContext;
  };

}
#endif // __TRADEMGEN_TRADEMGEN_SERVICE_HPP
