// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
// Boost
#include <boost/make_shared.hpp>
// StdAir
#include <stdair/basic/ProgressStatusSet.hpp>
#include <stdair/basic/BasConst_Request.hpp>
#include <stdair/bom/BomManager.hpp>
#include <stdair/bom/EventStruct.hpp>
#include <stdair/bom/BookingRequestStruct.hpp>
#include <stdair/bom/TravelSolutionStruct.hpp>
#include <stdair/bom/CancellationStruct.hpp>
#include <stdair/factory/FacBom.hpp>
#include <stdair/factory/FacBomManager.hpp>
#include <stdair/service/Logger.hpp>
// SEvMgr
#include <sevmgr/SEVMGR_Service.hpp>
// TraDemGen
#include <trademgen/basic/DemandCharacteristics.hpp>
#include <trademgen/basic/DemandDistribution.hpp>
#include <trademgen/bom/DemandStruct.hpp>
#include <trademgen/bom/DemandStream.hpp>
#include <trademgen/command/DemandManager.hpp>

namespace TRADEMGEN {

  // //////////////////////////////////////////////////////////////////////
  void DemandManager::
  buildSampleBomStd (SEVMGR::SEVMGR_ServicePtr_T ioSEVMGR_ServicePtr,
                     stdair::RandomGeneration& ioSharedGenerator,
                     const POSProbabilityMass_T& iPOSProbMass) {
    // Sanity check
    assert (ioSEVMGR_ServicePtr != NULL);

    // Key of the demand stream
    const stdair::AirportCode_T lOrigin ("SIN");
    const stdair::AirportCode_T lDestination ("BKK");
    const stdair::Date_T lDepDate (2011, 2, 14);
    const stdair::CabinCode_T lCabin ("Y");

    //
    const DemandStreamKey lDemandStreamKey (lOrigin, lDestination, lDepDate,
                                            lCabin);

    // DEBUG
    // STDAIR_LOG_DEBUG ("Demand stream key: " << lDemandStreamKey.describe());
    
    // Distribution for the number of requests
    const stdair::MeanValue_T lDemandMean (10.0);
    const stdair::StdDevValue_T lDemandStdDev (1.0);
    const DemandDistribution lDemandDistribution (lDemandMean, lDemandStdDev);
    
    // Seed
    const stdair::RandomSeed_T& lRequestDateTimeSeed =
      generateSeed (ioSharedGenerator);
    const stdair::RandomSeed_T& lDemandCharacteristicsSeed =
      generateSeed (ioSharedGenerator);

    //
    ArrivalPatternCumulativeDistribution_T lDTDProbDist;
    lDTDProbDist.insert(ArrivalPatternCumulativeDistribution_T::value_type(-330,
                                                                           0));
    lDTDProbDist.insert(ArrivalPatternCumulativeDistribution_T::value_type(-40,
                                                                           0.2));
    lDTDProbDist.insert(ArrivalPatternCumulativeDistribution_T::value_type(-20,
                                                                           0.6));
    lDTDProbDist.insert(ArrivalPatternCumulativeDistribution_T::value_type(-1,
                                                                           1.0));
    //
    POSProbabilityMassFunction_T lPOSProbDist;
    lPOSProbDist.insert (POSProbabilityMassFunction_T::value_type ("BKK", 0.3));
    lPOSProbDist.insert (POSProbabilityMassFunction_T::value_type ("SIN", 0.7));
    //
    ChannelProbabilityMassFunction_T lChannelProbDist;
    lChannelProbDist.insert (ChannelProbabilityMassFunction_T::value_type ("DF",
                                                                           0.1));
    lChannelProbDist.insert (ChannelProbabilityMassFunction_T::value_type ("DN",
                                                                           0.3));
    lChannelProbDist.insert (ChannelProbabilityMassFunction_T::value_type ("IF",
                                                                           0.4));
    lChannelProbDist.insert (ChannelProbabilityMassFunction_T::value_type ("IN",
                                                                           0.2));
    //
    TripTypeProbabilityMassFunction_T lTripProbDist;
    lTripProbDist.insert (TripTypeProbabilityMassFunction_T::value_type ("RO",
                                                                         0.6));
    lTripProbDist.insert (TripTypeProbabilityMassFunction_T::value_type ("RI",
                                                                         0.2));
    lTripProbDist.insert (TripTypeProbabilityMassFunction_T::value_type ("OW",
                                                                         0.2));
    //
    StayDurationProbabilityMassFunction_T lStayProbDist;
    lStayProbDist.insert(StayDurationProbabilityMassFunction_T::value_type(0,
                                                                           0.1));
    lStayProbDist.insert(StayDurationProbabilityMassFunction_T::value_type(1,
                                                                           0.1));
    lStayProbDist.insert(StayDurationProbabilityMassFunction_T::value_type(2,
                                                                           .15));
    lStayProbDist.insert(StayDurationProbabilityMassFunction_T::value_type(3,
                                                                           .15));
    lStayProbDist.insert(StayDurationProbabilityMassFunction_T::value_type(4,
                                                                           .15));
    lStayProbDist.insert(StayDurationProbabilityMassFunction_T::value_type(5,
                                                                           .35));
    //
    FrequentFlyerProbabilityMassFunction_T lFFProbDist;
    lFFProbDist.insert(FrequentFlyerProbabilityMassFunction_T::value_type("P",
                                                                          0.01));
    lFFProbDist.insert(FrequentFlyerProbabilityMassFunction_T::value_type("G",
                                                                          0.05));
    lFFProbDist.insert(FrequentFlyerProbabilityMassFunction_T::value_type("S",
                                                                          0.15));
    lFFProbDist.insert(FrequentFlyerProbabilityMassFunction_T::value_type("M",
                                                                          0.3));
    lFFProbDist.insert(FrequentFlyerProbabilityMassFunction_T::value_type("N",
                                                                          0.49));
    //
    PreferredDepartureTimeContinuousDistribution_T lPrefDepTimeProbDist;
    lPrefDepTimeProbDist.
      insert (PreferredDepartureTimeContinuousDistribution_T::value_type (6 * stdair::HOUR_CONVERTED_IN_SECONDS, 0));
    lPrefDepTimeProbDist.
      insert (PreferredDepartureTimeContinuousDistribution_T::value_type (7 * stdair::HOUR_CONVERTED_IN_SECONDS,
                                                                          0.1));
    lPrefDepTimeProbDist.
      insert (PreferredDepartureTimeContinuousDistribution_T::value_type (9 * stdair::HOUR_CONVERTED_IN_SECONDS,
                                                                          0.3));
    lPrefDepTimeProbDist.
      insert (PreferredDepartureTimeContinuousDistribution_T::value_type (17 * stdair::HOUR_CONVERTED_IN_SECONDS,
                                                                          0.4));
    lPrefDepTimeProbDist.
      insert (PreferredDepartureTimeContinuousDistribution_T::value_type (19 * stdair::HOUR_CONVERTED_IN_SECONDS,
                                                                          0.80));
    lPrefDepTimeProbDist.
      insert (PreferredDepartureTimeContinuousDistribution_T::value_type (20 * stdair::HOUR_CONVERTED_IN_SECONDS,
                                                                          0.95));
    lPrefDepTimeProbDist.
      insert (PreferredDepartureTimeContinuousDistribution_T::value_type (22 * stdair::HOUR_CONVERTED_IN_SECONDS,
                                                                          1));
    //
    ValueOfTimeContinuousDistribution_T lTimeValueProbDist;
    lTimeValueProbDist.insert(ValueOfTimeContinuousDistribution_T::value_type(15,
                                                                              0));
    lTimeValueProbDist.insert(ValueOfTimeContinuousDistribution_T::value_type(60,
                                                                              1));

    //
    const stdair::WTP_T lWTP (1000.0);
    const stdair::ChangeFeesRatio_T lChangeFees (0.5);
    const stdair::Disutility_T lChangeFeeDisutility (50);
    const stdair::NonRefundableRatio_T lNonRefundable (0.5);
    const stdair::Disutility_T lNonRefundableDisutility (50);


    // Delegate the call to the dedicated command
    DemandStream& lDemandStream = 
      createDemandStream (ioSEVMGR_ServicePtr, lDemandStreamKey, lDTDProbDist,
                          lPOSProbDist, lChannelProbDist, lTripProbDist,
                          lStayProbDist, lFFProbDist,
                          lChangeFees, lChangeFeeDisutility,
                          lNonRefundable, lNonRefundableDisutility,
                          lPrefDepTimeProbDist,
                          lWTP, lTimeValueProbDist, lDemandDistribution,
                          ioSharedGenerator.getBaseGenerator(),
                          lRequestDateTimeSeed,
                          lDemandCharacteristicsSeed, iPOSProbMass);

    // Calculate the expected total number of events for the current
    // demand stream
    const stdair::NbOfRequests_T& lExpectedTotalNbOfEvents =
      lDemandStream.getMeanNumberOfRequests();

    /**
     * Initialise the progress statuses, specific to the booking request type
     */
    ioSEVMGR_ServicePtr->addStatus (stdair::EventType::BKG_REQ,
                                    lExpectedTotalNbOfEvents);
  }

