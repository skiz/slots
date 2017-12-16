#ifndef ASSET_MANAGER_H
#define ASSET_MANAGER_H

class AssetManager {
  public:
    void Init(const char* path);
    void Cleanup();
    bool Mount(const char* path, const char* target);
    char* ReadBytes(const char* filename);
    const char* GetLastError(); 
  private:
};

#endif
