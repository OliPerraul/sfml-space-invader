

#include <list>
#include <memory>

#include "level.h"
#include "object.h";
#include "invader.h"
#include "barrier.h"

using namespace std;

const int MARGIN = 32;
const int INVADER_SEPARATION = 8;

const int ROW_SIZE = 16;

const vector<vector<int>> INVADERS =
{
	vector<int>(ROW_SIZE, InvaderType::LARGE),
	vector<int>(ROW_SIZE, InvaderType::LARGE),
	vector<int>(ROW_SIZE, InvaderType::MEDIUM),
	vector<int>(ROW_SIZE, InvaderType::MEDIUM),
	vector<int>(ROW_SIZE, InvaderType::SMALL),
	vector<int>(ROW_SIZE, InvaderType::SMALL),
};

const int NUM_ROWS = INVADERS.size();

const int ROW_MOVEMENT_DELAY = 1;

const int SHOOT_DELAY = 1;

const int SPEED = 16;


const int BARRIER_SEPARATION = 128;
const int NUM_BARRIERS = 5;


const vector<Vector2> MOVEMENT_PATTERN
{
	{1, 0}, {1, 0}, {0, 1}, {-1, 0}, {-1, 0}, {0, 1}
};

Level* Level::_instance = nullptr;

Level * Level::instance()
{
	return _instance;
}

void Level::add(const shared_ptr<Object>& object)
{
	_added.push_back(shared_ptr<Object>(object));
}

void Level::add(const shared_ptr<PlayerBullet>& bullet)
{
	add(dynamic_pointer_cast<Object>(bullet));
	_playerBullets.push_back(bullet);
}

void Level::add(const shared_ptr<InvaderBullet>& bullet)
{
	add(dynamic_pointer_cast<Object>(bullet));
	_invaderBullets.push_back(bullet);
}

void Level::remove(Object* object)
{
	if (object == nullptr)
		return;

	object->setActive(false);
	_removed.push_back(object);
}

Level::Level()
{
	_instance = this;

	// Create invaders
	shared_ptr<Object> obj;
	int k = 0;
	for (auto i = INVADERS.begin(); i != INVADERS.end(); i++)
	{
		int l = 0;
		for (auto j = i->begin(); j != i->end(); j++)
		{
			_objects.push_back(
					obj = shared_ptr<Object>((Object*)new Invader(
					Vector2(0, MARGIN) + // add top margin
					Vector2(SCREEN_WIDTH, 0) / 2 - // offset in middle
					Vector2( // minus half the row size
						i->size() * INVADER_SIZE +
						i->size() * INVADER_SEPARATION,
						0) / 2 +
					Vector2(// pfset in the mid
						INVADER_SIZE * l + INVADER_SEPARATION * l,
						k * INVADER_SIZE + INVADER_SEPARATION * k),
						(InvaderType)*j
					)));
			_invaders.push_back(dynamic_pointer_cast<Invader>(obj));
			l++;
		}

		k++;
	}

	// Create Player
	_objects.push_back(make_shared<Player>(
		Vector2(SCREEN_WIDTH, 0) / 2 + // offset in middle
		Vector2(0, SCREEN_HEIGHT) - // add top margin
		Vector2(0, MARGIN) - // add bottom margin
		Vector2(0, MARGIN)
		));

	// Create barriers
	for (int i =0; i < NUM_BARRIERS; i++)
	{
		_objects.push_back(make_shared<Barrier>(
			Vector2(SCREEN_WIDTH, 0) / 2 + // offset in middle
			Vector2(
				BARRIER_WIDTH * i + BARRIER_SEPARATION * i,
				0) / 2 -
			Vector2( // minus half the row size
				NUM_BARRIERS * INVADER_SIZE +
				NUM_BARRIERS * INVADER_SEPARATION,
				0) +
			Vector2(0, SCREEN_HEIGHT) - // add top margin
			Vector2(0, MARGIN) - // add bottom margin
			Vector2(0, MARGIN) - // add bottom margin
			Vector2(0, MARGIN)));
	}
}

Level::~Level()
{
}

void Level::draw(sf::RenderTarget &target) {

	for (auto object = _objects.begin(); object != _objects.end(); object++)
	{
		if ((*object) == nullptr)
			continue;

		(*object)->draw(target);
	}
}

void Level::onPollEvent(sf::Event event)
{
	for (auto object = _objects.begin(); object != _objects.end(); object++)
	{
		if ((*object) == nullptr)
			continue;

		(*object)->onPollEvent(event);
	}
}

void Level::update(sf::Time deltaTime)
{
	// Invader shooting
	// Only allow open invader to shoot
	_shootSeconds += deltaTime.asSeconds();
	if (_shootSeconds >= SHOOT_DELAY)
	{
		int x = rand() % ROW_SIZE;
		
		for (int y = 0; y < NUM_ROWS; y++)
		{
			int i = x + ROW_SIZE * (NUM_ROWS - 1 - y);
			if (_invaders[i]->isActive())
			{
				_invaders[i]->shoot();
				break;
			}
		}

		_shootSeconds = 0;
	}

	_movementSeconds += deltaTime.asSeconds();

	// Invader movement
	if (_movementSeconds >= ROW_MOVEMENT_DELAY)
	{
		for (int i = 0; i < ROW_SIZE; i++)
		{		
			_invaders[i + ROW_SIZE * (NUM_ROWS - 1 - _currentRow)]->position += MOVEMENT_PATTERN[_currentMovement] * SPEED;
		}

		_currentRow = (_currentRow + 1) % NUM_ROWS;
		if (_currentRow == 0)
		{
			_currentMovement = (_currentMovement + 1) % MOVEMENT_PATTERN.size();
		}
		_movementSeconds = 0;
	}

	for (
		auto object = _objects.begin(); 
		object != _objects.end(); 
		object++)
	{
		if ((*object) == nullptr)
			continue;

		(*object)->update(deltaTime);
	}

	// Add outside iteration
	for (
		auto object = _added.begin();
		object != _added.end();
		object++)
	{
		_objects.push_back(*object);
	}

	_added.clear();

	// Remove outside iteration
	for (
		auto object = _removed.begin();
		object != _removed.end();
		object++)
	{
		remove_if(
			_objects.begin(),
			_objects.end(),
			[object](shared_ptr<Object> x)
		{
			return x.get() == *object;
		});

		remove_if(
			_invaderBullets.begin(),
			_invaderBullets.end(),
			[object](shared_ptr<InvaderBullet> x)
		{
			return x.get() == *object;
		});

		remove_if(
			_playerBullets.begin(),
			_playerBullets.end(),
			[object](shared_ptr<PlayerBullet> x)
		{
			return x.get() == *object;
		});
	}

	_removed.clear();
}