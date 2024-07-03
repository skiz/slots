#include <iostream>
#include "engine.h"
#include "main_state.h"
#include "SDL_ttf.h"
#include "pay_state.h"
#include "big_win_state.h"
#include "signal.h"
#include "highlighter.h"
#include "credits_changed_message.h"

MainState MainState::state;

void MainState::Init(Engine *e)
{
  engine_ = e;
  LoadAssets();

  reel_ = engine_->accounting->GetReel(); // Why does accounting have the reel?

  reel_->GenerateSymbols(5, 3); // So we are the reel commander now?

  // subscribe to system events
  // engine_->events->SystemSignal.connect_member(this, &MainState::HandleEvent);

  /*
   * TODO: All of this shit should be emitted from a single EventManager
   * and everyone that wants to send messages should call EventManager::Send()
   * This should remove the dependency on accounting for messaging.
   *
   */
  engine_->accounting->CreditsChanged.connect_member(this, &MainState::OnCreditsChanged);
  engine_->accounting->MoneyInserted.connect_member(this, &MainState::OnMoneyInserted);
  engine_->accounting->LinesUpdated.connect_member(this, &MainState::OnLinesUpdated);
  engine_->accounting->BetUpdated.connect_member(this, &MainState::OnBetUpdated);
  engine_->accounting->ReelsUpdate.connect_member(this, &MainState::UpdateReels);
  engine_->accounting->BigWin.connect_member(this, &MainState::BigWin);
  engine_->accounting->Win.connect_member(this, &MainState::Win);
  engine_->accounting->SpinStarted.connect_member(this, &MainState::SpinStarted);
  engine_->accounting->SpinStopped.connect_member(this, &MainState::SpinStopped);
  engine_->accounting->SpinComplete.connect_member(this, &MainState::SpinComplete);

  engine_->accounting->EmitCreditsChanged(); // Kindly trigger please..

  engine_->events->EnableBetting(); // vs EmitSystemEvent(ENABLE_BETTING); ?

  UpdateText("Play max credits for a bigger BONUS!");

  // TODO: move this to it's own clearly defined method
  for (int i = 0; i < 5; ++i)
  {
    spinning_[i] = false;
    vertical_offset_[i] = 0;
  }
}

void MainState::Cleanup()
{
  // TTF_CloseFont(credit_font_);
  // TTF_CloseFont(font_);

  for (auto s : signal_bindings_)
  {
    engine_->events->SystemSignal.disconnect(s);
  }
}

void MainState::HandleEvent(SystemEvent e)
{
  switch (e)
  {
  case UPDATE_REELS:
    // TODO: I assume this is just left over debug?
    std::cout << "UPDATE REELS!" << std::endl;
  default:
    break;
  }
}

void MainState::SpinStarted()
{
  // Stop any bonus bells
  engine_->audio->StopSound(4);

  UpdateText("Good Luck!");
  engine_->audio->PlaySound("/main/sound/spin.wav");
  engine_->audio->PlayMusic("/main/sound/reels.wav");
  engine_->audio->ResumeMusic();
  for (int i = 0; i < 5; i++)
  {
    spinning_[i] = true;
    ScheduleStop(i, 2000 + i * 200);
  }
}

void MainState::SpinStopped()
{
  engine_->audio->PauseMusic();
  for (int i = 0; i < 5; i++)
  {
    StopNext();
  }
}

void MainState::SpinComplete()
{
  if (BonusTriggered())
  {
    engine_->audio->PlaySound("/main/sound/bell.ogg", 4);
    std::cout << "BONUS!" << std::endl;
  }
}

void MainState::ScheduleStop(int col, int ms)
{
  if (stop_timer_[col])
  {
    SDL_RemoveTimer(stop_timer_[col]);
    stop_timer_[col] = 0;
  }
  stop_timer_[col] = SDL_AddTimer(ms, &MainState::StopColumn, this);
}

Uint32 MainState::StopColumn(Uint32 /*interval*/, void *param)
{
  ((MainState *)param)->StopNext();
  return 0L;
}

