#include <iostream>

#include "accounting.h"
#include "system_event.h"
#include "engine.h"
#include "signal.h"

const unsigned int BIG_WIN = 1000;

// TODO add tests for this as it should be the most tested...
void Accounting::Init(Engine* e) {
  engine_ = e;
  cents_ = 0;
  bet_ = 0;
  paid_credits_ = 0;
  lines_ = 0;
  max_bet_ = 5;
  max_lines_ = 20;
  text_ = const_cast<char*>("Ready");
  engine_->events->SystemSignal.connect_member(this, &Accounting::HandleEvent);
  // TODO: Load last credit state from play log (optional) in case of reset/power/etc.
}

void Accounting::Cleanup() {}

void Accounting::HandleEvent(SystemEvent e) {
  switch (e) {
    case SPIN:
      InitiateSpin();
      break;
    case BET_MAX:
      BetMax();
      break;
    case COIN_IN:
      MoneyInserted(COIN_AMOUNT);
      break;
    case BILL_IN:
      MoneyInserted(BILL_AMOUNT);
      break;
    case BET_UP:
      TriggerBetUpdate(1);
      break;
    case BET_DOWN:
      TriggerBetUpdate(-1);
      break;
    case LINE_UP:
      TriggerLinesUpdate(1);
      break;
    case LINE_DOWN:
      TriggerLinesUpdate(-1);
      break;
    default:
      break;
  }
}

Reel* Accounting::GetReel() {
  return &reel_;
}

void Accounting::BetMax() {
  if (InsufficientFunds(max_bet_, max_lines_)) return;
  lines_ = max_lines_;
  bet_ = max_bet_;
  InitiateSpin();
}

void Accounting::MoneyInserted(unsigned int amount) {
  char txtbuf[50];
  float famt = static_cast<float>(amount) / 100;

  // TODO: this should be a signal for money inserted with amount
  if (amount > COIN_AMOUNT) {
    engine_->audio->PlaySound("assets/main/sound/chime2.ogg");
    sprintf(txtbuf, "Bill Accepted $%2.2f", famt);
  } else {
    engine_->audio->PlaySound("assets/main/sound/coin.wav");
    sprintf(txtbuf, "Coin Accepted $%2.2f", famt);
  }

  text_ = txtbuf;

  cents_ += amount;
  //std::cout << cents_ << " (" << Credits() << " credits)" << std::endl;

  TriggerCreditUpdate();
  TriggerTextUpdate();
}

void Accounting::InitiateSpin() {
  if (InsufficientFunds(bet_, lines_)) return;
  if (bet_ == 0 || lines_ == 0) return;
  char txtbuf[50];
  sprintf(txtbuf, "Good Luck!");
  text_ = txtbuf;
  TriggerTextUpdate();

  // remove bet from credit pool
  cents_ -= bet_ * lines_ * CENTS_PER_CREDIT;
  CreditUpdate.emit(Credits());

  reel_.GenerateSymbols(5, 3);
  reel_.GenerateWinningLines(lines_);

  int won = reel_.GetCreditsWon() * bet_;
  if (won > 0) {
    if (won >= BIG_WIN) {
      TriggerBigWin(won);
    } else {
      sprintf(txtbuf, "You won %d credits!", won);
      text_ = txtbuf;
      TriggerTextUpdate();
    }

    cents_ += won * CENTS_PER_CREDIT;
    //std::cout << "Winnings (cents): " << won << std::endl;
    // reel_.DumpLines();
  }
  paid_credits_ = won;
  BetUpdate.emit(Bet());
  LinesUpdate.emit(Lines());
  CreditUpdate.emit(Credits());
  TotalUpdate.emit(Total());
  PaidUpdate.emit(Paid());
  ReelsUpdate.emit();
}

void Accounting::TriggerCreditUpdate() {
  CreditUpdate.emit(Credits());
}

void Accounting::TriggerBigWin(const unsigned int amount) {
  BigWin.emit(amount);
}

void Accounting::TriggerTextUpdate() {
  TextUpdate.emit(text_);
}

void Accounting::TriggerPaidUpdate() {
  PaidUpdate.emit(Paid());
}

void Accounting::TriggerTotalUpdate() {
  TotalUpdate.emit(Total());
}

void Accounting::TriggerUpdateReels() {
  ReelsUpdate.emit();
}

void Accounting::TriggerBetUpdate(int num) {
  if (num == 1) { // increase
    if (InsufficientFunds(bet_+1, lines_)) return;
    if (bet_ == max_bet_) return;
    bet_++;
  } else { // decrease
    if (bet_ == 0) return;
    bet_--;
  }

  // int new_bet = bet_ * lines_;
  // cents_ += (current_bet_ - new_bet) * CENTS_PER_CREDIT;
  // current_bet_ = new_bet;

  //std::cout << "Bet updated: " << bet_ << std::endl;
  BetUpdate.emit(Bet());
  LinesUpdate.emit(Lines());
  CreditUpdate.emit(Credits());
  TotalUpdate.emit(Total());
}

void Accounting::TriggerLinesUpdate(int num) {
  if (num == 1) { //increase
    if (InsufficientFunds(bet_, lines_+1)) return;
    if (lines_ == max_lines_) return;
    lines_++;
  } else {
    if (lines_ == 0) return;
    lines_--;
  }
  
  //int new_bet = bet_ * lines_;
  // cents_ += (current_bet_ - new_bet) * CENTS_PER_CREDIT;
  //current_bet_ = new_bet;

  // std::cout << "Lines updated: " << lines_ << std::endl;
  BetUpdate.emit(Bet());
  LinesUpdate.emit(Lines());
  CreditUpdate.emit(Credits());
  TotalUpdate.emit(Total());
}

bool Accounting::InsufficientFunds(int bet, int lines) {
  return Credits() == 0 || uint(bet*lines) > Credits();
}

unsigned int Accounting::Credits() {
  return cents_ / CENTS_PER_CREDIT;
}

unsigned int Accounting::Paid() {
  return paid_credits_;
}

unsigned int Accounting::Bet() {
  return bet_;
}

unsigned int Accounting::Lines() {
  return lines_;
}

unsigned int Accounting::Total() {
  return bet_ * lines_;
}

const char* Accounting::Text() {
  return text_;
}
