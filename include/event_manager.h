#ifndef EVENT_MANAGER_H
#define EVENT_MANAGER_H

#include "SDL.h"
#include <unordered_map>
#include <vector>
#include "signal.h"

/**
 * The EventManager handles key presses and other SDL input events through 
 * customizable mappings.
 *
 * It takes a SDL_Event, and fires defined events within the current state.
 *
 */

enum SystemEvent : int {
  COIN_IN = 1,
  BILL_IN,
  CASH_OUT,
  SERVICE,
  SPIN,
  MAX_BET,
  LINE_1,
  LINE_2,
  LINE_3,
  LINE_4,
  LINE_5,
  BET_1,
  BET_2,
  BET_3,
  BET_4,
  BET_5,
  AUX_1,
  AUX_2,
  AUX_3,
  AUX_4,
  AUX_5,
  VOL_UP,
  VOL_DOWN,
  QUIT,
  POP_STATE
};

class EventManager {
  public:
    void Init();
    void Cleanup();
    void AddMapping(int sdlkey, SystemEvent ref);
    void HandleKeyPress(int sdlkey);
    void HandleEvents();
    Signal<SystemEvent> SystemSignal;
  private:
    std::unordered_map<int, std::vector<SystemEvent>> map_;
};

#endif
