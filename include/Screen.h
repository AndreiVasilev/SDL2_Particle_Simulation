//
// Created by Andrei Vasilev on 1/12/17.
//

#ifndef PARTICLE_SIMULATION_SCREEN_H
#define PARTICLE_SIMULATION_SCREEN_H

#include <SDL2/SDL.h>
#include "Swarm.h"
#include <iostream>

namespace ps {

    class Screen {
        public:
            Screen();
            virtual ~Screen();
            void update();
            void box_blur();
            void load_swarm(Swarm &);
            bool quit_program();

        private:
            void init_SDL();
            void init_window();
            void init_renderer();
            void init_texture();
            void init_buffers();
            void set_pixel_color(int, int, Uint8, Uint8, Uint8);
            void get_avg_color(int, int, Uint8 &, Uint8 &, Uint8 &);

        private:
            const static int SCREEN_WIDTH {1000};
            const static int SCREEN_HEIGHT {800};
            SDL_Window *m_window { nullptr };
            SDL_Renderer *m_renderer { nullptr };
            SDL_Texture *m_texture { nullptr };
            Uint32 *m_main_buffer { nullptr };
            Uint32 *m_blur_buffer { nullptr };
            SDL_Event m_event;
    };

} /* namespace ps */


#endif //PARTICLE_SIMULATION_SCREEN_H