  // //////////////////////////////////////////////////////////////////////
  DemandStream& DemandManager::createDemandStream
  (SEVMGR::SEVMGR_ServicePtr_T ioSEVMGR_ServicePtr,
   const DemandStreamKey& iKey,
   const ArrivalPatternCumulativeDistribution_T& iArrivalPattern,
   const POSProbabilityMassFunction_T& iPOSProbMass,
   const ChannelProbabilityMassFunction_T& iChannelProbMass,
   const TripTypeProbabilityMassFunction_T& iTripTypeProbMass,
   const StayDurationProbabilityMassFunction_T& iStayDurationProbMass,
   const FrequentFlyerProbabilityMassFunction_T& iFrequentFlyerProbMass,
   const stdair::ChangeFeesRatio_T& iChangeFeeProb,
   const stdair::Disutility_T& iChangeFeeDisutility,
   const stdair::NonRefundableRatio_T& iNonRefundableProb,
   const stdair::Disutility_T& iNonRefundableDisutility,
   const PreferredDepartureTimeContinuousDistribution_T& iPreferredDepartureTimeContinuousDistribution,
   const stdair::WTP_T& iMinWTP,
   const ValueOfTimeContinuousDistribution_T& iValueOfTimeContinuousDistribution,
   const DemandDistribution& iDemandDistribution,
   stdair::BaseGenerator_T&  ioSharedGenerator,
   const stdair::RandomSeed_T& iRequestDateTimeSeed,
   const stdair::RandomSeed_T& iDemandCharacteristicsSeed,
   const POSProbabilityMass_T& iDefaultPOSProbablityMass) {

    // Sanity check
    assert (ioSEVMGR_ServicePtr != NULL);

	// 
    DemandStream& oDemandStream =
      stdair::FacBom<DemandStream>::instance().create (iKey);

    oDemandStream.setAll (iArrivalPattern, iPOSProbMass,
                          iChannelProbMass, iTripTypeProbMass,
                          iStayDurationProbMass, iFrequentFlyerProbMass,
                          iChangeFeeProb, iChangeFeeDisutility,
                          iNonRefundableProb, iNonRefundableDisutility,
                          iPreferredDepartureTimeContinuousDistribution,
                          iMinWTP, iValueOfTimeContinuousDistribution,
                          iDemandDistribution, ioSharedGenerator,
                          iRequestDateTimeSeed, iDemandCharacteristicsSeed,
                          iDefaultPOSProbablityMass);

    ioSEVMGR_ServicePtr->addEventGenerator (oDemandStream);

    return oDemandStream;
  }
    
