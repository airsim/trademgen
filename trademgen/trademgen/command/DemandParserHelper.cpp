// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
// StdAir
#include <stdair/bom/BomRoot.hpp>
#include <stdair/service/Logger.hpp>
// TraDemGen
#include <trademgen/command/DemandParserHelper.hpp>
//#include <trademgen/command/DemandGenerator.hpp>

namespace TRADEMGEN {

  namespace DemandParserHelper {
      
    // //////////////////////////////////////////////////////////////////
    //  Semantic actions
    // //////////////////////////////////////////////////////////////////

    ParserSemanticAction::
    ParserSemanticAction (DemandStruct_T& ioDemand)
      : _demand (ioDemand) {
    }      

    // //////////////////////////////////////////////////////////////////
    storePreferredDepartureDate::
    storePreferredDepartureDate (DemandStruct_T& ioDemand)
      : ParserSemanticAction (ioDemand) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storePreferredDepartureDate::operator() (iterator_t iStr,
                                                  iterator_t iStrEnd) const {
      _demand._prefDepDate = _demand.getDate();
        
      // Reset the number of seconds
      _demand._itSeconds = 0;
    }
      
    // //////////////////////////////////////////////////////////////////
    storeOrigin::storeOrigin (DemandStruct_T& ioDemand)
      : ParserSemanticAction (ioDemand) {
    }

    // //////////////////////////////////////////////////////////////////
    void storeOrigin::operator() (iterator_t iStr, iterator_t iStrEnd) const {
      stdair::AirportCode_T lOrigin (iStr, iStrEnd);
      _demand._origin = lOrigin;
    }

    // //////////////////////////////////////////////////////////////////
    storeDestination::storeDestination (DemandStruct_T& ioDemand)
      : ParserSemanticAction (ioDemand) {
    }

    // //////////////////////////////////////////////////////////////////
    void storeDestination::operator() (iterator_t iStr,
                                       iterator_t iStrEnd) const {
      stdair::AirportCode_T lDestination (iStr, iStrEnd);
      _demand._destination = lDestination;
    }

    // //////////////////////////////////////////////////////////////////
    storeCabin::storeCabin (DemandStruct_T& ioDemand)
      : ParserSemanticAction (ioDemand) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storeCabin::operator() (char iChar) const { 
      _demand._cabinCode = iChar; 
      //std::cout << "Cabin code: " << iChar << std::endl;
    }
    
    // //////////////////////////////////////////////////////////////////
    storeDemandMean::storeDemandMean (DemandStruct_T& ioDemand)
      : ParserSemanticAction (ioDemand) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storeDemandMean::operator() (double iReal) const { 
      _demand._demandMean = iReal; 
      //std::cout << "Demand mean: " << iReal << std::endl;
    }

    // //////////////////////////////////////////////////////////////////
    storeDemandStdDev::storeDemandStdDev (DemandStruct_T& ioDemand)
      : ParserSemanticAction (ioDemand) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storeDemandStdDev::operator() (double iReal) const { 
      _demand._demandStdDev = iReal; 
      //std::cout << "Demand stddev: " << iReal << std::endl;
    }

    // //////////////////////////////////////////////////////////////////
    storePosCode::storePosCode (DemandStruct_T& ioDemand)
      : ParserSemanticAction (ioDemand) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storePosCode::operator() (iterator_t iStr, iterator_t iStrEnd) const {
      const stdair::AirportCode_T lPosCode (iStr, iStrEnd);
      _demand._itPosCode = lPosCode;
      //std::cout << "Pos code: " << lPosCode << std::endl;
    }

    // //////////////////////////////////////////////////////////////////
    storePosProbMass::storePosProbMass (DemandStruct_T& ioDemand)
      : ParserSemanticAction (ioDemand) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storePosProbMass::operator() (double iReal) const {
      const bool hasInsertBeenSuccessfull = 
        _demand._posProbDist.
        insert (PosProbDist_T::value_type (_demand._itPosCode, iReal)).second;
      if (hasInsertBeenSuccessfull == false) {
        STDAIR_LOG_ERROR ("The same POS code ('" << _demand._itPosCode
                          << "') has probably been given twice");
        throw CodeDuplicationException();
      }
      
      //std::cout << "PosProbMass: " << iReal << std::endl;
    }

