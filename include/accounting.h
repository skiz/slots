#ifndef ACCOUNTING_H
#define ACCOUNTING_H

#include "system_event.h"
#include "signal.h"
#include "reel.h"
#include "credits_changed_message.h"
#include <SQLiteCpp/SQLiteCpp.h>

class Engine;

const unsigned int COIN_AMOUNT = 100;
const unsigned int BILL_AMOUNT = 10000;
const unsigned int CENTS_PER_CREDIT = 5;
const unsigned int BIG_WIN = 1000;

/**
 * Accounting is responsible for all monetary transactions.
 * This includes handling all user events related to money.
 */
class Accounting {
  public:
    void Init(Engine* e);
    void Init(SQLite::Database* db);
    void Cleanup();
    void HandleEvent(SystemEvent e);
    void InsertedMoney(unsigned int amount);
    void BetMax();
    void InitiateSpin();
    void CompleteSpin();
    void TriggerUpdateReels();
    void TriggerBigWin(unsigned int amount); // Implies external knowledge
    void TriggerWin(unsigned int amount);    // Implies external knowledge
    void TriggerBetUpdate(int num);
    void TriggerLinesUpdate(int num);
    void TriggerSpinStarted();
    void TriggerSpinStopped();
    void TriggerSpinComplete();
    bool InsufficientFunds(int bet, int lines);

    void EmitCreditsChanged();

    unsigned int Credits();
    unsigned int Paid();
    unsigned int Bet();
    unsigned int Lines();
    unsigned int Total();
    const char* Text();

    Signal<const unsigned int &>MoneyInserted;
    Signal<const CreditsChangedMessage&> CreditsChanged;

    Signal<const unsigned int &> BigWin;  ///Ugh Go Away
    Signal<const unsigned int &> Win;     /// Bleh no!
    Signal<>ReelsUpdate;
    Signal<>SpinStarted;
    Signal<>SpinStopped;
    Signal<>SpinComplete;
    Signal<const char*>TextUpdate;
    Reel *GetReel();
  private:
    Reel reel_;
    Engine* engine_; // not owned
    
    unsigned int cents_ = 0;         // available funds in cents

    bool spinning_ = false;          // who the fuck cares? accounting shouldn't
    
    unsigned int bet_ = 0;           // visual counter
    unsigned int lines_ = 0;         // visual counter
    unsigned int max_bet_ = 0;
    unsigned int max_lines_ = 0;
    unsigned int paid_credits_ = 0;  // amount paid for last spin
    char* text_;                     // text to send as message (REMOVE)
    SQLite::Database* db_;
};

#endif
