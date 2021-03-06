#ifndef __TRADEMGEN_TRADEMGEN_SERVICE_HPP
#define __TRADEMGEN_TRADEMGEN_SERVICE_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// StdAir
#include <stdair/stdair_basic_types.hpp>
#include <stdair/stdair_demand_types.hpp>
#include <stdair/stdair_maths_types.hpp>
#include <stdair/stdair_json.hpp>
#include <stdair/stdair_service_types.hpp>
#include <stdair/basic/DemandGenerationMethod.hpp>
#include <stdair/bom/BookingRequestTypes.hpp>
#include <stdair/bom/EventTypes.hpp>
#include <stdair/bom/EventStruct.hpp>
// SEvMgr
#include <sevmgr/SEVMGR_Types.hpp>
// TraDemGen
#include <trademgen/TRADEMGEN_Types.hpp>

// Forward declarations
namespace stdair {  
  class BomRoot;
  struct ProgressStatusSet;
  struct BasLogParams;
  struct BasDBParams;
  struct BookingRequestStruct;
  struct DemandCharacteristics;
  struct DemandDistribution;
  struct EventStruct;
  struct TravelSolutionStruct;
}

namespace TRADEMGEN {

  /// Forward declarations
  class TRADEMGEN_ServiceContext; 
  struct DemandStreamKey;
  
  /**
   * @brief class holding the services related to Travel Demand Generation.
   */
  class TRADEMGEN_Service {
  public:
    // ////////////////// Constructors and Destructors //////////////////    
    /**
     * @brief Constructor.
     *
     * The initTrademgenService() method is called; see the corresponding
     * documentation for more details.
     *
     * A reference on an output stream is given, so that log outputs
     * can be directed onto that stream.
     *
     * Moreover, database connection parameters are given, so that a
     * session can be created on the corresponding database.
     *
     * @param const stdair::BasLogParams& Parameters for the output log stream.
     * @param const stdair::BasDBParams& Parameters for the database access.
     * @param const stdair::RandomSeed_T& Seed for the random generation.
     */
    TRADEMGEN_Service (const stdair::BasLogParams&, const stdair::BasDBParams&,
                       const stdair::RandomSeed_T&);

    /**
     * Constructor.
     *
     * The initTrademgenService() method is called; see the corresponding
     * documentation for more details.
     *
     * A reference on an output stream is given, so that log outputs
     * can be directed onto that stream.
     *
     * @param const stdair::BasLogParams& Parameters for the output log stream.
     * @param const stdair::RandomSeed_T& Seed for the random generation.
     */
    TRADEMGEN_Service (const stdair::BasLogParams&, const stdair::RandomSeed_T&);

    /**
     * Constructor.
     *
     * The initTrademgenService() method is called; see the
     * corresponding documentation for more details.
     *
     * Moreover, as no reference on any output stream is given,
     * neither any database access parameter is given, it is assumed
     * that the StdAir log service has already been initialised with
     * the proper log output stream by some other methods in the calling
     * chain (for instance, when the TRADEMGEN_Service is itself being
     * initialised by another library service such as TVLSIM_Service).
     *
     * @param stdair::STDAIR_ServicePtr_T Handler on the STDAIR_Service. 
     * @param SEVMGR::SEVMGR_ServicePtr_T Handler on the SEVMGR_Service.
     * @param const stdair::RandomSeed_T& Seed for the random generation.
     */
    TRADEMGEN_Service (stdair::STDAIR_ServicePtr_T, 
		       SEVMGR::SEVMGR_ServicePtr_T, 
		       const stdair::RandomSeed_T&);
    
    /**
     * Parse the demand input file.
     *
     * The CSV file, describing the parameters of the demand to be generated
     * for the simulator, is parsed and instantiated in memory accordingly.
     *
     * @param const DemandFilePath& Filename of the input demand file.
     */
    void parseAndLoad (const DemandFilePath&);

    /**
     * Destructor.
     */
    ~TRADEMGEN_Service();


