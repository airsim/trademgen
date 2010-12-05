#ifndef __TRADEMGEN_BAS_BASCONST_DEMANDGENERATION_HPP
#define __TRADEMGEN_BAS_BASCONST_DEMANDGENERATION_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
#include <string>
// TRADEMGEN
#include <trademgen/basic/DemandCharacteristicTypes.hpp>

namespace TRADEMGEN {
  /** Default MAX/MIN WTP ratio. */
  extern const double DEFAULT_MAX_MIN_WTP_RATIO;

  /** Default MAX Advance Purchase. */
  extern const double DEFAULT_MAX_ADVANCE_PURCHASE;

  /** Default PoS probability mass. */
  extern const POSProbabilityMassFunction_T DEFAULT_POS_PROBALILITY_MASS;

  /** Default continuous distribution. */
  extern const ContinuousDistribution_T DEFAULT_CONTINUOUS_DISTRIBUTION;


  /** Helper for default distribution creation. */
  struct DefaultMap {
    
    /** Default PoS probability mass. */
    static POSProbabilityMassFunction_T createPOSProMass () {
      POSProbabilityMassFunction_T oMap;
      oMap["SIN"] = 0.44; oMap["HKG"] = 0.04; oMap["CGK"] = 0.04;
      oMap["SYD"] = 0.04; oMap["BKK"] = 0.04; oMap["LHR"] = 0.03;
      oMap["MEL"] = 0.03; oMap["KUL"] = 0.03; oMap["MNL"] = 0.03;
      oMap["PVG"] = 0.03; oMap["PER"] = 0.02; oMap["BNE"] = 0.02;
      oMap["NRT"] = 0.02; oMap["DPS"] = 0.02; oMap["SGN"] = 0.02;
      oMap["PEN"] = 0.02; oMap["FRA"] = 0.02; oMap["PEK"] = 0.02;
      oMap["HKT"] = 0.02; oMap["AKT"] = 0.02; oMap["SFO"] = 0.01;
      oMap["ICN"] = 0.01; oMap["TPE"] = 0.01; oMap["row"] = 0.02;
      return oMap;
    }

    /** Default continuous distribution. */
    static ContinuousDistribution_T createContinuousDistribution () {
      ContinuousDistribution_T oMap;       
    
      oMap[1.10] = 0.0;   oMap[1.40] = 0.812; oMap[1.45] = 0.833; 
      oMap[1.50] = 0.854; oMap[1.55] = 0.876; oMap[1.60] = 0.897;
      oMap[1.70] = 0.909; oMap[1.80] = 0.918; oMap[2.00] = 0.930; 
      oMap[2.30] = 0.939; oMap[2.60] = 0.952; oMap[3.00] = 0.961;
      oMap[3.30] = 0.973; oMap[3.40] = 0.982; oMap[3.44] = 0.988; 
      oMap[3.47] = 0.994; oMap[3.50] = 1.0; 
    
      // oMap[0.0]  = 0.0;   oMap[0.17] = 0.812; oMap[0.25] = 0.833; 
      // oMap[0.34] = 0.854; oMap[0.43] = 0.876; oMap[0.49] = 0.897;
      // oMap[0.53] = 0.909; oMap[0.61] = 0.918; oMap[0.69] = 0.930; 
      // oMap[0.75] = 0.939; oMap[0.80] = 0.952; oMap[0.85] = 0.961;
      // oMap[0.88] = 0.973; oMap[0.90] = 0.982; oMap[0.95] = 0.988; 
      // oMap[0.98] = 0.994; oMap[1.0] = 1.0;
      
      return oMap;
    }
  };
  
}
#endif // __TRADEMGEN_BAS_BASCONST_DEMANDGENERATION_HPP
