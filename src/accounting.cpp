#include <iostream>

#include "accounting.h"
#include "system_event.h"
#include "engine.h"
#include "signal.h"
#include "ledger_record.h"
#include "spin_record.h"
#include <SQLiteCpp/SQLiteCpp.h>


void Accounting::Init(Engine* e) {
  engine_ = e; // only needed for sound. DEPRECATE
  cents_ = 0;
  bet_ = 0;
  paid_credits_ = 0;
  lines_ = 0;
  max_bet_ = 5;
  max_lines_ = 20;
  text_ = const_cast<char*>("Ready");
  engine_->events->SystemSignal.connect_member(this, &Accounting::HandleEvent);

  db_ = new SQLite::Database("/tmp/slots.db", SQLite::OPEN_READWRITE|SQLite::OPEN_CREATE);

  // TODO: Don't reset every time, only when missing...
  // ie: SELECT name FROM sqlite_master WHERE type='table' AND name='table_name';
  LedgerRecord::Reset(db_);
  SpinRecord::Reset(db_);
  // TODO: Load last credit state from play log (optional) in case of reset/power/etc.
  // Should be able to just load the last spin record (which should allow us to set the
  // reels to the same position along with wins, credits, etc, etc. but do this outside
  // of this class please...
}

void Accounting::Init(SQLite::Database* db) {
  db_ = db;
  cents_ = 0;
  bet_ = 0;
  paid_credits_ = 0;
  lines_ = 0;
  max_bet_ = 5;
  max_lines_ = 20;
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
      LedgerRecord::Create(db_, COIN_INSERTED, COIN_AMOUNT);
      InsertedMoney(COIN_AMOUNT);
      break;
    case BILL_IN:
      LedgerRecord::Create(db_, BILL_INSERTED, BILL_AMOUNT);
      InsertedMoney(BILL_AMOUNT);
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
    case REELS_STOPPED:
      CompleteSpin();
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

void Accounting::InsertedMoney(unsigned int amount) {
  cents_ += amount;

  MoneyInserted.emit(amount);
  EmitCreditsChanged();
}

void Accounting::InitiateSpin() {
  paid_credits_ = 0;
  EmitCreditsChanged();
  if (spinning_) {
    TriggerSpinStopped();
    return;
  } // for manual stop
  if (InsufficientFunds(bet_, lines_)) return;
  if (bet_ == 0 || lines_ == 0) return;

  spinning_ = true;
  TriggerSpinStarted();
  
  // remove bet from credit pool
  cents_ -= bet_ * lines_ * CENTS_PER_CREDIT;
  EmitCreditsChanged();
  //CreditUpdate.emit(Credits());

  reel_.GenerateSymbols(5, 3);
  reel_.GenerateWinningLines(lines_);

  // Record the spin history
  SpinRecord::Create(db_, 
      SPIN_RECORD_MAIN,
      reel_.GetSymbols(),
      Bet(),   // bet credits
      Lines(), // bet lines
      Total(), // credits * lines
      reel_.GetCreditsWon() * bet_,
      reel_.GetCreditsWon() * bet_ * CENTS_PER_CREDIT,
      bet_ * lines_ * CENTS_PER_CREDIT);

  EmitCreditsChanged();
}

// TODO: Accounting shouldn't be dealing with all this crap...
void Accounting::CompleteSpin() {
  if (!spinning_) { return; }
  TriggerSpinStopped();
  spinning_ = false;
  const unsigned int won = reel_.GetCreditsWon() * bet_;
  if (won > 0) {
    if (won >= BIG_WIN) {
      TriggerBigWin(won);
    } else {
      TriggerWin(won);
    }
    cents_ += won * CENTS_PER_CREDIT;
  }
  
  paid_credits_ = won;
  ReelsUpdate.emit();
}

void Accounting::TriggerBigWin(const unsigned int amount) {
  BigWin.emit(amount);
}

void Accounting::TriggerWin(const unsigned int amount) {
  Win.emit(amount);
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
  EmitCreditsChanged();
}

void Accounting::TriggerSpinStarted() {
  SpinStarted.emit();
}

void Accounting::TriggerSpinStopped() {
  SpinStopped.emit();
}

void Accounting::EmitCreditsChanged() {
  CreditsChangedMessage m;
  m.credits_ = Credits();
  m.bet_ = Bet();
  m.lines_ = Lines();
  m.total_ = Total();
  m.paid_ = Paid();
  CreditsChanged.emit(m);
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

  EmitCreditsChanged();
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