    // //////////////////////////////////////////////////////////////////
    storeChannelCode::storeChannelCode (DemandStruct_T& ioDemand)
      : ParserSemanticAction (ioDemand) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storeChannelCode::operator() (iterator_t iStr,
                                       iterator_t iStrEnd) const {
      const std::string lChannelCodeStr (iStr, iStrEnd);
      const ChannelCode lChannelCode (lChannelCodeStr);
      _demand._itChannelCode = lChannelCode.getCode();
      //std::cout << "Channel code: " << lChannelCode << std::endl;
    }

    // //////////////////////////////////////////////////////////////////
    storeChannelProbMass::storeChannelProbMass (DemandStruct_T& ioDemand)
      : ParserSemanticAction (ioDemand) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storeChannelProbMass::operator() (double iReal) const {
      const bool hasInsertBeenSuccessfull = 
        _demand._channelProbDist.
        insert (ChannelProbDist_T::value_type (_demand._itChannelCode,
                                               iReal)).second;
      if (hasInsertBeenSuccessfull == false) {
        STDAIR_LOG_ERROR ("The same channel type code ('"
                          << ChannelCode (_demand._itChannelCode)
                          << "') has probably been given twice");
        throw CodeDuplicationException();
      }
      
      //std::cout << "ChannelProbMass: " << iReal << std::endl;
    }

    // //////////////////////////////////////////////////////////////////
    storeTripCode::storeTripCode (DemandStruct_T& ioDemand)
      : ParserSemanticAction (ioDemand) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storeTripCode::operator() (iterator_t iStr,
                                       iterator_t iStrEnd) const {
      const std::string lTripCodeStr (iStr, iStrEnd);
      const TripCode lTripCode (lTripCodeStr);
      _demand._itTripCode = lTripCode.getCode();
      //std::cout << "Trip code: " << lTripCode << std::endl;
    }

    // //////////////////////////////////////////////////////////////////
    storeTripProbMass::storeTripProbMass (DemandStruct_T& ioDemand)
      : ParserSemanticAction (ioDemand) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storeTripProbMass::operator() (double iReal) const {
      const bool hasInsertBeenSuccessfull = 
        _demand._tripProbDist.
        insert (TripProbDist_T::value_type (_demand._itTripCode,
                                               iReal)).second;
      if (hasInsertBeenSuccessfull == false) {
        STDAIR_LOG_ERROR ("The same trip type code ('"
                          << TripCode (_demand._itTripCode)
                          << "') has probably been given twice");
        throw CodeDuplicationException();
      }
      
      //std::cout << "TripProbMass: " << iReal << std::endl;
    }

    // //////////////////////////////////////////////////////////////////
    storeStayCode::storeStayCode (DemandStruct_T& ioDemand)
      : ParserSemanticAction (ioDemand) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storeStayCode::operator() (unsigned int iInteger) const {
      const stdair::DayDuration_T lStayDuration (iInteger);
      _demand._itStayDuration = lStayDuration;
      //std::cout << "Stay code: " << lStayDuration << std::endl;
    }

    // //////////////////////////////////////////////////////////////////
    storeStayProbMass::storeStayProbMass (DemandStruct_T& ioDemand)
      : ParserSemanticAction (ioDemand) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storeStayProbMass::operator() (double iReal) const {
      const bool hasInsertBeenSuccessfull = 
        _demand._stayProbDist.
        insert (StayProbDist_T::value_type (_demand._itStayDuration,
                                            iReal)).second;
      if (hasInsertBeenSuccessfull == false) {
        STDAIR_LOG_ERROR ("The same stay duration ('" << _demand._itStayDuration
                          << "') has probably been given twice");
        throw CodeDuplicationException();
      }
      
      //std::cout << "StayProbMass: " << iReal << std::endl;
    }

    // //////////////////////////////////////////////////////////////////
    storeFFCode::storeFFCode (DemandStruct_T& ioDemand)
      : ParserSemanticAction (ioDemand) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storeFFCode::operator() (iterator_t iStr, iterator_t iStrEnd) const {
      const std::string oneChar (iStr, iStrEnd);
      const FFCode lFFCode (oneChar.at(0));
      _demand._itFFCode = lFFCode.getCode();
      //std::cout << "FF code: " << lFFCode << std::endl;
    }

