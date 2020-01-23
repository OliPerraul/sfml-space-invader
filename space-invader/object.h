
#ifndef OBJECT_H
#define OBJECT_H

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include <memory>

#include "utils.h"
#include "sprite.h"

using namespace std;

class Object
{

public:
	Vector2 position;
	unique_ptr<sf::IntRect> collider;

	Object(Vector2 position)
	{
		this->position = position;
	}

	~Object() {}

	virtual void start() {}
	virtual void update(sf::Time deltaTime);
	virtual void draw(sf::RenderTarget &target);

	inline void setActive(bool active) { _isActive = active; }
	inline bool isActive() { return _isActive; }
	inline virtual void onPollEvent(sf::Event event) {}

	inline shared_ptr<SpriteController> spriteController() { return _spriteController; }

protected:
	shared_ptr<SpriteController> _spriteController;

private:
	bool _isActive = true;
};


class Explosion : public Object
{
public:
	Explosion(Vector2 pos);
	void draw(sf::RenderTarget &target) override;
	virtual void update(sf::Time deltaTime) override;
private:
	float _seconds = 0;
	float _limit = 2;
};



#endif
