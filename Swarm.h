//
// Created by Andrei Vasilev on 1/13/17.
//

#ifndef PARTICLE_SIMULATION_SWARM_H
#define PARTICLE_SIMULATION_SWARM_H

#include "Particle.h"
#include "Screen.h"

namespace ps {

    class Swarm {
        public:
            const static int N_PARTICLES{6000};

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

} /* namespace ps */

#endif //PARTICLE_SIMULATION_SWARM_H