    // //////////////////////////////////////////////////////////////////
    storeFFProbMass::storeFFProbMass (DemandStruct_T& ioDemand)
      : ParserSemanticAction (ioDemand) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storeFFProbMass::operator() (double iReal) const {
      const bool hasInsertBeenSuccessfull = 
        _demand._ffProbDist.insert (FFProbDist_T::value_type (_demand._itFFCode,
                                                              iReal)).second;
      if (hasInsertBeenSuccessfull == false) {
        STDAIR_LOG_ERROR ("The same Frequent Flyer code ('"
                          << FFCode (_demand._itFFCode)
                          << "') has probably been given twice");
        throw CodeDuplicationException();
      }
      
      //std::cout << "FfProbMass: " << iReal << std::endl;
    }

    // //////////////////////////////////////////////////////////////////
    storePrefDepTime::storePrefDepTime (DemandStruct_T& ioDemand)
      : ParserSemanticAction (ioDemand) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storePrefDepTime::operator() (iterator_t iStr,
                                       iterator_t iStrEnd) const {
      _demand._itPrefDepTime = _demand.getTime();
        
      // Reset the number of minutes and seconds
      _demand._itMinutes = 0;
      _demand._itSeconds = 0;
    }

    // //////////////////////////////////////////////////////////////////
    storePrefDepTimeProbMass::storePrefDepTimeProbMass (DemandStruct_T& ioDemand)
      : ParserSemanticAction (ioDemand) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storePrefDepTimeProbMass::operator() (double iReal) const {
      const bool hasInsertBeenSuccessfull = _demand._prefDepTimeProbDist.
        insert (PrefDepTimeProbDist_T::value_type (_demand._itPrefDepTime,
                                                   iReal)).second;
      if (hasInsertBeenSuccessfull == false) {
        STDAIR_LOG_ERROR ("The same preferred departure time ('"
                          << _demand._itPrefDepTime
                          << "') has probably been given twice");
        throw CodeDuplicationException();
      }
      
      //std::cout << "PrefDepTimeProbMass: " << iReal << std::endl;
    }

    // //////////////////////////////////////////////////////////////////
    doEndDemand::doEndDemand (stdair::BomRoot& ioBomRoot,
                              DemandStruct_T& ioDemand)
      : ParserSemanticAction (ioDemand), _bomRoot (ioBomRoot) {
    }
    
    // //////////////////////////////////////////////////////////////////
    // void doEndDemand::operator() (char iChar) const {
    void doEndDemand::operator() (iterator_t iStr, iterator_t iStrEnd) const {
        
      // DEBUG: Display the result
      STDAIR_LOG_DEBUG ("Demand: " << _demand.describe());

      // Create the Demand BOM objects
      //DemandGenerator::generateDemandCharacteristicObjects (_bomRoot, _demand);
    }

      
    // ///////////////////////////////////////////////////////////////////
    //
    //  Utility Parsers
    //
    // ///////////////////////////////////////////////////////////////////
    /** 1-digit-integer parser */
    int1_p_t int1_p;
    
    /** 2-digit-integer parser */
    uint2_p_t uint2_p;
    
    /** Up-to-2-digit-integer parser */
    uint1_2_p_t uint1_2_p;

    /** 4-digit-integer parser */
    uint4_p_t uint4_p;
    
    /** Up-to-4-digit-integer parser */
    uint1_4_p_t uint1_4_p;

    /** Airline Code Parser: repeat_p(2,3)[chset_p("0-9A-Z")] */
    repeat_p_t airline_code_p (chset_t("0-9A-Z").derived(), 2, 3);
      
    /** Flight Number Parser: limit_d(0u, 9999u)[uint1_4_p] */
    bounded1_4_p_t flight_number_p (uint1_4_p.derived(), 0u, 9999u);

    /** Year Parser: limit_d(2000u, 2099u)[uint4_p] */
    bounded4_p_t year_p (uint4_p.derived(), 2000u, 2099u);
      
    /** Month Parser: limit_d(1u, 12u)[uint2_p] */
    bounded2_p_t month_p (uint2_p.derived(), 1u, 12u);

    /** Day Parser: limit_d(1u, 31u)[uint2_p] */
    bounded2_p_t day_p (uint2_p.derived(), 1u, 31u);
     
