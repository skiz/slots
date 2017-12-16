#ifndef ASSET_MANAGER_H
#define ASSET_MANAGER_H

class AssetManager {
  public:
    void Init(const char* path);
    void Cleanup();
    bool AddSearchPath(const char* path);
    char* ReadBytes(const char* filename);
  private:
};

#endif
