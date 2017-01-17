
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
This class does all of the heavy lifting. It is in charge of all SDL functionality, such as generating the window, renderer, and texture objects, as well as generating the color values of the particles and loading them on screen. I will only be explaining the functions that have to do with the functionality of this program specifically, which excludes the SDL functions.
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
