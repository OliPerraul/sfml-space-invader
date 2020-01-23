
#include "invader.h"
#include "level.h"


Invader::Invader(Vector2 pos, InvaderType type) : Object(pos)
{
	_spriteController = make_shared<SpriteController>(Vector2(type, 0), INVADER_SIZE, INVADER_SIZE);
	collider = make_unique<sf::IntRect>(pos.x, pos.y, 32, 32);
}

void Invader::draw(sf::RenderTarget &target) {

	Object::draw(target);
}


void Invader::update(sf::Time deltaTime)
{
	Object::update(deltaTime);

	for (
		auto bullet = Level::instance()->playerBulletsBegin();
		bullet != Level::instance()->playerBulletsEnd();
		bullet++)
	{
		if ((*bullet) == nullptr)
			continue;

		if ((*bullet)->collider->intersects(*collider))
		{
			Level::instance()->remove(bullet->get());
			Level::instance()->remove(this);
			return;
		}
	}
}

void Invader::shoot()
{
	Level::instance()->add(make_shared<InvaderBullet>(position));
}


InvaderBullet::InvaderBullet(Vector2 pos) : Object(pos)
{
	_spriteController = make_shared<SpriteController>(Vector2(0, 4), 32, 32);
	collider = make_unique<sf::IntRect>(pos.x, pos.y, 16, 16);
}

void InvaderBullet::draw(sf::RenderTarget &target) {
	Object::draw(target);
}

void InvaderBullet::update(sf::Time deltaTime)
{
	position += Vector2(0, _velocity);
	Object::update(deltaTime);
}

