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
#include <trademgen/command/DemandGenerator.hpp>

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

      // TODO: do not harcode the preferred arrival date
      _demand._prefArrDate = _demand._prefDepDate;
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
    storePassengerType::storePassengerType (DemandStruct_T& ioDemand)
      : ParserSemanticAction (ioDemand) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storePassengerType::operator() (iterator_t iStr,
                                         iterator_t iStrEnd) const {
      const std::string oneChar (iStr, iStrEnd);
      const stdair::PassengerType lPaxType (oneChar.at(0));
      _demand._paxType = lPaxType; 
      //STDAIR_LOG_DEBUG ("Passenger type: " << lPaxType);
    }
    
    // //////////////////////////////////////////////////////////////////
    storeDemandMean::storeDemandMean (DemandStruct_T& ioDemand)
      : ParserSemanticAction (ioDemand) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storeDemandMean::operator() (double iReal) const { 
      _demand._demandMean = iReal; 
      //STDAIR_LOG_DEBUG ("Demand mean: " << iReal);
    }

    // //////////////////////////////////////////////////////////////////
    storeDemandStdDev::storeDemandStdDev (DemandStruct_T& ioDemand)
      : ParserSemanticAction (ioDemand) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storeDemandStdDev::operator() (double iReal) const { 
      _demand._demandStdDev = iReal; 
      //STDAIR_LOG_DEBUG ("Demand stddev: " << iReal);
    }

    // //////////////////////////////////////////////////////////////////
    storePosCode::storePosCode (DemandStruct_T& ioDemand)
      : ParserSemanticAction (ioDemand) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storePosCode::operator() (iterator_t iStr, iterator_t iStrEnd) const {
      const stdair::AirportCode_T lPosCode (iStr, iStrEnd);
      _demand._itPosCode = lPosCode;
      //STDAIR_LOG_DEBUG ("Pos code: " << lPosCode);
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
      
      //STDAIR_LOG_DEBUG ("PosProbMass: " << iReal);
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
      //STDAIR_LOG_DEBUG ("Channel code: " << lChannelCode);
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
      
      //STDAIR_LOG_DEBUG ("ChannelProbMass: " << iReal);
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
      //STDAIR_LOG_DEBUG ("Trip code: " << lTripCode);
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
      
      //STDAIR_LOG_DEBUG ("TripProbMass: " << iReal);
    }

    // //////////////////////////////////////////////////////////////////
    storeStayCode::storeStayCode (DemandStruct_T& ioDemand)
      : ParserSemanticAction (ioDemand) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storeStayCode::operator() (unsigned int iInteger) const {
      const stdair::DayDuration_T lStayDuration (iInteger);
      _demand._itStayDuration = lStayDuration;
      //STDAIR_LOG_DEBUG ("Stay duration: " << lStayDuration);
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
      
      //STDAIR_LOG_DEBUG ("StayProbMass: " << iReal);
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
      //STDAIR_LOG_DEBUG ("FF code: " << lFFCode);
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
      
      //STDAIR_LOG_DEBUG ("FfProbMass: " << iReal);
    }

    // //////////////////////////////////////////////////////////////////
    storePrefDepTime::storePrefDepTime (DemandStruct_T& ioDemand)
      : ParserSemanticAction (ioDemand) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storePrefDepTime::operator() (iterator_t iStr,
                                       iterator_t iStrEnd) const {
      _demand._itPrefDepTime = _demand.getTime();
        
      // DEBUG
      // STDAIR_LOG_DEBUG ("Pref dep time: " << _demand._itHours << ":"
      //                   << _demand._itMinutes << ":" << _demand._itSeconds
      //                   << " ==> " << _demand._itPrefDepTime);
      
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
      
      //STDAIR_LOG_DEBUG ("PrefDepTimeProbMass: " << iReal);
    }

    // //////////////////////////////////////////////////////////////////
    storeWTP::storeWTP (DemandStruct_T& ioDemand)
      : ParserSemanticAction (ioDemand) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storeWTP::operator() (double iReal) const {
      _demand._itWTP = iReal;
      //STDAIR_LOG_DEBUG ("WTP: " << iReal);
    }

    // //////////////////////////////////////////////////////////////////
    storeWTPProbMass::storeWTPProbMass (DemandStruct_T& ioDemand)
      : ParserSemanticAction (ioDemand) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storeWTPProbMass::operator() (double iReal) const {
      const bool hasInsertBeenSuccessfull =
        _demand._wtpProbDist.insert (WTPProbDist_T::value_type (_demand._itWTP,
                                                                iReal)).second;
      if (hasInsertBeenSuccessfull == false) {
        STDAIR_LOG_ERROR ("The same WTP ('" << _demand._itWTP
                          << "') has probably been given twice");
        throw CodeDuplicationException();
      }
      
      //STDAIR_LOG_DEBUG ("WTPProbMass: " << iReal);
    }

    // //////////////////////////////////////////////////////////////////
    storeTimeValue::storeTimeValue (DemandStruct_T& ioDemand)
      : ParserSemanticAction (ioDemand) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storeTimeValue::operator() (unsigned int iInteger) const {
      const stdair::Duration_T lTimeValue (iInteger, 0, 0);
      _demand._itTimeValue = lTimeValue;
      //STDAIR_LOG_DEBUG ("Time value: " << lTimeDuration);
    }

    // //////////////////////////////////////////////////////////////////
    storeTimeValueProbMass::storeTimeValueProbMass (DemandStruct_T& ioDemand)
      : ParserSemanticAction (ioDemand) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storeTimeValueProbMass::operator() (double iReal) const {
      const bool hasInsertBeenSuccessfull = 
        _demand._timeValueProbDist.
        insert (TimeValueProbDist_T::value_type (_demand._itTimeValue,
                                                 iReal)).second;
      if (hasInsertBeenSuccessfull == false) {
        STDAIR_LOG_ERROR ("The same time value ('" << _demand._itTimeValue
                          << "') has probably been given twice");
        throw CodeDuplicationException();
      }
      
      //STDAIR_LOG_DEBUG ("TimeValueProbMass: " << iReal);
    }

    // //////////////////////////////////////////////////////////////////
    storeDTD::storeDTD (DemandStruct_T& ioDemand)
      : ParserSemanticAction (ioDemand) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storeDTD::operator() (unsigned int iInteger) const {
      const stdair::DayDuration_T lDTD (iInteger);
      _demand._itDTD = lDTD;
      //STDAIR_LOG_DEBUG ("DTD: " << lDTD);
    }

    // //////////////////////////////////////////////////////////////////
    storeDTDProbMass::storeDTDProbMass (DemandStruct_T& ioDemand)
      : ParserSemanticAction (ioDemand) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storeDTDProbMass::operator() (double iReal) const {
      const bool hasInsertBeenSuccessfull =
        _demand._dtdProbDist.insert (DTDProbDist_T::value_type (_demand._itDTD,
                                                                iReal)).second;
      if (hasInsertBeenSuccessfull == false) {
        STDAIR_LOG_ERROR ("The same DTD ('" << _demand._itDTD
                          << "') has probably been given twice");
        throw CodeDuplicationException();
      }
      
      //STDAIR_LOG_DEBUG ("DTDProbMass: " << iReal);
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
      DemandGenerator::createDemandCharacteristics (_bomRoot, _demand);
                                 

      // Clean the lists
      _demand._posProbDist.clear();
      _demand._channelProbDist.clear();
      _demand._tripProbDist.clear();
      _demand._stayProbDist.clear();
      _demand._ffProbDist.clear();
      _demand._prefDepTimeProbDist.clear(); 
      _demand._wtpProbDist.clear();
      _demand._timeValueProbDist.clear();
      _demand._dtdProbDist.clear();
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

    /** Up-to-3-digit-integer parser */
    uint1_3_p_t uint1_3_p;

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
    bounded1_2_p_t hours_p (uint1_2_p.derived(), 0u, 23u);

    /** Minute Parser: limit_d(0u, 59u)[uint2_p] */
    bounded2_p_t minutes_p (uint2_p.derived(), 0u, 59u);

    /** Second Parser: limit_d(0u, 59u)[uint2_p] */
    bounded2_p_t seconds_p (uint2_p.derived(), 0u, 59u);

    /** Passenger type parser: chset_p("A-Z") */
    chset_t passenger_type_code_p ("A-Z");

    /** Family code parser */
    int1_p_t family_code_p;
      
    /** Class Code List Parser: repeat_p(1,26)[chset_p("A-Z")] */
    repeat_p_t class_code_list_p (chset_t("A-Z").derived(), 1, 26);

    /** Stay duration Parser: limit_d(0u, 999u)[uint3_p] */
    bounded1_3_p_t stay_duration_p (uint1_3_p.derived(), 0u, 23u);


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

      demand =
        pref_dep_date
        >> ';' >> origin >> ';' >> destination
        >> ';' >> passenger_type[storePassengerType(self._demand)]
        >> ';' >> demand_params
        >> ';' >> pos_dist
        >> ';' >> channel_dist
        >> ';' >> trip_dist
        >> ';' >> stay_dist
        >> ';' >> ff_dist
        >> ';' >> pref_dep_time_dist
        >> ';' >> wtp_dist
        >> ';' >> time_value_dist
        >> ';' >> dtd_dist
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

      passenger_type =
        boost::spirit::classic::ch_p('L')
        | boost::spirit::classic::ch_p('B')
        | boost::spirit::classic::ch_p('F')
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
        (stay_duration_p)[storeStayCode(self._demand)]
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

      wtp_dist =
        wtp_pair >> *( ',' >> wtp_pair )
        ;

      wtp_pair =
        (boost::spirit::classic::ureal_p)[storeWTP(self._demand)]
        >> ':' >> wtp_share
        ;

      wtp_share =
        (boost::spirit::classic::ureal_p)[storeWTPProbMass(self._demand)]
        ;

      time_value_dist =
        time_value_pair >> *( ',' >> time_value_pair )
        ;

      time_value_pair =
        (uint1_2_p)[storeTimeValue(self._demand)]
        >> ':' >> time_value_share
        ;

      time_value_share =
        (boost::spirit::classic::ureal_p)[storeTimeValueProbMass(self._demand)]
        ;

      dtd_dist =
        dtd_pair >> *( ',' >> dtd_pair )
        ;

      dtd_pair =
        (boost::spirit::classic::ureal_p)[storeDTD(self._demand)]
        >> ':' >> dtd_share
        ;

      dtd_share =
        (boost::spirit::classic::ureal_p)[storeDTDProbMass(self._demand)]
        ;

      // BOOST_SPIRIT_DEBUG_NODE (DemandParser);
      BOOST_SPIRIT_DEBUG_NODE (demand_list);
      BOOST_SPIRIT_DEBUG_NODE (demand);
      BOOST_SPIRIT_DEBUG_NODE (demand_end);
      BOOST_SPIRIT_DEBUG_NODE (pref_dep_date);
      BOOST_SPIRIT_DEBUG_NODE (date);
      BOOST_SPIRIT_DEBUG_NODE (origin);
      BOOST_SPIRIT_DEBUG_NODE (destination);
      BOOST_SPIRIT_DEBUG_NODE (passenger_type);
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
      BOOST_SPIRIT_DEBUG_NODE (wtp_dist);
      BOOST_SPIRIT_DEBUG_NODE (wtp_pair);
      BOOST_SPIRIT_DEBUG_NODE (wtp_share);
      BOOST_SPIRIT_DEBUG_NODE (time_value_dist);
      BOOST_SPIRIT_DEBUG_NODE (time_value_pair);
      BOOST_SPIRIT_DEBUG_NODE (time_value_share);
      BOOST_SPIRIT_DEBUG_NODE (dtd_dist);
      BOOST_SPIRIT_DEBUG_NODE (dtd_pair);
      BOOST_SPIRIT_DEBUG_NODE (dtd_share);
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
  DemandFileParser::
  DemandFileParser (stdair::BomRoot& ioBomRoot,
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
