#include "Screen.h"

int main() {

    ps::Screen screen;
    ps::Swarm swarm;

    while(!screen.quit_program()) {

        // Load current particle swarm.
        screen.load_swarm(swarm);

        // Update SDL window with new particle positions and colors
        screen.update();

        // Manipulate particle positions.
        swarm.move(SDL_GetTicks());

        // Apply gaussian blur effect.
        screen.box_blur();
    }

    return 0;
}