  public:
    // ////////////////// Business support methods //////////////////    
    /**
     * Build a sample BOM tree, made of a single DemandStream object.
     *
     * As of now (March 2011), it corresponds to:
     * <ul>
     *  <li>Origin: SIN</li>
     *  <li>Destination: BKK</li>
     *  <li>Preferred departure date: 2011-02-14</li>
     *  <li>Preferred cabin: Y (Economy)</li>
     *  <li>POS distribution:
     *    <ul>
     *      <li>BKK: 30%</li>
     *      <li>SIN: 70%</li>
     *    </ul>
     *  </li>
     *  <li>Channel distribution:
     *    <ul>
     *      <li>Direct Offline:   10%</li>
     *      <li>Direct Online:    30%</li>
     *      <li>Indirect Offline: 40%</li>
     *      <li>Indirect Online:  20%</li>
     *    </ul>
     *  </li>
     *  <li>Trip type distribution:
     *    <ul>
     *      <li>Outbound: 60%</li>
     *      <li>Inbound:  20%</li>
     *      <li>One-way:  20%</li>
     *    </ul>
     *  </li>
     *  <li>Arrival pattern distribution:
     *    <ul>
     *      <li>330 DTD:   0%</li>
     *      <li> 40 DTD:  20%</li>
     *      <li> 20 DTD:  60%</li>
     *      <li>  1 DTD: 100%</li>
     *    </ul>15:0, 60:1
     *  </li>
     *  <li>Stay duration distribution:
     *    <ul>
     *      <li>0 day:   10%</li>
     *      <li>1 day:   10%</li>
     *      <li>2 days:  15%</li>
     *      <li>3 days:  15%</li>
     *      <li>4 days:  15%</li>
     *      <li>5 days:  35%</li>
     *    </ul>
     *  </li>
     *  <li>Frequent flyer distribution:
     *    <ul>
     *      <li>Platinum:  1%</li>
     *      <li>Gold:      5%</li>
     *      <li>Silver:   15%</li>
     *      <li>Member:   30%</li>
     *      <li>No card:  49%</li>
     *    </ul>
     *  </li>
     *  <li>Preferred departure time (cumulative distribution):
     *    <ul>
     *      <li>6am:    0%</li>
     *      <li>7am:   10%</li>
     *      <li>9am:   30%</li>
     *      <li>5pm:   40%</li>
     *      <li>7pm:   80%</li>
     *      <li>8pm:   95%</li>
     *      <li>10pm: 100%</li>
     *    </ul>
     *  </li>
     *  <li>Value of time distribution:
     *    <ul>
     *      <li>15 min:   0%</li>
     *      <li>60 min: 100%</li>
     *    </ul>
     *  </li>
     *  <li>WTP: 200</li>
     *  <li>Number of requests: Normal (mu = 10.0, std_dev = 1.0)</li>
     *  <li>Change fee: 20; Non refundable; Saturday night stay</li>
     * </ul>
     */
    void buildSampleBom();

    /**
     * Clone the persistent BOM object.
     */
    void clonePersistentBom ();

    /**
     * Build all the complementary links in the given bom root object.
     * \note Do nothing for now.
     */
    void buildComplementaryLinks (stdair::BomRoot&); 

    /**
     * Build a sample booking request structure.
     *
     * As of now (March 2011), the sample booking request is made of the
     * following parameters:
     * <ul>
     *  <li>Return trip (inbound): LHR-SYD (POS: LHR, Channel: DN), </li>
     *  <li>Departing 10-JUN-2011 around 8:00, staying 7 days</li>
     *  <li>Requested on 15-MAY-2011 at 10:00</li>
     *  <li>Economy cabin, 3 persons, FF member</li>
     *  <li>WTP: 1000.0 EUR</li>
     *  <li>Dis-utility: 100.0 EUR/hour</li>
     * </ul>
     *
     * As of now (March 2011), the CRS-related booking request is made
     * of the following parameters:
     * <ul>
     *  <li>Return trip (inbound): SIN-BKK (POS: SIN, Channel: IN), </li>
     *  <li>Departing 30-JAN-2010 around 10:00, staying 7 days</li>
     *  <li>Requested on 22-JAN-2010 at 10:00</li>
     *  <li>Economy cabin, 3 persons, FF member</li>
     *  <li>WTP: 1000.0 EUR</li>
     *  <li>Dis-utility: 100.0 EUR/hour</li>
     * </ul>
     *
     * \see stdair::CmdBomManager for more details.
     *
     * @param const bool isForCRS Whether the sample booking request is for CRS.
     * @return BookingRequestStruct& Sample booking request structure.
     */
    stdair::BookingRequestStruct
    buildSampleBookingRequest (const bool isForCRS = false);

    /**
     * Display the list of airlines, as held within the sample database.
     */
    void displayAirlineListFromDB() const;

