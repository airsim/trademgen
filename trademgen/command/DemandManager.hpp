#ifndef __TRADEMGEN_CMD_DEMANDMANAGER_HPP
#define __TRADEMGEN_CMD_DEMANDMANAGER_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// StdAir
#include <stdair/stdair_basic_types.hpp>
#include <stdair/basic/RandomGeneration.hpp>
#include <stdair/basic/DemandGenerationMethod.hpp>
#include <stdair/bom/BookingRequestTypes.hpp>
#include <stdair/command/CmdAbstract.hpp>
// SEvMgr
#include <sevmgr/SEVMGR_Types.hpp>
// TraDemGen
#include <trademgen/TRADEMGEN_Types.hpp>
#include <trademgen/basic/DemandCharacteristicsTypes.hpp>
#include <trademgen/bom/DemandStreamKey.hpp>

// Forward declarations
namespace stdair {
  struct EventStruct;
  struct ProgressStatusSet;
  struct TravelSolutionStruct;
}

namespace TRADEMGEN {

  // Forward declarations
  struct DemandDistribution;
  struct DemandStruct;
  class DemandStream;
  namespace DemandParserHelper {
    struct doEndDemand;
  }

  /**
   * @brief Utility class for Demand and DemandStream objects.
   */
  class DemandManager : public stdair::CmdAbstract {
    friend struct DemandParserHelper::doEndDemand;
    friend class TRADEMGEN_Service;
    
  private:
    // //////// Business methodes //////////
    /**
     * Generate a sample BOM tree, made of a single DemandStream object.
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
    static void buildSampleBomStd (SEVMGR::SEVMGR_ServicePtr_T,
                                   stdair::RandomGeneration&,
                                   const POSProbabilityMass_T&);

    // Demand sample bom for partnerships study.
    static void buildSampleBom (SEVMGR::SEVMGR_ServicePtr_T,
                                stdair::RandomGeneration&,
                                const POSProbabilityMass_T&);

    /**
     * Generate the Demand objects corresponding to the given
     * Flight-Period, and add them to the given EventQueue
     * thanks to the SEvMgr service handler.
     *
     * @param SEVMGR::SEVMGR_ServicePtr_T Pointer on the SEvMgr service
     * handler.
     * @param stdair::RandomGeneration& Boost uniform generator.
     */
    static void createDemandCharacteristics (SEVMGR::SEVMGR_ServicePtr_T,
                                             stdair::RandomGeneration&,
                                             const POSProbabilityMass_T&,
                                             const DemandStruct&);

    /**
     * Generate the random seed for the demand characteristic
     * distributions.
     *
     * @param stdair::RandomGeneration& Boost uniform generator.
     * @return stdair::RandomSeed_T The generated seed.
     */
    static stdair::RandomSeed_T generateSeed (stdair::RandomGeneration&);

    /**
     * Create a demand stream object and add it into the BOM tree.
     *
     * <br>That method returns the expected number of events to be
     * generated by that demand stream. That number is expected, not
     * actual, because it corresponds to an average. The actual number
     * (of events to be generated) is then randomly drawn, according
     * to a normal distribution following the parameters (mean and
     * standard deviation) given as parameter within the
     * DemandDistribution structure.
     *
     * @param SEVMGR::SEVMGR_ServicePtr_T Pointer on the SEvMgr service
     * handler in order to add the demand stream object to the BOM tree.
     * @param const DemandStreamKey& A string identifying uniquely the
     *   demand stream (e.g., "SIN-HND 2010-Feb-08 Y").
     * @param const DemandDistribution& Parameters (mean, standard
     *   deviation) of the probability distribution for the demand
     *   generation.
     * @return DemandStream& The newly created DemandStream object.
     */
    static DemandStream&
    createDemandStream (SEVMGR::SEVMGR_ServicePtr_T,
                        const DemandStreamKey&,
                        const ArrivalPatternCumulativeDistribution_T&,
                        const POSProbabilityMassFunction_T&,
                        const ChannelProbabilityMassFunction_T&,
                        const TripTypeProbabilityMassFunction_T&,
                        const StayDurationProbabilityMassFunction_T&,
                        const FrequentFlyerProbabilityMassFunction_T&,
                        const stdair::ChangeFeesRatio_T&,
                        const stdair::Disutility_T&,
                        const stdair::NonRefundableRatio_T&,
                        const stdair::Disutility_T&,
                        const PreferredDepartureTimeContinuousDistribution_T&,
                        const stdair::WTP_T&,
                        const ValueOfTimeContinuousDistribution_T&,
                        const DemandDistribution&, stdair::BaseGenerator_T&,
                        const stdair::RandomSeed_T&,
                        const stdair::RandomSeed_T&,
                        const POSProbabilityMass_T&);

