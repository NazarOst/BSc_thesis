#include "stdafx.h"
#include "GUI.h"
#include "Logic.h"
#include <iostream>
#include <string>
#include "GPU.h"

#include <SFML/Graphics.hpp>


GUI::GUI(Logic logic, int mode)
{
	this->boardWidth = logic.getBoardWidth();
	this->boardHeight = logic.getBoardHeigth();
	if ((boardWidth <= 100) || (boardHeight <= 100)) {
		cellSize = 5;
		distance += cellSize;
	}
	if (((boardWidth > 100) || (boardHeight > 100)) && ((boardWidth <= 200) || (boardHeight <= 200)) ){
		cellSize = 2;
		distance += cellSize;
	}
	if ((boardWidth > 200) || (boardHeight > 200)) {
		cellSize = 2;
		distance = cellSize;
	}

	switch (mode)
	{
		case 1: {
			drawBoardCPU(logic); 
			break;
		};
		case 2: {
			drawBoardG(logic); 
			break;
		};
	}
	
}


GUI::~GUI()
{
}

void GUI::drawBoardCPU(Logic logic)
{
	sf::RenderWindow window(sf::VideoMode(boardWidth*distance, boardHeight*distance + placeForText), "Life CPU");
	sf::RectangleShape aliveCell(sf::Vector2f(cellSize, cellSize));
	sf::RectangleShape deadCell(sf::Vector2f(cellSize, cellSize));

	sf::Text text1;
	sf::Text text2;
	text2.setPosition(0,11);
	sf::Font font;
	font.loadFromFile("arial.ttf");
	text1.setFont(font);
	text2.setFont(font);

	std::string curGenStr = "Current generation: ";
	std::string timeStr = "Time per generation: ";

	std::string genNum = std::to_string(logic.getGeneration());
	std::string timeNum = std::to_string(logic.getTimePerGeneration());
	std::string milis = " ms";

	std::string firstLine ="";
	std::string secondLine = "";

	text1.setString(firstLine.append(curGenStr).append(genNum));
	text1.setCharacterSize(11);
	text1.setFillColor(sf::Color::White);

	text2.setString(secondLine.append(timeStr).append(timeNum).append(milis));
	text2.setCharacterSize(11);
	text2.setFillColor(sf::Color::White);

	sf::Vertex line[2] =
	{
		sf::Vertex(sf::Vector2f(0.f, placeForText)),
		sf::Vertex(sf::Vector2f(boardWidth*distance, placeForText))
	};

	aliveCell.setFillColor(alive);
	deadCell.setFillColor(dead);

	std::ofstream out("CPU.txt");
	out << "CPU[ms] \n";

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear();
		for (int i = 0; i<boardHeight; ++i) {
			for (int j = 0; j<boardWidth; ++j) {
				if (logic.isAliveCPU(i, j)) {
					aliveCell.setPosition(j*distance, i*distance + placeForText);
					window.draw(aliveCell);
				}
				else
				{
					deadCell.setPosition(j*distance, i*distance + placeForText);
					window.draw(deadCell);
				}
			}
		}

		genNum = std::to_string(logic.getGeneration());
		timeNum = std::to_string(logic.getTimePerGeneration());

		firstLine = "";
		secondLine = "";

		text1.setString(firstLine.append(curGenStr).append(genNum));
		text2.setString(secondLine.append(timeStr).append(timeNum).append(milis));

		window.draw(text1);
		window.draw(text2);
		window.draw(line, 2, sf::Lines);
		
		window.display();

		logic.performNextStepCPU();
		out << logic.getTimePerGeneration();
		out << "\n";
	}
	out.close();
}

void GUI::drawBoardG(Logic logic)
{
	GPU gpu = GPU();
	sf::RenderWindow window(sf::VideoMode(boardWidth*distance, boardHeight*distance + placeForText), "Life GPU");
	sf::RectangleShape aliveCell(sf::Vector2f(cellSize, cellSize));
	sf::RectangleShape deadCell(sf::Vector2f(cellSize, cellSize));

	sf::Text text1;
	sf::Text text2;
	text2.setPosition(0, 11);
	sf::Font font;
	font.loadFromFile("arial.ttf");
	text1.setFont(font);
	text2.setFont(font);

	std::string curGenStr = "Current generation: ";
	std::string timeStr = "Time per generation: ";

	std::string genNum = std::to_string(logic.getGeneration());
	std::string timeNum = std::to_string(logic.getTimePerGeneration());
	std::string milis = " ns";

	std::string firstLine = "";
	std::string secondLine = "";

	text1.setString(firstLine.append(curGenStr).append(genNum));
	text1.setCharacterSize(11);
	text1.setFillColor(sf::Color::White);

	text2.setString(secondLine.append(timeStr).append(timeNum).append(milis));
	text2.setCharacterSize(11);
	text2.setFillColor(sf::Color::White);

	sf::Vertex line[2] =
	{
		sf::Vertex(sf::Vector2f(0.f, placeForText)),
		sf::Vertex(sf::Vector2f(boardWidth*distance, placeForText))
	};

	aliveCell.setFillColor(alive);
	deadCell.setFillColor(dead);

	std::ofstream out("GPU.txt");
	out << "GPU[ns] \n";

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear();
		for (int i = 0; i<boardHeight; ++i) {
			for (int j = 0; j<boardWidth; ++j) {
				if (logic.isAliveGPU(i, j)) {
					aliveCell.setPosition(j*distance, i*distance + placeForText);
					window.draw(aliveCell);
				}
				else
				{
					deadCell.setPosition(j*distance, i*distance + placeForText);
					window.draw(deadCell);
				}
			}
		}

		genNum = std::to_string(logic.getGeneration());
		timeNum = std::to_string(gpu.getTimePerGeneration());

		firstLine = "";
		secondLine = "";

		text1.setString(firstLine.append(curGenStr).append(genNum));
		text2.setString(secondLine.append(timeStr).append(timeNum).append(milis));

		window.draw(text1);
		window.draw(text2);
		window.draw(line, 2, sf::Lines);

		window.display();

		logic.setVecAsMain(gpu.generateNextVec(logic.getVecSize(), logic.getBoardWidth(), logic.getBoardVec()));
		logic.iterateGeneration();
		out << gpu.getTimePerGeneration();
		out << "\n";
		
		gpu.cleanUpMemory();
	}
	out.close();
	gpu.fullCleanUp();
}