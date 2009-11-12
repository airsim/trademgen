// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
// FORECAST Common
#include <forecast/factory/FacSupervisor.hpp>
#include <forecast/factory/FacForecastServiceContext.hpp>
#include <forecast/service/FORECAST_ServiceContext.hpp>

namespace FORECAST {

  FacForecastServiceContext* FacForecastServiceContext::_instance = NULL;

  // //////////////////////////////////////////////////////////////////////
  FacForecastServiceContext::~FacForecastServiceContext () {
    _instance = NULL;
  }

  // //////////////////////////////////////////////////////////////////////
  FacForecastServiceContext& FacForecastServiceContext::instance () {

    if (_instance == NULL) {
      _instance = new FacForecastServiceContext();
      assert (_instance != NULL);
      
      FacSupervisor::instance().registerServiceFactory (_instance);
    }
    return *_instance;
  }

  // //////////////////////////////////////////////////////////////////////
  FORECAST_ServiceContext& FacForecastServiceContext::
  create (const std::string& iServiceName) {
    FORECAST_ServiceContext* aFORECAST_ServiceContext_ptr = NULL;

    aFORECAST_ServiceContext_ptr = new FORECAST_ServiceContext (iServiceName);
    assert (aFORECAST_ServiceContext_ptr != NULL);

    // The new object is added to the Bom pool
    _pool.push_back (aFORECAST_ServiceContext_ptr);

    return *aFORECAST_ServiceContext_ptr;
  }

}
