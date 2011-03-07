#ifndef __TRADEMGEN_BAS_BASCONST_DEMANDGENERATION_HPP
#define __TRADEMGEN_BAS_BASCONST_DEMANDGENERATION_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <string>
// StdAir
#include <stdair/stdair_maths_types.hpp>
// TraDemGen
#include <trademgen/basic/DemandCharacteristicsTypes.hpp>

namespace TRADEMGEN {

  /** Default PoS probability mass. */
  extern const POSProbabilityMassFunction_T DEFAULT_POS_PROBALILITY_MASS;

  /** Default PoS probability mass. */
  struct DefaultMap {
    static POSProbabilityMassFunction_T create();
  };

  /** Default base generator. Just here to initialise objects
      (e.g., stdair::RandomGeneration) with default generator. They
      are then replaced by a generator, for which the state can better
      be tracked/stored. */
  extern stdair::BaseGenerator_T DEFAULT_BASE_GENERATOR;

  /** Default uniform generator. Just here to initialise objects
      (e.g., stdair::RandomGeneration) with default generator. They
      are then replaced by a generator, for which the state can better
      be tracked/stored. */
  extern stdair::UniformGenerator_T DEFAULT_UNIFORM_GENERATOR;

  /** Default random uniform real distribution. */
  extern const stdair::UniformDistribution_T DEFAULT_UNIFORM_REAL_DISTRIBUTION;

}
#endif // __TRADEMGEN_BAS_BASCONST_DEMANDGENERATION_HPP
