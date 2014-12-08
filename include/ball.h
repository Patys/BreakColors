#pragma once

#include <SFML/Graphics.hpp>
#include <ParticleSystem_b.h>

class Ball
{
 public:
 Ball(sf::Vector2f _pos, sf::Vector2f _vel, sf::Vector2f _size):
  pos(_pos), vel(_vel), size(_size), particles_emitter(50)
    {}

  void update();

  const sf::Vector2f getPosition() { return pos; }
  void setPosition(sf::Vector2f _pos) { pos = _pos; }

  const sf::Vector2f getVelocity() { return vel; }
  void setVelocity(sf::Vector2f _vel) { vel = _vel; }

  const sf::Vector2f getSize() { return size; }
  void setSize(sf::Vector2f _size) { size = _size; }

  void setTexture(const sf::Texture &tex);
  void setParticleTexture(const sf::Texture &tex);


  void draw(sf::RenderWindow *window);
 private:

  sf::Vector2f pos;
  sf::Vector2f vel;
  sf::Vector2f size;
  sf::Sprite sprite;
  sf::Sprite sprite_particle;
  ParticleSystem_b::Emitter particles_emitter;
};
