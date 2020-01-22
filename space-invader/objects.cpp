
#include "objects.h"
#include "utils.h";
#include "level.h"

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>


const int FRAME_SIZE = 16;
const int ANIM_FPS = 2;

SpriteController::SpriteController(Vector2 offset)
{
	_offset = offset;
	if (!_texture.loadFromFile(resourcePath("\\sprites\\spritesheet.png")))
		cout << "fail" << endl;

	_sprite.setTexture(&_texture);
	_sprite.setSize(sf::Vector2<float>(SPRITE_SIZE, SPRITE_SIZE));
}

void SpriteController::update(sf::Time deltaTime)
{
	_seconds += deltaTime.asSeconds();
	_currentFrame = _seconds * ANIM_FPS;
	_currentFrame %= 2;
}

void SpriteController::draw(sf::RenderTarget &target, Vector2 position)
{
	int left = (_offset.x *2 + _currentFrame) * FRAME_SIZE;
	int top = _offset.y * FRAME_SIZE;

	_sprite.setPosition(position.x, position.y);
	_sprite.setTextureRect({ left, top, FRAME_SIZE, FRAME_SIZE });
	target.draw(_sprite);
}

void Object::update(sf::Time deltaTime)
{
	_spriteController->update(deltaTime);
}

void Object::draw(sf::RenderTarget &target)
{
	_spriteController->draw(target, position);
}

Invader::Invader(Vector2 pos, InvaderType type) : Object(pos)
{
	_spriteController = make_shared<SpriteController>(Vector2(type,0));
}

void Invader::draw(sf::RenderTarget &target) {

	Object::draw(target);
}

Player::Player(Vector2 pos) : Object(pos)
{
	_spriteController = make_shared<SpriteController>(Vector2(0, 1));
}

void Player::draw(sf::RenderTarget &target) {
	Object::draw(target);
}

void Player::onPollEvent(sf::Event event)
{
	if (isShootPressed(event))
	{
		shoot();
	}
}

void Player::update(sf::Time deltaTime)
{
	Object::update(deltaTime);

	if (isLeftPressed() && !isRightPressed())
		_input = -1;
	else if (!isLeftPressed() && isRightPressed())
		_input = 1;
	else 
		_input = 0;

	_velocity = _input * _speed;

	position += Vector2(_velocity, 0);
}

void Player::shoot()
{
	Level::instance()->add(shared_ptr<Object>((Object*)new Bullet(position)));
}

Bullet::Bullet(Vector2 pos) : Object(pos)
{

}

void Bullet::draw(sf::RenderTarget &target) {
	Object::draw(target);
}

void Bullet::update(sf::Time deltaTime)
{
	Object::update(deltaTime);

	position += Vector2(0, _velocity);
}