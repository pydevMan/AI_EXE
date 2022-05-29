#include <vector>
#include "setup_opencl.hh"

int 
setup_OpenCL(){
    cl_uint     num_platforms;
  
    // set up platform
    cl_int clStatus = clGetPlatformIDs(0, NULL, &num_platforms);
    platforms = (cl_platform_id *)
    malloc(sizeof(cl_platform_id)*num_platforms);
    clStatus = clGetPlatformIDs(num_platforms, platforms, NULL);

    //Get the devices list and choose the device you want to run on
    cl_uint           num_devices;

    clStatus = clGetDeviceIDs( platforms[0], CL_DEVICE_TYPE_ACCELERATOR, 0,NULL, &num_devices);
    devices = (cl_device_id *) 
    malloc(sizeof(cl_device_id)*num_devices);
    clStatus = clGetDeviceIDs( platforms[0],CL_DEVICE_TYPE_ACCELERATOR, num_devices, devices, NULL);

    // Create one OpenCL context for each device in the platform
    context = clCreateContext( NULL, num_devices, devices, NULL, NULL, &clStatus);

    cl_command_queue command_queue = clCreateCommandQueue(context, devices[0], 0, &clStatus);
    return 0;
}


int
setup_matrix_mull()
{
    cl_int err;


    const char** device_code = (const char**)&matrix_mull_source;

    matrix_mull = clCreateProgramWithSource(context, 1, device_code, (const size_t *)sizeof(device_code), NULL);

    // make the kernal

    err = clBuildProgram(matrix_mull, 1, devices, NULL, NULL, NULL);

    matrix_mull_kernel = clCreateKernel(matrix_mull, "mat_mull", &err);
    return 0;
}


int cleanup(){
    clFlush(command_queue);
    clFinish(command_queue);
    clReleaseKernel(matrix_mull_kernel);
    clReleaseProgram(matrix_mull);
    clReleaseCommandQueue(command_queue);
    clReleaseContext(context);
    return 0;
}