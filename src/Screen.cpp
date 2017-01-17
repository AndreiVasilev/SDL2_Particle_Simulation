//
// Created by Andrei Vasilev on 1/12/17.
//

#include "Screen.h"

namespace ps {

    Screen::Screen() : m_window{nullptr}, m_renderer{nullptr}, m_texture{nullptr}, m_main_buffer{nullptr} {
        // Initialize all required SDL functionality and create SDL objects.
        init_SDL();
        init_window();
        init_renderer();
        init_texture();
        init_buffers();
    }

    Screen::~Screen() {
        // Destroy all SDL related objects.
        delete [] m_main_buffer;
        delete [] m_blur_buffer;
        SDL_DestroyRenderer(m_renderer);
        SDL_DestroyTexture(m_texture);
        SDL_DestroyWindow(m_window);
        SDL_Quit();
    }

    void Screen::init_SDL() {
        // Initialize SDL library. Returns 0 if successful.
        // Must be called before using any SDL functionality.
        if(SDL_Init(SDL_INIT_VIDEO) != 0) {
            std::cout << "SDL_Init Error: \n" << SDL_GetError() << std::endl;
            exit(1);
        }
    }


    void Screen::init_window() {
        // Creates an SDL window struct. Returns NULL on failure.
        m_window = SDL_CreateWindow(
            "Particle Simulation",       // Window title
            SDL_WINDOWPOS_UNDEFINED,     // Initial x position
            SDL_WINDOWPOS_UNDEFINED,     // Initial y position
            SCREEN_WIDTH,                // Width, in pixels
            SCREEN_HEIGHT,               // Height, in pixels
            SDL_WINDOW_ALLOW_HIGHDPI     // Increases resolution
        );

        // Check if window creation failed.
        if(m_window == nullptr) {
            std::cout << "SDL_CreateWindow Error: \n" << SDL_GetError() << std::endl;
            exit(2);
        }
    }


    void Screen::init_renderer() {
        // Creates an SDL renderer struct: a rendering context relative to the state/information
        // of the associated window. Textures to be rendered pass through this renderer and when
        // done rendering are sent to the window to be displayed.
        m_renderer = SDL_CreateRenderer(
            m_window,                      // Window associated with the renderer.
            -1,                            // Index of rendering driver to initialize, -1 to use first available.
            SDL_RENDERER_PRESENTVSYNC |    // Synchronize rendering with window refresh rate.
            SDL_RENDERER_ACCELERATED);     // Allow renderer to use hardware acceleration.

        // Check if renderer creation failed.
        if(m_renderer == nullptr) {
            std::cout << "SDL_CreateRenderer Error: \n" << SDL_GetError() << std::endl;
            SDL_DestroyWindow(m_window);
            exit(3);
        }
    }


    void Screen::init_texture() {
        // Creates an SDL texture struct: a set of pixel data to be sent to the renderer.
        m_texture = SDL_CreateTexture(
            m_renderer,                    // The rendering context that this texture will pass through.
            SDL_PIXELFORMAT_RGBA8888,      // The format of the pixel color information, RGBA 8 bits per channel.
            SDL_TEXTUREACCESS_STATIC,      // How the texture is accessed when updated.
            SCREEN_WIDTH,                  // Texture width in pixels, equals associated window width.
            SCREEN_HEIGHT                  // Texture height in pixels, equals associated window height.
        );

        // Check if texture creation failed.
        if(m_texture == nullptr) {
            std::cout << "SDL_CreateTexture Error: \n" << SDL_GetError() << std::endl;
            SDL_DestroyRenderer(m_renderer);
            SDL_DestroyWindow(m_window);
            exit(3);
        }
    }


    void Screen::init_buffers() {
        // Initialize main_buffer and blur_buffer memory. During runtime, buffers are loaded with
        // particle objects and then fed to the SDL texture to be rendered within the SDL window.
        m_main_buffer = new Uint32[SCREEN_HEIGHT * SCREEN_WIDTH];
        memset(m_main_buffer, 0, SCREEN_HEIGHT * SCREEN_WIDTH * sizeof(Uint32));

        m_blur_buffer = new Uint32[SCREEN_HEIGHT * SCREEN_WIDTH];
        memset(m_blur_buffer, 0, SCREEN_HEIGHT * SCREEN_WIDTH * sizeof(Uint32));
    }


