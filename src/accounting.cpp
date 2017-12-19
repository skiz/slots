#include <iostream>

#include "accounting.h"
#include "system_event.h"
#include "engine.h"
#include "signal.h"

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
    case COIN_IN:
      MoneyInserted(COIN_AMOUNT);
      break;
    case BILL_IN:
      MoneyInserted(BILL_AMOUNT);
      break;
    case BET:
      TriggerBetUpdate();
      break;
    case LINE:
      TriggerLinesUpdate();
      break;
    default:
      break;
  }
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

void Accounting::TriggerCreditUpdate() {
  CreditUpdate.emit(Credits());
}

void Accounting::TriggerTextUpdate() {
  TextUpdate.emit(text_);
}

void Accounting::TriggerPaidUpdate() {
  PaidUpdate.emit(Paid());
}

void Accounting::TriggerBetUpdate() {
  if (lines_ == 0) ++lines_;
  if (InsufficientFunds(bet_+1, lines_)) return;
  if (bet_ == max_bet_) return;
  bet_++;
  current_bet_ = bet_ * lines_;
  cents_ -= current_bet_;
  std::cout << "Bet updated: " << bet_ << std::endl;
  BetUpdate.emit(Bet());
  LinesUpdate.emit(Lines());
  CreditUpdate.emit(Credits());
}

void Accounting::TriggerLinesUpdate() {
  if (InsufficientFunds(bet_, lines_+1)) return;
  if (lines_ == max_lines_) return;
  
  lines_++;
  current_bet_ = bet_ * lines_;
  cents_ -= current_bet_;
  std::cout << "Lines updated: " << lines_ << std::endl;
  LinesUpdate.emit(Lines());
  CreditUpdate.emit(Credits());
}

bool Accounting::InsufficientFunds(int bet, int lines) {
  return Credits() == 0 || uint(bet*lines) > Credits();
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