  // //////////////////////////////////////////////////////////////////////
  void DemandManager::
  createDemandCharacteristics (SEVMGR::SEVMGR_ServicePtr_T ioSEVMGR_ServicePtr,
                               stdair::RandomGeneration& ioSharedGenerator,
                               const POSProbabilityMass_T& iPOSProbMass,
                               const DemandStruct& iDemand) {
    // Sanity check
    assert (ioSEVMGR_ServicePtr != NULL);
	
    //
    stdair::BaseGenerator_T& lSharedGenerator =
      ioSharedGenerator.getBaseGenerator();
    
    // Parse the date period and DoW and generate demand characteristics.
    const stdair::DatePeriod_T lDateRange = iDemand._dateRange;
    for (boost::gregorian::day_iterator itDate = lDateRange.begin();
         itDate != lDateRange.end(); ++itDate) {
      const stdair::Date_T& currentDate = *itDate;

      // Retrieve, for the current day, the Day-Of-the-Week (thanks to Boost)
      const unsigned short currentDoW = currentDate.day_of_week().as_number();
        
      // The demand structure stores which Days (-Of-the-Week) are
      // active within the week. For each day (Mon., Tue., etc.), a boolean
      // states whether the Flight is active for that day.
      const stdair::DoWStruct& lDoWList = iDemand._dow;
      const bool isDoWActive = lDoWList.getStandardDayOfWeek (currentDoW);

      if (isDoWActive == true) {
        const DemandStreamKey lDemandStreamKey (iDemand._origin,
                                                iDemand._destination,
                                                currentDate,
                                                iDemand._prefCabin);
        // DEBUG
        // STDAIR_LOG_DEBUG ("Demand stream key: " << lDemandStreamKey.describe());
        
        //
        const DemandDistribution lDemandDistribution (iDemand._demandMean,
                                                      iDemand._demandStdDev);
        
        // Seed
        const stdair::RandomSeed_T& lRequestDateTimeSeed =
          generateSeed (ioSharedGenerator);
        const stdair::RandomSeed_T& lDemandCharacteristicsSeed =
          generateSeed (ioSharedGenerator);
        
        // Delegate the call to the dedicated command
        DemandStream& lDemandStream = 
          createDemandStream (ioSEVMGR_ServicePtr, lDemandStreamKey,
                              iDemand._dtdProbDist, iDemand._posProbDist,
                              iDemand._channelProbDist,
                              iDemand._tripProbDist,
                              iDemand._stayProbDist, iDemand._ffProbDist,
                              iDemand._changeFeeProb,
                              iDemand._changeFeeDisutility,
                              iDemand._nonRefundableProb,
                              iDemand._nonRefundableDisutility,
                              iDemand._prefDepTimeProbDist,
                              iDemand._minWTP,
                              iDemand._timeValueProbDist,
                              lDemandDistribution, lSharedGenerator,
                              lRequestDateTimeSeed,
                              lDemandCharacteristicsSeed,
                              iPOSProbMass);
        
        // Calculate the expected total number of events for the current
        // demand stream
        const stdair::NbOfRequests_T& lExpectedTotalNbOfEvents =
          lDemandStream.getMeanNumberOfRequests();
        
        /**
         * Initialise the progress statuses, one specific to the
         * booking request type.
         */
        ioSEVMGR_ServicePtr->addStatus (stdair::EventType::BKG_REQ,
                                        lExpectedTotalNbOfEvents);
      }
    }
  }

  // ////////////////////////////////////////////////////////////////////
  stdair::RandomSeed_T DemandManager::
  generateSeed (stdair::RandomGeneration& ioSharedGenerator) {
    stdair::RealNumber_T lVariateUnif = ioSharedGenerator() * 1e9;
    stdair::RandomSeed_T oSeed = static_cast<stdair::RandomSeed_T>(lVariateUnif);
    return oSeed;
  }
  
