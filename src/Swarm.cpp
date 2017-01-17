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


    const Particle * const Swarm::get_particles() {
        return m_particle_array;
    }


} /* namespace ps */
