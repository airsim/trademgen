// STL
#include <cassert>
#include <iostream>
// TraDemGen
#include "RandomGeneration.hpp"

namespace TRADEMGEN {
  
  // //////////////////////////////////////////////////////////////////////
  RandomGeneration::RandomGeneration (const RandomSeed_T& iSeed)
    : _seed (iSeed), _generator (iSeed),
      _uniformGenerator (_generator, boost::uniform_real<> (0, 1)) {
    init ();
  }

  // //////////////////////////////////////////////////////////////////////
  RandomGeneration::~RandomGeneration () {
  }

  // //////////////////////////////////////////////////////////////////////
  void RandomGeneration::init () {
  }

  // //////////////////////////////////////////////////////////////////////
  double RandomGeneration::generateInstance (const Rate_T& iRate) {
	// double lMean = 1.0 / static_cast<double> (iRate);
	double lVariateExp = _uniformGenerator();
	return lVariateExp;
  }

}
