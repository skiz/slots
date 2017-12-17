#ifndef ACCOUNTING_H
#define ACCOUNTING_H

#include "system_event.h"
#include "signal.h"


class Engine;

// TODO: make this configurable
const unsigned int COIN_AMOUNT = 25;
const unsigned int BILL_AMOUNT = 10000;
const unsigned int CENTS_PER_CREDIT = 1;

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
    void TriggerCreditUpdate();
    void TriggerPaidUpdate();
    void TriggerTextUpdate();
    void TriggerBetUpdate();
    unsigned int Credits();
    unsigned int Paid();
    unsigned int Bet();
    const char* Text();
    Signal<const unsigned int &> CreditUpdate;
    Signal<const unsigned int &> PaidUpdate;
    Signal<const unsigned int &> BetUpdate;
    Signal<const char*>TextUpdate;
  private:
    Engine* engine_;
    unsigned int cents_ = 0;
    unsigned int paid_credits_ = 0;
    unsigned int bet_ = 0;
    char* text_;
};

#endif
