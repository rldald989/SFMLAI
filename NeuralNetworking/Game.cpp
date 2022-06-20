#include "Game.h"

//private
//Initialize
void Game::initVariables()
{
	this->window = nullptr;

	//Game logic
	this->points = 0;
	this->enemySpawnTimerMax = 5.0f;
	this->enemySpawnTimer = this->enemySpawnTimerMax;
	this->maxEnemies = 80.0f;
}

void Game::initEnemies()
{
	this->enemy.setPosition(10.0f, 10.0f);
	this->enemy.setSize(sf::Vector2f(100.0f, 100.0f));
	this->enemy.setScale(sf::Vector2f(0.21f, 0.21f));
	this->enemy.setFillColor(sf::Color(40, 44, 52, 255));
	this->enemy.setOrigin(0, 50);
	//this->enemy.setOutlineColor(sf::Color(29, 32, 39, 255));
	//this->enemy.setOutlineThickness(4.5f);
}

void Game::initPlayer()
{
	this->player.setPosition(10.0f, 10.0f);
	this->player.setSize(sf::Vector2f(100.0f, 100.0f));
	this->player.setScale(sf::Vector2f(0.6f, 0.6f));
	this->player.setFillColor(sf::Color(160, 110, 253, 255));
	this->player.setOrigin(100, 100);
}

void Game::initWindow()
{
	this->videoMode.width = 1920 / 1.5;
	this->videoMode.height = 1080 / 1.5;
	
	this->window = new sf::RenderWindow(this->videoMode, "Collision Testing", sf::Style::Titlebar | sf::Style::Close);

	this->window->setFramerateLimit(60);
}

//Constructers / Destructors
Game::Game()
{
	this->initVariables();
	this->initWindow();
	this->initEnemies();
	this->initPlayer();
}

Game::~Game()
{
	delete this->window;
}

//Accessors
const bool Game::running() const 
{
	return this->window->isOpen();
}

//Functions
void Game::spawnEnemy()
{
	/*
		@return void
		
		Spawns enemies and sets their colors and positions.
		-Sets a random position.
		-Sets a random color.
		-Adds enemy to the vector.
	*/
	
	this->enemy.setPosition(
		(rand() % (int)(this->videoMode.width - this->enemy.getSize().x)),
		100
	);

	this->enemy.setFillColor(sf::Color::Green);
	
	//spawn the enemy
	this->enemies.push_back(this->enemy);

	//Remove enemies at end of screen
	
}

void Game::pollEvents()
{
	//Event polling
	while (this->window->pollEvent(this->ev))
	{
		switch (this->ev.type)
		{
			case
			sf::Event::Closed:
				this->window->close();
				break;
			case sf::Event::KeyPressed:
				if (this->ev.key.code == sf::Keyboard::Escape)
				{
					this->window->close();
				}
				
				updatePlayer();
		}
	}
	
}

//Updating
void Game::updateMousePositions()
{
	/*
		@return void
	
		Update the mous positions
		- Mouse position relative to the window
	*/
	
	this->mousePosWindow = sf::Mouse::getPosition(*this->window);
	this->mousePosView = this->window->mapPixelToCoords(this->mousePosWindow);
}

void Game::updateEnemies()
{
	/*
		@return void
		
		Updates th enemy spawn timer and spawns enemies.
		when the total amount of enemies is the max.
		Moves the enemies downwards.
		Removes the enemies at the edge of the screen. //TODO
	*/
	
	//Updating the timer for enemy spawning
	if (this->enemies.size() < this->maxEnemies) 
	{
			
		if (this->enemySpawnTimer >= this->enemySpawnTimerMax) 
		{
			//Spawn the enemy and reset the timer

			this->spawnEnemy();
			
			//if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
			//{
			//	//Spawn the enemy where the mouse is
			//	
			//}
			this->enemySpawnTimer = 0.0f;
		}
		else
			this->enemySpawnTimer += 1.0f;
	}
	
	//Move and updating enemies
	for (int i =0; i < this->enemies.size(); i++)
	{
		this->randomAngle = rand() % 360;
		enemy.rotate(this->randomAngle * this->deltaTime);
		
		bool deleted = false;
		
		//Set the enemy's position in the direction that they are rotated
		this->enemies[i].move(
			this->enemies[i].getSize().x * cos(this->enemies[i].getRotation() * M_PI / 180) * this->deltaTime,
			this->enemies[i].getSize().y * sin(this->enemies[i].getRotation() * M_PI / 180) * this->deltaTime
		);
		
		//Make sure enemies can't spawn inside of other enemies
		for (int j = 0; j < this->enemies.size(); j++)
		{
			if (i != j)
			{
				if (this->enemies[i].getGlobalBounds().intersects(this->enemies[j].getGlobalBounds()))
				{
					this->enemies[i].move(
						this->enemies[i].getSize().x * cos(this->enemies[i].getRotation() * M_PI / 180) * this->deltaTime,
						this->enemies[i].getSize().y * sin(this->enemies[i].getRotation() * M_PI / 180) * this->deltaTime
					);
				}
			}
		}

		this->Colliders(M_PI);
		
		//Check if clicked upon
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) 
		{
			if (this->enemies[i].getGlobalBounds().contains(this->mousePosView))
			{
				deleted = true;
				
				//Gain points
				this->points += 10.0f;
			}
		}

		//If the enemy is past the bottom of the screen
		if (this->enemies[i].getPosition().y > this->window->getSize().y || this->enemies[i].getPosition().y > (this->window->getSize().y * -1))
		{
			deleted = true;
		}
		
		if (this->enemies[i].getPosition().x > this->window->getSize().x || this->enemies[i].getPosition().x > (this->window->getSize().x * -1))
		{
			deleted = true;
		}
		
		if (deleted)
			this->enemies.erase(this->enemies.begin() + i);
	}

}

