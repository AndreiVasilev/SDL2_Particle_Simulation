//
// Created by Andrei Vasilev on 1/13/17.
//

#include <math.h>
#include <random>
#include "Particle.h"

namespace ps {

    // Particle constructor. Assigns particle initial position (center of window), as well
    // as giving it a unique speed (-1 to 1) and a unique angular direction (0 to 2pi)
    Particle::Particle() : m_x_cord(0), m_y_cord(0) {
        initialize();
    }

    void Particle::initialize() {
        m_x_cord = 0, m_y_cord = 0;

        std::random_device rd;
        std::uniform_real_distribution<double> speed_dist(-1, 1);
        std::uniform_real_distribution<double> angle_dist(0, 2 * M_PI);

        m_speed = pow(speed_dist(rd) * 0.04, 2);
        m_direction = angle_dist(rd);
    }

    void Particle::move_particle(int interval) {
        // Changes particle direction from straight to curved/circular
        m_direction += (interval * 0.0005);

        // Changes particle coordinates from cartesian to polar
        m_x_speed = m_speed * sin(m_direction);
        m_y_speed = m_speed * cos(m_direction);

        // Moves particles
        m_x_cord += (m_x_speed * interval);
        m_y_cord += (m_y_speed * interval);

        // If particle moves outside of the SDL window, reinitialize it to
        // the center of the window and give it a new direction and speed.
        if(m_x_cord < -1 || m_x_cord > 1 || m_y_cord < -1 || m_y_cord >1)
            initialize();
    }

} /* namespace ps */
