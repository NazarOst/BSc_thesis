#include "GPU.h"

GPU::GPU()
{
	loadKernelFromFile();
	getPlatformInfo();
	getDeviceInfo();
	createContext();
	createCommandQueue();

	createProgram();
	buildProgram();
	createKernel();
	
}


GPU::~GPU()
{
}


void GPU::loadKernelFromFile()
{
	fp = fopen("life_logic_kernel.cl", "r");
	if (!fp)
	{
		fprintf(stderr, "Failed to load kernel.\n");
		exit(1);
	}
	source_str = (char*)malloc(MAX_SOURCE_SIZE);
	source_size = fread(source_str, 1, MAX_SOURCE_SIZE, fp);
	fclose(fp);
}

void GPU::getPlatformInfo()
{
	ret = clGetPlatformIDs(1, &platform_id, &ret_num_platforms);
}

void GPU::getDeviceInfo()
{
	ret = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_GPU, 1, &device_id, &ret_num_devices);
}

void GPU::createContext()
{
	context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &ret);
}

void GPU::createCommandQueue()
{
	command_queue = clCreateCommandQueue(context, device_id, 0, &ret);
}

void GPU::createMemoryBuffers(int list_size)
{
	in_mem_obj = clCreateBuffer(context, CL_MEM_READ_ONLY, list_size * sizeof(int), NULL, &ret);
	out_mem_obj = clCreateBuffer(context, CL_MEM_WRITE_ONLY, list_size * sizeof(int), NULL, &ret);
}

void GPU::setupInput(int list_size, int *boardVec)
{
	ret = clEnqueueWriteBuffer(command_queue, in_mem_obj, CL_TRUE, 0, list_size * sizeof(int), boardVec, 0, NULL, NULL);
}

void GPU::createProgram()
{
	program = clCreateProgramWithSource(context, 1, (const char **)&source_str, (const size_t *)&source_size, &ret);
}

void GPU::buildProgram()
{
	ret = clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);
}

void GPU::createKernel()
{
	kernel = clCreateKernel(program, "life_logic", &ret);
}

void GPU::setArgsToKernel(int board_size)
{
	ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&in_mem_obj);
	ret = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&out_mem_obj);
	ret = clSetKernelArg(kernel, 2, sizeof(unsigned int), &board_size);
}

void GPU::executeKernel(int list_size)
{
	size_t global_item_size = list_size; // Process the entire lists
	size_t local_item_size = 64; // Divide work items into groups of 64

	auto start = std::chrono::high_resolution_clock::now();

	ret = clEnqueueNDRangeKernel(command_queue, kernel, 1, NULL, &global_item_size, &local_item_size, 0, NULL, NULL);

	auto finish = std::chrono::high_resolution_clock::now();
	
	setTimePerGeneration(std::chrono::duration_cast<
						std::chrono::nanoseconds>(
						finish - start).count());
}

int * GPU::getOutput(int list_size)
{
	int *opt = (int*)malloc(sizeof(int)*list_size);
	ret = clEnqueueReadBuffer(command_queue, out_mem_obj, CL_TRUE, 0, list_size * sizeof(int), opt, 0, NULL, NULL);
	return opt;
}

void GPU::cleanUpMemory()
{
	ret = clReleaseMemObject(in_mem_obj);
	ret = clReleaseMemObject(out_mem_obj);
}

void GPU::fullCleanUp()
{
	ret = clFlush(command_queue);
	ret = clFinish(command_queue);
	ret = clReleaseKernel(kernel);
	ret = clReleaseProgram(program);
	ret = clReleaseMemObject(in_mem_obj);
	ret = clReleaseMemObject(out_mem_obj);
	ret = clReleaseCommandQueue(command_queue);
	ret = clReleaseContext(context);
}

int * GPU::generateNextVec(int list_size, int board_size, int * inputVec)
{
	createMemoryBuffers(list_size);
	setupInput(list_size, inputVec);

	setArgsToKernel(board_size);
	
	executeKernel(list_size);

	int *opt = getOutput(list_size);
	return opt;
}

void GPU::setTimePerGeneration(long int time)
{
	this->timePerGeneration = time;
}

long int GPU::getTimePerGeneration()
{
	return timePerGeneration;
}



