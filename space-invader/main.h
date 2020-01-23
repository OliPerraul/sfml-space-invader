#pragma once

int main();

class Game
{
public:
	
	int score = 0;
	bool isGameOver = false;

	inline static Game* instance() { return _instance; }
	
	Game();
	~Game() {}

	int run();
	

private:

	static Game* _instance;	
};