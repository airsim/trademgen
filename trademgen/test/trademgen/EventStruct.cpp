// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// TraDemGen
#include <BasTypes.hpp>
#include <EventStruct.hpp>
#include <Request.hpp>

namespace TRADEMGEN {
  
  // //////////////////////////////////////////////////////////////////////
  EventStruct::EventStruct (const EventType_T& iEventType, const Datetime_T& iDatetime, Request * const pRequest)
  : _eventType (iEventType),
    _eventDatetime (iDatetime),
    _ptrRequest (NULL) {
    if (iEventType == "Request") {
      _ptrRequest = pRequest;
    }
  }
  
  // //////////////////////////////////////////////////////////////////////
  EventStruct::EventStruct (const Datetime_T& iDatetime) 
  :  _eventType (""),
     _eventDatetime (iDatetime),
     _ptrRequest (NULL) {
  }
  
  
  // //////////////////////////////////////////////////////////////////////
  EventStruct::EventStruct (const EventStruct& iEventStruct) {
    _eventType = iEventStruct._eventType;
    _eventDatetime = iEventStruct._eventDatetime;
    _ptrRequest = iEventStruct._ptrRequest;
  }
  
  // //////////////////////////////////////////////////////////////////////
  EventStruct::~EventStruct () {
    _ptrRequest = NULL;
    //_ptrDummy = NULL;
  }

}