    void Screen::update() {
        // Updates the texture containing the pixel data.
        SDL_UpdateTexture(
            m_texture,                      // Texture to be updated.
            NULL,                           // Area in texture to update, NULL for entire area.
            m_main_buffer,                  // Updated pixel data being sent to the texture.
            SCREEN_WIDTH * sizeof(Uint32)   // Number of bytes in a row of pixel data.
        );

        // Copies the newly updated texture into the renderer.
        SDL_RenderCopy(
            m_renderer,    // The renderer to be updated.
            m_texture,     // The source texture.
            NULL,          // Amount of texture to be copied, NULL for entire texture.
            NULL           // Amount of renderer to be updated, NULL for entire renderer.
        );

        // Loads the renderer to the SDL window.
        SDL_RenderPresent(m_renderer);
    }


    void Screen::load_swarm(Swarm &swarm) {
        // Load swarm particles.
        const Particle * const p_particles = swarm.get_particles();

        // Generate colors to update swarm.
        int elapsed = SDL_GetTicks();
        unsigned char red = static_cast<unsigned char>(   (sin(elapsed * 0.0001) + 1) * 128  ),
                      green = static_cast<unsigned char>( (sin(elapsed * 0.0002) + 1) * 128  ),
                      blue = static_cast<unsigned char>(  (sin(elapsed * 0.0003) + 1) * 128  );

        // Load swarm with updated colors into main_buffer.
        for(int i = 0; i < Swarm::N_PARTICLES; ++i) {
            ps::Particle particle = p_particles[i];
            int x = static_cast<int>((particle.m_x_cord + 1) * SCREEN_WIDTH / 2);
            int y = static_cast<int>(particle.m_y_cord * SCREEN_WIDTH / 2 + SCREEN_HEIGHT/2);
            set_pixel_color(x, y, red, green, blue);
        }
    }


    void Screen::box_blur() {
        // Main_buffer and blur_buffer are swapped, blur is applied to blur_buffer, and then
        // during each next iteration buffers are swapped again to display the blur on screen.
        std::swap(m_main_buffer, m_blur_buffer);

        // Iterates through each pixel in the buffer, gets average color of surrounding pixels,
        // and applies average surrounding color to each pixel. Creates a gaussian blur effect.
        for(int y = 0; y < SCREEN_HEIGHT; ++y) {
            for(int x = 0; x < SCREEN_WIDTH; ++x) {
                Uint8 red{0}, green{0}, blue{0};
                get_avg_color(x, y, red, green, blue);
                set_pixel_color(x, y, red, green, blue);
            }
        }
    }


    void Screen::get_avg_color(int x, int y, Uint8 &red, Uint8 &green, Uint8 &blue) {
        int red_total{0}, green_total{0}, blue_total{0};

        // Totals the color values of the input pixel (x, y) and the 8 pixels that surround it.
        for(int row = -1; row <= 1; ++row) {
            for(int col = -1; col <= 1; ++col) {
                int current_x = x + col,
                    current_y = y + row;

                // Only grab values of pixels that are actually within the window
                if(current_x >= 0 && current_x < SCREEN_WIDTH &&
                   current_y >= 0 && current_y < SCREEN_HEIGHT) {

                    // Get the color value of the current pixel
                    Uint32 color = m_blur_buffer[current_x + (current_y * SCREEN_WIDTH)];

                    // Bit shift RGB values of pixel color into color totals.
                    red_total += static_cast<Uint8>(color >> 24);
                    green_total += static_cast<Uint8>(color >> 16);
                    blue_total += static_cast<Uint8>(color >> 8);
                }
            }
        }
        // Averages the color values of the input pixel (x, y) and its 8 surrounding pixels.
        // Changing the denominator here can create interesting visual effects.
        red = static_cast<Uint8>(red_total/9);
        green = static_cast<Uint8>(green_total/9);
        blue = static_cast<Uint8>(blue_total/9);
    }


    void Screen::set_pixel_color(int x, int y, Uint8 red, Uint8 green, Uint8 blue) {

        // Ignore particles with coordinates outside the boundaries of the SDL window
        if(x < 0 || x >= ps::Screen::SCREEN_WIDTH || y < 0 || y >= ps::Screen::SCREEN_HEIGHT)
            return;

        Uint32 color{0};

        // Bit shift colors into proper positions within color (RGBA)
        color += red;
        color <<= 8;
        color += green;
        color <<= 8;
        color += blue;
        color <<= 8;
        color += 0xFF;   // Alpha channel set to opaque

        m_main_buffer[x + (y * SCREEN_WIDTH)] = color;
    }


    bool Screen::quit_program() {
        // Check for SDL events. If the window is closed, quit the program.
        while(SDL_PollEvent(&m_event)) {
            if(m_event.type == SDL_QUIT)
                return true;
        }
        return false;
    }

} /* namespace ps */
