#include "engine.h"
#include "main_state.h"

using namespace std;

int main(int argc, char *argv[]) {
  Engine e;
  e.Init(argc, argv);
  e.ChangeState(MainState::Instance());
  while (e.Running()) {
    e.HandleEvents();
    e.Update();
    e.Draw();
  }
  e.Cleanup();
  return 0;
}
