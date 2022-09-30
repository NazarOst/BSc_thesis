#include "stdafx.h"
#include "Logic.h"
#include <iostream>
#include <random>
#include <Windows.h>


Logic::Logic()
{
	allocateBoardMemory();
	setBoardAsMain(generateEmptyBoard(getBoardHeigth(), getBoardWidth()));
	fillByDeadCells();
	seedRandomArea();
	setupInitialBoardVec();
	iterateGeneration();
}

Logic::~Logic()
{
	releaseBoardmemory();
}

void Logic::setVecSize(int H, int W)
{
	VEC_SIZE = W*H;
}

int Logic::getBoardWidth()
{
	return BOARD_SIZE;
}

int Logic::getVecSize()
{
	return VEC_SIZE;
}

int Logic::getBoardHeigth()
{
	return BOARD_SIZE;
}

void Logic::iterateGeneration() 
{
	generation++;
}

int Logic::getGeneration()
{
	return generation;
}

void Logic::setTimePerGeneration(long int time) // calculation time
{
	this->timePerGeneration = time;
}

long int Logic::getTimePerGeneration()
{
	return timePerGeneration;
}

void Logic::allocateBoardMemory()
{
	board = (int **)malloc(sizeof(int *) * BOARD_SIZE);;
	for (int i = 0; i < BOARD_SIZE; i++)
	{
		board[i] = (int *)malloc(sizeof(int) * BOARD_SIZE);
	}
}

int** Logic::generateEmptyBoard(int H, int W)
{
	int **tempBoard;
	tempBoard = (int **)malloc(sizeof(int *) * BOARD_SIZE);;
	for (int i = 0; i < BOARD_SIZE; i++) 
	{
		tempBoard[i] = (int *)malloc(sizeof(int) * BOARD_SIZE);
	}
	
	return tempBoard;
}

void Logic::setBoardAsMain(int **inputBoard)
{
	releaseBoardmemory();
	board = inputBoard;
}

int** Logic::getBoard() {
	return board;
}

void Logic::releaseBoardmemory()
{
	int i;
	for (i = 0; i < BOARD_SIZE; ++i)
	{
		free(board[i]);
	}
	free(board);
}

void Logic::setupInitialBoardVec()
{
	for (int i=0; i<getVecSize(); i++) 
	{
		boardVec[i] = board[i / BOARD_SIZE][i%BOARD_SIZE];
	}
}

void Logic::setVecAsMain(int * boardVec)
{
	free(this->boardVec);
	this->boardVec = boardVec;
}

int * Logic::getBoardVec()
{
	return boardVec;
}

void Logic::printBoard()
{
	for (int i = 0; i < getBoardHeigth(); i++)
	{
		for (int j = 0; j < getBoardWidth(); j++)
		{
			std::cout << board[i][j];
		}
		std::cout << std::endl;
	}
}

void Logic::performNextStepCPU()
{
	setBoardAsMain(generateNextBoard());
	iterateGeneration();
}

void Logic::fillByDeadCells() 
{
	for (int i = 0; i < getBoardHeigth(); i++)
	{
		for (int j = 0; j < getBoardWidth(); j++)
		{
			//board[i][j] = 176; //ascii of texture
			board[i][j] = 0;
		}
	}
}

int Logic::checkCell(int x, int y)
{
	//int neighborsAmount = 0;
	if ( (x >= 0) && (x < getBoardHeigth() ) && (y >= 0) && ( y < getBoardWidth() ))
	{
		if (board[x][y] == 1) // 64
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
	return 0;
}

int Logic::checkNeighbors(int x, int y)
{
	int neighborsAmount = 0;
	//for positions 1,2,3,7,8,9
	neighborsAmount += checkCell(x-1, y-1);
	neighborsAmount += checkCell(x-1, y);
	neighborsAmount += checkCell(x-1, y+1);
	neighborsAmount += checkCell(x+1, y-1);
	neighborsAmount += checkCell(x+1, y);
	neighborsAmount += checkCell(x+1, y+1);
	//for position 4
	neighborsAmount += checkCell(x, y-1);
	//for position 6
	neighborsAmount += checkCell(x, y+1);

	return neighborsAmount;
}

bool Logic::isAliveCPU(int x, int y)
{
	if (board[x][y] == 1) { //==64
		return true;
	}
	else
	{
		return false;
	}
}

bool Logic::isAliveGPU(int x, int y)
{
	int temp = x*BOARD_SIZE + y;
	if (boardVec[temp] == 1) { 
		return true;
	}
	else
	{
		return false;
	}
}

int** Logic::generateNextBoard()
{
	int **nextStepBoard = generateEmptyBoard(getBoardWidth(), getBoardHeigth());

	long int before = GetTickCount();

	for (int i=0; i<getBoardHeigth(); i++)
	{
		for (int j = 0; j < getBoardWidth(); j++)
		{
			int neighbors = checkNeighbors(i, j);
			if ((neighbors == 3) && (board[i][j] != 1)) //if 3 neighbors and is not alive -> goes alive
			{
				nextStepBoard[i][j] = 1;
			}
			else if (((neighbors == 2) || (neighbors == 3)) && (board[i][j] == 1))//if 2 or 3 neighbors and is alive -> stay alive
			{
				nextStepBoard[i][j] = 1;
			}
			 else //dies
			{
				 nextStepBoard[i][j] = 0;
			}
		}
	}

	long int after = GetTickCount();
	setTimePerGeneration(after - before);

	return nextStepBoard;
}

void Logic::seedFigure1()
{
	for (int i = ((getBoardWidth()/2)-4); i < ((getBoardWidth()/2)-1); i++ )
	{
		board[getBoardHeigth() / 2][i] = 1;
    }

	board[(getBoardHeigth() / 2) - 1][(getBoardWidth() / 2) - 3] = 1;
	board[(getBoardHeigth() / 2) - 1][(getBoardWidth() / 2) + 3] = 1;

	for (int i = ((getBoardWidth()/2)+2); i < ((getBoardWidth()/2) +5); i++)
	{
		board[getBoardHeigth() / 2][i] = 1;
	}
}

void Logic::testSeed() 
{
	//board[vertical][horizontal] -> board[H][W]
	board[1][1] = 1; //64
	board[1][2] = 1; //64
}

void Logic::seedRandomArea()
{
	float coefficient = 0.5; //50%
	int totalCells = getBoardHeigth() * getBoardWidth();
	int biomass = totalCells*coefficient;

	std::random_device rd; // obtain a random number from hardware
	std::mt19937 eng(rd()); // seed the generator
	std::uniform_int_distribution<> distrW(0, getBoardWidth()-1); // define the width range
	std::uniform_int_distribution<> distrH(0, getBoardHeigth()-1); // define the width range

	while (biomass>0) {
		int i = distrH(eng);
		int j = distrW(eng);

		if (!isAliveCPU(i, j)) {
			board[j][i] = 1;
			biomass--;
		}
		
	}


}
