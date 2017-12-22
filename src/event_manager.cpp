#include <vector>
#include <unordered_map>
#include <iostream>
#include <map>

#include "event_manager.h"

std::map<SystemEvent, int> EventManager::bettingEvents = {
  {BET_MAX,1},
  {BET_UP,1},
  {BET_DOWN,1},
  {LINE_UP,1},
  {LINE_DOWN,1},
  {CASH_OUT,1},
};

void EventManager::Init() {
  AddMapping(SDLK_ESCAPE, POP_STATE);  // for debugging
  AddMapping(SDLK_1, COIN_IN);
  AddMapping(SDLK_2, BILL_IN);
  AddMapping(SDLK_3, LINE_UP);
  AddMapping(SDLK_4, LINE_DOWN);
  AddMapping(SDLK_5, BET_UP);
  AddMapping(SDLK_6, BET_DOWN);
  AddMapping(SDLK_RETURN, BET_MAX);
  AddMapping(SDLK_SPACE, SPIN);
}

void EventManager::DisableBetting() {
  betting_enabled_ = false;
}

void EventManager::EnableBetting() {
  betting_enabled_ = true;
}

void EventManager::Cleanup() {}

void EventManager::HandleKeyPress(int sdlkey) {
  if (map_.count(sdlkey) > 0) {
    for (auto& e : map_[sdlkey]) {
      if (!betting_enabled_) {
	std::map<SystemEvent, int>::iterator it = bettingEvents.find(e);
	if(it != bettingEvents.end()) {
	    std::cout << "Ignoring betting event" << std::endl;
	    return;
	}
      }
      SystemSignal.emit(e);
    }
  }
}

void EventManager::AddMapping(int sdlkey, SystemEvent ref) {
  if (map_.count(sdlkey) == 0) {
    map_[sdlkey] = std::vector<SystemEvent>();
  }
  map_[sdlkey].push_back(ref);
}

void EventManager::HandleEvents() {
  SDL_Event event;
  if (SDL_PollEvent(&event)) {
    switch (event.type) {
      case SDL_MOUSEBUTTONDOWN:
	break;
      case SDL_QUIT:
	SystemSignal.emit(QUIT);
	break;
      case SDL_KEYDOWN:
	HandleKeyPress(event.key.keysym.sym);
      default:
	break;
    }
  }
}
