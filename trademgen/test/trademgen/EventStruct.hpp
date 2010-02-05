#ifndef __TRADEMGEN_EVENTSTRUCT_HPP
#define __TRADEMGEN_EVENTSTRUCT_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// TraDemGen
#include <BasTypes.hpp>

namespace TRADEMGEN {
  
  /** Forward declaration */
  class Request;
  
  /** Event struct. */
  struct EventStruct {
  
  public:
    // ///////////// Getters ///////////
    /** Get the event type */
    const EventType_T& getEventType () const {
      return _eventType;
    }
    
    /** Get the event datetime */
    const Datetime_T& getEventDatetime () const {
      return _eventDatetime;
    }
    
    /** Get the pointer Request event. */
    const Request* getPointerToRequestEvent () const {
      return _ptrRequest;
    }
    
  public:
    // ////////// Constructors and destructors /////////
    /** Constructor. */
    EventStruct (const EventType_T&, const Datetime_T&, Request * const);
    EventStruct (const Datetime_T&);
    
    /** Copy constructor. */
    EventStruct (const EventStruct&);
  
    /** Destructor. */
    virtual ~EventStruct ();
  
  private:
    /** Default constructors. */
    EventStruct ();
    

  private:
    // ////////// Attributes //////////
    
    /** Event type */
    EventType_T _eventType;
    
    /** Event datetime */
    Datetime_T _eventDatetime;
    
    /** Pointer to Request event */
    Request * _ptrRequest;

  };

}
#endif // __TRADEMGEN_EVENTSTRUCT_HPP