    /**
     * Get the expected number of events/requests to be generated for
     * all the demand streams.
     *
     * Calls the SEvMgr service with the same name
     * "getExpectedTotalNumberOfRequestsToBeGenerated", which computes
     * that number.
     *
     * \note That number usually corresponds to an expectation (i.e.,
     *       the mean value of a random distribution). The actual number
     *       will be drawn when calling the generateFirstRequests() method.
     *
     * @return const stdair::Count_T& Expected number of events to be
     *   generated.
     */
    const stdair::Count_T& getExpectedTotalNumberOfRequestsToBeGenerated() const;

    /**
     * Get the actual number of events/requests to be generated for
     * all the demand streams.
     *
     * Calls the SEvMgr service with the same name
     * "getActualTotalNumberOfRequestsToBeGenerated", which computes
     * that number.
     *
     * \note That number has been drawn when calling the
     *       generateFirstRequests() method.
     *
     * @return const stdair::Count_T& Expected number of events to be
     *   generated.
     */
    const stdair::Count_T& getActualTotalNumberOfRequestsToBeGenerated() const;

    /**
     * Check whether enough requests have already been generated for
     * the demand stream which corresponds to the given key.
     *
     * @param const DemandStreamKey& A string identifying uniquely the
     *   demand stream (e.g., "SIN-HND 2010-Feb-08 Y").
     * @param const stdair::DemandGenerationMethod&
     *        States whether the demand generation must be performed
     *        following the method based on statistic orders.
     *        The alternative method, while more "intuitive", is also a
     *        sequential algorithm.
     * @return bool Whether or not there are still events to be
     *   generated for that demand stream.
     */
    const bool
    stillHavingRequestsToBeGenerated (const stdair::DemandStreamKeyStr_T&,
                                      stdair::ProgressStatusSet&,
                                      const stdair::DemandGenerationMethod&) const;

    /**
     * Browse the list of demand streams and generate the first
     * request of each stream.
     *
     * @param const stdair::DemandGenerationMethod&
     *        States whether the demand generation must be performed
     *        following the method based on statistic orders.
     *        The alternative method, while more "intuitive", is also a
     *        sequential algorithm.
     * @return stdair::Count_T The expected total number of events to
     *         be generated
     */
    stdair::Count_T
    generateFirstRequests (const stdair::DemandGenerationMethod&) const;

    /**
     * Generate a request with the demand stream which corresponds to
     * the given key.
     *
     * @param const DemandStreamKey& A string identifying uniquely the
     *   demand stream (e.g., "SIN-HND 2010-Feb-08 Y").
     * @param const stdair::DemandGenerationMethod&
     *        States whether the demand generation must be performed
     *        following the method based on statistic orders.
     *        The alternative method, while more "intuitive", is also a
     *        sequential algorithm.
     * @return stdair::BookingRequestPtr_T (Boost) shared pointer on
     *   the booking request structure, which has just been created.
     */
    stdair::BookingRequestPtr_T
    generateNextRequest (const stdair::DemandStreamKeyStr_T&,
                         const stdair::DemandGenerationMethod&) const;

    /**
     * States whether a demand stream with the given key is used to
     * generate demand.
     *
     * @param const DemandStreamKey& A string identifying uniquely the
     *   demand stream (e.g., "SIN-HND 2010-Feb-08 Y").
     */
    bool hasDemandStream (const stdair::DemandStreamKeyStr_T&) const;

    /**
     * Pop the next coming (in time) event, and remove it from the
     * event queue thanks to the SEvMgr service.
     * <ul>
     *   <li>The next coming (in time) event corresponds to the event
     *     having the earliest date-time stamp. In other words, it is
     *     the first/front element of the event queue.</li>
     *   <li>That (first) event/element is then removed from the event
     *     queue</li>
     *   <li>The progress status is updated for the corresponding
     *     demand stream.</li>
     * </ul>
     *
     * @return stdair::EventStruct A copy of the event structure,
     *   which comes first in time from within the event queue.
     */
    stdair::ProgressStatusSet popEvent (stdair::EventStruct&) const;

    /**
     * States whether the event queue has reached the end.
     *
     * Calls the SEvMgr service with the same name "isQueueDone",
     * which states whether the event queue has reached the end.
     *
     * For now, that method states whether the event queue is empty.
     */
    bool isQueueDone() const;

    /**
     * Generate the potential cancellation event.
     */
    bool generateCancellation (const stdair::TravelSolutionStruct&,
                               const stdair::PartySize_T&,
                               const stdair::DateTime_T&,
                               const stdair::Date_T&) const;

