#ifndef BIG_WIN_STATE_H
#define BIG_WIN_STATE_H

#include "state.h"
#include "SDL_ttf.h"

class BigWinState : public State {
  public:
    void Init(Engine* e);
    void Cleanup();
    void Pause();
    void Resume();
    void Update();
    void Draw();
    void StoreBackground();
    static BigWinState* Instance() {
      return &state;
    }
  protected:
    int dir_ = 0;
    int frame_ = 0;
    int size_ = 250;
    TTF_Font* font_;
    unsigned int total_ = 0;
    unsigned int amount_ = 0;
    SDL_Texture* bg_;
    SDL_Texture* big_win_;
    SDL_Texture* counter_;
    BigWinState() {}

  private:
    Engine* engine_; // not owned
    static BigWinState state;
};


#endif