    /**
     * State whether there are still events to be generated for
     * the demand stream, for which the key is given as parameter.
     *
     * @param SEVMGR::SEVMGR_ServicePtr_T Pointer on the SEvMgr service
     * handler.
     * @param const DemandStreamKey& A string identifying uniquely the
     *   demand stream (e.g., "SIN-HND 2010-Feb-08 Y").
     * @param stdair::ProgressStatusSet
     * @param const stdair::DemandGenerationMethod&
     *        States whether the demand generation must be performed
     *        following the method based on statistic orders.
     *        The alternative method, while more "intuitive", is also a
     *        sequential algorithm.
     * @return bool Whether or not there are still some events to be
     *   generated.
     */
    static const bool
    stillHavingRequestsToBeGenerated (SEVMGR::SEVMGR_ServicePtr_T,
                                      const stdair::DemandStreamKeyStr_T&,
                                      stdair::ProgressStatusSet&,
                                      const stdair::DemandGenerationMethod&);

    /**
     * Generate the first event/booking request for every demand
     * stream.
     *
     * @param SEVMGR::SEVMGR_ServicePtr_T Pointer on the SEvMgr service
     * handler.
     * @param stdair::RandomGeneration& Random generator.
     * @param const stdair::DemandGenerationMethod&
     *        States whether the demand generation must be performed
     *        following the method based on statistic orders.
     *        The alternative method, while more "intuitive", is also a
     *        sequential algorithm.
     * @return stdair::Count_T The actual total number of events to
     *         be generated, for all the demand stream.
     */
    static stdair::Count_T generateFirstRequests (SEVMGR::SEVMGR_ServicePtr_T,
                                                  stdair::RandomGeneration&,
                                                  const stdair::DemandGenerationMethod&);

    /**
     * Generate a request with the demand stream, for which the key is
     * given as parameter.
     *
     * The state of the random generator, passed as parameter, is
     * altered, reflecting the random generations made within that
     * method.
     *
     * /note In the output, the booking request has not been necessarily added
     * into the queue. Indeed, if the generated booking request date was
     * posterior to the booking request departure date (which is possible with
     * the poisson process method), then it has not been added to the queue.
     * Furthermore, when it occurs, we know the demand stream into question has
     * been fully generated.
     *
     * @param SEVMGR::SEVMGR_ServicePtr_T Pointer on the SEvMgr service
     * handler.
     * @param stdair::RandomGeneration& Random generator.
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
    static stdair::BookingRequestPtr_T
    generateNextRequest (SEVMGR::SEVMGR_ServicePtr_T, stdair::RandomGeneration&,
                         const stdair::DemandStreamKeyStr_T&,
                         const stdair::DemandGenerationMethod&);

    /**
     * Reset the context of the demand streams for another demand
     * generation without having to reparse the demand input file.
     *
     * @param SEVMGR::SEVMGR_ServicePtr_T Pointer on the SEvMgr service
     * handler.
     * @param stdair::BaseGenerator_T& The shared generator for the number
     *   of requests generation.
     */
    static void reset (SEVMGR::SEVMGR_ServicePtr_T, stdair::BaseGenerator_T&);

    /**
     * Generate the potential cancellation event.
     */
    static bool generateCancellation (stdair::RandomGeneration&,
                                      const stdair::TravelSolutionStruct&,
                                      const stdair::PartySize_T&,
                                      const stdair::DateTime_T&,
                                      const stdair::Date_T&,
                                      stdair::EventStruct& ioEventStruct);
  };

}
#endif // __TRADEMGEN_CMD_DEMANDMANAGER_HPP
