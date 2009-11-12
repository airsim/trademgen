#ifndef __FORECAST_SVC_FORECASTSERVICECONTEXT_HPP
#define __FORECAST_SVC_FORECASTSERVICECONTEXT_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <string>
// Forecast
#include <forecast/FORECAST_Types.hpp>
#include <forecast/service/ServiceAbstract.hpp>

// Forward declarations
namespace soci {
  class session;
}

namespace FORECAST {

  /** Class holding the context of the Forecast services. */
  class FORECAST_ServiceContext : public ServiceAbstract {
    friend class FacForecastServiceContext;
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
    /** Display the short FORECAST_ServiceContext content. */
    const std::string shortDisplay() const;
    
    /** Display the full FORECAST_ServiceContext content. */
    const std::string display() const;

    
  private:
    // /////// Construction / initialisation ////////
    /** Constructors. */
    FORECAST_ServiceContext ();
    FORECAST_ServiceContext (const std::string&);
    FORECAST_ServiceContext (const FORECAST_ServiceContext&);

    /** Destructor. */
    ~FORECAST_ServiceContext();
      
  private:
    /** SOCI session. */
    soci::session* _sociSession;
  };

}
#endif // __FORECAST_SVC_FORECASTSERVICECONTEXT_HPP
