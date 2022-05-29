#ifndef __SETUP_OPENCL_H__
#define __SETUP_OPENCL_H__

#include <stdio.h>
#include <stdlib.h>
#ifdef __APPLE__
#include <OpenCL/cl.h>
#else
#include <CL/cl.h>
#endif

#define USE_ONLY_ACCELERATOR

static cl_platform_id * platforms;
static cl_device_id *devices;
static cl_context context;
static cl_command_queue command_queue;


static const char* matrix_mull_source = 
"// Settings\n"
"#define RX 8\n"
"#define RY 4\n"
"#define RK (RY)\n"
" \n"
"// Mimic clBlas (4x8 register tiling with vector data-types)\n"
"__kernel void mat_mull(const int M, const int N, const int K,\n"
"                       const __global float8* restrict A,\n"
"                       const __global float4* restrict B,\n"
"                       __global float8* C) {\n"
"    \n"
"    // Allocate register space\n"
"    float aReg[RK][RX];\n"
"    float bReg[RY][RK];\n"
"    float acc[RY][RX];\n"
" \n"
"    // Initialise the accumulation registers\n"
"    for (int y=0; y<RY; y++) {\n"
"        for (int x=0; x<RX; x++) {\n"
"            acc[y][x] = 0.0;\n"
"        }\n"
"    }\n"
" \n"
"    // Loop over all tiles\n"
"    const int numTiles = K/RK;\n"
"    for (int t=0; t<numTiles; t++) {\n"
" \n"
"        // Load a tile of A and B into register memory\n"
"        for (int y=0; y<RY; y++) {\n"
" \n"
"            // Load the data\n"
"            float8 aVec = A[(RK*t + y)*(M/RX) + get_global_id(0)];\n"
"            float4 bVec = B[(RY*get_global_id(1) + y)*numTiles + t];\n"
" \n"
"            // Store the vector of A into registers\n"
"            aReg[y][0] = aVec.s0;\n"
"            aReg[y][1] = aVec.s1;\n"
"            aReg[y][2] = aVec.s2;\n"
"            aReg[y][3] = aVec.s3;\n"
"            aReg[y][4] = aVec.s4;\n"
"            aReg[y][5] = aVec.s5;\n"
"            aReg[y][6] = aVec.s6;\n"
"            aReg[y][7] = aVec.s7;\n"
" \n"
"            // Store the vector of B into registers\n"
"            bReg[y][0] = bVec.x;\n"
"            bReg[y][1] = bVec.y;\n"
"            bReg[y][2] = bVec.z;\n"
"            bReg[y][3] = bVec.w;\n"
"        }\n"
" \n"
"        // Perform the computations\n"
"        for (int k=0; k<RK; k++) {\n"
"            for (int y=0; y<RY; y++) {\n"
"                for (int x=0; x<RX; x++) {\n"
"                    acc[y][x] += aReg[k][x] * bReg[y][k];\n"
"                }\n"
"            }\n"
"        }\n"
"    }\n"
" \n"
"    // Store the final results in C\n"
"    for (int y=0; y<RY; y++) {\n"
"        float8 accVec;\n"
"        accVec.s0 = acc[y][0];\n"
"        accVec.s1 = acc[y][1];\n"
"        accVec.s2 = acc[y][2];\n"
"        accVec.s3 = acc[y][3];\n"
"        accVec.s4 = acc[y][4];\n"
"        accVec.s5 = acc[y][5];\n"
"        accVec.s6 = acc[y][6];\n"
"        accVec.s7 = acc[y][7];\n"
"        C[(y + RY*get_global_id(1))*(M/RX) + get_global_id(0)] = accVec;\n"
"    }\n"
"}\n";


static cl_program matrix_mull;
static cl_kernel matrix_mull_kernel;

//settup all the open cl stuff
int setup_OpenCL();

//set up our matrix multiply program
int setup_matrix_mull();

//setup the matrix addition program
int setup_matrix_add();


//clean up open cl
int cleanup();
#endif