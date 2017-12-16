#include "event_manager.h"

void EventManager::Init(Engine* e) {
  engine_ = e;
}

void EventManager::Cleanup() {
}

void EventManager::Register(int ref, SDL_Event* event) {
}

void Fire(SDL_Event* event) {
}
