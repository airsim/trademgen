#ifndef __TRADEMGEN_EVENTQUEUE_HPP
#define __TRADEMGEN_EVENTQUEUE_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// TraDemGen
#include <BasTypes.hpp>
#include <EventStruct.hpp>

namespace TRADEMGEN {
  
  /** Event queue. */
  class EventQueue {
  
  public:
    typedef std::pair<Datetime_T, EventStruct> EventListElement_T;
    typedef std::multimap<Datetime_T, EventStruct> EventList_T;
  
  public:
    // ///////////// Getters ///////////
    // /** Get the event type */
    //const EventType_T& getEventType () const {
    //  return _eventType;
    //}
    
  public:
    // ////////// Constructors and destructors /////////
    /** Constructor. */
    EventQueue (const Datetime_T&);
  
    /** Destructor. */
    virtual ~EventQueue ();
  
  public:
    // ////////// Business methods /////////
    /** Pop event */
    EventStruct * popEvent ();
    
    /** Add event */
    void addEvent (const EventStruct&);
    
    /** Is queue done */
    const bool isQueueDone () const;
    
    
    // ////////// Debug methods /////////
    /** Queue size */
    const Count_T getQueueSize () const;
    
    /** Is queue empty */
    const bool isQueueEmpty () const;
    
    /** Get position of current */
    const Count_T getPositionOfCurrent () const;
  
  private:
    /** Default constructors. */
    EventQueue (const EventQueue&);

  private:
    // ////////// Attributes //////////
    
    /** Event list */
    EventList_T _eventList;
    
    /** Pointer to current event */
    EventList_T::iterator _currentEvent;

  };

}
#endif // __TRADEMGEN_EVENTQUEUE_HPP