// Stops the first non-stopped column
void MainState::StopNext()
{
  for (int i = 0; i < 5; i++)
  {
    if (spinning_[i])
    {
      spinning_[i] = false;
      SDL_RemoveTimer(stop_timer_[i]);
      engine_->audio->PlaySound("/main/sound/spin.wav");
      if (i == 4)
      {
        engine_->events->SystemSignal.emit(REELS_STOPPED);
        engine_->audio->StopSound(3);
        return;
      }

      if (stop_timer_[i])
      {
        SDL_RemoveTimer(stop_timer_[i]);
        stop_timer_[i] = 0;
      }

      // If there is a chance of a bonus, reschedule
      // timers to spin longer and act differently.
      if (HasPossibleBonus(i))
      {
        engine_->audio->StopSound(3);
        engine_->audio->PlaySound("/main/sound/bonus_2.ogg", 3);
        for (int z = i + 1; z < 5; ++z)
        {
          if (stop_timer_[z])
          {
            SDL_RemoveTimer(stop_timer_[z]);
            stop_timer_[z] = 0;
          }
          ScheduleStop(z, 2000 + z * 200);
        }
        ScheduleStop(i, 4000);
      }
      else
      {
        engine_->audio->StopSound(3);
      }

      return;
    }
  }
}

bool MainState::HasPossibleBonus(int col_index)
{
  // Highlighter::DumpBits(bonus_highlight_[col_index]);
  return bonus_highlight_[col_index].count() > 0;
}

bool MainState::BonusTriggered()
{
  return bonus_highlight_[4].count() > 0;
}

void MainState::LoadAssets()
{
  engine_->assets->Mount("assets/main", "/main");

  SDL_Surface *ss = engine_->assets->LoadSurface("/main/images/b.png");
  bg_ = SDL_CreateTextureFromSurface(engine_->renderer, ss);
  SDL_FreeSurface(ss);

  // Add gray overlay texture
  SDL_Surface *s;
  s = SDL_CreateRGBSurface(0, 2000, 2000, 32, 0, 0, 0, 0);
  SDL_Rect rect = {0, 0, 2000, 2000};
  SDL_FillRect(s, &rect, SDL_MapRGB(s->format, 205, 203, 206));
  gray_ = SDL_CreateTextureFromSurface(engine_->renderer, s);
  SDL_FreeSurface(s);

  font_ = engine_->assets->LoadFont("/main/fonts/sans.ttf", 40);
  credit_font_ = engine_->assets->LoadFont("/main/fonts/digital.ttf", 65);
  button_font_ = engine_->assets->LoadFont("/main/fonts/sans.ttf", 20);
  LoadReelSymbols();

  SetupButtons();
}

void MainState::LoadReelSymbols()
{
  engine_->assets->Mount("assets/reels", "/reels");
  LoadSymbol(CHERRY, "/reels/cherry.png");
  LoadSymbol(BAR, "/reels/bar.png");
  LoadSymbol(DOUBLE_BAR, "/reels/bars.png");
  LoadSymbol(TEN, "/reels/banana.png");
  LoadSymbol(JACK, "/reels/heart.png");
  LoadSymbol(QUEEN, "/reels/crown.png");
  LoadSymbol(KING, "/reels/ruby.png");
  LoadSymbol(ACE, "/reels/diamond.png");
  LoadSymbol(WILD, "/reels/clover.png");
  LoadSymbol(BONUS, "/reels/bonus.png");
  LoadSymbol(JACKPOT, "/reels/bigwin.png");
  LoadSymbol(ALT1, "/reels/gem.png");
  LoadSymbol(ALT2, "/reels/money.png");
  LoadSymbol(ALT3, "/reels/lemon.png");
  LoadSymbol(ALT4, "/reels/orange.png");
  LoadSymbol(ALT5, "/reels/melon.png");
  LoadSymbol(ALT6, "/reels/sberry.png");
  LoadSymbol(ALT7, "/reels/bell.png");
  LoadSymbol(ALT8, "/reels/grape.png");
  LoadSymbol(ALT9, "/reels/melon2.png");
}

