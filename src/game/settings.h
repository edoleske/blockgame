#ifndef BLOCKGAME_SETTINGS_H
#define BLOCKGAME_SETTINGS_H


// Singleton that persists user-selected settings
class Settings {
public:
    Settings();
    ~Settings();

    static Settings* getInstance();

    // Chunk radius to load and render
    int renderDistance = 12;
private:
    static Settings* _instance;
};


#endif //BLOCKGAME_SETTINGS_H
