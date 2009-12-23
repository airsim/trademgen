#ifndef __TRADEMGEN_SVC_TRADEMGENSERVICECONTEXT_HPP
#define __TRADEMGEN_SVC_TRADEMGENSERVICECONTEXT_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <string>
// Trademgen
#include <trademgen/TRADEMGEN_Types.hpp>
#include <trademgen/service/ServiceAbstract.hpp>

// Forward declarations
namespace soci {
  class session;
}

namespace TRADEMGEN {

  /** Class holding the context of the Trademgen services. */
  class TRADEMGEN_ServiceContext : public ServiceAbstract {
    friend class FacTrademgenServiceContext;
  public:
    // ///////// Getters //////////
    /** Get the SOCI Session. */
    soci::session* getSociSession () const {
      return _sociSession;
    }

    /** Get the SOCI Session. */
    soci::session& getSociSessionHandler () const;

    
    // ///////// Setters //////////
    /** Set the SOCI Session. */
    void setSociSession (soci::session& ioSociSession) {
      _sociSession = &ioSociSession;
    }


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
    /** SOCI session. */
    soci::session* _sociSession;
  };

}
#endif // __TRADEMGEN_SVC_TRADEMGENSERVICECONTEXT_HPP
