#ifndef PLAYER_H
#define PLAYER_H

#include "object.h"
#include "sprite.h"

const int PLAYER_SIZE = 32;

class Player : public Object
{
public:
	Player(Vector2 pos);
	void onPollEvent(sf::Event event) override;
	void draw(sf::RenderTarget &target) override;
	virtual void update(sf::Time deltaTime) override;
	void shoot();
private:
	float _input = 0;
	float _speed = 2;
	float _velocity = 0;
};

class PlayerBullet : public Object
{
public:
	PlayerBullet(Vector2 pos);
	void draw(sf::RenderTarget &target) override;
	virtual void update(sf::Time deltaTime) override;
private:
	float _velocity = 2;
};

#endif
