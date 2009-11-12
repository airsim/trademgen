#ifndef __FORECAST_FAC_FACFORECASTSERVICECONTEXT_HPP
#define __FORECAST_FAC_FACFORECASTSERVICECONTEXT_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <string>
// Forecast
#include <forecast/factory/FacServiceAbstract.hpp>

namespace FORECAST {

  /** Forward declarations. */
  class FORECAST_ServiceContext;

  /** Factory for Bucket. */
  class FacForecastServiceContext : public FacServiceAbstract {
  public:

    /** Provide the unique instance.
        <br> The singleton is instantiated when first used
        @return FacForecastServiceContext& */
    static FacForecastServiceContext& instance();

    /** Destructor.
        <br> The Destruction put the _instance to NULL
        in order to be clean for the next
        FacForecastServiceContext::instance() */
    ~FacForecastServiceContext();

    /** Create a new FORECAST_ServiceContext object.
        <br>This new object is added to the list of instantiated objects.
        @return FORECAST_ServiceContext& The newly created object. */
    FORECAST_ServiceContext& create (const std::string& iServiceName);

    
  protected:
    /** Default Constructor.
        <br>This constructor is protected in order to ensure the singleton
        pattern.*/
    FacForecastServiceContext () {}

  private:
    /** The unique instance.*/
    static FacForecastServiceContext* _instance;
  };

}
#endif // __FORECAST_FAC_FACFORECASTSERVICECONTEXT_HPP
