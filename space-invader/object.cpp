
#include "object.h"
#include "utils.h";
#include "level.h"

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>


void Object::update(sf::Time deltaTime)
{
	_spriteController->update(deltaTime);

	if (collider != nullptr)
	{
		collider->left = position.x;
		collider->top = position.y;
	}
}

void Object::draw(sf::RenderTarget &target)
{
	_spriteController->draw(target, position);
}

Explosion::Explosion(Vector2 pos) : Object(pos)
{
	_spriteController = make_unique<SpriteController>(Vector2(0, 4), 32, 32);
}

void Explosion::draw(sf::RenderTarget &target) {
	Object::draw(target);
}

void Explosion::update(sf::Time deltaTime)
{
	Object::update(deltaTime);

	_seconds += deltaTime.asSeconds();
	if (_seconds >= _limit)
	{
		Level::instance()->remove(this);
	}

}
