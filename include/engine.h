#ifndef ENGINE_H
#define ENGINE_H

#include <vector>
#include "SDL.h"
#include "sound_system.h"
#include "asset_manager.h"
#include "timer.h"

class State;

class Engine {
  public:
    void Init(int argc, char** argv);
    void Cleanup();
    void ChangeState(State* s);
    void PushState(State* s);
    void PopState();
    void HandleEvents();
    void Update();
    void Draw();
    void Quit();
    void StartFPSTimer();
    void UpdateFPS();
    bool Running();
    SDL_Window* window;
    SDL_Renderer* renderer;
    SoundSystem* audio;
    AssetManager* assets;
  private:
    Timer fpsTimer_;
    float fps_ = 0.0;
    int frameCount;
    std::vector<State*> states_;
    bool running_;
    bool fullscreen_;
};

#endif

