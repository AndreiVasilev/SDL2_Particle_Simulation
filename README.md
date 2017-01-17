
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
