#ifndef MAIN_STATE_H
#define MAIN_STATE_H

#include "state.h"
#include "sdl_ttf.h"

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
    void UpdateCredits(const unsigned int &amount);
    void RenderCredits();
  protected:
    MainState() {}
  private:
    Engine* engine_; // not owned
    static MainState state;
    //SDL_Texture* screen_;
    SDL_Texture* bg_;
    SDL_Texture* credits_;
    
    TTF_Font* credit_font_;
};


#endif