  // ////////////////////////////////////////////////////////////////////
  const bool DemandManager::
  stillHavingRequestsToBeGenerated (SEVMGR::SEVMGR_ServicePtr_T ioSEVMGR_ServicePtr,
                                    const stdair::DemandStreamKeyStr_T& iKey,
                                    stdair::ProgressStatusSet& ioPSS,
                                    const stdair::DemandGenerationMethod& iDemandGenerationMethod) {
    // Sanity check
    assert (ioSEVMGR_ServicePtr != NULL);
    
    // Retrieve the DemandStream which corresponds to the given key.
    const DemandStream& lDemandStream =
      ioSEVMGR_ServicePtr->getEventGenerator<DemandStream,stdair::DemandStreamKeyStr_T>(iKey);

    // Retrieve the progress status of the demand stream.
    stdair::ProgressStatus
      lProgressStatus (lDemandStream.getNumberOfRequestsGeneratedSoFar(),
                       lDemandStream.getMeanNumberOfRequests(),
                       lDemandStream.getTotalNumberOfRequestsToBeGenerated());
    ioPSS.setSpecificGeneratorStatus (lProgressStatus, iKey);
    
    return lDemandStream.stillHavingRequestsToBeGenerated (iDemandGenerationMethod);
  }

  // ////////////////////////////////////////////////////////////////////
  stdair::BookingRequestPtr_T DemandManager::
  generateNextRequest (SEVMGR::SEVMGR_ServicePtr_T ioSEVMGR_ServicePtr,
                       stdair::RandomGeneration& ioGenerator,
                       const stdair::DemandStreamKeyStr_T& iKey,
                       const stdair::DemandGenerationMethod& iDemandGenerationMethod) {
    // Sanity check
    assert (ioSEVMGR_ServicePtr != NULL);

    // Retrieve the DemandStream which corresponds to the given key.
    DemandStream& lDemandStream =
      ioSEVMGR_ServicePtr->getEventGenerator<DemandStream,stdair::DemandStreamKeyStr_T>(iKey);

    // Generate the next booking request
    stdair::BookingRequestPtr_T lBookingRequest =
      lDemandStream.generateNextRequest (ioGenerator,
                                         iDemandGenerationMethod);

    const stdair::DateTime_T& lBookingRequestDateTime =
      lBookingRequest->getRequestDateTime();
    const stdair::Date_T& lBookingRequestDate =
      lBookingRequestDateTime.date();
    const stdair::Duration_T& lBookingRequestTime =
      lBookingRequestDateTime.time_of_day();
    const stdair::Date_T& lPreferedDepartureDate =
      lBookingRequest->getPreferedDepartureDate();
    const stdair::Duration_T& lPreferedDepartureTime =
      lBookingRequest->getPreferredDepartureTime(); 

    if ((lPreferedDepartureDate > lBookingRequestDate) ||
        (lPreferedDepartureDate == lBookingRequestDate &&
         lPreferedDepartureTime > lBookingRequestTime)) {

      // Create an event structure
      stdair::EventStruct lEventStruct (stdair::EventType::BKG_REQ,
                                        lBookingRequest);

      /**
       \note When adding an event in the event queue, the event can be
       altered. That happens when an event already exists, in the
       event queue, with exactly the same date-time stamp. In that
       case, the date-time stamp is altered for the newly added event,
       so that the unicity on the date-time stamp can be guaranteed.
      */
      ioSEVMGR_ServicePtr->addEvent (lEventStruct);

    } else {

      // Update the expected number of eventss for the given event type (i.e.,
      // booking request)
      stdair::Count_T lCurrentBRNumber = 
	ioSEVMGR_ServicePtr->getActualTotalNumberOfEventsToBeGenerated (stdair::EventType::BKG_REQ);
      lCurrentBRNumber--;
      ioSEVMGR_ServicePtr->updateStatus (stdair::EventType::BKG_REQ, lCurrentBRNumber);

    }

    return lBookingRequest;
  }

  // ////////////////////////////////////////////////////////////////////
  stdair::Count_T DemandManager::
  generateFirstRequests (SEVMGR::SEVMGR_ServicePtr_T ioSEVMGR_ServicePtr,
                         stdair::RandomGeneration& ioGenerator,
                         const stdair::DemandGenerationMethod& iDemandGenerationMethod) {
    // Sanity check
    assert (ioSEVMGR_ServicePtr != NULL);

    // Actual total number of events to be generated
    stdair::NbOfRequests_T lActualTotalNbOfEvents = 0.0;

    // Retrieve the DemandStream list
    const DemandStreamList_T& lDemandStreamList =
      ioSEVMGR_ServicePtr->getEventGeneratorList<DemandStream>();

    for (DemandStreamList_T::const_iterator itDemandStream =
           lDemandStreamList.begin();
         itDemandStream != lDemandStreamList.end(); ++itDemandStream) {
      DemandStream* lDemandStream_ptr = *itDemandStream;
      assert (lDemandStream_ptr != NULL);

      lDemandStream_ptr->setBoolFirstDateTimeRequest(true);

      // Calculate the expected total number of events for the current
      // demand stream
      const stdair::NbOfRequests_T& lActualNbOfEvents =
        lDemandStream_ptr->getTotalNumberOfRequestsToBeGenerated();
      lActualTotalNbOfEvents += lActualNbOfEvents;

      // Retrieve the key of the demand stream
      const DemandStreamKey& lKey = lDemandStream_ptr->getKey();

      // Check whether there are still booking requests to be generated
      const bool stillHavingRequestsToBeGenerated =
        lDemandStream_ptr->stillHavingRequestsToBeGenerated (iDemandGenerationMethod);
   
      if (stillHavingRequestsToBeGenerated) {
        // Generate the next event (booking request), and insert it
        // into the event queue
        generateNextRequest (ioSEVMGR_ServicePtr, ioGenerator,
                             lKey.toString(),
                             iDemandGenerationMethod);
      }
    }
    
    // Update the progress status for the given event type (i.e.,
    // booking request)
    ioSEVMGR_ServicePtr->updateStatus (stdair::EventType::BKG_REQ,
				       lActualTotalNbOfEvents);

    // Retrieve the actual total number of events to be generated
    const stdair::Count_T oTotalNbOfEvents = std::floor (lActualTotalNbOfEvents);

    //
    return oTotalNbOfEvents;
  }
  
