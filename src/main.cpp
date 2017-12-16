#include "engine.h"
#include "main_state.h"

using namespace std;

int main(int argc, char *argv[]) {
  Engine e;
  e.Init(argc, argv);
  e.ChangeState(MainState::Instance());
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
