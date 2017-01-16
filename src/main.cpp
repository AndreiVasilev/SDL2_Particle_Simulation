#include "Screen.h"
#include "Swarm.h"

int main() {

    ps::Screen screen;
    ps::Swarm swarm;

    while(!screen.quit_program()) {

        // Draw Particles
        int elapsed = SDL_GetTicks();
        swarm.move(elapsed);

        unsigned char red = static_cast<unsigned char>(   (sin(elapsed * 0.0001) + 1) * 128  );
        unsigned char green = static_cast<unsigned char>( (sin(elapsed * 0.0002) + 1) * 128  );
        unsigned char blue = static_cast<unsigned char>(  (sin(elapsed * 0.0003) + 1) * 128  );

        const ps::Particle * const p_particles = swarm.get_particles();

        for(int i = 0; i < ps::Swarm::N_PARTICLES; ++i) {
            ps::Particle particle = p_particles[i];
            int x = static_cast<int>((particle.m_x_cord + 1) * ps::Screen::SCREEN_WIDTH / 2);
            int y = static_cast<int>(particle.m_y_cord * ps::Screen::SCREEN_WIDTH / 2 + ps::Screen::SCREEN_HEIGHT/2);

            screen.set_pixel_color(x, y, red, green, blue);
        }
        
        screen.box_blur();


        // Update SDL window with new particle positions and colors
        screen.update();
    }

    return 0;
}
