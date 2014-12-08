#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <cmath>

#include <ResourceMenager.h>
#include <ParticleSystem_b.h>
#include <maps.hpp>

#include <ball.h>

/**< sprawdzanie kolizji AABB */
bool isCollision(float x1, float y1, float w1, float h1,
                 float x2, float y2, float w2, float h2 )
{
  if( x1 + w1 >= x2 && x1 <= x2 + w2 && y1 + h1 >= y2 && y1 <= y2 + h2 )
    return( true );
  return( false );
}

/**< struktura zaiwerajaca informacje o klockach */
struct BLOCK
{
  BLOCK(int _x, int _y, int _wx, int _hy, int _type):
    x(_x), y(_y), wx(_wx), hy(_hy), type(_type) { }
  int x, y;
  int wx,hy;
  int type;
};

/**< struktura zawierajaca informacje o bonusach */
struct BONUS
{
  BONUS(sf::Vector2f _pos, sf::Vector2f _vel, sf::Vector2f _size, int _type):
    pos(_pos), vel(_vel), size_spr(_size), type(_type), particle_emitter(20)
  {
    particle_emitter.setSpan(8);
  }
  sf::Vector2f pos;
  sf::Vector2f vel;
  sf::Vector2f size_spr;
  int type; // 1-pilka, 2-szybciej deska, 3-wolniej deska
  ParticleSystem_b::Emitter particle_emitter;
};

/**< struktura zawierajaca informacje o rakiecie */
struct ROCKET
{
  ROCKET(sf::Vector2f _pos, sf::Vector2f _vel, sf::Vector2f _size):
    pos(_pos), vel(_vel), size_spr(_size), particle_emitter(50)
  {
    particle_emitter.setSpan(10);
    particle_emitter.setVelocityParticles(0, 2);
  }

  sf::Vector2f pos;
  sf::Vector2f vel;
  sf::Vector2f size_spr;
  ParticleSystem_b::Emitter particle_emitter;
};

