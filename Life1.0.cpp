#include "stdafx.h"
#include "Logic.h"
#include "GUI.h"
#include <iostream>
#include <SFML/Graphics.hpp>
//#include "CL/cl.hpp"

using namespace std;

int main()
{ 
	//cl::Platform platforms;
	int tH, tW; //temp width and height
	cout << "Input board heigth: ";
	cin >> tH;
	cout << "Input board width: ";
	cin >> tW;
	
	Logic logic = Logic(tH, tW);

	//Logic logic = Logic(20, 20);

	GUI gui = GUI(logic, logic.getBoard(), logic.getBoardWidth(), logic.getBoardHeigth());

	logic.~Logic();
	gui.~GUI();
	return 0;
}

