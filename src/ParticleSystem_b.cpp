#include "ParticleSystem_b.h"
#include <math.h>

using namespace ParticleSystem_b;

Emitter::Emitter(int number_of_particles):
    m_number_particles(number_of_particles)
{
    for(int x = 0; x < m_number_particles; x++)
    {
        m_particles.push_back(Particle_b());
        m_particles.back().live_time = rand() % 40 + 1;
    }
    setVelocityParticles(0, 0);
    setPosition(-100, -100);
    setLiveTimeParticles(20);
    m_start_live_time = 0;
    m_span = 5;
}

void Emitter::setPosition(float x, float y)
{
    m_start_position.x = x;
    m_start_position.y = y;
}

Vector2f Emitter::getPosition()
{
    return m_start_position;
}

void Emitter::setVelocityParticles(float x, float y)
{
    m_start_velocity.x = x;
    m_start_velocity.y = y;
}

Vector2f Emitter::getVelocityParticles()
{
    return m_start_velocity;
}

void Emitter::setLiveTimeParticles(float livetime)
{
    m_end_live_time = livetime;
}

float Emitter::getLiveTimeParticles()
{
    return m_end_live_time;
}

void Emitter::setNumberOfParticles(int num_of_particles)
{
    while(num_of_particles > m_number_particles)
    {
        m_particles.pop_back();
    }
    while(num_of_particles < m_number_particles)
    {
        m_particles.push_back(Particle_b());
    }
}

int Emitter::getNumberOfParticles()
{
    return m_number_particles;
}

void Emitter::update()
{
    for(unsigned int x = 0; x < m_particles.size(); x++)
    {
        if(m_particles[x].live_time > m_end_live_time)
        {
            m_particles[x].live_time = m_start_live_time + rand() % 10 + 1;

            m_particles[x].velocity.x = m_start_velocity.x + (rand() % 3 - 1)/5.f;
            m_particles[x].velocity.y = m_start_velocity.y + (rand() % 3 - 1)/5.f;

            m_particles[x].position.x = m_start_position.x;
            m_particles[x].position.y = m_start_position.y;
        }
        else
        {
            m_particles[x].live_time += 1;

            m_particles[x].position.x += m_particles[x].velocity.x;//+ static_cast<float>(fmod(rand(), m_span)) - (m_span / 2)) / 2);
            m_particles[x].position.y += m_particles[x].velocity.y; //+ static_cast<float>(fmod(rand(), m_span)) - (m_span / 2)) / 2);
        }
    }
}

void Emitter::setSpan(float span)
{
    m_span = span;
}

float Emitter::getSpan()
{
    return m_span;
}

Particle_b Emitter::getParticle(int x)
{
    return m_particles[x];
}
