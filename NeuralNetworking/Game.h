#pragma once

#include <iostream>
#include <vector>
#include <ctime>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>

/*
	Class that acts as the game engine.
	Wrapper class.
*/


class Game
{
private:
	//Variables
	sf::RenderWindow* window;
	sf::VideoMode videoMode;
	sf::Event ev;

	//Mouse positions
	sf::Vector2i mousePosWindow;
	sf::Vector2f mousePosView;
	
	//Game logic
	int points;
	float enemySpawnTimer;
	float enemySpawnTimerMax;
	int maxEnemies;
	float randomAngle;
	float enemySpeed;
	float M_PI = 3.14159265358979323846;
	//deltaTime
	float deltaTime;
	sf::Clock clock;
	
	//Game objects
	std::vector<sf::RectangleShape> enemies;
	sf::RectangleShape enemy;
	sf::RectangleShape player;
	
	//Neural network
	
	//linear function
	float linear(float x)
	{
		return x;
	}

	//Sigmoid function
	float sigmoid(float x)
	{
		return (1.0f / (1.0f + exp(-x)));
	}

	// tanh function
	float tanh_func(float x)
	{
		return tanh(x);
	}

	//relu
	float relu(float x)
	{
		return std::max(0.0f, x);
	}
	
	//Enemy health
	std::vector<float> enemiesHealth;
	float enemyHealth;
	
	//allow inputs to have a maximum of
	float inputs[3][1] = { {1.0f}, {0.5f}, {enemyHealth} };
	float weights[3][2] = { {0.1f, 0.2f},
						   {0.2f, 0.4f},
						   {0.3f, 0.4f} };
	float biases[3] = { 1.0f, 1.0f, 1.0f };
	float output[2];

	float sum = 0.0f;

	
	//Private functions
	void initVariables();
	void initEnemies();
	void initPlayer();
	void initWindow();
	
public:
	//Cosntructors / Destructors
	Game();
	virtual ~Game();

	//Accessors
	const bool running() const;

	//Functions
	void spawnEnemy();
	
	void pollEvents();
	void updateMousePositions();
	
	void updateEnemies();
	void updatePlayer();
	void update();
	
	void renderEnemies();
	void renderPlayer();
	void render();

	void Brain();
	void Colliders(float M_PI);
	void DeltaTime();
};

