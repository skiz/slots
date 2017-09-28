#include "engine.h"
#include "blank_state.h"

int main(int argc, char *argv[]) {
  Engine e;
  e.Init();
  e.ChangeState(BlankState::Instance());
  e.StartFPSTimer();
  while (e.Running()) {
    e.HandleEvents();
    e.Update();
    e.Draw();
    e.UpdateFPS();
  }
  e.Cleanup();
  return 0;
}
