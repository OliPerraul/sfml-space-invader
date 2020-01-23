
#include <iostream>

#include "sprite.h"

using namespace std;

const int FRAME_SIZE = 16;
const int ANIM_FPS = 2;

SpriteController::SpriteController(Vector2 offset, int width, int height)
{
	_offset = offset;
	if (!_texture.loadFromFile(resourcePath("\\sprites\\spritesheet.png")))
		cout << "fail" << endl;

	_sprite.setTexture(_texture, true);
	_sprite.setScale(sf::Vector2<float>(2, 2));
}

void SpriteController::update(sf::Time deltaTime)
{
	_seconds += deltaTime.asSeconds();
	_currentFrame = _seconds * ANIM_FPS;
	_currentFrame %= 2;
}

void SpriteController::draw(sf::RenderTarget &target, Vector2 position)
{
	int left = (_offset.x * 2 + _currentFrame) * FRAME_SIZE;
	int top = _offset.y * FRAME_SIZE;

	_sprite.setPosition(position.x, position.y);
	_sprite.setTextureRect({ left, top, FRAME_SIZE, FRAME_SIZE });
	target.draw(_sprite);
}