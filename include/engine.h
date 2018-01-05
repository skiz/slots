#ifndef ENGINE_H
#define ENGINE_H

#include <vector>
#include "SDL.h"
#include "accounting.h"
#include "sound_system.h"
#include "asset_manager.h"
#include "event_manager.h"
#include "timer.h"

class State;

/**
 * The engine is the core component.  It handles state changes,
 * and manages and owns all subsystems.
 */
class Engine {
  public:
    void Init(int argc, char** argv);
    void Cleanup();
    void ChangeState(State* s);
    void PushState(State* s);
    void PopState();
    void PushAsyncState(State* s);
    void PopAsyncState();
    void HandleEvents();
    void Update();
    void Draw();
    void Quit();
    void StartFPSTimer();
    void UpdateFPS();
    bool Running();
    void HandleEvent(SystemEvent);
    
    SDL_Window* window;
    SDL_Renderer* renderer;

    SoundSystem* audio;
    AssetManager* assets;
    EventManager* events;
    Accounting* accounting;
  private:
    Timer fpsTimer_;
    float fps_ = 0.0;
    int frameCount_;
    std::vector<State*> states_;
    std::vector<State*> astates_;
    bool running_;
};

#endif

