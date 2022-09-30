#pragma once
//#include <iostream>
#include "CL\cl.hpp"
#include "Logic.h"
#include <fstream>
//#include <Windows.h>
#include <chrono>

#define __NO_STD_VECTOR // Use cl::vector instead of STL version
#define __CL_ENABLE_EXCEPTIONS
#define MAX_SOURCE_SIZE (0x100000)

using namespace std;

class GPU
{
private:
	FILE *fp;
	char *source_str;
	size_t source_size;
	cl_platform_id platform_id = NULL;
	cl_device_id device_id = NULL;
	cl_uint ret_num_devices;
	cl_uint ret_num_platforms;
	cl_int ret;
	cl_context context;
	cl_command_queue command_queue;
	cl_mem in_mem_obj;
	cl_mem out_mem_obj;
	cl_program program;
	cl_kernel kernel;

	double timePerGeneration = 0;


public:
	GPU();
	~GPU();

	void loadKernelFromFile();
	void getPlatformInfo();
	void getDeviceInfo();
	void createContext();
	void createCommandQueue();

	void createMemoryBuffers(int list_size);

	void setupInput(int list_size, int *boardVec);

	void createProgram();
	void buildProgram();

	void createKernel();
	void setArgsToKernel(int board_size);
	void executeKernel(int list_size);
	int *getOutput(int list_size);
	
	void cleanUpMemory();
	void fullCleanUp();

	int *generateNextVec(int list_size, int board_size, int * inputVec);

	void setTimePerGeneration(long int time);
	long int getTimePerGeneration();

};

