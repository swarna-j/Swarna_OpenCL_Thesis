//
// File:       vector_add.c
//
// Abstract:   A simple compute example showing basic usage of OpenCL to find the
//             sum of two vectors (Z = aX + bY + c; X,Y,Z are vectors of fixed size; 
//             a,b,c are scalar coefficients)            
//             
// Version:    <1.0>
//
// Disclaimer: IMPORTANT:  This Apple software is supplied to you by Apple Inc. ("Apple")
//             in consideration of your agreement to the following terms, and your use,
//             installation, modification or redistribution of this Apple software
//             constitutes acceptance of these terms.  If you do not agree with these
//             terms, please do not use, install, modify or redistribute this Apple
//             software.
//
//             In consideration of your agreement to abide by the following terms, and
//             subject to these terms, Apple grants you a personal, non - exclusive
//             license, under Apple's copyrights in this original Apple software ( the
//             "Apple Software" ), to use, reproduce, modify and redistribute the Apple
//             Software, with or without modifications, in source and / or binary forms;
//             provided that if you redistribute the Apple Software in its entirety and
//             without modifications, you must retain this notice and the following text
//             and disclaimers in all such redistributions of the Apple Software. Neither
//             the name, trademarks, service marks or logos of Apple Inc. may be used to
//             endorse or promote products derived from the Apple Software without specific
//             prior written permission from Apple.  Except as expressly stated in this
//             notice, no other rights or licenses, express or implied, are granted by
//             Apple herein, including but not limited to any patent rights that may be
//             infringed by your derivative works or by other works in which the Apple
//             Software may be incorporated.
//
//             The Apple Software is provided by Apple on an "AS IS" basis.  APPLE MAKES NO
//             WARRANTIES, EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION THE IMPLIED
//             WARRANTIES OF NON - INFRINGEMENT, MERCHANTABILITY AND FITNESS FOR A
//             PARTICULAR PURPOSE, REGARDING THE APPLE SOFTWARE OR ITS USE AND OPERATION
//             ALONE OR IN COMBINATION WITH YOUR PRODUCTS.
//
//             IN NO EVENT SHALL APPLE BE LIABLE FOR ANY SPECIAL, INDIRECT, INCIDENTAL OR
//             CONSEQUENTIAL DAMAGES ( INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
//             SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
//             INTERRUPTION ) ARISING IN ANY WAY OUT OF THE USE, REPRODUCTION, MODIFICATION
//             AND / OR DISTRIBUTION OF THE APPLE SOFTWARE, HOWEVER CAUSED AND WHETHER
//             UNDER THEORY OF CONTRACT, TORT ( INCLUDING NEGLIGENCE ), STRICT LIABILITY OR
//             OTHERWISE, EVEN IF APPLE HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// Copyright ( C ) 2008 Apple Inc. All Rights Reserved.
//

////////////////////////////////////////////////////////////////////////////////

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif

////////////////////////////////////////////////////////////////////////////////

// Use a static data size for simplicity
//
#define DATA_SIZE 3
#define A 2
#define B 3
#define C 1

////////////////////////////////////////////////////////////////////////////////
// Function to extract cl kernel for execution
static char* load_program_source(const char *filename)
{
    struct stat statbuf;
    FILE        *fh;
    char        *source;

    fh = fopen(filename, "r");
    if (fh == 0)
        return 0;

    stat(filename, &statbuf);
    source = (char *) malloc(statbuf.st_size + 1);
    fread(source, statbuf.st_size, 1, fh);
    source[statbuf.st_size] = '\0';
    fclose(fh);

    return source;
}
////////////////////////////////////////////////////////////////////////////////

