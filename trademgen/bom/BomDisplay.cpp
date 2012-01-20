// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <ostream>
// StdAir
#include <stdair/basic/BasConst_BomDisplay.hpp>
#include <stdair/bom/BomManager.hpp>
// SEvMgr
#include <sevmgr/SEVMGR_Service.hpp>
#include <sevmgr/SEVMGR_Types.hpp>
// TraDemGen
#include <trademgen/bom/DemandStream.hpp>
#include <trademgen/bom/BomDisplay.hpp>

namespace TRADEMGEN {

  /**
   * Helper singleton structure to store the current formatting flags
   * of any given output stream. The flags are re-set at the
   * structure destruction.
   */
  struct FlagSaver {
  public:
    /** Constructor. */
    FlagSaver (std::ostream& oStream)
      : _oStream (oStream), _streamFlags (oStream.flags()) {
    }

    /** Destructor. */
    ~FlagSaver() {
      // Reset formatting flags of the given output stream
      _oStream.flags (_streamFlags);
    }
    
  private:
    /** Reference on the STL stream, for which the flags must be saved. */
    std::ostream& _oStream;
    /** Saved STL stream flags. */
    std::ios::fmtflags _streamFlags;
  };

  // ////////////////////////////////////////////////////////////////////
  std::string BomDisplay::csvDisplay (const SEVMGR::SEVMGR_ServicePtr_T iSEVMGR_ServicePtr) {
    std::ostringstream oStream;

	//
	assert (iSEVMGR_ServicePtr != NULL);

    /**
     * Bom root level (only)
     */
    oStream << std::endl;
    oStream << "==============================================================="
            << std::endl;
    oStream << "EventQueue: " << iSEVMGR_ServicePtr->describeKey() << std::endl;
    oStream << "==============================================================="
            << std::endl;

    // Check whether there are DemandStream objects
    const bool hasEventGeneratorList =
      iSEVMGR_ServicePtr->hasEventGeneratorList<DemandStream>();
    if (hasEventGeneratorList == false) {
      return oStream.str();
    }
    
    // Retrieve the DemandStream list
    const DemandStreamList_T& lDemandStreamList =
      iSEVMGR_ServicePtr->getEventGeneratorList<DemandStream>();

    // Browse the inventories
    for (DemandStreamList_T::const_iterator itDemandStream =
           lDemandStreamList.begin();
         itDemandStream != lDemandStreamList.end(); ++itDemandStream) {
      DemandStream* lDemandStream_ptr = *itDemandStream;
      assert (lDemandStream_ptr != NULL);

      // Display the demand stream
      csvDisplay (oStream, *lDemandStream_ptr);
    }

    return oStream.str();
  }

  // ////////////////////////////////////////////////////////////////////
  void BomDisplay::csvDisplay (std::ostream& oStream,
                               const DemandStream& iDemandStream) {
    // Save the formatting flags for the given STL output stream
    FlagSaver flagSaver (oStream);

    /**
     * DemandStream level (only)
     */
    oStream << "+++++++++++++++++++++++++++++++++++++++++++++++++" << std::endl;
    oStream << iDemandStream.display();
    oStream << "+++++++++++++++++++++++++++++++++++++++++++++++++" << std::endl;
  }

}
