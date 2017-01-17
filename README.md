
# Particle Simulation Using SDL Graphics Library

This program renders a swarm of particles on screen that spin around in a cirular fashion and change colors. This is my second ever object oriented project and biggest project to date. I am sure that there is ways in which this program could be improved, and I will try to implement improvements as I continue to learn more.

<h2>The Simulation In Action (Click Images For Video)</h2>
<p align="left">
  <a href="https://youtu.be/w7CoTIjSGRQ"><img src="https://s29.postimg.org/daaszf7hz/Screen_Shot_2017_01_16_at_10_10_33_AM.png" width="400px"/></a>
  <a href="https://youtu.be/w7CoTIjSGRQ"><img src="https://s30.postimg.org/wz9c6nwqp/Screen_Shot_2017_01_16_at_10_28_36_AM.png" width="400px"/></a>
</p>
<p align="left">
  <a href="https://youtu.be/w7CoTIjSGRQ"><img src="https://s30.postimg.org/rlw2avxgx/Screen_Shot_2017_01_16_at_10_31_37_AM.png" width="400px"/></a>
  <a href="https://youtu.be/w7CoTIjSGRQ"><img src="https://s30.postimg.org/4vwxhwe9d/Screen_Shot_2017_01_16_at_10_31_57_AM.png" width="400px"/></a>
</p>

<h2>How It Works</h2>
A 2D array of color values is loaded to the screen in a repeatedly fast fashion. The brightest color values are localized to one pixel at a time, indicating where the "particles" are on screen. These color values are shifted throughout the 2D array (and in turn from pixel to pixel on screen) which creates the illusion that there are particles flying around. 

<br>
<h3>The "Particle" Class</h3>

Each particle (or focal point of brightness) contains 3 essential parts: an x-coordinate, a y-coordinate, and a vector (a speed and direction). As the program runs, the vector is what dictates where the particle will move to next.
```sh
    class Particle {
        public:
            double m_x_cord;
            double m_y_cord;

        public:
            Particle();
            virtual ~Particle() {};
            void move_particle(int);

        private:
            double m_x_vector;
            double m_y_vector;
            double m_speed;
            double m_direction;

        private:
            void initialize();
    };

```
<br><br>
Particles are initialized center screen, given a random speed (-1 to 1), and a random direction (0 to 2pi).
```sh
   void Particle::initialize() {
        std::random_device rd;
        std::uniform_real_distribution<double> speed_dist(-1, 1);
        std::uniform_real_distribution<double> angle_dist(0, 2 * M_PI);
        m_x_cord = 0, m_y_cord = 0;

        // Generate random particle speed. Speed is squared causing
        // particle distribution to be exponential instead of linear.
        m_speed = pow(speed_dist(rd) * 0.04, 2);

        // Generate random particle direction between 0 and 2 Pi.
        m_direction = angle_dist(rd);
    }
```
<br><br>
Particles move by having their coordinates changed. These coordinates are saved as floating point numbers, and during each cycle of the program these coordinates are slightly changed by adding vector values to them. These vector values are also constantly changing. However, because the SDL window is represented by a grid of integer values, particle coordinate values are truncated. This means that particles only move when their x or y coordinates change to a new whole-number value. 
```sh
    void Particle::move_particle(int interval) {
        // Changes particle direction from straight to curved/circular.
        m_direction += (interval * 0.0005);

        // Assign particle vector. Using trig here changes particle coordinates
        // from cartesian to polar. Using different trig functions here can cause
        // very interesting visual effects.
        m_x_vector = m_speed * sin(m_direction);
        m_y_vector = m_speed * cos(m_direction);

        // Moves particle by vector values.
        m_x_cord += (m_x_vector * interval);
        m_y_cord += (m_y_vector * interval);

        // If particle moves outside of the SDL window, reinitialize it to
        // the center of the window and give it a new direction and speed.
        if(m_x_cord < -1 || m_x_cord > 1 || m_y_cord < -1 || m_y_cord >1)
            initialize();
    }
```
<br><br>
<h3>The Swarm Class</h3>

