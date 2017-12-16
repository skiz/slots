#include "event_manager.h"
#include <gtest/gtest.h>
#include <iostream>

class KeyCounter {
  public:
    void Add(KeyEvent k) {
      cnt++;
    }
    int cnt = 0;
};

TEST(EventManagerTest, KeyMappingEmitter) {
  EventManager em;
  KeyCounter c;
  em.Init();
  em.AddMapping(SDLK_1, VOL_UP);
  em.KeyPress.connect_member(&c, &KeyCounter::Add);
  em.KeyPressed(SDLK_1);
  em.KeyPressed(SDLK_2);
  ASSERT_EQ(c.cnt, 1);
}

TEST(EventManagerTest, SupportsMultipleMappings) {
  EventManager em;
  KeyCounter c;
  em.Init();
  em.AddMapping(SDLK_1, VOL_UP);
  em.AddMapping(SDLK_1, VOL_DOWN);
  em.KeyPress.connect_member(&c, &KeyCounter::Add);
  em.KeyPressed(SDLK_1);
  ASSERT_EQ(c.cnt, 2);
}

