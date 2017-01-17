
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
<b>The "Particle" Class</b>

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
<br>
Particles move by having their coordinates changed. These coordinates are saved as floating point numbers, and during each cycle of the program these coordinates are slightly changed by adding vector values to them. These vector values are also constantly changing. However, because the SDL window is represented by a grid of integer values, particle coordinate values are truncated. This means that the particles only move when either of their x or y coordinates changes to new whole-number value.
```sh
    void Particle::move_particle(int interval) {
        // Changes particle direction from straight to curved/circular.
        m_direction += (interval * 0.0005);

        // Generate particle vector. Using trig here changes particle coordinate plane
        // from cartesian to polar. Using different trig functions here can cause very
        // interesting visual effects.
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