The swarm class is a container for all particles. All particles are saved within an array and can be accessed individually. From here you can also change the total amount of particles that will be created when the program is run.
```sh
    class Swarm {
        public:
            const static int N_PARTICLES{6000};  // Change to adjust particle count.

        public:
            Swarm();
            virtual ~Swarm();
            void move(int);
            const Particle * const get_particles();


        private:
            Particle *m_particle_array { nullptr };
            int last_clock;
            int interval;
    };
```
<br><br>
The only method carried out by the swarm class is move(), which calls the particle.move() function (explained above) on all particles within the particle swarm array.
```sh
void Swarm::move(int elapsed) {
       /* Interval between last particle position update and current update is used
        * to ensure that particle movement stays consistent and proportional to the
        * amount of time between updates. Helps to maintain smooth movement. During
        * the first actual update interval is kept at zero to prevent a large particle
        * position jump from occurring. */
        if(last_clock == 0)
            interval = 0;
        else
            interval = elapsed - last_clock;

        last_clock = elapsed;

        // Update all particle positions within the swarm.
        for(int i = 0; i < N_PARTICLES; ++i)
            m_particle_array[i].move_particle(interval);
    }
```
<br><br>
<h3>The Screen Class</h3>
This class does all of the heavy lifting. It is in charge of all SDL functionality, such as generating the window, renderer, and texture objects, as well as generating the color values of the particles and loading them on screen. The pixels (particles) that are saved within the swarm class array are loaded into a buffer, the buffer is loaded into a texture, the texture is loaded into the renderer, and the renderer renders the pixels into the SDL window.
```sh
class Screen {
        public:
            const static int SCREEN_WIDTH {1000};
            const static int SCREEN_HEIGHT {800};

        public:
            Screen();
            virtual ~Screen();
            void init_SDL();
            void init_window();
            void init_renderer();
            void init_texture();
            void init_buffers();
            void update();
            void box_blur();
            void load_swarm(Swarm &);
            void set_pixel_color(int, int, Uint8, Uint8, Uint8);
            void get_avg_color(int, int, Uint8 &, Uint8 &, Uint8 &);
            bool quit_program();

        private:
            SDL_Window *m_window { nullptr };
            SDL_Renderer *m_renderer { nullptr };
            SDL_Texture *m_texture { nullptr };
            Uint32 *m_main_buffer { nullptr };
            Uint32 *m_blur_buffer { nullptr };
            SDL_Event m_event;
    };
```
<br><br>
The screen.load_swarm() method is where the magic starts to happen. The coordinates saved within each particle object are looked at and a corresponding position within the buffer is pulled up (which corresponds to a position in the 2D pixel array of the SDL window). The set_pixel_color() method then takes that position in the buffer and fills it with a hexidecimal color code. In turn, what appears on screen is a pixel (or particle) with that assigned color.
```sh
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
```
```sh
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
```
<br><br>
Once all particles have been looked at, and the buffer has been correspondingly filled with color values, the buffer is loaded into the texture, the texture goes to the renderer, and the renderer is loaded on screen.
```sh
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
```
<br><br>
After this, the particles within the swarm array are moved (through a process explained earlier) for the next program cycle. After the particles have been moved, a gaussian blur is applied to the currently rendered SDL window. This blur effect is created by averaging out colors. Each pixel on screen is effectively reassigned a color which is the average of itself and the 8 pixels that surround it.
However, this process is not carried out by looking at the actual screen, but instead by looking at the buffer that was just previously loaded. The buffer is processed, colors are averaged out, and then the buffer is reloaded to the screen. 

This effect causes the "particles" to look as though they are leaving a trail behind as they travel. This effect also takes care of having to clear the screen. Normally, you would have to clear the texture every time you load new particles otherwise the particles would basically draw lines across the screen as they move. However, because the color is constantly being averaged out, the only areas that remain colorful are areas that are close to particles. The other areas average out to the color black, effectively clearing the screen of particle lines after the particles move.
```sh
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
```
```sh
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
```
<br>
<h2>Conclusion</h2>
I had a lot of fun creating this project and I also learned a LOT. I plan on continuing to learn the SDL library and will hopefully soon be creating a project that is some sort of combination between this program and my genetic algorithm. Possibly training particles to solve some sort of problem? We shall see!
