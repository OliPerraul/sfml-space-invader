
#include "level.h"
#include "objects.h";

#include <list>
#include <memory>

using namespace std;

const int MARGIN = 32;
const int SEPARATION = 8;

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

const int SPEED = 16;

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
	_objects.push_back(object);
}

void Level::remove(const shared_ptr<Object>& object)
{

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
						i->size() * SPRITE_SIZE +
						i->size() * SEPARATION,
						0) / 2 +
					Vector2(
						SPRITE_SIZE * l + SEPARATION * l,
						k * SPRITE_SIZE + SEPARATION * k),

						(InvaderType)*j
					)));
			_invaders.push_back(obj);

			l++;
		}

		k++;
	}

	// Create Player
	_player = make_shared<Player>(
		Vector2(SCREEN_WIDTH, 0) / 2 + // offset in middle
		Vector2(0, SCREEN_HEIGHT) - // add top margin
		Vector2(0, MARGIN) - // add bottom margin
		Vector2(0, MARGIN)
		);
}

Level::~Level()
{
}

void Level::draw(sf::RenderTarget &target) {

	_player->draw(target);

	for (auto object = _objects.begin(); object != _objects.end(); object++)
	{
		(*object)->draw(target);
	}
}

void Level::onPollEvent(sf::Event event)
{
	_player->onPollEvent(event);
}

void Level::update(sf::Time deltaTime)
{
	_player->update(deltaTime);

	_seconds += deltaTime.asSeconds();

	// Invader movement
	if (_seconds >= ROW_MOVEMENT_DELAY)
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
		_seconds = 0;
	}

	for (auto object = _objects.begin(); object != _objects.end(); object++)
	{
		(*object)->update(deltaTime);
	}
}