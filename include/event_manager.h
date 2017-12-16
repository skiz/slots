#ifndef EVENT_MANAGER_H
#define EVENT_MANAGER_H

#include "engine.h"
#include "SDL.h"

/**
 * The EventManager handles key presses and other SDL input events through 
 * customizable mappings.
 *
 * It takes a SDL_Event, and fires defined events within the current state.
 *
 * ie:
 *  Register(COIN_IN, SDLK_1);
 *  Fire(SDLK_1);
 *
 *  The current state will receive a COIN_IN event.
 *
 */

#define COIN_IN 1
#define BILL_IN 2
#define CASH_OUT_BUTTON 3
#define SERVICE_BUTTON 4
#define SPIN_BUTTON 5
#define LINE_BUTTON 6
#define MAX_BET_BUTTON 7
#define LINE_BUTTON_1 8
#define LINE_BUTTON_2 9
#define LINE_BUTTON_3 10
#define LINE_BUTTON_4 11
#define LINE_BUTTON_5 12
#define CREDIT_BUTTON_1 13
#define CREDIT_BUTTON_2 14
#define CREDIT_BUTTON_3 15
#define CREDIT_BUTTON_4 16
#define CREDIT_BUTTON_5 17

class EventManager {
  public:
    void Init(Engine* e);
    void Cleanup();
    void Register(int ref, SDL_Event* event);
    void Fire(SDL_Event* event);
  private:
    Engine* engine_; // not owned
    
};

#endif
