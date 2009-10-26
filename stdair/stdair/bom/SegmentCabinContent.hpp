#ifndef __STDAIR_BOM_SEGMENTCABINCONTENT_HPP
#define __STDAIR_BOM_SEGMENTCABINCONTENT_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STDAIR 
#include <stdair/bom/BomContent.hpp>
#include <stdair/bom/SegmentCabinKey.hpp>

namespace stdair {

  /** Class representing the actual attributes for an airline segment-cabin. */
  class SegmentCabinContent : public BomContent {
  public:
    // Type definitions.
    /** Definition allowing to retrieve the associated BOM key type. */
    typedef SegmentCabinKey_T BomKey_T;

  public:
    // /////////// Getters ///////////// 
    /** Get the segment-cabin key. */
    const BomKey_T& getKey() const {
      return _key;
    }

  public:
    // /////////// Display support methods /////////
    /** Dump a Business Object into an output stream.
        @param ostream& the output stream. */
    virtual void toStream (std::ostream& ioOut) const = 0;

    /** Read a Business Object from an input stream.
        @param istream& the input stream. */
    virtual void fromStream (std::istream& ioIn) = 0;

   /** Get the serialised version of the Business Object. */
    virtual std::string toString() const = 0;
    
    /** Get a string describing the whole key (differentiating two objects
        at any level). */
    virtual const std::string describeKey() const = 0;

    /** Get a string describing the short key (differentiating two objects
        at the same level). */
    virtual const std::string describeShortKey() const = 0;

    
  protected:
    /** Default constructors. */
    SegmentCabinContent (const BomKey_T&);
    SegmentCabinContent (const SegmentCabinContent&);

    /** Destructor. */
    virtual ~SegmentCabinContent();

  protected:
    // Attributes
    /** The key of both structure and content objects. */
    BomKey_T _key;

  };

}
#endif // __STDAIR_BOM_SEGMENTCABINCONTENT_HPP

