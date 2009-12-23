// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
// TRADEMGEN Common
#include <trademgen/factory/FacSupervisor.hpp>
#include <trademgen/factory/FacTrademgenServiceContext.hpp>
#include <trademgen/service/TRADEMGEN_ServiceContext.hpp>

namespace TRADEMGEN {

  FacTrademgenServiceContext* FacTrademgenServiceContext::_instance = NULL;

  // //////////////////////////////////////////////////////////////////////
  FacTrademgenServiceContext::~FacTrademgenServiceContext () {
    _instance = NULL;
  }

  // //////////////////////////////////////////////////////////////////////
  FacTrademgenServiceContext& FacTrademgenServiceContext::instance () {

    if (_instance == NULL) {
      _instance = new FacTrademgenServiceContext();
      assert (_instance != NULL);
      
      FacSupervisor::instance().registerServiceFactory (_instance);
    }
    return *_instance;
  }

  // //////////////////////////////////////////////////////////////////////
  TRADEMGEN_ServiceContext& FacTrademgenServiceContext::
  create (const std::string& iServiceName) {
    TRADEMGEN_ServiceContext* aTRADEMGEN_ServiceContext_ptr = NULL;

    aTRADEMGEN_ServiceContext_ptr = new TRADEMGEN_ServiceContext (iServiceName);
    assert (aTRADEMGEN_ServiceContext_ptr != NULL);

    // The new object is added to the Bom pool
    _pool.push_back (aTRADEMGEN_ServiceContext_ptr);

    return *aTRADEMGEN_ServiceContext_ptr;
  }

}
