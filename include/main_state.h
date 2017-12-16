#ifndef MAIN_STATE_H
#define MAIN_STATE_H

#include "state.h"

class MainState : public State {
  public:
    void Init(Engine* e);
    void Cleanup();
    void LoadAssets();
    void Pause();
    void Resume();
    void Update();
    void Draw();
    void HandleEvent(SystemEvent e);
    static MainState* Instance() {
      return &state;
    }
  protected:
    MainState() {}

  private:
    Engine* engine_; // not owned
    static MainState state;
    SDL_Texture* bg_;
};


#endif
