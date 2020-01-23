#ifndef BARRIER_H
#define BARRIER_H

#include "object.h"
#include "sprite.h"


const int BARRIER_WIDTH = 64;
const int BARRIER_HEIGHT = 32;

class BarrierSpriteController : public SpriteController
{
	//// TODO remove used for collision
	//friend class SpriteController;
public:

	BarrierSpriteController(Vector2 offset);

	void impact(Vector2 pos);

	void update(sf::Time deltaTime) override;

	void draw(sf::RenderTarget &target, Vector2 position) override;
};

class Barrier : public Object

{
public:
	Barrier(Vector2 pos);
	void draw(sf::RenderTarget &target) override;
	virtual void update(sf::Time deltaTime) override;
private:
	shared_ptr<BarrierSpriteController> _barrierSpriteController;
};


#endif
