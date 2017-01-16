//
// Created by Andrei Vasilev on 1/13/17.
//

#ifndef PARTICLE_SIMULATION_PARTICLE_H
#define PARTICLE_SIMULATION_PARTICLE_H

namespace ps {

    class Particle {
        public:
            double m_x_cord;
            double m_y_cord;

        public:
            Particle();
            virtual ~Particle() {};
            void move_particle(int);

        private:
            double m_x_speed;
            double m_y_speed;
            double m_speed;
            double m_direction;

        private:
            void initialize();
    };

} /* namespace ps */


#endif //PARTICLE_SIMULATION_PARTICLE_H
