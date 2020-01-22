#ifndef LEVEL_H
#define LEVEL_H

#include <array>
#include <memory>
#include <mutex>

#include "objects.h"

class Level
{
public:
	static Level* instance();

	void add(const shared_ptr<Object>& object);
	void remove(const shared_ptr<Object>& object);
	Level();
	~Level();
	
	void onPollEvent(sf::Event event);
	void update(sf::Time deltaTime);
	void draw(sf::RenderTarget &target);

private:

	static Level* _instance;

	int _currentRow = 0;
	int _currentMovement = 0;
	float _seconds = 0;
	vector<shared_ptr<Object>> _invaders;
	vector<shared_ptr<Object>> _objects;
	shared_ptr<Player> _player;
	//mutex _objectsMutex;
};

#endif