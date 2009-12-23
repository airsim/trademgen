#ifndef __TRADEMGEN_FAC_FACTRADEMGENSERVICECONTEXT_HPP
#define __TRADEMGEN_FAC_FACTRADEMGENSERVICECONTEXT_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <string>
// Trademgen
#include <trademgen/factory/FacServiceAbstract.hpp>

namespace TRADEMGEN {

  /** Forward declarations. */
  class TRADEMGEN_ServiceContext;

  /** Factory for Bucket. */
  class FacTrademgenServiceContext : public FacServiceAbstract {
  public:

    /** Provide the unique instance.
        <br> The singleton is instantiated when first used
        @return FacTrademgenServiceContext& */
    static FacTrademgenServiceContext& instance();

    /** Destructor.
        <br> The Destruction put the _instance to NULL
        in order to be clean for the next
        FacTrademgenServiceContext::instance() */
    ~FacTrademgenServiceContext();

    /** Create a new TRADEMGEN_ServiceContext object.
        <br>This new object is added to the list of instantiated objects.
        @return TRADEMGEN_ServiceContext& The newly created object. */
    TRADEMGEN_ServiceContext& create (const std::string& iServiceName);

    
  protected:
    /** Default Constructor.
        <br>This constructor is protected in order to ensure the singleton
        pattern.*/
    FacTrademgenServiceContext () {}

  private:
    /** The unique instance.*/
    static FacTrademgenServiceContext* _instance;
  };

}
#endif // __TRADEMGEN_FAC_FACTRADEMGENSERVICECONTEXT_HPP
