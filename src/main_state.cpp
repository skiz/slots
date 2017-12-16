#include "engine.h"
#include "main_state.h"

MainState MainState::state;

void MainState::Init(Engine* e) {
  engine_ = e;

  facade_ = e->assets->LoadSurface("facade.png");
}

void MainState::Cleanup() {
}

void MainState::Pause() {
  engine_->audio->PauseMusic();
}

void MainState::Resume() {
  engine_->audio->ResumeMusic();
}

void MainState::Update() {}

void MainState::Draw() {}

