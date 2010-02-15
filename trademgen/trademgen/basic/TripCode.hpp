#ifndef __TRADEMGEN_BAS_TRIPCODE_HPP
#define __TRADEMGEN_BAS_TRIPCODE_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <string>
// StdAir
#include <stdair/basic/StructAbstract.hpp>

namespace TRADEMGEN {

  /** Enumeration of Frequent Flyer codes. */
  struct TripCode : public stdair::StructAbstract {
  public:
    typedef enum { 
      OUTBOUND = 0,
      INBOUND,
      ONEWAY,
      LAST_VALUE
    } EN_TripCode;

    /** Get the label as a string. */
    static const std::string& getLabel (const EN_TripCode&);

    /** Get the label as a single char. */
    static const std::string& getCodeLabel (const EN_TripCode&);

    /** List the labels. */
    static std::string describeLabels();

    /** Get the enumerated value. */
    EN_TripCode getCode() const;

    /** Give a description of the structure (for display purposes). */
    const std::string describe() const;

    
  public:
    /** Constructor. */
    TripCode (const EN_TripCode&);
    /** Constructor. */
    TripCode (const std::string& iCode);

    
  private:
    /** String version of the enumeration. */    
    static const std::string _labels[LAST_VALUE];
    /** Code version of the enumeration. */    
    static const std::string _codeLabels[LAST_VALUE];


  private:
    // //////// Attributes /////////
    /** Frequent flyer code. */
    EN_TripCode _code;
  };

}
#endif // __TRADEMGEN_BAS_TripCODE_HPP
