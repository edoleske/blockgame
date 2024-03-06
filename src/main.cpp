#include "common.h"
#include "game/game.h"

int main() {
    Game game(800, 600);

    if (!game.isInitialized()) {
        std::cerr << "Failed to initialize game" << std::endl;
        return 1;
    }

    game.loop();

    return 0;
}
