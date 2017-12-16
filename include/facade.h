#ifndef FACADE_H
#define FACADE_H

/**
 * Facade is the screen that is overlayed on the play field.
 *
 * It may have buttons, transparent areas for the wheels, etc
 *
 * How da fuck do we deal with buttons?
 */
class Facade {
  public:
    Facade();
    ~Facade();
    void Display();
    void SetResolution(int width, int height);
    void LoadFile(char* filename); //should be dealt with via assetmanager
    //SDL_Image GetImage();
  private:
    int width_;
    int height_;
    // SDL_Image* img;

};

#endif

