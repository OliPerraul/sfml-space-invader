#ifndef OBJECTS_H
#define OBJECTS_H

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include <memory>

#include "utils.h"

using namespace std;

const int SPRITE_SIZE = 32;

class SpriteController
{
public:

	SpriteController(Vector2 offset);

	void update(sf::Time deltaTime);

	void draw(sf::RenderTarget &target, Vector2 position);

private:
	float _seconds = 0;
	Vector2 _offset;
	int _currentFrame = 0;
	sf::Texture _texture;
	sf::RectangleShape _sprite;
};

class Object
{

public:
	Vector2 position;

	Object(Vector2 position) 
	{
		this->position = position;
	}

	~Object() {}

	virtual void start() {}
	virtual void update(sf::Time deltaTime);
	virtual void draw(sf::RenderTarget &target);

protected:
	shared_ptr<SpriteController> _spriteController;
};


enum InvaderType : int
{
	UFO = 0,
	LARGE = 1,
	MEDIUM = 2,
	SMALL = 3
};

class Invader : Object
{
public:
	Invader(Vector2 pos, InvaderType type);
	void draw(sf::RenderTarget &target) override;
};

class Player : Object
{
public:
	Player(Vector2 pos);
	void onPollEvent(sf::Event event);
	void draw(sf::RenderTarget &target) override;
	virtual void update(sf::Time deltaTime) override;
	void shoot();
private:
	float _input = 0;
	float _speed = 2;
	float _velocity = 0;
};

class Bullet : Object
{
public:
	Bullet(Vector2 pos);
	void draw(sf::RenderTarget &target) override;
	virtual void update(sf::Time deltaTime) override;
private:
	float _velocity = 2;
};




#endif
