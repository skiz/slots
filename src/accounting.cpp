#include <iostream>

#include "accounting.h"
#include "system_event.h"
#include "engine.h"

void Accounting::Init(Engine* e) {
  engine_ = e;
  std::cout << "Initializing Accounting..." << std::endl;
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
    default:
      break;
  }
}

void Accounting::MoneyInserted(unsigned int amount) {
  if (amount > COIN_AMOUNT) {
    engine_->audio->PlaySound("assets/main/sound/chime2.ogg");
  } else {
    engine_->audio->PlaySound("assets/main/sound/coin.wav");
  }

  cents_ += amount;
  std::cout << cents_ << " (" << Credits() << " credits)" << std::endl;
}

unsigned int Accounting::Credits() {
  return cents_ * CENTS_PER_CREDIT;
}
