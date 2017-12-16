#include "event_manager.h"
#include <gtest/gtest.h>
#include <iostream>

class KeyCounter {
  public:
    void Add(SystemEvent) {
      cnt++;
    }
    int cnt = 0;
};

TEST(EventManagerTest, KeyMappingEmitter) {
  EventManager em;
  KeyCounter c;
  em.Init();
  em.AddMapping(SDLK_1, VOL_UP);
  em.SystemSignal.connect_member(&c, &KeyCounter::Add);
  em.HandleKeyPress(SDLK_1);
  em.HandleKeyPress(SDLK_2);
  ASSERT_EQ(c.cnt, 1);
}

TEST(EventManagerTest, SupportsMultipleBinds) {
  EventManager em;
  KeyCounter c;
  em.Init();
  em.AddMapping(SDLK_1, VOL_UP);
  em.AddMapping(SDLK_1, VOL_DOWN);
  em.SystemSignal.connect_member(&c, &KeyCounter::Add);
  em.HandleKeyPress(SDLK_1);
  ASSERT_EQ(c.cnt, 2);
}

TEST(EventManagerTest, SupportsMultipleSlots) {
  EventManager em;
  KeyCounter c;
  em.Init();
  em.AddMapping(SDLK_1, VOL_UP);
  em.SystemSignal.connect_member(&c, &KeyCounter::Add);
  em.SystemSignal.connect_member(&c, &KeyCounter::Add);
  em.HandleKeyPress(SDLK_1);
  ASSERT_EQ(c.cnt, 2);
}

