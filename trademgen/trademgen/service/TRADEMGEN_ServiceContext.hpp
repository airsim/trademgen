#ifndef __TRADEMGEN_SVC_TRADEMGENSERVICECONTEXT_HPP
#define __TRADEMGEN_SVC_TRADEMGENSERVICECONTEXT_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <string>
// Boost
#include <boost/shared_ptr.hpp>
// STDAIR
#include <stdair/STDAIR_Types.hpp>
#include <stdair/bom/BookingRequestTypes.hpp>
// Trademgen
#include <trademgen/TRADEMGEN_Types.hpp>
#include <trademgen/service/ServiceAbstract.hpp>

// Forward declarations.
namespace stdair {
  struct DemandCharacteristics;
  struct DemandDistribution;
}
  
namespace TRADEMGEN {

  /** Class holding the context of the Trademgen services. */
  class TRADEMGEN_ServiceContext : public ServiceAbstract {
    friend class FacTRADEMGENServiceContext;
  public:
    // ///////// Getters //////////
    /** Get the pointer on the STDAIR service handler. */
    stdair::STDAIR_Service& getSTDAIR_Service () const;

  public:
    // ///////// Setters //////////
    /** Set the pointer on the STDAIR service handler. */
    void setSTDAIR_Service (stdair::STDAIR_ServicePtr_T);

  public:
    // ///////// Display Methods //////////
    /** Display the short TRADEMGEN_ServiceContext content. */
    const std::string shortDisplay() const;
    
    /** Display the full TRADEMGEN_ServiceContext content. */
    const std::string display() const;

    
  private:
    // /////// Construction / initialisation ////////
    /** Constructors. */
    TRADEMGEN_ServiceContext ();
    TRADEMGEN_ServiceContext (const std::string&);
    TRADEMGEN_ServiceContext (const TRADEMGEN_ServiceContext&);

    /** Destructor. */
    ~TRADEMGEN_ServiceContext();
      
  private:
    // ///////////// Children ////////////
    /** Standard Airline (StdAir) Service Handler. */
    stdair::STDAIR_ServicePtr_T _stdairService;

  private:
    // ////////////// Attributes ////////////////
    //
  };

}
#endif // __TRADEMGEN_SVC_TRADEMGENSERVICECONTEXT_HPP
