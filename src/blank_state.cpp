#include "engine.h"
#include "blank_state.h"

BlankState BlankState::state;

void BlankState::Init(Engine* e) {
  engine_ = e;
}

void BlankState::Cleanup() {
}

void BlankState::Pause() {
  engine_->audio->PauseMusic();  
}

void BlankState::Resume() {
  engine_->audio->ResumeMusic();  
}

void BlankState::Update(Engine* e) {
}

void BlankState::Draw(Engine* e) {
}


void BlankState::HandleEvents(Engine* e) {
  SDL_Event event;
  if (SDL_PollEvent(&event)) {
    switch (event.type) {
      case SDL_QUIT:
	e->Quit();
	break;
      case SDL_KEYDOWN:
	switch (event.key.keysym.sym) {
	  case SDLK_ESCAPE:
	    e->PopState();
	    break;
	}
	break;
    }
  }
}
