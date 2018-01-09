#ifndef PAY_STATE_H
#define PAY_STATE_H

#include "state.h"
#include "SDL_ttf.h"
#include "system_event.h"

// The state which shows animated payment when a standard win is triggered
class PayState : public State {
  public:
    void Init(Engine* e);
    void Reset();
    void Cleanup();
    void Pause();
    void Resume();
    void Update();
    void Draw();
    void DrawPaid(const unsigned int amount);
    void DrawCredits(const unsigned int amount);
    void DrawPaylines();
    void HandleEvent(SystemEvent e);
    static PayState* Instance() {
      return &state;
    }
  protected:
    int event_bind_ = -1;
    int continues_ = 0;
    int inc_amount_ = 10;
    int frame_inc_ = 2;
    int frame_ = 0;
    unsigned int total_ = 0;
    unsigned int amount_ = 0;
    PayState() {}
  private:
    Reel *reel_; // not owned
    Engine *engine_; // not owned
    static PayState state;
    SDL_Texture* blank_;
    std::vector<int> winning_paylines_; // winning paylines
    int num_wins_ = 0;          // number of winning paylines
    int payline_index_ = 0;     // payline we are currently showing
};

#endif
