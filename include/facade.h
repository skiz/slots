#ifndef FACADE_H
#define FACADE_H

#include "SDL_image.h"

/**
 * Facade is the screen that is overlayed on the play field.
 * It should be able to have animations, transparencies, etc.
 */
class Facade {
  public:
    Facade();
    ~Facade();
    void Display();
    void SetResolution(int width, int height);
    void LoadFile(char* filename); //should be dealt with via assetmanager
    // SDL_Image GetImage();
  private:
    int width_;
    int height_;
    SDL_Surface* img;

};

#endif

