#ifndef PARTICLESYSTEM_B_H_INCLUDED
#define PARTICLESYSTEM_B_H_INCLUDED

#include <vector>
#include <cstdlib>

/**< Flexible Particle System by Patryk Szczygło @Trintx */
namespace ParticleSystem_b
{
    /**< structure contain two variables */
    template<class T>
    struct Vector2
    {
        T x;
        T y;
    };

    /**< typedef to float vector */
    typedef Vector2<float> Vector2f;
    /**< typedef to int vector */
    typedef Vector2<int> Vector2i;

    /**< single particle */
    struct Particle_b
    {
        /**< particle's position */
        Vector2f position;
        /**< particle's velocity */
        Vector2f velocity;
        /**< time to reset */
        float live_time;
        /**< true to update particle */
        bool live;
    };

    /**< class which is an emitter of particles */
    class Emitter
    {
    public:
        Emitter(int number_of_particles);

        void update();

        void setPosition(float x, float y);
        Vector2f getPosition();

        void setNumberOfParticles(int num_of_particles);
        int getNumberOfParticles();

        void setLiveTimeParticles(float livetime);
        float getLiveTimeParticles();

        void setVelocityParticles(float x, float y);
        Vector2f getVelocityParticles();

        void setSpan(float span);
        float getSpan();

        Particle_b getParticle(int x);
    private:
        int m_number_particles;
        std::vector<Particle_b> m_particles;

        Vector2f m_start_position;
        Vector2f m_start_direction;
        Vector2f m_start_velocity;
        float m_span;
        float m_start_live_time;
        float m_end_live_time;
    };
};


#endif // PARTICLESYSTEM_B_H_INCLUDED
