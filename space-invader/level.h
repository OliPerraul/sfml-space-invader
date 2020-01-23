#ifndef LEVEL_H
#define LEVEL_H

#include <array>
#include <memory>
#include <mutex>

#include "object.h"
#include "player.h"
#include "invader.h"

class Level
{
public:
	static Level* instance();

	void add(const shared_ptr<Object>& object);
	void add(const shared_ptr<PlayerBullet>& object);
	void add(const shared_ptr<InvaderBullet>& object);
	void remove(Object* object);
	Level();
	~Level();
	
	void onPollEvent(sf::Event event);
	void update(sf::Time deltaTime);
	void draw(sf::RenderTarget &target);

	vector<shared_ptr<PlayerBullet>>::iterator playerBulletsBegin() { return _playerBullets.begin(); }
	vector<shared_ptr<PlayerBullet>>::iterator playerBulletsEnd() { return _playerBullets.end(); }
	vector<shared_ptr<InvaderBullet>>::iterator invaderBulletsBegin() { return _invaderBullets.begin(); }
	vector<shared_ptr<InvaderBullet>>::iterator invaderBulletsEnd() { return _invaderBullets.end(); }

private:

	static Level* _instance;

	int _currentRow = 0;
	int _currentMovement = 0;
	float _movementSeconds = 0;
	float _shootSeconds = 0;
	vector<shared_ptr<PlayerBullet>> _playerBullets;
	vector<shared_ptr<InvaderBullet>> _invaderBullets;
	vector<shared_ptr<Invader>> _invaders;
	vector<shared_ptr<Object>> _objects;
	vector<Object*> _removed;
	vector<shared_ptr<Object>> _added;	
};

#endif