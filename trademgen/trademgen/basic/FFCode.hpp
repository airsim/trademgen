#ifndef __TRADEMGEN_BAS_FFCODE_HPP
#define __TRADEMGEN_BAS_FFCODE_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <string>
// StdAir
#include <stdair/basic/StructAbstract.hpp>

namespace TRADEMGEN {

  /** Enumeration of Frequent Flyer codes. */
  struct FFCode : public stdair::StructAbstract {
  public:
    typedef enum { 
      PLATINUM = 0,
      GOLD,
      SILVER,
      MEMBER,
      NONE,
      LAST_VALUE
    } EN_FFCode;

    /** Get the label as a string. */
    static const std::string& getLabel (const EN_FFCode&);

    /** Get the label as a single char. */
    static char getCodeLabel (const EN_FFCode&);

    /** List the labels. */
    static std::string describeLabels();

    /** Get the enumerated value. */
    EN_FFCode getCode() const;

    /** Give a description of the structure (for display purposes). */
    const std::string describe() const;

  public:
    /** Constructor. */
    FFCode (const EN_FFCode&);
    /** Constructor. */
    FFCode (const char iChar);

    
  private:
    /** String version of the enumeration. */    
    static const std::string _labels[LAST_VALUE];
    /** Code version of the enumeration. */    
    static const char _codeLabels[LAST_VALUE];


  private:
    // //////// Attributes /////////
    /** Frequent flyer code. */
    EN_FFCode _code;
  };

}
#endif // __TRADEMGEN_BAS_FFCODE_HPP