void Game::updatePlayer()
{


	if (this->ev.key.code == sf::Keyboard::W)
	{
		this->player.move(0, -300.0f * this->deltaTime);
	}
	else if (this->ev.key.code == sf::Keyboard::S)
	{
		this->player.move(0, 300.0f * this->deltaTime);
	}
	else if (this->ev.key.code == sf::Keyboard::A)
	{
		this->player.move(-300.0f * this->deltaTime, 0);
	}
	else if (this->ev.key.code == sf::Keyboard::D)
	{
		this->player.move(300.0f * this->deltaTime, 0);
	}
}

void Game::update()
{
	this->pollEvents();

	this->updateMousePositions();

	this->updateEnemies();

	this->Brain();
	
	this->DeltaTime();
}

//Rendering
void Game::renderEnemies()
{
	for (auto& e : this->enemies)
	{
		this->window->draw(e);
	}
}

void Game::renderPlayer()
{
	this->window->draw(this->player);
}

void Game::render()
{
	/*
	*	@return void
	*	- clear old frame
	*	- render objects
	*   - display frame in window
	* 
	*	Renders the game objects.
	*/
	
	this->window->clear(sf::Color(40, 44, 52, 255));

	//Draw game objects
	this->renderEnemies();
	this->renderPlayer();
	
	this->window->display();
}

//Logic
void Game::Brain()
{
	//matrix multiplication
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 1; j++)
		{
			for (int k = 0; k < 2; k++)
			{
				sum += inputs[k][j] * weights[i][k];
			}
			//Apply activation function
			output[i] = relu(sum + biases[i]);
			sum = 0;
		}
	}

	// output layer
	for (int i = 0; i < 2; i++)
	{
		//std::cout << output[i] << " ";
	}

	//softmax function
	float exp_sum = 0.0f;

	for (int i = 0; i < 2; i++)
	{
		exp_sum += exp(output[i]);
		//Using the two outputs, if the first is greater than the second, rotate the enemy to the left, otherwise rotate the enemy to the right
		if (output[0] > output[1])
		{
			this->enemy.rotate(-1.0f * this->deltaTime);
		}
		else
		{
			this->enemy.rotate(1.0f * this->deltaTime);
		}
	}

	//std::cout << std::endl << std::endl;
	//std::cout << "Softmax output: " << std::endl;

	for (int i = 0; i < 2; i++)
	{
		//std::cout << exp(output[i]) / exp_sum << " ";
	}

}

void Game::Colliders(float M_PI)
{
	float rotationi = 0.0f;
	float rotationj = 0.0f;
	//Check if enemies are intersecting with each other, and if they are, have them move in the other direction
	for (int i = 0; i < this->enemies.size(); i++)
	{
		for (int j = 0; j < this->enemies.size(); j++)
		{
			if (i != j)
			{
				if (this->enemies[i].getGlobalBounds().intersects(this->enemies[j].getGlobalBounds()) || this->enemies[i].getGlobalBounds().intersects(this->player.getGlobalBounds()))
				{
					rotationi = this->enemies[i].getRotation();
					{
						rotationi = this->enemies[i].getRotation();
						{
							this->enemies[i].move(
								this->enemies[i].getSize().x * cos(this->enemies[i].getRotation() * M_PI / 180) * -0.01f * this->deltaTime,
								this->enemies[i].getSize().y * sin(this->enemies[i].getRotation() * M_PI / 180) * -0.01f * this->deltaTime
							);
							//Rotate the enemy when they collide
							this->enemies[i].rotate((rotationi += this->enemies[j].getRotation() * M_PI / 180) * this->deltaTime);
							this->enemies[j].rotate((rotationj += this->enemies[i].getRotation() * M_PI / 180) * this->deltaTime);
							
							//Turn enemies red when they collide
							this->enemies[i].setFillColor(sf::Color(255, 0, 0, 255));
							this->enemies[j].setFillColor(sf::Color(255, 0, 0, 255));
							
							//Turn enemies back to white when they stop colliding
							if (!this->enemies[i].getGlobalBounds().intersects(this->enemies[j].getGlobalBounds()))
							{
								this->enemies[i].setFillColor(sf::Color::Green);
								this->enemies[j].setFillColor(sf::Color::Green);
							}
							
							//Set the enemyHealth to a negative value when they collide
							if (this->enemies[i].getGlobalBounds().intersects(this->player.getGlobalBounds()))
							{
								this->enemyHealth -= 1;
							}
							
						}
					}
				}
			}
		}
	}
}

void Game::DeltaTime()
{
	this->deltaTime = this->clock.restart().asSeconds();
}



