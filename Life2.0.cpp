#include "stdafx.h"
#include "Logic.h"
#include "GUI.h"
#include "TimeTest.h"

#include <fstream>
#include <string>
#include <iostream>

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, char*, int nShowCmd)
//int main()
{
	Logic logic = Logic();
	GUI gui = GUI(logic, 2);
}

