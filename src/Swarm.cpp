//
// Created by Andrei Vasilev on 1/13/17.
//

#include "Swarm.h"

namespace ps {

    Swarm::Swarm() : last_clock(0) {
        m_particle_array = new Particle[N_PARTICLES];
    }

    Swarm::~Swarm() {
        delete [] m_particle_array;
    }

    void Swarm::move(int elapsed) {

        // If first cycle, do not amplify particle position by anything
        if(last_clock == 0)
            interval = 0;
        else
            interval = elapsed - last_clock;

        for(int i = 0; i < N_PARTICLES; ++i)
            m_particle_array[i].move_particle(interval);

        last_clock = elapsed;
    }

    const Particle * const Swarm::get_particles() {
        return m_particle_array;
    }


} /* namespace ps */
