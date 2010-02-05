#ifndef __TRADEMGEN_BOM_RANDOMGENERATION_HPP
#define __TRADEMGEN_BOM_RANDOMGENERATION_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// Boost Random
#include <boost/random/uniform_real.hpp>
#include <boost/random/variate_generator.hpp>
// TraDemGen
#include <BasTypes.hpp>

namespace TRADEMGEN {

  /** Random generator. */
  class RandomGeneration {
  public:
    // ///////////// Getters ///////////
    /** Get the seed of the random generator. */
    const RandomSeed_T& getSeed () const {
      return _seed;
    }
    
    // //////////// Business Methods /////////////
    /** Generate a randomized number following a uniform distribution between 0 (included) and 1 (excluded). */
    RealNumber_T generateUniform01 ();
    
    /** Generate a randomized number following a uniform distribution between a minimum (included) and a maximum (excluded) value. */
    RealNumber_T generateUniform (const RealNumber_T&, const RealNumber_T&);
    
    /** Generate a randomized number following a normal distribution specified by a mean and a standard deviation. */
    RealNumber_T generateNormal (const RealNumber_T&, const RealNumber_T&);
    
  public:
    // ////////// Constructors and destructors /////////
    /** Constructor. */
    RandomGeneration (const RandomSeed_T&);
  
    /** Destructor. */
    virtual ~RandomGeneration ();
  
  private:
    /** Default constructors. */
    RandomGeneration ();
    RandomGeneration (const RandomGeneration&);

    /** Initialize the random generator.
        <br>A uniform random number distribution is defined, which
        produces "real" values between 0 and 1 (0 inclusive, 1
        exclusive). */
    void init ();


  private:
    // ////////// Attributes //////////
    /** The seed of the random generator.
        <br>The seed is unsigned, otherwise the wrong overload may be
        selected when using mt19937 as the boost::base_generator_type. */
    RandomSeed_T _seed;

    /** Random number generator engine.
        <br>The random number generator is currently based on boost::minstd_rand.
        Alternates are boost::mt19937, boost::ecuyer1988. */
    BaseGenerator_T _generator;
    
    /** Random number generator.
        <br>It is initialized with a reproducible seed and a uniform
        distribution. */
    boost::variate_generator<BaseGenerator_T&,
                             boost::uniform_real<> > _uniformGenerator;
  };

}
#endif // __TRADEMGEN_BOM_RANDOMGENERATION_HPP
