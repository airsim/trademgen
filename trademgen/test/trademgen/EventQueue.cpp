// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// TraDemGen
#include <BasTypes.hpp>
#include <EventQueue.hpp>
#include <EventStruct.hpp>

namespace TRADEMGEN {
  
  // //////////////////////////////////////////////////////////////////////
  EventQueue::EventQueue (const Datetime_T& iEarliestDatetime) {
    // Earliest time is necessary in order to set _currentEvent "before" any event
    // Without it, _currentEvent would need to be set to the first event, the first
    // an event is added
    EventStruct lEarliestEvent (iEarliestDatetime);
    _eventList.insert(EventListElement_T (iEarliestDatetime, lEarliestEvent));
    _currentEvent = _eventList.begin ();
  }
  
  // //////////////////////////////////////////////////////////////////////
  EventQueue::~EventQueue () {
  }
  
  // //////////////////////////////////////////////////////////////////////
  EventStruct * EventQueue::popEvent () {
    EventStruct * oEventStruct_ptr = NULL;
    if (!isQueueDone ()) {
      ++_currentEvent;
      oEventStruct_ptr = &(_currentEvent->second);
    }
    return oEventStruct_ptr;
  }
  
  // //////////////////////////////////////////////////////////////////////
  void EventQueue::addEvent (const EventStruct& iEventStruct) {
    Datetime_T lEventDatetime = iEventStruct.getEventDatetime ();
    _eventList.insert( std::pair<Datetime_T, EventStruct> (lEventDatetime, iEventStruct) );
  }
  
  // //////////////////////////////////////////////////////////////////////
  const bool EventQueue::isQueueDone () const {
    bool oFlagDone = false;
    if (_currentEvent == _eventList.end ()) {
      oFlagDone = true;
    } else {
      EventList_T::iterator lNextEvent = _currentEvent;
      ++lNextEvent;
      if (lNextEvent == _eventList.end ()) {
        oFlagDone = true;
      }
    }
    return oFlagDone;
  }
  
  // //////////////////////////////////////////////////////////////////////
  const Count_T EventQueue::getQueueSize () const {
    if (_eventList.empty ()) {
      return 0;
    } else {
      return _eventList.size ();
    }
  }
  
  // //////////////////////////////////////////////////////////////////////
  const bool EventQueue::isQueueEmpty () const {
    return _eventList.empty ();
  }
  
  // //////////////////////////////////////////////////////////////////////
  const Count_T EventQueue::getPositionOfCurrent () const {
    int i = 0;
    bool flag = false;
    for (EventList_T::const_iterator it = _eventList.begin ();
        it != _eventList.end (); ++it) {
      if (it == _currentEvent) {
        flag = true;
        break;
      }
      ++i;
    }
    if (flag) {
      return i;
    } else {
      return -1;
    }
  }
  
}
