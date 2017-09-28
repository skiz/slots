#ifndef ASSET_MANAGER_H
#define ASSET_MANAGER_H

class AssetManager {
  public:
    void Init(const char* path);
    void Cleanup();
    bool AddSearchPath(char* archive);
    char* ReadBytes(char* filename);
  private:
};

#endif
