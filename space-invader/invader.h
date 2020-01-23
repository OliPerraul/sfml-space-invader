
#ifndef INVADER_H
#define INVADER_H

#include "object.h"
#include "sprite.h"

const int INVADER_SIZE = 32;

enum InvaderType : int
{
	UFO = 0,
	LARGE = 1,
	MEDIUM = 2,
	SMALL = 3
};

class Invader : public Object
{
public:
	Invader(Vector2 pos, InvaderType type);
	void draw(sf::RenderTarget &target) override;
	virtual void update(sf::Time deltaTime) override;
	void shoot();
private:
	
	float _seconds = 0;
	float _limit = 2;

};


class InvaderBullet : public Object
{
public:
	InvaderBullet(Vector2 pos);
	void draw(sf::RenderTarget &target) override;
	virtual void update(sf::Time deltaTime) override;
private:
	float _velocity = 2;

};



#endif
