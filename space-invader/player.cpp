
#include "player.h"
#include "level.h"
#include "main.h"

Player::Player(Vector2 pos) : Object(pos)
{
	collider = make_unique<sf::IntRect>(pos.x, pos.y, 32, 32);
	_spriteController = make_shared<SpriteController>(Vector2(0, 1), PLAYER_SIZE, PLAYER_SIZE);
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

	for (
		auto bullet = Level::instance()->invaderBulletsBegin();
		bullet != Level::instance()->invaderBulletsEnd();
		bullet++)
	{
		if ((*bullet) == nullptr)
			continue;

		if ((*bullet)->collider->intersects(*collider))
		{
			Level::instance()->remove(bullet->get());
			Level::instance()->remove(this);
			Game::instance()->isGameOver = true;
			return;
		}
	}

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
	Level::instance()->add(make_shared<PlayerBullet>(position));
}

PlayerBullet::PlayerBullet(Vector2 pos) : Object(pos)
{
	_spriteController = make_shared<SpriteController>(Vector2(0, 2), 32 ,32);
	collider = make_unique<sf::IntRect>(pos.x, pos.y, 16, 16);
}

void PlayerBullet::draw(sf::RenderTarget &target) {
	Object::draw(target);
}

void PlayerBullet::update(sf::Time deltaTime)
{
	Object::update(deltaTime);

	// TODO destory bullets?
	//for (
	//	auto bullet = Level::instance()->invaderBulletsBegin();
	//	bullet != Level::instance()->invaderBulletsEnd();
	//	bullet++)
	//{
	//	if ((*bullet)->collider->intersects(*collider))
	//	{
	//		Level::instance()->remove(bullet->get());
	//		Level::instance()->remove(this);
	//		return;
	//	}
	//}

	position -= Vector2(0, _velocity);
}
