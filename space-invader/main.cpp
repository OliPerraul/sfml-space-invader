#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include <iostream>
#include <filesystem>


#include "level.h"
#include "utils.h"
#include "main.h"

int main()
{
	// Create the main window
	sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "SFML window");
	window.setFramerateLimit(60);

	sf::Font font;
	if (!font.loadFromFile(resourcePath("\\fonts\\space_invaders.ttf")))
		return EXIT_FAILURE;

	sf::Clock clock;

	sf::Text text("Space Invader", font, 16);

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

		window.draw(text);
		level.draw(window);

		window.display();
	}
	return EXIT_SUCCESS;
}