    /** DOW (Day-Of-the-Week) Parser: repeat_p(7)[chset_p("0-1")] */
    repeat_p_t dow_p (chset_t("0-1").derived().derived(), 7, 7);

    /** Airport Parser: repeat_p(3)[chset_p("0-9A-Z")] */
    repeat_p_t airport_p (chset_t("0-9A-Z").derived(), 3, 3);
      
    /** Hour Parser: limit_d(0u, 23u)[uint2_p] */
    bounded2_p_t hours_p (uint2_p.derived(), 0u, 23u);

    /** Minute Parser: limit_d(0u, 59u)[uint2_p] */
    bounded2_p_t minutes_p (uint2_p.derived(), 0u, 59u);

    /** Second Parser: limit_d(0u, 59u)[uint2_p] */
    bounded2_p_t seconds_p (uint2_p.derived(), 0u, 59u);

    /** Cabin Code Parser: chset_p("A-Z") */
    chset_t cabin_code_p ("A-Z");

    /** Family code parser */
    int1_p_t family_code_p;
      
    /** Class Code List Parser: repeat_p(1,26)[chset_p("A-Z")] */
    repeat_p_t class_code_list_p (chset_t("A-Z").derived(), 1, 26);


    // //////////////////////////////////////////////////////////////////
    //  (Boost Spirit) Grammar Definition
    // //////////////////////////////////////////////////////////////////

    // //////////////////////////////////////////////////////////////////
    DemandParser::DemandParser (stdair::BomRoot& ioBomRoot,
                                DemandStruct_T& ioDemand) 
      : _bomRoot (ioBomRoot), _demand (ioDemand) {
    }

