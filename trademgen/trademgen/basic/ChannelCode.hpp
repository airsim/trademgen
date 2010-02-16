#ifndef __TRADEMGEN_BAS_CHANNELCODE_HPP
#define __TRADEMGEN_BAS_CHANNELCODE_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <string>
// StdAir
#include <stdair/basic/StructAbstract.hpp>

namespace TRADEMGEN {

  /** Enumeration of Frequent Flyer codes. */
  struct ChannelCode : public stdair::StructAbstract {
  public:
    typedef enum { 
      DIRECT_OFFLINE = 0,
      DIRECT_ONLINE,
      INDIRECT_OFFLINE,
      INDIRECT_ONLINE,
      LAST_VALUE
    } EN_ChannelCode;

    /** Get the label as a string. */
    static const std::string& getLabel (const EN_ChannelCode&);

    /** Get the label as a single char. */
    static const std::string& getCodeLabel (const EN_ChannelCode&);

    /** List the labels. */
    static std::string describeLabels();

    /** Get the enumerated value. */
    EN_ChannelCode getCode() const;

    /** Give a description of the structure (for display purposes). */
    const std::string describe() const;

    
  public:
    /** Constructor. */
    ChannelCode (const EN_ChannelCode&);
    /** Constructor. */
    ChannelCode (const std::string& iCode);

    
  private:
    /** String version of the enumeration. */    
    static const std::string _labels[LAST_VALUE];
    /** Code version of the enumeration. */    
    static const std::string _codeLabels[LAST_VALUE];


  private:
    // //////// Attributes /////////
    /** Channel code. */
    EN_ChannelCode _code;
  };

}
#endif // __TRADEMGEN_BAS_ChannelCODE_HPP
