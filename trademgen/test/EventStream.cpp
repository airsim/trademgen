// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
// TraDemGen
#include <EventStream.hpp>

namespace TRADEMGEN {

  // //////////////////////////////////////////////////////////////////////
  EventStream::EventStream (const RandomSeed_T& iSeed)
    : _key (""), _rate (0.0), _randomGenerator (iSeed) {
    init();
  }

  // //////////////////////////////////////////////////////////////////////
  EventStream::~EventStream () {
  }

  // //////////////////////////////////////////////////////////////////////
  void EventStream::init() {
  }
  
  // //////////////////////////////////////////////////////////////////////
  void EventStream::addEvent (const EventTime_T& iTime) {
	_events.push_back (iTime);
  }

  // //////////////////////////////////////////////////////////////////////
  const EventTime_T& EventStream::getLatestTime () const {
    assert (_events.empty() == false);
	return _events.back ();
  }

  // //////////////////////////////////////////////////////////////////////
  void EventStream::generateNext() {
	EventTime_T lLatest = 0.0;
    
	if (_events.empty() == false) {
      lLatest = EventStream::getLatestTime();
	}

    //
	const Duration_T lInterArrival = _randomGenerator.generateInstance (_rate);
	const EventTime_T lNext = lLatest + lInterArrival;

    //
	addEvent (lNext);
  }

  // //////////////////////////////////////////////////////////////////////
  void EventStream::displayAllEvents (std::ostream& ioStream) const {
	unsigned int idx = 1;
	for (EventList_T::const_iterator itEvent = _events.begin();
         itEvent != _events.end(); ++itEvent, ++idx) {
      ioStream << "Event " << idx << ": " << *itEvent << std::endl;
	}
  }

}