    // //////////////////////////////////////////////////////////////////
    template<typename ScannerT>
    DemandParser::definition<ScannerT>::
    definition (DemandParser const& self) {

      demand_list = *( boost::spirit::classic::comment_p("//")
                       | boost::spirit::classic::comment_p("/*", "*/")
                       | demand )
        ;

      /*
       PrefDepDate; Origin; Destination; Cabin; Mean; StdDev;
       PosDist; ChannelDist; TripTypeDist; StayDurationDist;
       FrequentFlyerDist; WTPDist; PrefDepTimeDist;
       (PrefArrivalDate; PrefArrivalTime;) TimeValueDist; 
       ValueOfTimeDist; ArrivalPatternDist;
       2010-02-08; SIN; BKK; M; 10.0; 1.0;
       SIN:0.7, BKK:0.2, row:0.1; DF:0.1, DN:0.3, IF:0.4, IN:0.2;
       RO:0.6, RI:0.2, OW:0.2; 0:0.1, 1:0.1, 2:0.15, 3:0.15, 4:0.15, 5:0.35;
       P:0.01, G:0.05, S:0.15, M:0.3, N:0.49;
       06:0, 07:0.1, 09:0.3, 17:0.4, 19:0.8, 20:0.95, 22:1
       100:0, 500:0.8, 2000:1; 15:0, 60:1; 330:0, 40:0.2, 20:0.6, 1:1;
      */
      
      demand =
        pref_dep_date >> ';' >> origin >> ';' >> destination  >> ';' >> cabin
        >> ';' >> demand_params
        >> ';' >> pos_dist
        >> ';' >> channel_dist
        >> ';' >> trip_dist
        >> ';' >> stay_dist
        >> ';' >> ff_dist
        >> ';' >> pref_dep_time_dist
        >> demand_end[doEndDemand(self._bomRoot, self._demand)]
        ;

      demand_end =
        boost::spirit::classic::ch_p(';')
        ;
      
      pref_dep_date =
        date[storePreferredDepartureDate(self._demand)]
        ;

      date =
        boost::spirit::classic::lexeme_d[
         (year_p)[boost::spirit::classic::assign_a(self._demand._itYear)]
         >> '-'
         >> (month_p)[boost::spirit::classic::assign_a(self._demand._itMonth)]
         >> '-'
         >> (day_p)[boost::spirit::classic::assign_a(self._demand._itDay)]
         ]
        ;

      origin =
        (airport_p)[storeOrigin(self._demand)]
        ;

      destination =
        (airport_p)[storeDestination(self._demand)]
        ;

      cabin =
        (cabin_code_p)[storeCabin(self._demand)]
        ;

      demand_params =
        (boost::spirit::classic::ureal_p)[storeDemandMean(self._demand)]
        >> ';'
        >> (boost::spirit::classic::ureal_p)[storeDemandStdDev(self._demand)]
        ;
      
      pos_dist =
        pos_pair >> *( ',' >> pos_pair )
        ;

      pos_pair =
        pos_code[storePosCode(self._demand)]
        >> ':' >> pos_share
        ;

      pos_code =
        airport_p
        | boost::spirit::classic::chseq_p("row")
        ;

      pos_share =
        (boost::spirit::classic::ureal_p)[storePosProbMass(self._demand)]
        ;

      channel_dist =
        channel_pair >> *( ',' >> channel_pair )
        ;

      channel_pair =
        channel_code[storeChannelCode(self._demand)]
        >> ':' >> channel_share
        ;

      channel_code =
        boost::spirit::classic::chseq_p("DF")
        | boost::spirit::classic::chseq_p("DN")
        | boost::spirit::classic::chseq_p("IF")
        | boost::spirit::classic::chseq_p("IN")
        ;

      channel_share =
        (boost::spirit::classic::ureal_p)[storeChannelProbMass(self._demand)]
        ;
      
      trip_dist =
        trip_pair >> *( ',' >> trip_pair )
        ;

      trip_pair =
        trip_code[storeTripCode(self._demand)]
        >> ':' >> trip_share
        ;

      trip_code =
        boost::spirit::classic::chseq_p("RO")
        | boost::spirit::classic::chseq_p("RI")
        | boost::spirit::classic::chseq_p("OW")
        ;

      trip_share =
        (boost::spirit::classic::ureal_p)[storeTripProbMass(self._demand)]
        ;
      
      stay_dist =
        stay_pair >> *( ',' >> stay_pair )
        ;

      stay_pair =
        (int1_p)[storeStayCode(self._demand)]
        >> ':' >> stay_share
        ;

      stay_share =
        (boost::spirit::classic::ureal_p)[storeStayProbMass(self._demand)]
        ;

      ff_dist =
        ff_pair >> *( ',' >> ff_pair )
        ;

      ff_pair =
        ff_code[storeFFCode(self._demand)]
        >> ':' >> ff_share
        ;

      ff_code =
        boost::spirit::classic::ch_p('P')
        | boost::spirit::classic::ch_p('G')
        | boost::spirit::classic::ch_p('S')
        | boost::spirit::classic::ch_p('M')
        | boost::spirit::classic::ch_p('N')
        ;

      ff_share =
        (boost::spirit::classic::ureal_p)[storeFFProbMass(self._demand)]
        ;
      
      pref_dep_time_dist =
        pref_dep_time_pair >> *( ',' >> pref_dep_time_pair )
        ;

      pref_dep_time_pair =
        (time)[storePrefDepTime(self._demand)]
        >> ':' >> pref_dep_time_share
        ;

      pref_dep_time_share =
        (boost::spirit::classic::ureal_p)[storePrefDepTimeProbMass(self._demand)]
        ;

      time =
        boost::spirit::classic::lexeme_d[
       (hours_p)[boost::spirit::classic::assign_a(self._demand._itHours)]
       >> !('.' >> (minutes_p)[boost::spirit::classic::assign_a(self._demand._itMinutes)])
       >> !('.' >> (seconds_p)[boost::spirit::classic::assign_a(self._demand._itSeconds)])
       ]
        ;

      // BOOST_SPIRIT_DEBUG_NODE (DemandParser);
      BOOST_SPIRIT_DEBUG_NODE (demand_list);
      BOOST_SPIRIT_DEBUG_NODE (demand);
      BOOST_SPIRIT_DEBUG_NODE (demand_end);
      BOOST_SPIRIT_DEBUG_NODE (pref_dep_date);
      BOOST_SPIRIT_DEBUG_NODE (date);
      BOOST_SPIRIT_DEBUG_NODE (origin);
      BOOST_SPIRIT_DEBUG_NODE (destination);
      BOOST_SPIRIT_DEBUG_NODE (cabin);
      BOOST_SPIRIT_DEBUG_NODE (demand_params);
      BOOST_SPIRIT_DEBUG_NODE (pos_dist);
      BOOST_SPIRIT_DEBUG_NODE (pos_pair);
      BOOST_SPIRIT_DEBUG_NODE (pos_code);
      BOOST_SPIRIT_DEBUG_NODE (pos_share);
      BOOST_SPIRIT_DEBUG_NODE (channel_dist);
      BOOST_SPIRIT_DEBUG_NODE (channel_pair);
      BOOST_SPIRIT_DEBUG_NODE (channel_code);
      BOOST_SPIRIT_DEBUG_NODE (channel_share);
      BOOST_SPIRIT_DEBUG_NODE (trip_dist);
      BOOST_SPIRIT_DEBUG_NODE (trip_pair);
      BOOST_SPIRIT_DEBUG_NODE (trip_code);
      BOOST_SPIRIT_DEBUG_NODE (trip_share);
      BOOST_SPIRIT_DEBUG_NODE (stay_dist);
      BOOST_SPIRIT_DEBUG_NODE (stay_pair);
      BOOST_SPIRIT_DEBUG_NODE (stay_share);
      BOOST_SPIRIT_DEBUG_NODE (ff_dist);
      BOOST_SPIRIT_DEBUG_NODE (ff_pair);
      BOOST_SPIRIT_DEBUG_NODE (ff_code);
      BOOST_SPIRIT_DEBUG_NODE (ff_share);
      BOOST_SPIRIT_DEBUG_NODE (pref_dep_time_dist);
      BOOST_SPIRIT_DEBUG_NODE (pref_dep_time_pair);
      BOOST_SPIRIT_DEBUG_NODE (pref_dep_time_share);
      BOOST_SPIRIT_DEBUG_NODE (time);
    }