void MainState::LoadSymbol(Symbol type, const char *filename)
{
  SDL_Surface *s = engine_->assets->LoadSurface(filename);
  reel_symbols_[type] = SDL_CreateTextureFromSurface(engine_->renderer, s);
  SDL_FreeSurface(s);
}

void MainState::SetupButtons()
{
  button_font_color_ = {0, 0, 0, 0}; // 255, 255, 255, 0};

  int by = 810;

  const char *btnFile = "/main/images/btn.png";

  cashBtn = new UIButton(engine_->renderer, btnFile);
  cashBtn->SetPosition(70, by);
  cashBtn->SetFont(button_font_, button_font_color_);
  cashBtn->SetText("CASHOUT");

  helpBtn = new UIButton(engine_->renderer, btnFile);
  helpBtn->SetPosition(220, by);
  helpBtn->SetFont(button_font_, button_font_color_);
  helpBtn->SetText("HELP");

  paysBtn = new UIButton(engine_->renderer, btnFile);
  paysBtn->SetPosition(220, by);
  paysBtn->SetFont(button_font_, button_font_color_);
  paysBtn->SetText("PAYLINES");

  linesBtn = new UIButton(engine_->renderer, btnFile);
  linesBtn->SetPosition(520, by);
  linesBtn->SetFont(button_font_, button_font_color_);
  linesBtn->SetText("LINES");

  betBtn = new UIButton(engine_->renderer, btnFile);
  betBtn->SetPosition(680, by);
  betBtn->SetFont(button_font_, button_font_color_);
  betBtn->SetText("BET");

  spinBtn = new UIButton(engine_->renderer, btnFile);
  spinBtn->SetPosition(1110, by);
  spinBtn->SetFont(button_font_, button_font_color_);
  spinBtn->SetText("SPIN");

  maxBtn = new UIButton(engine_->renderer, btnFile);
  maxBtn->SetPosition(1252, by);
  maxBtn->SetFont(button_font_, button_font_color_);
  maxBtn->SetText("BET MAX");
}

void MainState::StartSpin()
{
  for (int i = 0; i < 5; i++)
  {
    spinning_[i] = true;
  }
}

void MainState::StopReels()
{
  for (int i = 0; i < 5; i++)
  {
    spinning_[i] = false;
  }
}

void MainState::Pause()
{
  engine_->audio->PauseMusic();
  engine_->events->DisableBetting();
}

void MainState::Resume()
{
  engine_->events->EnableBetting();
}

// Reels were updated, lets render them.
void MainState::UpdateReels()
{
  bonus_highlight_ = Highlighter::GenerateFromReelSymbols(
      reel_->GetSymbols(), BONUS, engine_->accounting->Lines());
}

void MainState::BigWin(const unsigned int & /*amount*/)
{
  engine_->PushAsyncState(BigWinState::Instance());
}

void MainState::Win(const unsigned int & /*amount*/)
{
  engine_->PushAsyncState(PayState::Instance());
}

void MainState::Update()
{
  // This is where we will set up animations
}

void MainState::RenderCredits()
{
  int rw, rh;
  SDL_GetRendererOutputSize(engine_->renderer, &rw, &rh);
  SDL_Rect credit_pos;
  credit_pos.w = credit_width_;
  credit_pos.h = credit_height_;
  credit_pos.x = 270 - credit_width_;
  credit_pos.y = rh - credit_pos.h - 120;
  SDL_RenderCopy(engine_->renderer, credits_, NULL, &credit_pos);
}

void MainState::RenderBet()
{
  int rw, rh;
  SDL_GetRendererOutputSize(engine_->renderer, &rw, &rh);
  SDL_Rect bet_pos;
  bet_pos.w = bet_width_;
  bet_pos.h = bet_height_;
  bet_pos.x = 755 - bet_width_;
  bet_pos.y = rh - bet_pos.h - 120;
  SDL_RenderCopy(engine_->renderer, bet_, NULL, &bet_pos);
}

