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

void BlankState::Update() {
}

void BlankState::Draw() {
}