    // //////////////////////////////////////////////////////////////////
    template<typename ScannerT>
    boost::spirit::classic::rule<ScannerT> const&
    DemandParser::definition<ScannerT>::start() const {
      return demand_list;
    }
    
  }


  /////////////////////////////////////////////////////////////////////////
  //
  //  Entry class for the file parser
  //
  /////////////////////////////////////////////////////////////////////////

  // //////////////////////////////////////////////////////////////////////
  DemandFileParser::DemandFileParser (stdair::BomRoot& ioBomRoot,
                                      const std::string& iFilename)
    : _filename (iFilename), _bomRoot (ioBomRoot) {
    init();
  }

  // //////////////////////////////////////////////////////////////////////
  void DemandFileParser::init() {
    // Open the file
    _startIterator = iterator_t (_filename);

    // Check the filename exists and can be open
    if (!_startIterator) {
      STDAIR_LOG_ERROR ("The file " << _filename << " can not be open.");

      throw stdair::FileNotFoundException();
    }

    // Create an EOF iterator
    _endIterator = _startIterator.make_end();
  }
    
  // //////////////////////////////////////////////////////////////////////
  bool DemandFileParser::generateDemand () {
    bool oResult = false;
      
    STDAIR_LOG_DEBUG ("Parsing demand input file: " << _filename);

    // Initialise the parser (grammar) with the helper/staging structure.
    DemandParserHelper::DemandParser lDemandParser (_bomRoot, _demand);
      
    // Launch the parsing of the file and, thanks to the doEndDemand
    // call-back structure, the building of the whole BomRoot BOM
    // (i.e., including Inventory, FlightDate, LegDate, SegmentDate, etc.)
    boost::spirit::classic::parse_info<iterator_t> info =
      boost::spirit::classic::parse (_startIterator, _endIterator, lDemandParser,
                                     boost::spirit::classic::space_p);

    // Retrieves whether or not the parsing was successful
    oResult = info.hit;
      
    const std::string hasBeenFullyReadStr = (info.full == true)?"":"not ";
    if (oResult == true) {
      STDAIR_LOG_DEBUG ("Parsing of demand input file: " << _filename
                       << " succeeded: read " << info.length
                       << " characters. The input file has "
                       << hasBeenFullyReadStr
                       << "been fully read. Stop point: " << info.stop);
        
    } else {
      STDAIR_LOG_ERROR ("Parsing of demand input file: " << _filename
                       << " failed: read " << info.length
                       << " characters. The input file has "
                       << hasBeenFullyReadStr
                       << "been fully read. Stop point: " << info.stop);
      throw ParserException();
    }

    return oResult;
  }
    
}
