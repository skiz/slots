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
  LINE_1,
  LINE_2,
  LINE_3,
  LINE_4,
  LINE_5,
  BET_1,
  BET_2,
  BET_3,
  BET_4,
  BET_5,
  AUX_1,
  AUX_2,
  AUX_3,
  AUX_4,
  AUX_5,
  VOL_UP,
  VOL_DOWN,
  QUIT,
  POP_STATE
};

#endif
