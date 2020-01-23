#ifndef SPRITE_H
#define SPRITE_H

#include "utils.h"

class SpriteController
{

public:
	SpriteController(Vector2 offset, int width, int height);
	virtual void update(sf::Time deltaTime);
	virtual void draw(sf::RenderTarget &target, Vector2 position);

	inline sf::Sprite sprite() { return _sprite; }

protected:
	float _seconds = 0;
	Vector2 _offset;
	int _currentFrame = 0;
	sf::Texture _texture;
	sf::Sprite _sprite;
	sf::Image _image;
};


#endif