int main(int argc, char** argv)
{
    int err;                            // error code returned from api calls
      
    int data1[DATA_SIZE];               // Input data set 1 given to device
    int data2[DATA_SIZE];               // Input data set 2 given to device
    int results[DATA_SIZE];             // results returned from device
    unsigned int correct;               // number of correct results returned

    size_t global;                      // global domain size for our calculation
    size_t local;                       // local domain size for our calculation
    
    cl_platform_id platform_id[2];      // compute device platform id
    cl_device_id device_id;             // compute device id 
    cl_context context;                 // compute context
    cl_command_queue commands;          // compute command queue
    cl_program program;                 // compute program
    cl_kernel kernel;                   // compute kernel
    
    cl_mem input1;                      // device memory used for the input array
    cl_mem input2;                      // device memory used for the input array
    cl_mem output;                      // device memory used for the output array
    cl_event event;
    
    // Fill our data set with random float values
    //
    int i = 0;
    unsigned int count = DATA_SIZE;
    for(i = 0; i < count; i++)
    {
        data1[i] = rand() / RAND_MAX;
        data2[i] = rand() / RAND_MAX;
    }

    //Connect to a platform on device
    cl_uint num_platforms;
    err = clGetPlatformIDs(2, platform_id, &num_platforms);

    if (err != CL_SUCCESS)
    {
        printf("Error: Failed to locate opencl platform!\n");
        return EXIT_FAILURE;
    }
    else
    {         
        unsigned int index = 1;
        char platformName[1024];
        printf("\tNumber of Platforms : %u\n", num_platforms);
        clGetPlatformInfo (platform_id[index], CL_PLATFORM_NAME, sizeof(platformName), platformName, NULL);   
        printf("\tPlatform: %u\n\tPlatform Name: %s\n", index, platformName);                  
    }
    
    // Connect to a compute device
    int gpu = 1; // Select GPU 
    err = clGetDeviceIDs(platform_id[1], gpu ? CL_DEVICE_TYPE_GPU : CL_DEVICE_TYPE_CPU, 1, &device_id, NULL);    

    if (err != CL_SUCCESS)
    {
        printf("Error: Failed to create a device group!\n");
        return EXIT_FAILURE;
    }
   else
   {
       char deviceName[1024];      
       //Only one device available (Reference : "devquery.c". Hence not accessed as a matrix element
       clGetDeviceInfo(device_id, CL_DEVICE_NAME, sizeof(deviceName), deviceName, NULL); 
       printf("\tDevice: 0\n\tDevice Name: %s\n", deviceName); 
   }
  
    // Create a compute context 
    //
    context = clCreateContext(0, 1, &device_id, NULL, NULL, &err);
    if (!context)
    {
        printf("Error: Failed to create a compute context!\n");
        return EXIT_FAILURE;
    }

    // Create a command commands
    //
    commands = clCreateCommandQueue(context, device_id, CL_QUEUE_PROFILING_ENABLE, &err);
    if (!commands)
    {
        printf("Error: Failed to create a command commands!\n");
        return EXIT_FAILURE;
    }

    //Use function and load the kernel source from .cl files in the same folder
    //
    char *KernelSource = load_program_source("vector_add.cl");

    // Create the compute program from the source buffer
    //
    program = clCreateProgramWithSource(context, 1, (const char **) & KernelSource, NULL, &err);
    if (!program)
    {
        printf("Error: Failed to create compute program!\n");
        return EXIT_FAILURE;
    }

    // Build the program executable
    //
    err = clBuildProgram(program, 0, NULL, NULL, NULL, NULL);
    if (err != CL_SUCCESS)
    {
        size_t len;
        char buffer[2048];

        printf("Error: Failed to build program executable!\n");
        clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, sizeof(buffer), buffer, &len);
        printf("%s\n", buffer);
        exit(1);
    }

    // Create the compute kernel in the program we wish to run
    //
    kernel = clCreateKernel(program, "square", &err);
    if (!kernel || err != CL_SUCCESS)
    {
        printf("Error: Failed to create compute kernel! - %d\n",err);
        exit(1);
    }

    // Create the input and output arrays in device memory for our calculation
    //
    input1 = clCreateBuffer(context, CL_MEM_READ_ONLY,  sizeof(int) * count, NULL, NULL);
    input2 = clCreateBuffer(context, CL_MEM_READ_ONLY,  sizeof(int) * count, NULL, NULL);
    output = clCreateBuffer(context, CL_MEM_WRITE_ONLY, sizeof(int) * count, NULL, NULL);
    if (!input1 || !input2 || !output)
    {
        printf("Error: Failed to allocate device memory!\n");
        exit(1);
    }    
    
    // Write our data set into the input array in device memory 
    //
    err = clEnqueueWriteBuffer(commands, input1, CL_TRUE, 0, sizeof(int) * count, data1, 0, NULL, NULL);
    err|= clEnqueueWriteBuffer(commands, input2, CL_TRUE, 0, sizeof(int) * count, data2, 0, NULL, NULL);
    if (err != CL_SUCCESS)
    {
        printf("Error: Failed to write to source array!\n");
        exit(1);
    }

    // Set the arguments to our compute kernel
    //
    err = 0;
    err  = clSetKernelArg(kernel, 0, sizeof(cl_mem), &input1);
    err |= clSetKernelArg(kernel, 1, sizeof(cl_mem), &input2);
    err |= clSetKernelArg(kernel, 2, sizeof(cl_mem), &output);
    if (err != CL_SUCCESS)
    {
        printf("Error: Failed to set kernel arguments! %d\n", err);
        exit(1);
    }

    // Get the maximum work group size for executing the kernel on the device
    //
    err = clGetKernelWorkGroupInfo(kernel, device_id, CL_KERNEL_WORK_GROUP_SIZE, sizeof(local), &local, NULL);
    if (err != CL_SUCCESS)
    {
        printf("Error: Failed to retrieve kernel work group info! %d\n", err);
        exit(1);
    }

    // Execute the kernel over the entire range of our 1d input data set
    // using the maximum number of work group items for this device
    //
    global = count;
    err = clEnqueueNDRangeKernel(commands, kernel, 1, NULL, &global, NULL, 0, NULL, &event);
    if (err)
    {
        printf("Error: Failed to execute kernel!-%d\n",err);
        return EXIT_FAILURE;
    }

    // Wait for the command commands to get serviced before reading back results
    //
    clWaitForEvents(1, &event);
    clFinish(commands);
    cl_ulong time_start, time_end;
    double total_time;
    clGetEventProfilingInfo(event, CL_PROFILING_COMMAND_START, sizeof(time_start), &time_start, NULL);
    clGetEventProfilingInfo(event, CL_PROFILING_COMMAND_END, sizeof(time_end), &time_end, NULL);
    total_time = time_end - time_start;
    printf("cl:main timing:opencl clEnqueueNDRangeKernel %0.3f us\n", total_time / 1000.0);

    // Read back the results from the device to verify the output
    //
    err = clEnqueueReadBuffer( commands, output, CL_TRUE, 0, sizeof(int) * count, results, 0, NULL, NULL );  
    if (err != CL_SUCCESS)
    {
        printf("Error: Failed to read output array! %d\n", err);
        exit(1);
    }
    
    // Validate our results
    //
    correct = 0;
    for(i = 0; i < count; i++)
    {
        if(results[i] == A*data1[i] + B*data2[i] + C)
            correct++;
    }
    
    // Print a brief summary detailing the results
    //
    printf("Computed '%d/%d' correct values!\n", correct, count);
    
    // Shutdown and cleanup
    //
    clReleaseMemObject(input1);
    clReleaseMemObject(input2);
    clReleaseMemObject(output);
    clReleaseProgram(program);
    clReleaseKernel(kernel);
    clReleaseCommandQueue(commands);
    clReleaseContext(context);

    return 0;
}

