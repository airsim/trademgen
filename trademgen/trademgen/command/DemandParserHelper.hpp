#ifndef __TRADEMGEN_CMD_DEMANDPARSERHELPER_HPP
#define __TRADEMGEN_CMD_DEMANDPARSERHELPER_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <string>
// StdAir
#include <stdair/command/CmdAbstract.hpp>
// TraDemGen
#include <trademgen/TRADEMGEN_Types.hpp>
#include <trademgen/basic/BasParserTypes.hpp>
#include <trademgen/bom/DemandStruct.hpp>

// Forward declarations
namespace stdair {
  class BomRoot;
}

namespace TRADEMGEN {

  namespace DemandParserHelper {
    
    // ///////////////////////////////////////////////////////////////////
    //  Semantic actions
    // ///////////////////////////////////////////////////////////////////
    /** Generic Semantic Action (Actor / Functor) for the Demand Parser. */
    struct ParserSemanticAction {
      /** Actor Constructor. */
      ParserSemanticAction (DemandStruct_T&);
      /** Actor Context. */
      DemandStruct_T& _demand;
    };
      
    /** Store the preferred departure date. */
    struct storePreferredDepartureDate : public ParserSemanticAction {
      /** Actor Constructor. */
      storePreferredDepartureDate (DemandStruct_T&);
      /** Actor Function (functor). */
      void operator() (iterator_t iStr, iterator_t iStrEnd) const;
    };

    /** Store the origin. */
    struct storeOrigin : public ParserSemanticAction {
      /** Actor Constructor. */
      storeOrigin (DemandStruct_T&);
      /** Actor Function (functor). */
      void operator() (iterator_t iStr, iterator_t iStrEnd) const;
    };
  
    /** Store the destination. */
    struct storeDestination : public ParserSemanticAction {
      /** Actor Constructor. */
      storeDestination (DemandStruct_T&);
      /** Actor Function (functor). */
      void operator() (iterator_t iStr, iterator_t iStrEnd) const;
    };

    /** Store the preferred departure time. */
    struct storePreferredDepartureTime : public ParserSemanticAction {
      /** Actor Constructor. */
      storePreferredDepartureTime (DemandStruct_T&);
      /** Actor Function (functor). */
      void operator() (iterator_t iStr, iterator_t iStrEnd) const;
    };

    /** Store the ff code. */
    struct storeFfCode : public ParserSemanticAction {
      /** Actor Constructor. */
      storeFfCode (DemandStruct_T&);
      /** Actor Function (functor). */
      void operator() (char iChar) const;
    };
  
    /** Store the ff probability mass. */
    struct storeFfProbMass : public ParserSemanticAction {
      /** Actor Constructor. */
      storeFfProbMass (DemandStruct_T&);
      /** Actor Function (functor). */
      void operator() (double iReal) const;
    };
  
    /** Mark the end of the demand parsing. */
    struct doEndDemand : public ParserSemanticAction {
      /** Actor Constructor. */
      doEndDemand (stdair::BomRoot&, DemandStruct_T&);
      /** Actor Function (functor). */
      void operator() (iterator_t iStr, iterator_t iStrEnd) const;
      /** Actor Specific Context. */
      stdair::BomRoot& _bomRoot;
    };
  

    /////////////////////////////////////////////////////////////////////////
    //
    //  (Boost Spirit) Grammar Definition
    //
    /////////////////////////////////////////////////////////////////////////
    /**
       PrefDepDate; Origin; Destination; PosDist; ChannelDist; TripTypeDist;
       StayDurationDist; FrequentFlyerDist; PrefDateTimeDist; WTPDist;
       ValueOfTimeDist; ArrivalPatternDist;
       2010/02/08;SIN;BKK;M;SIN:0...
       
       Grammar:
       DOW                 ::= int
       FlightKey           ::= AirlineCode ';' FlightNumber
       ';' PreferredDepartureDate ';' DateRangeEnd ';' DOW
       LegKey              ::= BoardingPoint ';' OffPoint
       LegDetails          ::= PreferredDepartureTime ['/' BoardingDateOffSet]
       ';' OffTime ['/' BoardingDateOffSet]
       ';' Elapsed
       LegCabinDetails     ::= CabinCode ';' FfProbMass
       Leg                 ::= LegKey ';' LegDetails (';' CabinDetails)+
       SegmentKey          ::= BoardingPoint ';' OffPoint
       SegmentCabinDetails ::= CabinCode ';' Classes
       (';' FamilyCabinDetails)*
       FamilyCabinDetails  ::= CabinCode ';' Classes
       FullSegmentCabinDetails::= (';' SegmentCabinDetails)+
       GeneralSegments     ::= '0' (';' SegmentCabinDetails)+
       SpecificSegments    ::= '1' (';' SegmentKey
       ';' FullSegmentCabinDetails)+
       Segment             ::= GeneralSegment | SpecificSegment
       Demand        ::= FlightKey (';' Leg)+
       (';' Segment)+ ';' EndOfFlight
       EndOfFlight         ::= ';'
    */

    /** Grammar for the Flight-Period parser. */
    struct DemandParser : 
      public boost::spirit::classic::grammar<DemandParser> {

      DemandParser (stdair::BomRoot&, DemandStruct_T&);

      template <typename ScannerT>
      struct definition {
        definition (DemandParser const& self);
        
        // Instantiation of rules
        boost::spirit::classic::rule<ScannerT> demand_list, demand, demand_end,
          pref_dep_date, date, origin, destination, pref_dep_time, time,
          ff_dist, ff_pair, ff_code, ff_share;

        /** Entry point of the parser. */
        boost::spirit::classic::rule<ScannerT> const& start() const;
      };

      // Parser Context
      stdair::BomRoot& _bomRoot;
      DemandStruct_T& _demand;
    };

  }


  /////////////////////////////////////////////////////////////////////////
  //
  //  Entry class for the file parser
  //
  /////////////////////////////////////////////////////////////////////////
  /** Class wrapping the initialisation and entry point of the parser.
      <br>The seemingly redundancy is used to force the instantiation of
      the actual parser, which is a templatised Boost Spirit grammar.
      Hence, the actual parser is instantiated within that class object
      code. */
  class DemandFileParser : public stdair::CmdAbstract {
  public:
    /** Constructor. */
    DemandFileParser (stdair::BomRoot&, const stdair::Filename_T& iDemandInputFilename);

    /** Parse the demand input file. */
    bool generateInventories ();
      
  private:
    /** Initialise. */
    void init();
      
  private:
    // Attributes
    /** File-name of the CSV-formatted demand input file. */
    stdair::Filename_T _filename;

    /** Start iterator for the parser. */
    iterator_t _startIterator;
      
    /** End iterator for the parser. */
    iterator_t _endIterator;
      
    /** stdair::BomRoot. */
    stdair::BomRoot& _bomRoot;

    /** Flight-Period Structure. */
    DemandStruct_T _demand;
  };
    
}
#endif // __TRADEMGEN_CMD_DEMANDPARSERHELPER_HPP