void MainState::RenderLines()
{
  int rw, rh;
  SDL_GetRendererOutputSize(engine_->renderer, &rw, &rh);
  SDL_Rect lines_pos;
  lines_pos.w = lines_width_;
  lines_pos.h = lines_height_;
  lines_pos.x = 600 - lines_width_;
  lines_pos.y = rh - lines_pos.h - 120;
  SDL_RenderCopy(engine_->renderer, lines_, NULL, &lines_pos);
}

void MainState::Draw()
{
  SDL_RenderCopy(engine_->renderer, gray_, NULL, NULL);
  RenderSymbols();
  SDL_RenderCopy(engine_->renderer, bg_, NULL, NULL);
  RenderCredits();
  RenderPaid();
  RenderBet();
  RenderLines();
  RenderTotal();
  RenderMessageText();
  maxBtn->Render();
  betBtn->Render();
  spinBtn->Render();
  cashBtn->Render();
  linesBtn->Render();
  helpBtn->Render();
  // paysBtn->Render();
}

void MainState::UpdateCredits(const unsigned int &amount)
{
  const char *text = std::to_string(amount).c_str();
  SDL_Surface *textSurface = NULL;
  SDL_Color textColor = {255, 25, 25, 0};
  TTF_SizeText(credit_font_, text, &credit_width_, &credit_height_);
  textSurface = TTF_RenderText_Blended(credit_font_, text, textColor);
  credits_ = SDL_CreateTextureFromSurface(engine_->renderer, textSurface);
  SDL_FreeSurface(textSurface);
}

// TODO: Figure out what to do here. PayStates are calling this directly.
void MainState::UpdatePaid(const unsigned int &amount)
{
  const char *text = std::to_string(amount).c_str();
  SDL_Surface *textSurface = NULL;
  SDL_Color textColor = {255, 25, 25, 0};
  TTF_SizeText(credit_font_, text, &paid_width_, &paid_height_);
  textSurface = TTF_RenderText_Blended(credit_font_, text, textColor);
  paid_ = SDL_CreateTextureFromSurface(engine_->renderer, textSurface);
  SDL_FreeSurface(textSurface);
}

void MainState::UpdateBet(const unsigned int &amount)
{
  const char *text = std::to_string(amount).c_str();
  SDL_Surface *textSurface = NULL;
  SDL_Color textColor = {255, 25, 25, 0};
  TTF_SizeText(credit_font_, text, &bet_width_, &bet_height_);
  textSurface = TTF_RenderText_Blended(credit_font_, text, textColor);
  bet_ = SDL_CreateTextureFromSurface(engine_->renderer, textSurface);
  SDL_FreeSurface(textSurface);
}

void MainState::UpdateLines(const unsigned int &amount)
{
  const char *text = std::to_string(amount).c_str();
  SDL_Surface *textSurface = NULL;
  SDL_Color textColor = {255, 25, 25, 0};
  TTF_SizeText(credit_font_, text, &lines_width_, &lines_height_);
  textSurface = TTF_RenderText_Blended(credit_font_, text, textColor);
  lines_ = SDL_CreateTextureFromSurface(engine_->renderer, textSurface);
  SDL_FreeSurface(textSurface);
}

void MainState::UpdateTotal(const unsigned int &amount)
{
  const char *text = std::to_string(amount).c_str();
  SDL_Surface *textSurface = NULL;
  SDL_Color textColor = {255, 25, 25, 0};
  TTF_SizeText(credit_font_, text, &total_width_, &total_height_);
  textSurface = TTF_RenderText_Blended(credit_font_, text, textColor);
  total_ = SDL_CreateTextureFromSurface(engine_->renderer, textSurface);
  SDL_FreeSurface(textSurface);
}

