#ifndef BIG_WIN_STATE_H
#define BIG_WIN_STATE_H

#include "state.h"
#include "SDL_ttf.h"
#include "system_event.h"
#include "sprite_emitter.h"

class BigWinState : public State {
  public:
    void Init(Engine* e);
    void Reset();
    void Cleanup();
    void Pause();
    void Resume();
    void Update();
    void Draw();
    void HandleEvent(SystemEvent e);
    static BigWinState* Instance() {
      return &state;
    }
  protected:
    int event_bind_ = -1;
    int continues_ = 0;
    bool exiting_ = false;
    int inc_amount_ = 10;
    int frame_inc_ = 2;
    int dir_ = 0;
    int frame_ = 0;
    int size_ = 200;
    TTF_Font* font_;
    unsigned int total_ = 0;
    unsigned int amount_ = 0;
    SDL_Texture* bg_;
    SDL_Texture* big_win_;
    SDL_Texture* counter_;
    SpriteEmitter* coin_emitter_;
    BigWinState() {}
    bool initialized = false;
  private:
    Engine* engine_; // not owned
    static BigWinState state;
};


#endif

