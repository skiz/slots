#ifndef EVENT_MANAGER_H
#define EVENT_MANAGER_H

#include "SDL.h"
#include <unordered_map>
#include <vector>
#include "signal.h"
#include "system_event.h"

/**
 * The EventManager handles key presses and other SDL input events through 
 * customizable mappings.
 *
 * It takes a SDL_Event, and fires defined events within the current state.
 *
 */
class EventManager {
  public:
    void Init();
    void Cleanup();
    void AddMapping(int sdlkey, SystemEvent ref);
    void HandleKeyPress(int sdlkey);
    void HandleEvents();
    void AddDefaultMappings();
    void DisableBetting();
    void EnableBetting();
    Signal<SystemEvent> SystemSignal;
    static std::map<SystemEvent, int> bettingEvents;
  private:
    bool betting_enabled_ = 1;
    std::unordered_map<int, std::vector<SystemEvent>> map_;
};

#endif
