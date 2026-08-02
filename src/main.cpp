#include "common.h"
#include "log.h"
#include "game/game.h"

// STB_IMAGE_IMPLEMENTATION must be imported once in a source file
// So we define it in main after importing the headers
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

int main() {
    Log::instance();
    LOG_INFO("Initializing game");

    Game game(800, 600);

    if (!game.isInitialized()) {
        LOG_ERROR("Failed to initialize game");
        return 1;
    }

    game.loop();

    return 0;
}