int main()
{
  /**< creating window */
  sf::RenderWindow window(sf::VideoMode(800, 600), "BreakColors");
  window.setFramerateLimit(60);

  /**< rand */
  srand(time(0));

  /**< resource manager to keep textures */
  ResourceMenager tex_manager;

  /**< sprites to render */
  sf::Sprite spr_blocks(tex_manager.getTexture("data/blocks.png"));
  sf::Sprite spr_ball(tex_manager.getTexture("data/ball.png"));
  sf::Sprite spr_desk(tex_manager.getTexture("data/desk.png"));
  spr_desk.setTextureRect(sf::IntRect(0,0,96,32));
  sf::Sprite spr_bonus(tex_manager.getTexture("data/bonus.png"));
  sf::Sprite spr_background_menu(tex_manager.getTexture("data/background.png"));
  sf::Sprite background(tex_manager.getTexture("data/background5.png"));
  sf::Sprite spr_rocket(tex_manager.getTexture("data/rocket.png"));
  sf::Sprite spr_particle(tex_manager.getTexture("data/star.png"));


  /**< font used in menu and game */
  sf::Font font;
  font.loadFromFile("data/Russo_One.ttf");

  /**< Texts in menu and game */
  sf::Text score;
  score.setFont(font);
  score.setCharacterSize(16);
  score.setColor(sf::Color(255,255,255));

  sf::Text menu_restart;
  menu_restart.setFont(font);
  menu_restart.setCharacterSize(28);
  menu_restart.setColor(sf::Color(255,255,255));
  menu_restart.setString("Restart");
  menu_restart.setPosition(450,200);

  sf::Text menu_credits;
  menu_credits.setFont(font);
  menu_credits.setCharacterSize(28);
  menu_credits.setColor(sf::Color(255,255,255));
  menu_credits.setString("Credits");
  menu_credits.setPosition(450,250);

  sf::Text menu_exit;
  menu_exit.setFont(font);
  menu_exit.setCharacterSize(28);
  menu_exit.setColor(sf::Color(255,255,255));
  menu_exit.setString("Exit");
  menu_exit.setPosition(450,300);

  sf::Text menu_credits_text;
  menu_credits_text.setFont(font);
  menu_credits_text.setCharacterSize(24);
  menu_credits_text.setColor(sf::Color(0,0,0));
  menu_credits_text.setString("GAME\nCREATED\nBY PATYS :D");
  menu_credits_text.setPosition(60,380);

  sf::Text menu_score;
  menu_score.setFont(font);
  menu_score.setCharacterSize(24);
  menu_score.setColor(sf::Color(0,0,0));
  menu_score.setPosition(20,20);

  sf::Text menu_resume;
  menu_resume.setFont(font);
  menu_resume.setCharacterSize(28);
  menu_resume.setColor(sf::Color(255,255,255));
  menu_resume.setString("Resume");
  menu_resume.setPosition(450,150);

  sf::Text text_rocket;
  text_rocket.setFont(font);
  text_rocket.setCharacterSize(14);
  text_rocket.setColor(sf::Color(255,255,255));
  text_rocket.setString("You Have Rocket!!! CLICK");
  text_rocket.setPosition(300,50);

  /**< actual score and live's player */
  int score_game = 0;
  int lives = 3;
  bool has_rocket = false;
  int rand_bonus = 30;

  /**< vector contain blocks, bonuses and balls */
  std::vector<BLOCK> blocks;
  std::vector<BONUS> bonuses;
  std::vector<ROCKET> rockets;
  std::vector<Ball> balls;

  /*
    for(int x = 0; x < 15; x++)
    {
    for(int y = 0; y < 6; y++)
    {
    if(map_one[y][x] == 1)
    blocks.push_back(BLOCK(x * 48 + 48, y * 48 + 48, 32, 32, 0));
    if(map_one[y][x] == 2)
    blocks.push_back(BLOCK(x * 48 + 48, y * 48 + 48, 32, 32, 1));
    if(map_one[y][x] == 3)
    blocks.push_back(BLOCK(x * 48 + 48, y * 48 + 48, 32, 32, 2));
    }
    }
  */

  /**< variables to physics desk */
  sf::Vector2f pos_desk(470,568);
  sf::Vector2f size_desk(96,32);
  sf::Vector2f delta_desk(0,0);
  sf::Vector2f coefficient_desk(1,1);

  /**< particles for desk */
  ParticleSystem_b::Emitter particle_desk(30);
  particle_desk.setVelocityParticles(-delta_desk.x, -delta_desk.y);
  particle_desk.setSpan(0);

  /**< states of levels */
  bool state_game = false; // false - menu, true - game
  bool draw_credits = false;
  int level = 0;
  bool first_run = true;

  while (window.isOpen())
    {
      /**< handle events */
      sf::Event event;
      while (window.pollEvent(event))
        {
	  if (event.type == sf::Event::Closed)
	    window.close();
	  if(event.type == sf::Event::KeyPressed)
            {
	      switch(event.key.code)
                {
                case sf::Keyboard::Escape:
		  if(state_game == false)
		    state_game = true;
		  else state_game = false;
		  break;
                default:
		  break;
                }
            }
	  if(event.type == sf::Event::MouseButtonPressed)
            {
	      switch(event.mouseButton.button)
                {
                case sf::Mouse::Left:
		  if(state_game == true && has_rocket == true)
                    {
		      rockets.push_back(ROCKET(sf::Vector2f(pos_desk.x + size_desk.x / 2 - 16, pos_desk.y + size_desk.y / 2 - 32), sf::Vector2f(0,-5), sf::Vector2f(32,64)));
		      has_rocket = false;
                    }
		  break;
                default:
		  break;
                }
            }

        }
      /**< play */
      if(state_game == true)
        {
	  window.setMouseCursorVisible(false);
	  /**< if have not any blocks */
	  if(blocks.empty())
            {
	      /**< go to next level */
	      level++;
	      /**< first level load from: maps.hpp */
	      // TODO: fuck off this loading
	      if(level == 1)
                {
		  /**< size map: 13x5 */
		  for(int x = 0; x < 13; x++)
                    {
		      for(int y = 0; y < 5; y++)
                        {
			  if(map_two[y][x] == 1)
			    blocks.push_back(BLOCK(x * 48 + 80, y * 48 + 80, 32, 32, 0));
			  if(map_two[y][x] == 2)
			    blocks.push_back(BLOCK(x * 48 + 80, y * 48 + 80, 32, 32, 1));
			  if(map_two[y][x] == 3)
			    blocks.push_back(BLOCK(x * 48 + 80, y * 48 + 80, 32, 32, 2));
                        }
                    }
		  /**< remove all balls and add new */
		  balls.clear();
		  balls.push_back(Ball(sf::Vector2f(pos_desk.x + 32, pos_desk.y - 32), sf::Vector2f(5,-5),sf::Vector2f(32,32)));
		  balls.back().setTexture(tex_manager.getTexture("data/ball.png"));
		  balls.back().setParticleTexture(tex_manager.getTexture("data/star1.png"));

                }
	      /**< another generate randomize */
	      else if(level >= 2)
                {
		  /**< size map: 13x5 */
		  for(int x = 0; x < 13; x++)
                    {
		      for(int y = 0; y < 5; y++)
                        {
			  blocks.push_back(BLOCK(x * 48 + 80, y * 48 + 80, 32, 32, rand()%3));
                        }
                    }
		  /**< remove all balls and add new */
		  balls.clear();
		  balls.push_back(Ball(sf::Vector2f(pos_desk.x + 32, pos_desk.y - 32), sf::Vector2f(5,-5),sf::Vector2f(32,32)));
		  balls.back().setTexture(tex_manager.getTexture("data/ball.png"));
		  balls.back().setParticleTexture(tex_manager.getTexture("data/star1.png"));
                }
            }

	  /**< if have not any lives, go to menu */
	  if(lives < 0)
	    state_game = false;

	  /**< controlling desk */
	  if(sf::Mouse::getPosition(window).x < 396)
            {
	      delta_desk.x -= 4;
	      sf::Mouse::setPosition(sf::Vector2i(400,300),window);
            }
	  else if(sf::Mouse::getPosition(window).x > 404)
            {
	      delta_desk.x += 4;
	      sf::Mouse::setPosition(sf::Vector2i(400,300),window);
            }
	  if(delta_desk.x > 0)
	    delta_desk.x -= 2;
	  else if(delta_desk.x < 0)
	    delta_desk.x += 2;

	  if(delta_desk.x > 16)
	    delta_desk.x = 16;
	  else if(delta_desk.x < -16)
	    delta_desk.x = -16;

	  /**< leave area of game */
	  if(pos_desk.x + delta_desk.x < 0)
            {
	      //delta_desk.x = -delta_desk.x * 0.3;
	      delta_desk.x = 0;
	      pos_desk.x = 0;

            }
	  if(pos_desk.x + delta_desk.x > 800 - size_desk.x)
            {
	      //delta_desk.x = -delta_desk.x * 0.3;
	      delta_desk.x = 0;
	      pos_desk.x = 800-size_desk.x;
            }

	  /**< balls updating */
	  for(size_t x = 0; x < balls.size(); x++)
	    {
	      balls[x].update();
	      
	      // collsion with desk
	      //TODO: Ball getSize()
	      if(isCollision(balls[x].getPosition().x, balls[x].getPosition().y,
			     32, 32, // here need Ball getSize()
			     pos_desk.x + delta_desk.x, pos_desk.y,
			     size_desk.x, size_desk.y))
                {
		  balls[x].setVelocity(sf::Vector2f(balls[x].getVelocity().x,
						    -balls[x].getVelocity().y));
		  balls[x].setPosition(sf::Vector2f(balls[x].getPosition().x,
						    balls[x].getPosition().y + balls[x].getVelocity().y * 2));
                }
	      // collision with bottom
	      if(balls[x].getPosition().y > 600 - 36)
                {
		  if(balls.size() <= 1)
		    lives--;
		  if(balls.size() > 1)
		    balls.erase(balls.begin() + x);
                }

            }

	  /**< blocks update */
	  for(size_t x = 0; x < blocks.size(); x++)
            {
	      for(size_t i = 0; i < balls.size(); i++)
                {
		  /**< collision with balls */
		  if(isCollision(balls[i].getPosition().x, balls[i].getPosition().y,
				 32, 32, //TODO: Ball getSize()
				 blocks[x].x, blocks[x].y, blocks[x].wx, blocks[x].hy))
                    {
		      // TODO: check and optymalise
		      if(balls[i].getPosition().x + 32 < blocks[x].x)
			balls[i].setVelocity(sf::Vector2f(balls[i].getVelocity().x - balls[i].getVelocity().x,
							  balls[i].getVelocity().y));
		      if(balls[i].getPosition().x > blocks[x].x+32)
			balls[i].setVelocity(sf::Vector2f(balls[i].getVelocity().x - balls[i].getVelocity().x,
							  balls[i].getVelocity().y));
		      if(balls[i].getPosition().y + 32 < blocks[x].y)
		        balls[i].setVelocity(sf::Vector2f(balls[i].getVelocity().x,
							  balls[i].getVelocity().y - balls[i].getVelocity().y));
		      if(balls[i].getPosition().y > blocks[x].y+32)
			balls[i].setVelocity(sf::Vector2f(balls[i].getVelocity().x,
							  balls[i].getVelocity().y - balls[i].getVelocity().y));

		      // add bonus
		      if(rand()%100 + 1 < rand_bonus)
                        {
			  bonuses.push_back(BONUS(sf::Vector2f(blocks[x].x,blocks[x].y), sf::Vector2f(0,4),
						  sf::Vector2f(32,32),rand()%6+1));
                        }

		      blocks.erase(blocks.begin()+x);
		      //balls[i].pos.y += balls[i].vel.y * 2;
		      score_game++;

                    }
                }

            }


	  /**< update bonuses */
	  for(size_t x = 0; x < bonuses.size(); x++)
            {
	      bonuses[x].pos += bonuses[x].vel;

	      bonuses[x].particle_emitter.setPosition(bonuses[x].pos.x + 16, bonuses[x].pos.y + 16);
	      //bonuses[x].particle_emitter.setVelocityParticles(bonuses[x].vel.x,bonuses[x].vel.y);
	      bonuses[x].particle_emitter.update();


	      if(bonuses[x].pos.y < 0)
		bonuses.erase(bonuses.begin()+x);
	      /**< collision with desk */
	      if(isCollision(bonuses[x].pos.x, bonuses[x].pos.y, bonuses[x].size_spr.x, bonuses[x].size_spr.y,
			     pos_desk.x + delta_desk.x, pos_desk.y,
			     size_desk.x, size_desk.y))
                {
		  /**< apply bonus */
		  if(bonuses[x].type == 1)
		    {
		      balls.push_back(Ball(sf::Vector2f(pos_desk.x + 32, pos_desk.y - 32), sf::Vector2f(5,-5),sf::Vector2f(32,32)));
		      balls.back().setTexture(tex_manager.getTexture("data/ball.png"));
		      balls.back().setParticleTexture(tex_manager.getTexture("data/star1.png"));
		    }
		  else if(bonuses[x].type == 2)
                    {
		      lives++;
		      score_game+=10;
                    }
		  else if(bonuses[x].type == 3)
                    {
		      score_game += 50;
                    }
		  else if(bonuses[x].type == 4)
                    {
		      if(size_desk.x == 96)
                        {
			  spr_desk.setTextureRect(sf::IntRect(96,0,128,32));
			  size_desk.x = 128;
                        }
		      else if(size_desk.x < 96)
                        {
			  spr_desk.setTextureRect(sf::IntRect(0,0,96,32));
			  size_desk.x = 96;
                        }
		      score_game+=5;
                    }
		  else if(bonuses[x].type == 5)
                    {
		      if(size_desk.x == 96)
                        {
			  spr_desk.setTextureRect(sf::IntRect(224,0,64,32));
			  size_desk.x = 64;
                        }
		      else if(size_desk.x > 96)
                        {
			  spr_desk.setTextureRect(sf::IntRect(0,0,96,32));
			  size_desk.x = 96;
                        }
		      score_game+=5;
                    }
		  else if(bonuses[x].type == 6)
                    {
		      has_rocket = true;
		      score_game+=15;
                    }

		  bonuses.erase(bonuses.begin()+x);
                }
            }

	  /**< update rockets */
	  for(size_t x = 0; x < rockets.size(); x++)
            {
	      rockets[x].pos += rockets[x].vel;


	      rockets[x].particle_emitter.setPosition(rockets[x].pos.x + 16, rockets[x].pos.y + 32);
	      //rockets[x].particle_emitter.setVelocityParticles(rockets[x].vel.x,rockets[x].vel.y);
	      rockets[x].particle_emitter.update();

	      for(size_t i = 0; i < blocks.size(); i++)
                {
		  if(isCollision(rockets[x].pos.x, rockets[x].pos.y, 32, 64,
				 blocks[i].x, blocks[i].y, 32, 32))
                    {
		      blocks.erase(blocks.begin() + i);
                    }
                }
            }

	  /**< update desk */
	  pos_desk.x += delta_desk.x;

	  /**< update desk's particles */
	  particle_desk.setPosition(pos_desk.x + size_desk.x / 2, pos_desk.y + size_desk.y / 2);
	  if(delta_desk.x > 4 || delta_desk.x < -4)
	    particle_desk.setVelocityParticles((rand()%2-1),(rand()%2-1));
	  else
	    particle_desk.setVelocityParticles(0,0);
	  particle_desk.update();

	  /**< showing score and lives */
	  std::ostringstream _score_string;
	  _score_string << score_game;
	  
	  std::ostringstream _lives_string;
	  _lives_string << lives;
	  
	  score.setString( "Points: " + _score_string.str() + "\n" +
			   "Lives: " + _lives_string.str());
	  sf::FloatRect textRect = score.getLocalBounds();
	  score.setOrigin(textRect.left + textRect.width/2.0f,
			  textRect.top  + textRect.height/2.0f);
	  score.setPosition(sf::Vector2f(400,textRect.top  + textRect.height/2.0f + 10));
	    

	  /**< drawing */
	  window.clear();
	  window.draw(background);
	  for(unsigned int x = 0; x < blocks.size(); x++)
            {
	      spr_blocks.setPosition(blocks[x].x, blocks[x].y);
	      if(blocks[x].type == 0)
                {
		  spr_blocks.setTextureRect(sf::IntRect(0,0,32,32));
                }
	      else if(blocks[x].type == 1)
                {
		  spr_blocks.setTextureRect(sf::IntRect(32,0,32,32));
                }
	      else if(blocks[x].type == 2)
                {
		  spr_blocks.setTextureRect(sf::IntRect(64,0,32,32));
                }
	      window.draw(spr_blocks);
            }
	  for(size_t x = 0; x < bonuses.size(); x++)
            {
	      spr_bonus.setPosition(bonuses[x].pos);

	      for(int i = 0; i < bonuses[x].particle_emitter.getNumberOfParticles(); i++)
                {
		  spr_particle.setPosition(bonuses[x].particle_emitter.getParticle(i).position.x-4,
					   bonuses[x].particle_emitter.getParticle(i).position.y-4);
		  if(bonuses[x].type == 1)
                    {
		      spr_bonus.setTextureRect(sf::IntRect(0,0,32,32));
		      spr_particle.setTextureRect(sf::IntRect(0,0,8,8));
                    }
		  else if(bonuses[x].type == 2)
                    {
		      spr_bonus.setTextureRect(sf::IntRect(32,0,32,32));
		      spr_particle.setTextureRect(sf::IntRect(24,0,8,8));
                    }
		  else if(bonuses[x].type == 3)
                    {
		      spr_bonus.setTextureRect(sf::IntRect(64,0,32,32));
		      spr_particle.setTextureRect(sf::IntRect(16,0,8,8));
                    }
		  else if(bonuses[x].type == 4)
                    {
		      spr_bonus.setTextureRect(sf::IntRect(96,0,32,32));
		      spr_particle.setTextureRect(sf::IntRect(8,0,8,8));
                    }
		  else if(bonuses[x].type == 5)
                    {
		      spr_bonus.setTextureRect(sf::IntRect(128,0,32,32));
		      spr_particle.setTextureRect(sf::IntRect(32,0,8,8));
                    }
		  else if(bonuses[x].type == 6)
                    {
		      spr_bonus.setTextureRect(sf::IntRect(160,0,32,32));
		      spr_particle.setTextureRect(sf::IntRect(24,0,8,8));
                    }
		  else std::cout << "BUG wyswietalaniea bonusa" << std::endl;
		  window.draw(spr_particle);
                }
	      window.draw(spr_bonus);
            }
	  for(int i = 0; i < particle_desk.getNumberOfParticles(); i++)
            {
	      spr_particle.setPosition(particle_desk.getParticle(i).position.x-4,
				       particle_desk.getParticle(i).position.y-4);
	      spr_particle.setTextureRect(sf::IntRect(8,0,8,8));

	      window.draw(spr_particle);
            }
	  spr_desk.setPosition(pos_desk);
	  window.draw(spr_desk);
	  for(size_t x = 0; x < balls.size(); x++)
            {
	      balls[x].draw(&window);
            }
	  for(size_t x = 0; x < rockets.size(); x++)
            {
	      spr_rocket.setPosition(rockets[x].pos);
	      for(int i = 0; i < rockets[x].particle_emitter.getNumberOfParticles(); i++)
                {
		  spr_particle.setPosition(rockets[x].particle_emitter.getParticle(i).position.x-4,
					   rockets[x].particle_emitter.getParticle(i).position.y-4);
		  spr_particle.setTextureRect(sf::IntRect(24,0,8,8));

		  window.draw(spr_particle);
                }
	      window.draw(spr_rocket);
            }
	  window.draw(score);
	  if(has_rocket == true)
	    window.draw(text_rocket);
	  window.display();
        }
      ///////////////////////////////////////////////////////////////////////////
      /**< menu */
      else //state_game == false
        {
	  window.setMouseCursorVisible(true);

	  /**< show last score */
	  if(score_game != 0)
            {
	      std::ostringstream score_text;
	      score_text << score_game;
	      menu_score.setString("Last score: " + score_text.str());
            }
	  if(first_run)
            {
	      menu_restart.setString("Start");
            }
	  else
            {
	      menu_restart.setString("Restart");
            }

	  /**< check click on start/restart */
	  sf::FloatRect text_restart_rect = menu_restart.getLocalBounds();
	  if(sf::Mouse::isButtonPressed(sf::Mouse::Left) &&
	     isCollision(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y,1,1,
			 menu_restart.getPosition().x, menu_restart.getPosition().y,
			 text_restart_rect.width, text_restart_rect.height))
            {
	      /**< generating new game */
	      first_run = false;
	      state_game = true;
	      draw_credits = false;
	      has_rocket = false;
	      lives = 3;
	      score_game = 0;
	      blocks.clear();
	      bonuses.clear();
	      balls.clear();
	      for(int x = 0; x < 13; x++)
                {
		  for(int y = 0; y < 5; y++)
                    {
		      if(map_one[y][x] == 1)
			blocks.push_back(BLOCK(x * 48 + 80, y * 48 + 80, 32, 32, 0));
		      if(map_one[y][x] == 2)
			blocks.push_back(BLOCK(x * 48 + 80, y * 48 + 80, 32, 32, 1));
		      if(map_one[y][x] == 3)
			blocks.push_back(BLOCK(x * 48 + 80, y * 48 + 80, 32, 32, 2));
                    }
                }
	      if(size_desk.x != 96)
                {
		  spr_desk.setTextureRect(sf::IntRect(0,0,96,32));
		  size_desk.x = 96;
                }
	      balls.push_back(Ball(sf::Vector2f(pos_desk.x + 32, pos_desk.y - 32), sf::Vector2f(5,-5),sf::Vector2f(32,32)));
	      balls.back().setTexture(tex_manager.getTexture("data/ball.png"));
	      balls.back().setParticleTexture(tex_manager.getTexture("data/star1.png"));

            }

	  /**< resume game */
	  if(!first_run && lives >= 0)
            {
	      sf::FloatRect text_resume_rect = menu_resume.getLocalBounds();
	      if(sf::Mouse::isButtonPressed(sf::Mouse::Left) &&
		 isCollision(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y,1,1,
			     menu_resume.getPosition().x, menu_resume.getPosition().y,
			     text_resume_rect.width, text_resume_rect.height))
                {
		  state_game = true;
		  draw_credits = false;
                }
            }

	  /**< exit */
	  sf::FloatRect text_exit_rect = menu_exit.getLocalBounds();
	  if(sf::Mouse::isButtonPressed(sf::Mouse::Left) &&
	     isCollision(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y,1,1,
			 menu_exit.getPosition().x, menu_exit.getPosition().y,
			 text_exit_rect.width, text_exit_rect.height))
            {
	      window.close();
            }
	  /**< credits */
	  sf::FloatRect text_credits_rect = menu_credits.getLocalBounds();
	  if(sf::Mouse::isButtonPressed(sf::Mouse::Left) &&
	     isCollision(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y,1,1,
			 menu_credits.getPosition().x, menu_credits.getPosition().y,
			 text_credits_rect.width, text_credits_rect.height))
            {
	      draw_credits = true;
            }

	  /**< drawing */
	  window.clear();
	  window.draw(spr_background_menu);
	  window.draw(menu_restart);
	  window.draw(menu_credits);
	  window.draw(menu_exit);
	  if(!first_run && lives >= 0)
            {
	      window.draw(menu_resume);
            }
	  if(score_game != 0)
            {
	      window.draw(menu_score);
            }
	  if(draw_credits)
            {
	      window.draw(menu_credits_text);
            }
	  window.display();
        }

    }
  return EXIT_SUCCESS;
}
