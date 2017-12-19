#ifndef MAIN_STATE_H
#define MAIN_STATE_H

#include "state.h"
#include "sdl_ttf.h"
#include "ui_button.h"

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
    void UpdatePaid(const unsigned int &amount);
    void UpdateBet(const unsigned int &amount);
    void UpdateLines(const unsigned int &amount);
    void UpdateTotal(const unsigned int &amount);
    void RenderCredits();
    void RenderPaid();
    void RenderBet();
    void RenderLines();
    void RenderTotal();
    void UpdateText(const char* text);
    void RenderMessageText();
    void SetupButtons();
  protected:
    MainState() {}
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

    int credit_width_, credit_height_;
    int paid_width_, paid_height_;
    int bet_width_, bet_height_;
    int lines_width_, lines_height_;
    int text_width_, text_height_;
    int total_width_, total_height_;

    UIButton *maxBtn, *betBtn, *spinBtn, *cashBtn;
    UIButton *linesBtn, *helpBtn, *paysBtn;
};


#endif
