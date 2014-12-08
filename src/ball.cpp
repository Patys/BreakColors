#include <ball.h>

void Ball::update()
{

  //collision with walls
  if(pos.x + vel.x < 10)
    {
      pos.x += abs(vel.x) * 2;
      vel.x =- vel.x;
    }
  if(pos.x + vel.x > 790-size.x)
    {
      pos.x -= abs(vel.x) * 2;
      vel.x =- vel.x;
    }
  if(pos.y + vel.y < 10)
    {
      pos.y += abs(vel.y) * 2;
      vel.y =- vel.y;
    }
  if(pos.y + vel.y > 600 - size.y)
    {
      pos.y -= abs(vel.y) * 2;
      vel.y =- vel.y;
    }

  // max velocity
  if(vel.y > 6)
    vel.y = 6;
  else if(vel.y < -6)
    vel.y = -6;

  if(vel.y > 0 && vel.y < 3)
    vel.y++;
  else if(vel.y < 0 && vel.y > -3)
    vel.y--;

  // update particles
  particles_emitter.setPosition(pos.x + size.x / 2 - 1,
				pos.y + size.y / 2 - 1);
  particles_emitter.setVelocityParticles((rand()%2-1),(rand()%2-1));
  particles_emitter.update();

  pos += vel;
}

void Ball::setTexture(const sf::Texture &tex)
{
  sprite.setTexture(tex);
}

void Ball::setParticleTexture(const sf::Texture &tex)
{
  sprite_particle.setTexture(tex);
}

sf::Vector2f Ball::getPosition()
{
  return pos;
}

sf::Vector2f Ball::getVelocity()
{
  return vel;
}

void Ball::draw(sf::RenderWindow *window)
{
  for(int i = 0; i < particles_emitter.getNumberOfParticles(); i++)
    {
      sprite_particle.setPosition(particles_emitter.getParticle(i).position.x-4,
				  particles_emitter.getParticle(i).position.y-4);
      window->draw(sprite_particle);
    }
  sprite.setPosition(pos);
  window->draw(sprite);
}