    /**
     * Reset the context of the demand streams for another demand generation
     * without having to reparse the demand input file.
     */
    void reset() const;  

    /**
     * Get the overall progress status (for the whole event queue).
     */
    const stdair::ProgressStatus& getProgressStatus () const; 

    /**
     * Get the progress status for the given event type (e.g., booking
     * request, optimisation notification, schedule change, break point).
     */
    const stdair::ProgressStatus& getProgressStatus (const stdair::EventType::EN_EventType&) const;
    
  public:
    // //////////////// Export support methods /////////////////
    /**
     * Dispatch the JSon command string to the SEvMgr service.
     * (Only SEvMgr has json export commands for now).
     * 
     * @param const stdair::JSONString& Input string which contained the JSon
     *        command string.
     * @return std::string Output string in which the asking objects are
     *         logged/dumped with a JSon format.
     */
    std::string jsonHandler (const stdair::JSONString&) const;

  public:
    // //////////////// Display support methods /////////////////
    /**
     * Recursively display (dump in the returned string) the objects
     * of the BOM tree.
     *
     * @return std::string Output string in which the BOM tree is
     *        logged/dumped.
     */
    std::string csvDisplay() const;

    /**
     * Display (dump in the returned string) the event list of the event queue.
     *
     * @return std::string Output string in which the events are
     *        logged/dumped.
     */
    std::string list () const;

    /**
     * Display (dump in the returned string) the events with the given type
     *
     * @return std::string Output string in which the events are
     *        logged/dumped.
     */
    std::string list (const stdair::EventType::EN_EventType&) const;

    /**
     * Display (dump in the returned string) the demand streams
     *
     * @return std::string Output string in which the demand streams are
     *        logged/dumped.
     */
    std::string displayDemandStream () const;


  private:
    // ////////////////// Constructors and Destructors //////////////////    
    /**
     * Default Constructors, which must not be used.
     */
    TRADEMGEN_Service();
    
    /**
     * Default copy constructor.
     */
    TRADEMGEN_Service (const TRADEMGEN_Service&);

    /**
     * Initialise the STDAIR service (including the log service).
     *
     * A reference on the root of the BOM tree, namely the BomRoot
     * object, is stored within the service context for later use.
     *
     * @param const stdair::BasLogParams& Parameters for the output
     *        log stream.
     * @param const stdair::BasDBParams& Parameters for the database
     *        access.
     */
    stdair::STDAIR_ServicePtr_T initStdAirService (const stdair::BasLogParams&,
                                                   const stdair::BasDBParams&);
    
    /**
     * Initialise the STDAIR service (including the log service).
     *
     * A reference on the root of the BOM tree, namely the BomRoot
     * object, is stored within the service context for later use.
     *
     * @param const stdair::BasLogParams& Parameters for the output
     *        log stream.
     */
    stdair::STDAIR_ServicePtr_T initStdAirService (const stdair::BasLogParams&);
    
    /**
     * Initialise the SEvMgr service.
     */
    void initSEVMGRService();
    
    /**
     * Attach the STDAIR service (holding the log and database services) to
     * the TRADEMGEN_Service.
     *
     * @param stdair::STDAIR_ServicePtr_T Reference on the STDAIR service.
     * @param const bool State whether or not TraDemGen owns the STDAIR
     *        service resources.
     */
    void addStdAirService (stdair::STDAIR_ServicePtr_T ioSTDAIR_ServicePtr,
                           const bool iOwnStdairService); 

    /**
     * Attach the SEVMGR service to the TRADEMGEN_Service.
     *
     * @param SEVMGR::SEVMGR_ServicePtr_T Reference on the SEVMGR service.
     */
    void addSEVMGRService (SEVMGR::SEVMGR_ServicePtr_T ioSEVMGR_ServicePtr);
    
    /**
     * Initialise the (TraDemGen) service context (i.e., the
     * TRADEMGEN_ServiceContext object).
     *
     * @param const stdair::RandomSeed_T& Seed for the random generation.
     */
    void initServiceContext (const stdair::RandomSeed_T&);

    /**
     * Initialise.
     *
     * Nothing is being done at that stage. The buildSampleBom() method may
     * be called later.
     */
    void initTrademgenService();

    /**
     * Finalise.
     */
    void finalise();

    
  private:
    // ///////// Service Context /////////
    /**
     * TraDemGen context.
     */
    TRADEMGEN_ServiceContext* _trademgenServiceContext;
  };

}
#endif // __TRADEMGEN_TRADEMGEN_SERVICE_HPP