  // ////////////////////////////////////////////////////////////////////
  void DemandManager::reset (SEVMGR::SEVMGR_ServicePtr_T ioSEVMGR_ServicePtr,
                             stdair::BaseGenerator_T& ioShareGenerator) {
    // Sanity check
    assert (ioSEVMGR_ServicePtr != NULL);

    // TODO: check whether it is really necessary to destroy the
    // objects manually. Indeed, FacSupervisor::cleanAll() should
    // destroy any BOM object.

    // Reset all the DemandStream objects
    const DemandStreamList_T& lDemandStreamList =
      ioSEVMGR_ServicePtr->getEventGeneratorList<DemandStream>();
    for (DemandStreamList_T::const_iterator itDS = lDemandStreamList.begin();
         itDS != lDemandStreamList.end(); ++itDS) {
      DemandStream* lCurrentDS_ptr = *itDS;
      assert (lCurrentDS_ptr != NULL);
      
      lCurrentDS_ptr->reset (ioShareGenerator);
    }

    /**
     * Reset the EventQueue object.
     *
     * \note As the DemandStream objects are attached to the EventQueue
     * instance, that latter has to be resetted after the DemandStream
     * objects.
     */
    ioSEVMGR_ServicePtr->reset();
  }
  
  // ////////////////////////////////////////////////////////////////////
  bool DemandManager::
  generateCancellation (stdair::RandomGeneration& ioGenerator,
                        const stdair::TravelSolutionStruct& iTravelSolution,
                        const stdair::PartySize_T& iPartySize,
                        const stdair::DateTime_T& iRequestTime,
                        const stdair::Date_T& iDepartureDate,
                        stdair::EventStruct& ioEventStruct) {

    // Draw a random number to decide if we generate a
    // cancellation. For instance, the probability will be hardcoded.
    // The cancellation time will be generated uniformly.
    double lRandomNumber = ioGenerator();
   
    if (lRandomNumber >= 0.05) {
      return false;
    }
    lRandomNumber /= 0.05;
    
    // Hardcode the latest cancellation time.
    const stdair::Time_T lMidNight =
      boost::posix_time::hours (0);
    const stdair::DateTime_T lDepartureDateTime =
      boost::posix_time::ptime (iDepartureDate, lMidNight);

    // Time to departure.
    const stdair::Duration_T lTimeToDeparture = lDepartureDateTime-iRequestTime;

    // Cancellation time to departure
    const long lTimeToDepartureInSeconds = lTimeToDeparture.total_seconds();
    const long lCancellationTimeToDepartureInSeconds =
      static_cast<long> (lTimeToDepartureInSeconds * lRandomNumber);
    const stdair::Duration_T lCancellationTimeToDeparture (0, 0, lCancellationTimeToDepartureInSeconds);
    
    // Cancellation time
    const stdair::DateTime_T lCancellationTime =
      lDepartureDateTime - lCancellationTimeToDeparture;
    const stdair::Duration_T lTimeBetweenCancellationAndTheRequest = 
      lCancellationTime - iRequestTime;

    if (lTimeBetweenCancellationAndTheRequest.is_negative() == true) { 
      return false;
    }

    // Build the list of Class ID's.
    stdair::BookingClassIDList_T lClassIDList;
    
    const stdair::ClassObjectIDMapHolder_T& lClassObjectIDMapHolder =
      iTravelSolution.getClassObjectIDMapHolder();
    const stdair::FareOptionStruct& lChosenFareOption =
      iTravelSolution.getChosenFareOption ();
    const stdair::ClassList_StringList_T& lClassPath =
      lChosenFareOption.getClassPath(); 
    const stdair::SegmentPath_T& lSegmentPath =
      iTravelSolution.getSegmentPath();
    stdair::ClassList_StringList_T::const_iterator itClassKeyList =
      lClassPath.begin();
    for (stdair::ClassObjectIDMapHolder_T::const_iterator itClassObjectIDMap =
           lClassObjectIDMapHolder.begin();
         itClassObjectIDMap != lClassObjectIDMapHolder.end();
         ++itClassObjectIDMap, ++itClassKeyList) {
      const stdair::ClassObjectIDMap_T& lClassObjectIDMap = *itClassObjectIDMap;
      
      // TODO: Remove this hard-coded part.
      std::ostringstream ostr;
      const stdair::ClassList_String_T& lClassList = *itClassKeyList;
      assert (lClassList.empty() == false);

      ostr << lClassList.at(0);
      const stdair::ClassCode_T lClassCode (ostr.str());

      stdair::ClassObjectIDMap_T::const_iterator itClassID =
        lClassObjectIDMap.find (lClassCode);
      assert (itClassID != lClassObjectIDMap.end());
      const stdair::BookingClassID_T& lClassID = itClassID->second;

      lClassIDList.push_back (lClassID);

      
    }
    
    // Create the cancellation.
    stdair::CancellationStruct lCancellationStruct (lSegmentPath,
                                                    lClassIDList,
                                                    iPartySize,
                                                    lCancellationTime);
    
    stdair::CancellationPtr_T lCancellation_ptr =
      boost::make_shared<stdair::CancellationStruct> (lCancellationStruct);

    // Create an event structure
    stdair::EventStruct lEventStruct (stdair::EventType::CX, lCancellation_ptr);
    ioEventStruct = lEventStruct;
    
    return true;
  }
  
