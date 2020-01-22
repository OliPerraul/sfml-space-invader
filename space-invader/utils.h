#ifndef UTILS_H
#define UTILS_H

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>


#include <iostream>
#include <filesystem>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

typedef sf::Vector2<int> Vector2;

inline std::string resourcePath(std::string resource) {
	return std::filesystem::current_path().concat(resource).string();
}

inline bool isLeftPressed() {
	return (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) || (sf::Keyboard::isKeyPressed(sf::Keyboard::A));
}

inline bool isRightPressed() {
	return (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) || (sf::Keyboard::isKeyPressed(sf::Keyboard::D));
}

inline bool isShootPressed(sf::Event event)
{
	if (event.type == sf::Event::EventType::KeyPressed)
	{
		if (event.key.code == sf::Keyboard::Space || event.key.code == sf::Keyboard::Enter)
			return true;
	}

	return false;
}



#endif