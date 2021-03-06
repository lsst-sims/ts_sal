// ============================================================================
//
// = CONTEXT
//   SALLV Generic Client for LabVIEW
//
// = FILENAME
//   EventManager.h
//
// = AUTHOR
//   Nicolas Leclercq
//
// ============================================================================

#ifndef _EVENT_MANAGER_H_
#define _EVENT_MANAGER_H_

//=============================================================================
// DEPENDENCIEs
//=============================================================================
#include <map>

//=============================================================================
// CLASS : EventCallback
//=============================================================================
class EventCallback : public SALLV::CallBack 
{
  friend class EventManager;

public:
  virtual void push_event (SALLV::EventData* _evt_data);
  //- SALLV actual callback method

  const std::string& subsys_name (void);
  //- returns the associated subsys name 

  const std::string& attribute_name (void);
  //- returns the associated attribute name 

  LVUserEventRef lv_evt_refnum (void);
  //- returns the associated lv event identifier

private:
  EventCallback (const std::string& dev, 
                 const std::string& attr, 
                 LVUserEventRef lv_evt_refnum);
  //- ctor

  virtual ~EventCallback (void);
  //- dtor

  void push_lv_event (SALLV::EventData* _da)
       throw (SALLV::DevFailed);
  //- converts a SALLV event into a labview event

  std::string dev_;
  //- dev-name

  std::string attr_;
  //- attr-name

  LVUserEventRef lv_evt_refnum_;
  //- lv event identifier

  int SALLV_evt_refnum_;
  //- SALLV event identifier
};

//=============================================================================
// TYPEDEFs
//=============================================================================
typedef std::map<LVUserEventRef, EventCallback*> EventCallbackMap;
//- EventCallbackMap: a STL map of <EventData*> indexed by LabVIEW evt refnum.
typedef EventCallbackMap::iterator EventCallbackMapIterator;
//- EventCallbackMapIterator: an EventCallbackMap iterator;

//=============================================================================
// CLASS : EventManager [SINGLETON]
//=============================================================================
class EventManager 
{
  friend class ObjectManager;

public:

  static EventManager* instance (void);
  //- Returns the unique instance of <EventManager>.
  
  void subscribe (const std::string& dev, 
                  const std::string& attr,
                  LVUserEventRef lv_evt_refnum,
                  SALLV::EventType evt_type = SALLV::PERIODIC_EVENT,
                  const std::vector<std::string>& filters = EventManager::default_periodic_filter)
      throw (SALLV::DevFailed);
  //- Subscribes to the specified SALLV event, returns its identifier.

  void unsubscribe (LVUserEventRef lv_evt_refnum)
      throw (SALLV::DevFailed);
  //- Unbscribes to the specified SALLV event.

  bool events_have_been_used (void);
  //- Returns true if SALLV events have been used, false otherwise.

private:

  static int init (void);
  //- Instanciates the singleton.
  
  static void cleanup (void);
  //- Releases the singleton.

  static EventManager* instance_;
  //- The unique instance of <EventManager>.

  static std::vector<std::string> default_periodic_filter;
  //- default event filters (empty)

  void clear (void);
  //- clear the EventCallback repository

  bool subscription_exists (LVUserEventRef lv_evt_refnum);
  //- returns true if a subscription exists for the specified lv evt refnum
  //- returns false otherwise

  omni_mutex lock_;
  //- make this EventManager thread-safe

  EventCallbackMap evt_cb_map_;
  //- the actual repository (see typedef above).

  bool events_have_been_used_;
  //- true if SALLV events have been used, false otherwise.

  // = Disallow these operations (except for ObjectManager).
  //-------------------------------------------------------
  EventManager();
  EventManager (const EventManager&);
  virtual ~EventManager();
  void operator= (const EventManager&); 
};

//=============================================================================
// #DEFINEs
//=============================================================================
#define EVT_MANAGER (EventManager::instance())

//=============================================================================
// INLINED CODE
//=============================================================================
#if defined (_LV_INLINE_)
  #include "EventManager.i"
#endif 

#endif // _EVENT_MANAGER_H_

   
   
