#ifndef ACCOUNTING_H
#define ACCOUNTING_H

#include "system_event.h"
#include "signal.h"
#include "reel.h"


class Engine;

const unsigned int COIN_AMOUNT = 25;
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
    void Cleanup();
    void HandleEvent(SystemEvent e);
    void MoneyInserted(unsigned int amount);
    void BetMax();
    void InitiateSpin();
    void CompleteSpin();
    void TriggerCreditUpdate();
    void TriggerPaidUpdate();
    void TriggerTotalUpdate();
    void TriggerTextUpdate();
    void TriggerUpdateReels();
    void TriggerBigWin(unsigned int amount);
    void TriggerWin(unsigned int amount);
    void TriggerBetUpdate(int num);
    void TriggerLinesUpdate(int num);
    void TriggerSpinStarted();
    void TriggerSpinStopped();
    bool InsufficientFunds(int bet, int lines);
    unsigned int Credits();
    unsigned int Paid();
    unsigned int Bet();
    unsigned int Lines();
    unsigned int Total();
    const char* Text();
    Signal<const unsigned int &> CreditUpdate;
    Signal<const unsigned int &> PaidUpdate;
    Signal<const unsigned int &> BetUpdate;
    Signal<const unsigned int &> LinesUpdate;
    Signal<const unsigned int &> TotalUpdate;
    Signal<const unsigned int &> BigWin;
    Signal<const unsigned int &> Win;
    Signal<>ReelsUpdate;
    Signal<>SpinStarted;
    Signal<>SpinStopped;
    Signal<const char*>TextUpdate;
    Reel *GetReel();
  private:
    Reel reel_;
    Engine* engine_;
    
    unsigned int cents_ = 0;         // available funds in cents

    bool spinning_ = false;
    
    unsigned int bet_ = 0;           // visual counter
    unsigned int lines_ = 0;         // visual counter
    unsigned int max_bet_ = 0;
    unsigned int max_lines_ = 0;
    unsigned int current_bet_ = 0;   // current wager
    unsigned int paid_credits_ = 0;  // amount paid for last spin
    char* text_;                     // text to send as message

    // TODO: Actual accounting info, for ya know... accounting...
    unsigned long credits_played_ = 0;
    unsigned long credits_lost_   = 0;
    unsigned long games_played_   = 0;
    // TODO: Add play log in a signed, reloadable format
};

#endif
