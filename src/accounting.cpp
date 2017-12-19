#include <iostream>

#include "accounting.h"
#include "system_event.h"
#include "engine.h"
#include "signal.h"

// TODO add tests for this as it should be the most tested...
void Accounting::Init(Engine* e) {
  engine_ = e;
  cents_ = 0;
  bet_ = 0;
  paid_credits_ = 0;
  lines_ = 0;
  max_bet_ = 5;
  max_lines_ = 20;
  current_bet_ = 0;
  text_ = const_cast<char*>("Ready");
  std::cout << "Initializing Accounting... " << cents_ << std::endl;
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
void Accounting::BetMax() {
  if (InsufficientFunds(max_bet_, max_lines_, current_bet_)) return;

  lines_ = max_lines_;
  bet_ = max_bet_;

  int new_bet = bet_ * lines_;
  cents_ += current_bet_ - new_bet;
  current_bet_ = new_bet;

  InitiateSpin();
}

void Accounting::MoneyInserted(unsigned int amount) {
  char txtbuf[50];
  float famt = static_cast<float>(amount) / 100;

  if (amount > COIN_AMOUNT) {
    engine_->audio->PlaySound("assets/main/sound/chime2.ogg");
    sprintf(txtbuf, "Bill Accepted $%2.2f", famt);
  } else {
    engine_->audio->PlaySound("assets/main/sound/coin.wav");
    sprintf(txtbuf, "Coin Accepted $%2.2f", famt);
  }

  text_ = txtbuf;

  cents_ += amount;
  std::cout << cents_ << " (" << Credits() << " credits)" << std::endl;

  TriggerCreditUpdate();
  TriggerTextUpdate();
}

void Accounting::InitiateSpin() {
  if (bet_ == 0 || lines_ == 0) return;
  char txtbuf[50];
  sprintf(txtbuf, "Good Luck!");
  text_ = txtbuf;
  TriggerTextUpdate();

  reel_.GenerateSymbols(5, 3);
  reel_.GenerateWinningLines(lines_);

  int won = reel_.GetCreditsWon() * bet_;
  if (won > 0) {
    sprintf(txtbuf, "You won %d credits!", won);
    text_ = txtbuf;
    TriggerTextUpdate();
    cents_ += won;
    std::cout << "Winnings: " << won << std::endl;
    reel_.DumpLines();
  }

  bet_ = 0;
  current_bet_ = 0;
  lines_ = 0;
  BetUpdate.emit(Bet());
  LinesUpdate.emit(Lines());
  CreditUpdate.emit(Credits());
  //TextUpdate.emit(Text());
}

void Accounting::TriggerCreditUpdate() {
  CreditUpdate.emit(Credits());
}

void Accounting::TriggerTextUpdate() {
  TextUpdate.emit(text_);
}

void Accounting::TriggerPaidUpdate() {
  PaidUpdate.emit(Paid());
}

void Accounting::TriggerBetUpdate(int num) {
  if (num == 1) { // increase
    if (InsufficientFunds(bet_+1, lines_, 0)) return;
    if (bet_ == max_bet_) return;
    bet_++;
  } else { // decrease
    if (bet_ == 0) return;
    bet_--;
  }

  int new_bet = bet_ * lines_;
  cents_ += current_bet_ - new_bet;
  current_bet_ = new_bet;

  std::cout << "Bet updated: " << bet_ << std::endl;
  BetUpdate.emit(Bet());
  LinesUpdate.emit(Lines());
  CreditUpdate.emit(Credits());
}

void Accounting::TriggerLinesUpdate(int num) {
  if (num == 1) { //increase
    if (InsufficientFunds(bet_, lines_+1, 0)) return;
    if (lines_ == max_lines_) return;
    lines_++;
  } else {
    if (lines_ == 0) return;
    lines_--;
  }
  
  int new_bet = bet_ * lines_;
  cents_ += current_bet_ - new_bet;
  current_bet_ = new_bet;

  std::cout << "Lines updated: " << lines_ << std::endl;
  BetUpdate.emit(Bet());
  LinesUpdate.emit(Lines());
  CreditUpdate.emit(Credits());
}

bool Accounting::InsufficientFunds(int bet, int lines, int offset) {
  return Credits() == 0 || uint(bet*lines + offset) > Credits();
}

unsigned int Accounting::Credits() {
  return cents_ * CENTS_PER_CREDIT;
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

const char* Accounting::Text() {
  return text_;
}
