#include "event_manager.h"
#include <vector>
#include <unordered_map>

// Events should be handled here, and states should register slots
// for events handled here.  SDL events should be handled by event
// manager outside of the state, and simply trigger here.

// can we decouple the keyscan and have engine.HandleEvents wrap
// up the SDL input check?  much cleaner

void EventManager::Init() {
}

void EventManager::Cleanup() {
}

void EventManager::KeyPressed(int sdlkey) {
  if (map_.count(sdlkey) > 0) {
    for (auto& e : map_[sdlkey]) {
      KeyPress.emit(e);
    }
  }
}

void EventManager::AddMapping(int sdlkey, KeyEvent ref) {
  if (map_.count(sdlkey) == 0) {
    map_[sdlkey] = std::vector<KeyEvent>();
  }
  map_[sdlkey].push_back(ref);
}
