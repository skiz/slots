#ifndef MAIN_STATE_H
#define MAIN_STATE_H

#include "state.h"
#include "SDL_ttf.h"
#include "ui_button.h"
#include "credits_changed_message.h"

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

    void OnCreditsChanged(const CreditsChangedMessage &m);
    void OnMoneyInserted(const unsigned int &amount);

    void UpdateCredits(const unsigned int &amount);
    void UpdatePaid(const unsigned int &amount);
    void UpdateBet(const unsigned int &amount);
    void UpdateLines(const unsigned int &amount);
    void UpdateTotal(const unsigned int &amount);
    void StartSpin();
    void StopReels();
    void UpdateReels();
    void RenderCredits();
    void RenderPaid();
    void RenderBet();
    void RenderLines();
    void RenderTotal();
    void RenderSymbols();
    void LoadReelSymbols();
    void UpdateText(const char* text);
    void RenderMessageText();
    void SetupButtons();
    void BigWin(const unsigned int &amount);
    void Win(const unsigned int &amount);
    void SpinStarted();
    void SpinStopped();
    static Uint32 StopColumn(Uint32 interval, void *param);
    void ScheduleStop(int col, int ms);
    void StopNext();
  protected:
    MainState() {}

    void LoadSymbol(Symbol type, const char* filename);
  private:
    Engine* engine_; // not owned
    static MainState state;
    SDL_Texture* bg_;
    SDL_Texture* credits_;
    SDL_Texture* paid_;
    SDL_Texture* bet_;
    SDL_Texture* lines_;
    SDL_Texture* text_;
    SDL_Texture* total_;
    TTF_Font* credit_font_;
    TTF_Font* button_font_;
    SDL_Color button_font_color_;
    TTF_Font* font_;

    Reel *reel_; // not owned

    //SDL_Texture* reel_texture_; // temporary texture for animation

    SDL_Texture* gray_;

    std::map<Symbol, SDL_Texture*> reel_symbols_;
    std::map<int, Symbol> reel_position_;

    int credit_width_, credit_height_;
    int paid_width_, paid_height_;
    int bet_width_, bet_height_;
    int lines_width_, lines_height_;
    int text_width_, text_height_;
    int total_width_, total_height_;

    UIButton *maxBtn, *betBtn, *spinBtn, *cashBtn;
    UIButton *linesBtn, *helpBtn, *paysBtn;

    bool spinning_[5];
    int  vertical_offset_[5];
    int  spin_speed_ = 45;

    SDL_TimerID stop_timer_[5];
};


#endif
