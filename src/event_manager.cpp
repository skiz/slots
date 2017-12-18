#include <vector>
#include <unordered_map>

#include "event_manager.h"

void EventManager::Init() {
  AddMapping(SDLK_ESCAPE, POP_STATE);
  AddMapping(SDLK_1, COIN_IN);
  AddMapping(SDLK_2, BILL_IN);
  AddMapping(SDLK_SPACE, SPIN);
}

void EventManager::Cleanup() {}

void EventManager::HandleKeyPress(int sdlkey) {
  if (map_.count(sdlkey) > 0) {
    for (auto& e : map_[sdlkey]) {
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