void MainState::RenderSymbols()
{
  // We only have the final symbols at the moment, so just render those.
  // TODO: put more random symbols on the reel for visual effect.
  // This will also need to be overridden for near miss states, as we may
  // want to show a bunch of bonuses or wilds spinning by...
  int max_height = 220 * 3;

  SDL_Rect pos;

  for (int c = 0; c < 5; c++)
  {
    if (spinning_[c])
    {
      vertical_offset_[c] += spin_speed_;
      if (vertical_offset_[c] >= max_height)
      {
        vertical_offset_[c] = 0;
      }
    }
    else
    {
      vertical_offset_[c] = 0;
    }
  }

  for (auto s : reel_->GetSymbols())
  {
    int column = s.first % 5;
    pos.w = 220;
    pos.h = 220;
    pos.x = 87 + column * 263;
    pos.y = 80 + s.first / 5 * 200;
    pos.y = pos.y + vertical_offset_[column];
    SDL_RenderCopy(engine_->renderer, reel_symbols_[s.second], NULL, &pos);
    pos.y = pos.y - max_height;
    SDL_RenderCopy(engine_->renderer, reel_symbols_[s.second], NULL, &pos);
  }
}

void MainState::RenderPaid()
{
  int rw, rh;
  SDL_GetRendererOutputSize(engine_->renderer, &rw, &rh);
  SDL_Rect paid_pos;
  paid_pos.w = paid_width_;
  paid_pos.h = paid_height_;
  paid_pos.x = rw - paid_pos.w - 130;
  paid_pos.y = rh - paid_pos.h - 120;
  SDL_RenderCopy(engine_->renderer, paid_, NULL, &paid_pos);
}

void MainState::RenderTotal()
{
  int rw, rh;
  SDL_GetRendererOutputSize(engine_->renderer, &rw, &rh);
  SDL_Rect total_pos;
  total_pos.w = total_width_;
  total_pos.h = total_height_;
  total_pos.x = rw - total_pos.w - 490;
  total_pos.y = rh - total_pos.h - 120;
  SDL_RenderCopy(engine_->renderer, total_, NULL, &total_pos);
}

void MainState::UpdateText(const char *text)
{
  SDL_Surface *textSurface = NULL;
  SDL_Color textColor = {255, 255, 255, 0};
  TTF_SizeText(font_, text, &text_width_, &text_height_);
  textSurface = TTF_RenderText_Blended(font_, text, textColor);
  text_ = SDL_CreateTextureFromSurface(engine_->renderer, textSurface);
  SDL_FreeSurface(textSurface);
}

void MainState::RenderMessageText()
{
  int rw, rh;
  SDL_GetRendererOutputSize(engine_->renderer, &rw, &rh);
  SDL_Rect text_pos;
  text_pos.w = text_width_;
  text_pos.h = text_height_;
  text_pos.x = rw / 2 - text_width_ / 2;
  text_pos.y = 10;
  SDL_RenderCopy(engine_->renderer, text_, NULL, &text_pos);
}

void MainState::OnCreditsChanged(const CreditsChangedMessage &m)
{
  UpdateCredits(m.credits_);
  UpdateBet(m.bet_);
  UpdateLines(m.lines_);
  UpdateTotal(m.total_);
  UpdatePaid(m.paid_);
}

void MainState::OnLinesUpdated(const unsigned int &num)
{
  char txtbuf[50];
  sprintf(txtbuf, "Playing %d lines", num);
  UpdateText(txtbuf);

  engine_->audio->PlaySound("/main/sound/blip1.ogg");
}

void MainState::OnBetUpdated(const unsigned int &num)
{
  char txtbuf[50];
  sprintf(txtbuf, "Betting %d credits", num);
  UpdateText(txtbuf);

  engine_->audio->PlaySound("/main/sound/blip1.ogg");
}

void MainState::OnMoneyInserted(const unsigned int &amount)
{
  char txtbuf[50];
  float famt = static_cast<float>(amount) / 100;

  engine_->audio->PlaySound("/main/sound/chime2.ogg");
  sprintf(txtbuf, "Bill Accepted $%2.2f", famt);

  UpdateText(txtbuf);
}
