#pragma once
#include <Windows.h>
#include "GPU.h" 
#define __NO_STD_VECTOR // Use cl::vector instead of STL version
#define __CL_ENABLE_EXCEPTIONS

class Logic
{
	
private:
	int BOARD_SIZE = 256;
	int VEC_SIZE = BOARD_SIZE*BOARD_SIZE;
	int generation = 0;
	int timePerGeneration = 0;
	int **board;
	int *boardVec = (int*)malloc(sizeof(int)*VEC_SIZE);
	

public:
	//constructors
	Logic();
	~Logic(); 
	// board parameters
	void setVecSize(int H, int W);
	int getBoardHeigth();
	int getBoardWidth();
	int getVecSize();

	void iterateGeneration();
	int getGeneration();
	void setTimePerGeneration(long int time);
	long int getTimePerGeneration();

	// board manipulations
	void allocateBoardMemory();
	int **generateEmptyBoard(int H, int W);
	void setBoardAsMain(int **inputBoard);
	int **getBoard();
	void releaseBoardmemory();


	//vec manipulations
	void setupInitialBoardVec(); 
	void setVecAsMain(int *boardVec);
	int *getBoardVec();

	void printBoard(); //test function for printing board to the console

	void performNextStepCPU();

	// oparrations with cells
	void fillByDeadCells();
	int checkCell(int x, int y);
	int Logic::checkNeighbors(int x, int y);
	bool isAliveCPU(int x, int y);
	bool isAliveGPU(int x, int y);
	int **generateNextBoard();

	//seeding
	void seedFigure1();
	void testSeed();
	void seedRandomArea();
};

