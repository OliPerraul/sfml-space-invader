
#include "barrier.h"
#include "object.h"
#include "level.h"
#include "main.h"

const int FRAME_WIDTH = 32;
const int FRAME_HEIGHT = 16;

const int ANIM_FPS = 2;

BarrierSpriteController::BarrierSpriteController(Vector2 offset) : SpriteController(offset, 32, 16)
{
	_offset = offset;
	if (!_texture.loadFromFile(resourcePath("\\sprites\\barrier.png")))
		cout << "fail" << endl;

	_sprite.setTexture(_texture, true);	
	//_sprite.setScale(sf::Vector2<float>(2, 2));
	_image = _sprite.getTexture()->copyToImage();
	_texture.loadFromImage(_image);
}

const float DESTRUCTION_RADIUS = 5;

class BitmaskManager
{
public:
	~BitmaskManager() {
		std::map<const sf::Texture*, sf::Uint8*>::const_iterator end = Bitmasks.end();
		for (std::map<const sf::Texture*, sf::Uint8*>::const_iterator iter = Bitmasks.begin(); iter != end; iter++)
			delete[] iter->second;
	}

	sf::Uint8 GetPixel(const sf::Uint8* mask, const sf::Texture* tex, unsigned int x, unsigned int y) {
		if (x > tex->getSize().x || y > tex->getSize().y)
			return 0;

		return mask[x + y * tex->getSize().x];
	}

	sf::Uint8* GetMask(const sf::Texture* tex) {
		sf::Uint8* mask;
		std::map<const sf::Texture*, sf::Uint8*>::iterator pair = Bitmasks.find(tex);
		if (pair == Bitmasks.end())
		{
			sf::Image img = tex->copyToImage();
			mask = CreateMask(tex, img);
		}
		else
			mask = pair->second;

		return mask;
	}

	sf::Uint8* CreateMask(const sf::Texture* tex, const sf::Image& img) {
		sf::Uint8* mask = new sf::Uint8[tex->getSize().y*tex->getSize().x];

		for (unsigned int y = 0; y < tex->getSize().y; y++)
		{
			for (unsigned int x = 0; x < tex->getSize().x; x++)
				mask[x + y * tex->getSize().x] = img.getPixel(x, y).a;
		}

		Bitmasks.insert(std::pair<const sf::Texture*, sf::Uint8*>(tex, mask));

		return mask;
	}
private:
	std::map<const sf::Texture*, sf::Uint8*> Bitmasks;
};

BitmaskManager Bitmasks;

bool PixelPerfectTest(
	const sf::Sprite& Object1, 
	const sf::Sprite& Object2, 
	sf::Uint8 AlphaLimit,
	Vector2& pos) {
	
	sf::FloatRect Intersection;
	if (Object1.getGlobalBounds().intersects(Object2.getGlobalBounds(), Intersection)) {
		sf::IntRect O1SubRect = Object1.getTextureRect();
		sf::IntRect O2SubRect = Object2.getTextureRect();

		sf::Uint8* mask1 = Bitmasks.GetMask(Object1.getTexture());
		sf::Uint8* mask2 = Bitmasks.GetMask(Object2.getTexture());

		// Loop through our pixels
		for (int i = Intersection.left; i < Intersection.left + Intersection.width; i++) {
			for (int j = Intersection.top; j < Intersection.top + Intersection.height; j++) {

				sf::Vector2f o1v = Object1.getInverseTransform().transformPoint(i, j);
				sf::Vector2f o2v = Object2.getInverseTransform().transformPoint(i, j);

				// Make sure pixels fall within the sprite's subrect
				if (o1v.x > 0 && o1v.y > 0 && o2v.x > 0 && o2v.y > 0 &&
					o1v.x < O1SubRect.width && o1v.y < O1SubRect.height &&
					o2v.x < O2SubRect.width && o2v.y < O2SubRect.height) {

					if (Bitmasks.GetPixel(mask1, Object1.getTexture(), (int)(o1v.x) + O1SubRect.left, (int)(o1v.y) + O1SubRect.top) > AlphaLimit &&
						Bitmasks.GetPixel(mask2, Object2.getTexture(), (int)(o2v.x) + O2SubRect.left, (int)(o2v.y) + O2SubRect.top) > AlphaLimit)
					{
						pos = sf::Vector2((int)(o2v.x) + O2SubRect.left, (int)(o2v.y) + O2SubRect.top);
						return true;
					}

				}
			}
		}
	}

	return false;
}


void BarrierSpriteController::impact(Vector2 pos)
{
	bool modified = false;

	for (int x = 0; x < int(_image.getSize().x); x++)
	{
		for (int y = 0; y < int(_image.getSize().y); y++)
		{
			int a = x - pos.x;
			int b = y - pos.y;
			if (a*a + b*b <= DESTRUCTION_RADIUS * DESTRUCTION_RADIUS)
			{
				sf::Color p = _image.getPixel(x, y);
				p.a = 0;
				_image.setPixel(x, y, p);
				modified = true;
			}
		}
	}

	if (modified)
	{
		_texture.loadFromImage(_image);
		_sprite.setTexture(_texture, true);
		
	}

}

void BarrierSpriteController::update(sf::Time deltaTime)
{
	_currentFrame = 0;
}

void BarrierSpriteController::draw(sf::RenderTarget &target, Vector2 position)
{
	_sprite.setPosition(position.x, position.y);
	target.draw(_sprite);
}

Barrier::Barrier(Vector2 pos): Object(pos)
{
	_barrierSpriteController = make_shared<BarrierSpriteController>(Vector2(0, 6));
	_spriteController = dynamic_pointer_cast<SpriteController>(_barrierSpriteController);
	collider = make_unique<sf::IntRect>(pos.x, pos.y, 64, 32);
}


void Barrier::update(sf::Time deltaTime)
{
	_spriteController->update(deltaTime);

	if (collider != nullptr)
	{
		collider->left = position.x;
		collider->top = position.y;
	}

	for (
		auto bullet = Level::instance()->invaderBulletsBegin();
		bullet != Level::instance()->invaderBulletsEnd();
		bullet++)
	{
		if ((*bullet) == nullptr)
			continue;

		sf::IntRect intersect;
		if ((*bullet)->collider->intersects(*collider))
		{
			Vector2 pos;
			if (PixelPerfectTest(
				(*bullet)->spriteController()->sprite(),
				spriteController()->sprite(),
				0,
				pos
			))
			{
				Level::instance()->remove((*bullet).get());
				_barrierSpriteController->impact(pos);
			}
		}
	}

}

void Barrier::draw(sf::RenderTarget &target)
{
	_spriteController->draw(target, position);
}