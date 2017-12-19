#ifndef SYSTEM_EVENT_H
#define SYSTEM_EVENT_H

/**
 * System events triggered by the player or the system itself.
 */

// TODO: support paramaterized coins and bills (amount)
enum SystemEvent {
  COIN_IN = 1200,
  BILL_IN,
  CASH_OUT,
  SERVICE,
  SPIN,
  MAX_BET,
  BET_UP,
  BET_DOWN,
  LINE_UP,
  LINE_DOWN,
  VOL_UP,
  VOL_DOWN,
  QUIT,
  POP_STATE
};

#endif
