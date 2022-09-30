#pragma once
#include <SFML/Graphics.hpp>
#include "Logic.h"

class GUI
{
private:
	float cellSize;
	int boardWidth;
	int boardHeight;
	int distance = 1;
	float placeForText = 51;
	sf::Color alive = sf::Color::Yellow;
	sf::Color dead = sf::Color::Black;

public:
	GUI(Logic logic, int mode);
	~GUI();

	void drawBoardCPU(Logic logic);
	void drawBoardG(Logic logic);
};