  // //////////////////////////////////////////////////////////////////////
  void DemandManager::
  buildSampleBom (SEVMGR::SEVMGR_ServicePtr_T ioSEVMGR_ServicePtr,
                  stdair::RandomGeneration& ioSharedGenerator,
                  const POSProbabilityMass_T& iPOSProbMass) {
    // Sanity check
    assert (ioSEVMGR_ServicePtr != NULL);

    //
    ArrivalPatternCumulativeDistribution_T lDTDProbDist;
    lDTDProbDist.insert(ArrivalPatternCumulativeDistribution_T::value_type(-330,
                                                                           0));
    lDTDProbDist.insert(ArrivalPatternCumulativeDistribution_T::value_type(-150,
                                                                           0.1));
    lDTDProbDist.insert(ArrivalPatternCumulativeDistribution_T::value_type(-92,
                                                                           0.2));
    lDTDProbDist.insert(ArrivalPatternCumulativeDistribution_T::value_type(-55,
                                                                           0.3));
    lDTDProbDist.insert(ArrivalPatternCumulativeDistribution_T::value_type(-34,
                                                                           0.4));
    lDTDProbDist.insert(ArrivalPatternCumulativeDistribution_T::value_type(-21,
                                                                           0.5));
    lDTDProbDist.insert(ArrivalPatternCumulativeDistribution_T::value_type(-12,
                                                                           0.6));
    lDTDProbDist.insert(ArrivalPatternCumulativeDistribution_T::value_type(-6,
                                                                           0.7));
    lDTDProbDist.insert(ArrivalPatternCumulativeDistribution_T::value_type(-3,
                                                                           0.8));
    lDTDProbDist.insert(ArrivalPatternCumulativeDistribution_T::value_type(-1,
                                                                           0.9));
    lDTDProbDist.insert(ArrivalPatternCumulativeDistribution_T::value_type(0,
                                                                           1.0));

    //
    ChannelProbabilityMassFunction_T lChannelProbDist;
    lChannelProbDist.insert (ChannelProbabilityMassFunction_T::value_type ("DF",
                                                                           0.0));
    lChannelProbDist.insert (ChannelProbabilityMassFunction_T::value_type ("DN",
                                                                           0.0));
    lChannelProbDist.insert (ChannelProbabilityMassFunction_T::value_type ("IF",
                                                                           0.0));
    lChannelProbDist.insert (ChannelProbabilityMassFunction_T::value_type ("IN",
                                                                           1.0));

    //
    TripTypeProbabilityMassFunction_T lTripProbDist;
    lTripProbDist.insert (TripTypeProbabilityMassFunction_T::value_type ("RO",
                                                                         0.0));
    lTripProbDist.insert (TripTypeProbabilityMassFunction_T::value_type ("RI",
                                                                         0.0));
    lTripProbDist.insert (TripTypeProbabilityMassFunction_T::value_type ("OW",
                                                                         1.0));

    //
    StayDurationProbabilityMassFunction_T lStayProbDist;
    lStayProbDist.insert(StayDurationProbabilityMassFunction_T::value_type(0,
                                                                           0.1));
    lStayProbDist.insert(StayDurationProbabilityMassFunction_T::value_type(1,
                                                                           0.1));
    lStayProbDist.insert(StayDurationProbabilityMassFunction_T::value_type(2,
                                                                           .15));
    lStayProbDist.insert(StayDurationProbabilityMassFunction_T::value_type(3,
                                                                           .15));
    lStayProbDist.insert(StayDurationProbabilityMassFunction_T::value_type(4,
                                                                           .15));
    lStayProbDist.insert(StayDurationProbabilityMassFunction_T::value_type(5,
                                                                           .35));

    //
    FrequentFlyerProbabilityMassFunction_T lFFProbDist;
    lFFProbDist.insert(FrequentFlyerProbabilityMassFunction_T::value_type("P",
                                                                          0.1));
    lFFProbDist.insert(FrequentFlyerProbabilityMassFunction_T::value_type("G",
                                                                          0.01));
    lFFProbDist.insert(FrequentFlyerProbabilityMassFunction_T::value_type("S",
                                                                          0.09));
    lFFProbDist.insert(FrequentFlyerProbabilityMassFunction_T::value_type("M",
                                                                          0.4));
    lFFProbDist.insert(FrequentFlyerProbabilityMassFunction_T::value_type("N",
                                                                          0.4));

    //
    ValueOfTimeContinuousDistribution_T lTimeValueProbDist;
    lTimeValueProbDist.insert(ValueOfTimeContinuousDistribution_T::value_type(15,
                                                                              0));
    lTimeValueProbDist.insert(ValueOfTimeContinuousDistribution_T::value_type(60,
                                                                              1));

    /*===================================================================================*/

    // Key of the demand stream
    const stdair::AirportCode_T lSINOrigin ("SIN");
    const stdair::AirportCode_T lBKKDestination ("BKK");
    const stdair::Date_T lDepDate (2010, 2, 8);
    const stdair::CabinCode_T lCabin ("Y");

    //
    const DemandStreamKey lSINBKKDemandStreamKey (lSINOrigin, lBKKDestination, lDepDate,
                                                  lCabin);

    // DEBUG
    // STDAIR_LOG_DEBUG ("Demand stream key: " << lDemandStreamKey.describe());
    
    // Distribution for the number of requests
    const stdair::MeanValue_T lSINBKKDemandMean (60.0);
    const stdair::StdDevValue_T lSINBKKDemandStdDev (4.0);
    const DemandDistribution lSINBKKDemandDistribution (lSINBKKDemandMean, lSINBKKDemandStdDev);
    
    // Seed
    const stdair::RandomSeed_T& lSINBKKRequestDateTimeSeed =
      generateSeed (ioSharedGenerator);
    const stdair::RandomSeed_T& lSINBKKDemandCharacteristicsSeed =
      generateSeed (ioSharedGenerator);

    
    //
    POSProbabilityMassFunction_T lSINBKKPOSProbDist;    
    lSINBKKPOSProbDist.insert (POSProbabilityMassFunction_T::value_type ("SIN", 1.0));
    lSINBKKPOSProbDist.insert (POSProbabilityMassFunction_T::value_type ("BKK", 0.0)); 
        
    //
    PreferredDepartureTimeContinuousDistribution_T lSINPrefDepTimeProbDist;
    lSINPrefDepTimeProbDist.
      insert (PreferredDepartureTimeContinuousDistribution_T::value_type (6 * stdair::HOUR_CONVERTED_IN_SECONDS, 0));
    lSINPrefDepTimeProbDist.
      insert (PreferredDepartureTimeContinuousDistribution_T::value_type (8 * stdair::HOUR_CONVERTED_IN_SECONDS,
                                                                          0.7));
    lSINPrefDepTimeProbDist.
      insert (PreferredDepartureTimeContinuousDistribution_T::value_type (10 * stdair::HOUR_CONVERTED_IN_SECONDS,
                                                                          0.8));
    lSINPrefDepTimeProbDist.
      insert (PreferredDepartureTimeContinuousDistribution_T::value_type (12 * stdair::HOUR_CONVERTED_IN_SECONDS,
                                                                          0.9));
    lSINPrefDepTimeProbDist.
      insert (PreferredDepartureTimeContinuousDistribution_T::value_type (14 * stdair::HOUR_CONVERTED_IN_SECONDS,
                                                                          1.0));    

    //
    const stdair::WTP_T lSINBKKWTP (400.0);
    const stdair::ChangeFeesRatio_T lChangeFees (0.5);
    const stdair::Disutility_T lChangeFeeDisutility (50);
    const stdair::NonRefundableRatio_T lNonRefundable (0.5);
    const stdair::Disutility_T lNonRefundableDisutility (50);


    // Delegate the call to the dedicated command
    DemandStream& lSINBKKDemandStream = 
      createDemandStream (ioSEVMGR_ServicePtr, lSINBKKDemandStreamKey, lDTDProbDist,
                          lSINBKKPOSProbDist, lChannelProbDist, lTripProbDist,
                          lStayProbDist, lFFProbDist,
                          lChangeFees, lChangeFeeDisutility,
                          lNonRefundable, lNonRefundableDisutility,
                          lSINPrefDepTimeProbDist,
                          lSINBKKWTP, lTimeValueProbDist,
                          lSINBKKDemandDistribution,
                          ioSharedGenerator.getBaseGenerator(),
                          lSINBKKRequestDateTimeSeed,
                          lSINBKKDemandCharacteristicsSeed, iPOSProbMass);

    // Calculate the expected total number of events for the current
    // demand stream
    const stdair::NbOfRequests_T& lSINBKKExpectedNbOfEvents =
      lSINBKKDemandStream.getMeanNumberOfRequests();

    /*===================================================================================*/

    // Key of the demand stream
    const stdair::AirportCode_T lBKKOrigin ("BKK");
    const stdair::AirportCode_T lHKGDestination ("HKG");

    //
    const DemandStreamKey lBKKHKGDemandStreamKey (lBKKOrigin, lHKGDestination, lDepDate,
                                                  lCabin);

    // DEBUG
    // STDAIR_LOG_DEBUG ("Demand stream key: " << lDemandStreamKey.describe());
    
    // Distribution for the number of requests
    const stdair::MeanValue_T lBKKHKGDemandMean (60.0);
    const stdair::StdDevValue_T lBKKHKGDemandStdDev (4.0);
    const DemandDistribution lBKKHKGDemandDistribution (lBKKHKGDemandMean, lBKKHKGDemandStdDev);
    
    // Seed
    const stdair::RandomSeed_T& lBKKHKGRequestDateTimeSeed =
      generateSeed (ioSharedGenerator);
    const stdair::RandomSeed_T& lBKKHKGDemandCharacteristicsSeed =
      generateSeed (ioSharedGenerator);

    
    //
    POSProbabilityMassFunction_T lBKKHKGPOSProbDist;    
    lBKKHKGPOSProbDist.insert (POSProbabilityMassFunction_T::value_type ("BKK", 1.0));
    lBKKHKGPOSProbDist.insert (POSProbabilityMassFunction_T::value_type ("HKG", 0.0)); 
        
    //
    PreferredDepartureTimeContinuousDistribution_T lBKKPrefDepTimeProbDist;
    lBKKPrefDepTimeProbDist.
      insert (PreferredDepartureTimeContinuousDistribution_T::value_type (8 * stdair::HOUR_CONVERTED_IN_SECONDS, 0));
    lBKKPrefDepTimeProbDist.
      insert (PreferredDepartureTimeContinuousDistribution_T::value_type (10 * stdair::HOUR_CONVERTED_IN_SECONDS,
                                                                          0.2));
    lBKKPrefDepTimeProbDist.
      insert (PreferredDepartureTimeContinuousDistribution_T::value_type (1 * stdair::HOUR_CONVERTED_IN_SECONDS,
                                                                          0.6));
    lBKKPrefDepTimeProbDist.
      insert (PreferredDepartureTimeContinuousDistribution_T::value_type (14 * stdair::HOUR_CONVERTED_IN_SECONDS,
                                                                          0.8));
    lBKKPrefDepTimeProbDist.
      insert (PreferredDepartureTimeContinuousDistribution_T::value_type (16 * stdair::HOUR_CONVERTED_IN_SECONDS,
                                                                          1.0));    

    //
    const stdair::WTP_T lBKKHKGWTP (400.0);


    // Delegate the call to the dedicated command
    DemandStream& lBKKHKGDemandStream = 
      createDemandStream (ioSEVMGR_ServicePtr, lBKKHKGDemandStreamKey, lDTDProbDist,
                          lBKKHKGPOSProbDist, lChannelProbDist, lTripProbDist,
                          lStayProbDist, lFFProbDist,
                          lChangeFees, lChangeFeeDisutility,
                          lNonRefundable, lNonRefundableDisutility,
                          lBKKPrefDepTimeProbDist,
                          lBKKHKGWTP, lTimeValueProbDist,
                          lBKKHKGDemandDistribution,
                          ioSharedGenerator.getBaseGenerator(),
                          lBKKHKGRequestDateTimeSeed,
                          lBKKHKGDemandCharacteristicsSeed, iPOSProbMass);

    // Calculate the expected total number of events for the current
    // demand stream
    const stdair::NbOfRequests_T& lBKKHKGExpectedNbOfEvents =
      lBKKHKGDemandStream.getMeanNumberOfRequests();

    /*===================================================================================*/

    // Key of the demand stream

    //
    const DemandStreamKey lSINHKGDemandStreamKey (lSINOrigin, lHKGDestination, lDepDate,
                                                  lCabin);

    // DEBUG
    // STDAIR_LOG_DEBUG ("Demand stream key: " << lDemandStreamKey.describe());
    
    // Distribution for the number of requests
    const stdair::MeanValue_T lSINHKGDemandMean (60.0);
    const stdair::StdDevValue_T lSINHKGDemandStdDev (4.0);
    const DemandDistribution lSINHKGDemandDistribution (lSINHKGDemandMean, lSINHKGDemandStdDev);
    
    // Seed
    const stdair::RandomSeed_T& lSINHKGRequestDateTimeSeed =
      generateSeed (ioSharedGenerator);
    const stdair::RandomSeed_T& lSINHKGDemandCharacteristicsSeed =
      generateSeed (ioSharedGenerator);

    
    //
    POSProbabilityMassFunction_T lSINHKGPOSProbDist;    
    lSINHKGPOSProbDist.insert (POSProbabilityMassFunction_T::value_type ("SIN", 1.0));
    lSINHKGPOSProbDist.insert (POSProbabilityMassFunction_T::value_type ("HKG", 0.0)); 
        
    //
    const stdair::WTP_T lSINHKGWTP (750.0);


    // Delegate the call to the dedicated command
    DemandStream& lSINHKGDemandStream = 
      createDemandStream (ioSEVMGR_ServicePtr, lSINHKGDemandStreamKey, lDTDProbDist,
                          lSINHKGPOSProbDist, lChannelProbDist, lTripProbDist,
                          lStayProbDist, lFFProbDist, 
                          lChangeFees, lChangeFeeDisutility,
                          lNonRefundable, lNonRefundableDisutility,
                          lSINPrefDepTimeProbDist,
                          lSINHKGWTP, lTimeValueProbDist, lSINHKGDemandDistribution,
                          ioSharedGenerator.getBaseGenerator(),
                          lSINHKGRequestDateTimeSeed,
                          lSINHKGDemandCharacteristicsSeed, iPOSProbMass);

    // Calculate the expected total number of events for the current
    // demand stream
    const stdair::NbOfRequests_T& lSINHKGExpectedNbOfEvents =
      lSINHKGDemandStream.getMeanNumberOfRequests();

    /*===================================================================================*/

    /**
     * Initialise the progress statuses, specific to the booking request type
     */
    const stdair::NbOfRequests_T lExpectedTotalNbOfEvents =
      lSINBKKExpectedNbOfEvents + lBKKHKGExpectedNbOfEvents + lSINHKGExpectedNbOfEvents;
    ioSEVMGR_ServicePtr->addStatus (stdair::EventType::BKG_REQ,
                                    lExpectedTotalNbOfEvents);
  }

}

