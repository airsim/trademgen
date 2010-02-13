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
#include <trademgen/basic/FFCode.hpp>
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

    /** Store the cabin code. */
    struct storeCabin : public ParserSemanticAction {
      /** Actor Constructor. */
      storeCabin (DemandStruct_T&);
      /** Actor Function (functor). */
      void operator() (char iChar) const;
    };

    /** Store the demand mean value. */
    struct storeDemandMean : public ParserSemanticAction {
      /** Actor Constructor. */
      storeDemandMean (DemandStruct_T&);
      /** Actor Function (functor). */
      void operator() (double iReal) const;
    };
  
    /** Store the demand stdandard deviation value. */
    struct storeDemandStdDev : public ParserSemanticAction {
      /** Actor Constructor. */
      storeDemandStdDev (DemandStruct_T&);
      /** Actor Function (functor). */
      void operator() (double iReal) const;
    };
  
    /** Store the ff code. */
    struct storeFFCode : public ParserSemanticAction {
      /** Actor Constructor. */
      storeFFCode (DemandStruct_T&);
      /** Actor Function (functor). */
      void operator() (iterator_t iStr, iterator_t iStrEnd) const;
    };
  
    /** Store the ff probability mass. */
    struct storeFFProbMass : public ParserSemanticAction {
      /** Actor Constructor. */
      storeFFProbMass (DemandStruct_T&);
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
       PrefDepDate; Origin; Destination; Cabin; Mean; StdDev;
       PosDist; ChannelDist; TripTypeDist; StayDurationDist;
       FrequentFlyerDist; WTPDist; PrefDepTimeDist;
       (PrefArrivalDate; PrefArrivalTime;) TimeValueDist; 
       ValueOfTimeDist; ArrivalPatternDist;
       2010-02-08; SIN; BKK; M; 10.0; 1.0;
       SIN:0.7, BKK:0.2, row:0.1; DF:0.1, DN:0.3, IF:0.4, IN:0.2;
       RO:0.6, RI:0.2, OW:0.2; 0:0.1, 1:0.1, 2:0.15, 3:0.15, 4:0.15, 5:0.35;
       P:0.01, G:0.05, S:0.15, M:0.3, N:0.49;
       6:0, 7:0.1, 9:0.3, 17:0.4, 19:0.8, 20:0.95, 22:1;
       100:0, 500:0.8, 2000:1; 15:0, 60:1; 330:0, 40:0.2, 20:0.6, 1:1;
       
      Fixed:
        Prefered departure date (yyyy/mm/dd)
        Origin (3-char airport code)
        Destination (3-char airport code)
        Cabin (1-char cabin code)
      Observable:
        Mean
        StdDev
      Distribution with Probability Masses:
        POS
        Channel (D=direct, I=indirect, N=oNline, F=oFfline)
        Trip type(RO=outbound of round-trip,RI=inbound of round-trip,OW=one way)
        Stay duration (number of days)
        Frequent flyer (P=Platinum, G=Gold, S=Silver, M=Member, N=None)
      Continuous cumulative distribution:
        Preferred departure time (hh:mm:ss)
        WTP (moneraty value)
        Preferred arrival date (equal to Prefered departure date)
        Preferred arrival time (equal to Prefered departure time)
        Value of time
        Arrival pattern (DTD as a positive value)
    The main fields are separated by ';'
    Probability mass distributions are defined by comma-separated
      'value:probability' pairs
    Continuous cumulative distribution are defined by comma-separated
      'value:probability' pairs, sorted in increasing order of values.
    The meaning of probability is P(random variable<=value) = probability.


       Grammar:
       PreferredDepartureDate ::= date
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
          pref_dep_date, date, origin, destination, cabin, demand_params,
          pref_dep_time, time, ff_dist, ff_pair, ff_code, ff_share;

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
    DemandFileParser (stdair::BomRoot&,
                      const stdair::Filename_T& iDemandInputFilename);

    /** Parse the demand input file. */
    bool generateDemand ();
      
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
