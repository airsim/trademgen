#ifndef _TRADEMGEN_EVENT_STREAM_HPP
#define _TRADEMGEN_EVENT_STREAM_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <list>
#include <ostream>
// TraDemGen
#include <BasTypes.hpp>
#include <RandomGeneration.hpp>

namespace TRADEMGEN {

  /** Class modeling a stream of events. */
  class EventStream {
  public:
    // ///////////// Getters ///////////
    /** Get the key. */
    const Key_T& getKey() const {
      return _key;
    }
	
    /** Get the event arrival rate. */
    const Rate_T& getRate () const {
      return _rate;
    }

    // ///////////// Setters ///////////
    /** Set the key */
    void setKey (const Key_T& iKey) {
      _key = iKey;
    }

    /** Set the event arrival rate. */
    void setRate (const Rate_T& iRate) {
      _rate = iRate;
    }


  public:
    // /////////////// Business Methods //////////
    /** Generate the next event. */
    void generateNext();
    

  public:
    // ////////////// Display Support Methods //////////
    /** Display all events */
    void displayAllEvents (std::ostream&) const;

    
  public:
    // ////////// Constructors and destructors /////////
    /** Constructor by default */
    EventStream (const RandomSeed_T&);

    /** Destructor */
    virtual ~EventStream ();

  private:
    /** Default constructors. */
    EventStream ();
    EventStream (const EventStream&);

    /** Initialisation. */
    void init();
  
  private:
    /** Add an event in the dedicated list. */
    void addEvent (const Event_T&);
	
    /** Get the time of the latest event. */
    const EventTime_T& getLatestTime () const;

  
  private:
    // ////////// Attributes //////////
    /** Key */
    Key_T _key;
  
    /** Event arrival rate */
    Rate_T _rate;
  
    /** List of events */
    EventList_T _events;

    /** Associated random generator. */
    RandomGeneration _randomGenerator;
  };

}
#endif // _TRADEMGEN_EVENTSTREAM_HPP
