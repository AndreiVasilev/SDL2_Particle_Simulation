//
// Created by Andrei Vasilev on 1/13/17.
//

#include "Particle.h"

namespace ps {

    // Particle constructor. Assigns particle initial position (center of window), as well
    // as giving it a unique speed (-1 to 1) and a unique angular direction (0 to 2 pi)
    Particle::Particle() : m_x_cord(0), m_y_cord(0) {
        initialize();
    }


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


    void Particle::move_particle(int interval) {
        // Changes particle direction from straight to curved/circular.
        m_direction += (interval * 0.0005);

        // Generate particle vector. Using different trig functions here can cause very
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

} /* namespace ps */
