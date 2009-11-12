#ifndef __FORECAST_CMD_SOCISESSIONMANAGER_HPP
#define __FORECAST_CMD_SOCISESSIONMANAGER_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////

// Forward declarations
namespace soci {
  class session;
}

namespace FORECAST {
  
  // Forward declarations
  struct DBParams;

  /** Class handling the SOCI session. */
  class SociSessionManager {
    friend class FORECAST_Service;
  private:
    /** Initialise (MySQL) database connection. */
    static void init (soci::session*&, const DBParams&);
    
    /** Finalise (MySQL) database connection. */
    static void finalise (soci::session*&);

  private:
    /** Constructors. */
    SociSessionManager() {}
    SociSessionManager(const SociSessionManager&) {}
    /** Destructor. */
    ~SociSessionManager() {}
  };

}
#endif // __FORECAST_CMD_SOCISESSIONMANAGER_HPP
