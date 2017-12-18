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
    void UpdatePaid(const unsigned int &amount);
    void RenderCredits();
    void RenderPaid();

    void UpdateSpinText();
    void RenderSpinBtn();
    void RenderMaxBtn();
    void RenderCashBtn();

    void UpdateText(const char* text);
    void RenderText();
  protected:
    MainState() {}
  private:
    Engine* engine_; // not owned
    static MainState state;
    //SDL_Texture* screen_;
    SDL_Texture* bg_;
    SDL_Texture* credits_;
    SDL_Texture* paid_;
    SDL_Texture* text_;
    SDL_Texture* spin_btn_;
    SDL_Texture* spin_text_;
    SDL_Texture* max_btn_;
    SDL_Texture* cash_btn_;
    TTF_Font* credit_font_;
    TTF_Font* font_;

    int credit_width_, credit_height_;
    int paid_width_, paid_height_;
    int text_width_, text_height_;
    int spin_text_width_, spin_text_height_;
};


#endif
