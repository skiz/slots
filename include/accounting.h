#ifndef ACCOUNTING_H
#define ACCOUNTING_H

#include "system_event.h"

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
    unsigned int Credits();
  private:
    Engine* engine_;
    unsigned int cents_ = 0;
};

#endif
