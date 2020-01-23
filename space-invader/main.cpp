#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include <iostream>
#include <filesystem>


#include "level.h"
#include "utils.h"
#include "main.h"



int main()
{
	Game game;
	return game.run();
}

Game* Game::_instance = nullptr;

Game::Game()
{
	_instance = this;
}

int Game::run()
{

	// Create the main window
	sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "SPACE INVADER");
	window.setFramerateLimit(60);

	sf::Font font;
	if (!font.loadFromFile(resourcePath("\\fonts\\space_invaders.ttf")))
		return EXIT_FAILURE;

	sf::Clock clock;

	sf::Text titleText("Space Invader", font, 16);
	sf::Text gameOverText("GAME OVER", font, 32);

	Level level;

	while (window.isOpen())
	{
		// Process events
		sf::Event event;
		while (window.pollEvent(event))
		{
			// Close window: exit
			if (event.type == sf::Event::Closed)
				window.close();

			level.onPollEvent(event);
		}

		sf::Time dt = clock.restart();
		level.update(dt);

		// Clear screen
		window.clear();
		window.draw(titleText);
		gameOverText.setPosition(
			sf::Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT) / 2.0f -
			sf::Vector2f(gameOverText.getLocalBounds().width, 0) / 2.0f);

		if (isGameOver)
		{
			window.draw(gameOverText);
		}

		level.draw(window);
		window.display();
	}
	return EXIT_SUCCESS;
}