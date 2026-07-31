#include "common.h"
#include "game/game.h"

// STB_IMAGE_IMPLEMENTATION must be imported once in a source file
// So we define it in main after importing the headers
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

int main() {
    Game game(800, 600);

    if (!game.isInitialized()) {
        std::cerr << "Failed to initialize game" << std::endl;
        return 1;
    }

    game.loop();

    return 0;
}
