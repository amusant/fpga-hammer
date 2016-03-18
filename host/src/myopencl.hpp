
#include <CL/opencl.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "AOCL_Utils.h"
#define COLS 544
#define ROWS 288
#define PAGE_SIZE 0x1000
#define TOTAL_MEM 0x40000000
using namespace std;
using namespace aocl_utils;

class myopencl {
	cl_uint num_platforms;
	cl_platform_id platform;
	cl_uint num_devices;
	cl_device_id device;
	cl_context context;
	cl_command_queue queue1,queue2;
	cl_program program;
	cl_kernel kernel1,kernel2;
	cl_mem in_buffer, out_buffer;
	std::string aocxFilename;
	std::string deviceInfo;
  	int pixels; 
  	char info[256];
  	size_t sobelSize; 
	int fps_raw; 
  	cl_int status;
  	cl_event event1;
  	cl_event event2;
	cl_event eventq;
  	cl_ulong start, end;
	unsigned int thresh;
	unsigned long timer_addr=(unsigned long)&thresh;
	unsigned long addr_span_ext_control=(unsigned  long)&thresh;
	public:
	myopencl(){
		pixels=COLS*ROWS/2;
		sobelSize=1;
		fps_raw=0;
  		platform = findPlatform("Altera");
  		if (platform == NULL) {
  		  exit(1);
  		}

  		status = clGetDeviceIDs(platform, CL_DEVICE_TYPE_ALL, 1, &device, NULL);
  		checkError (status, "Error: could not query devices");
  		num_devices = 1; // always only using one device

  		clGetDeviceInfo(device, CL_DEVICE_NAME, sizeof(info), info, NULL);
  		deviceInfo = info;

  		context = clCreateContext(0, num_devices, &device, NULL, NULL, &status);
  		checkError(status, "Error: could not create OpenCL context");

  		queue1 = clCreateCommandQueue(context, device, 0, &status);
  		checkError(status, "Error: could not create command queue");

  		queue2 = clCreateCommandQueue(context, device, 0, &status);
  		checkError(status, "Error: could not create command queue");

  		std::string binary_file = getBoardBinaryFile("trojan", device);
  		std::cout << "Using AOCX: " << binary_file << "\n";
  		program = createProgramFromBinary(context, binary_file.c_str(), &device, 1);

  		status = clBuildProgram(program, num_devices, &device, "", NULL, NULL);
  		checkError(status, "Error: could not build program");

  		kernel1 = clCreateKernel(program, "trojan", &status);
  		checkError(status, "Error: could not create trojan kernel");


  		in_buffer = clCreateBuffer(context, CL_MEM_READ_WRITE |CL_MEM_ALLOC_HOST_PTR, sizeof(unsigned int) * 10, NULL, &status);
  		checkError(status, "Error: could not create device buffer");

  		out_buffer = clCreateBuffer(context, CL_MEM_WRITE_ONLY, sizeof(unsigned int) * (TOTAL_MEM/PAGE_SIZE), NULL, &status);
  		checkError(status, "Error: could not create output buffer");

  		status  = clSetKernelArg(kernel1, 0, sizeof(cl_mem), &out_buffer);
  		status |= clSetKernelArg(kernel1, 1, sizeof(cl_mem), &in_buffer);
  		//status |= clSetKernelArg(kernel1, 2, sizeof(unsigned long), &addr_span_ext_control);
  		//status |= clSetKernelArg(kernel2, 4, sizeof(cl_mem), &out_buffer);
  		checkError(status, "Error: could not set fdetect args");
	};
	void set_threshold(int thr){
		thresh=thr;
	}
		
	void enqueue(cl_uint *input){
  		status = clEnqueueWriteBuffer(queue1, in_buffer, CL_TRUE, 0, sizeof(unsigned int) * 10, input, 0, NULL, NULL);
  		checkError(status, "Error: could not copy data into device");

  		status = clFinish(queue1);
  		checkError(status, "Error: could not finish successfully");
	};
	void dequeue(cl_uint *output){
  		status = clEnqueueReadBuffer(queue2, out_buffer, CL_TRUE, 0, sizeof(unsigned int) * (TOTAL_MEM/PAGE_SIZE), output, 0, NULL, NULL);
  		checkError(status, "Error: could not copy data from device");

  		status = clFinish(queue2);
  		checkError(status, "Error: could not successfully finish copy");

	};
	void launch(){
  		status = clEnqueueNDRangeKernel(queue1, kernel1, 1, NULL, &sobelSize, &sobelSize, 0, NULL, NULL);
  		checkError(status, "Error: could not enqueue sobel filter");


  		//status = clEnqueueNDRangeKernel(queue2, kernel2, 1, NULL, &sobelSize, &sobelSize, 1, &eventq, &event2);
  		//checkError(status, "Error: could not enqueue sobel filter");

  		status  = clFinish(queue1);
  		checkError(status, "Error: could not finish successfully");

  		//status  = clFinish(queue2);
  		//checkError(status, "Error: could not finish successfully");
	};
	//void profile(){
  	//	status  = clGetEventProfilingInfo(event1, CL_PROFILING_COMMAND_START, sizeof(cl_ulong), &start, NULL);
  	//	status  = clGetEventProfilingInfo(event2, CL_PROFILING_COMMAND_END, sizeof(cl_ulong), &end, NULL);
  	//	//status |= clGetEventProfilingInfo(event2, CL_PROFILING_COMMAND_END, sizeof(cl_ulong), &end, NULL);
  	//	checkError(status, "Error: could not get profile information");
  	//	clReleaseEvent(event1);
  	//	clReleaseEvent(event2);

  	//	//fps_raw = (int)(1.0f / ((end - start) * 1e-9f));
  	//	fps_raw = end - start* 1e-9f;
	//};
	~myopencl()
	{
	  if (in_buffer) clReleaseMemObject(in_buffer);
	  if (out_buffer) clReleaseMemObject(out_buffer);
	  if (kernel1) clReleaseKernel(kernel1);
	  if (kernel2) clReleaseKernel(kernel2);
	  if (program) clReleaseProgram(program);
	  if (queue1) clReleaseCommandQueue(queue1);
	  if (queue2) clReleaseCommandQueue(queue2);
	  if (context) clReleaseContext(context);
	  cout << "TOTAL TIME " << fps_raw << endl;
	
	}
};
void cleanup() {
}