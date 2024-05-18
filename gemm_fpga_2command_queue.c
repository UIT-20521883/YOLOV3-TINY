#define __USE_MINGW_ANSI_STDIO 1
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifdef FPGA
#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif

#define MEM_SIZE (128)
#define MAX_SOURCE_SIZE (0x50000)

#ifdef FPGA
#include "gemm.h"
#include "cl_body.h"
#endif

#include "half.h"

// static cl_device_id device_id = NULL;
static cl_context context = NULL;
static cl_command_queue command_queue1, command_queue2, command_queue;
static cl_mem memobjA = NULL;
static cl_mem memobjB = NULL;
static cl_mem memobjC = NULL;
static cl_program program = NULL;
static cl_kernel kernel = NULL;
static cl_kernel kernels[MAX_ENV], kernels1[MAX_ENV], kernels2[MAX_ENV];
#define GEMMfW2 1
#define GEMMfW1 0
// static cl_platform_id platform_id = NULL;

int gemm_fpga_init()
{

    char *emulator_flag1 = getenv("CL_CONTEXT_EMULATOR_DEVICE_INTELFPGA");
    char *emulator_flag2 = getenv("CL_CONTEXT_EMULATOR_DEVICE_ALTERA");
    char *aocx = "gemm_fpga.aocx";
    if (emulator_flag1 && !strncmp(emulator_flag1, "1", 1))
    {
        aocx = "gemm_emu.aocx", printf("emulator_mode1:%s\n", aocx);
    }
    if (emulator_flag2 && !strncmp(emulator_flag2, "1", 1))
    {
        aocx = "gemm_emu.aocx", printf("emulator_mode2:%s\n", aocx);
    }
    const char *k_name1[1] = {"gemm_nn9W"};
    const char *k_name2[1] = {"gemm_nnfW"};
    find_CnKQ(
        "Intel(R) FPGA SDK for OpenCL(TM)",
        aocx,
        1,
        k_name1,
        &context, kernels1, &command_queue1);
        find_CnKQ(
        "Intel(R) FPGA SDK for OpenCL(TM)",
        aocx,
        1,
        k_name2,
        &context, kernels2, &command_queue2);
    return 0;
}

void wait_kernel_finish()
{
    cl_uint ret;
    clFinish(command_queue1);
    ret = clReleaseMemObject(memobjA);
    ret = clReleaseMemObject(memobjB);
    ret = clReleaseMemObject(memobjC);
    return;
}

#ifdef OPENEXR

#ifdef OPENEXR

void gemm_ntt_fpga_half(int M, int N, int K, float ALPHA,
                       half *A, int lda,
                       half *B, int ldb,
                       half *C, int ldc) {
    cl_int ret = 0;

    // Chia dữ liệu cho hai kernel
    half *A1 = A;                   // nửa đầu của A
    half *A2 = A;       // nửa sau của A
    half *B1 = B;                   // nửa đầu của B
    half *B2 = B + K * N / 2;       // nửa sau của B
    half *C1 = C;                   // nửa đầu của C
    half *C2 = C + M * N / 2;       // nửa sau của C

    // Tạo buffer cho từng nửa của dữ liệu
    cl_mem memobjA1 = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
                                    M * K * sizeof(cl_half), A1, &ret);
    checkErr(ret, "clCreateBuffer-A1");
    cl_mem memobjA2 = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
                                   M * K * sizeof(cl_half), A2, &ret);
    checkErr(ret, "clCreateBuffer-A2");
    cl_mem memobjB1 = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
                                    K * N /2* sizeof(cl_half), B1, &ret);
    checkErr(ret, "clCreateBuffer-B1");
    cl_mem memobjB2 = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
                                   K * N / 2 * sizeof(cl_half), B2, &ret);
    checkErr(ret, "clCreateBuffer-B2");
    cl_mem memobjC1 = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR,
                                    M * N /2* sizeof(cl_half), C1, &ret);
    checkErr(ret, "clCreateBuffer-C1");
    cl_mem memobjC2 = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR,
                                   M * N / 2 * sizeof(cl_half), C2, &ret);
    checkErr(ret, "clCreateBuffer-C2");

    // Sử dụng hai kernel riêng biệt
    cl_kernel kernel1 = kernels1[0];
    cl_kernel kernel2 = kernels2[0]; // Lấy kernel thứ hai
    int NN=N/2;
    int MM=M/1;
    // Thiết lập tham số cho kernel 1
    ret = clSetKernelArg(kernel1, 0, sizeof(cl_int), &MM);
    ret |= clSetKernelArg(kernel1, 1, sizeof(cl_int), &NN);
    ret |= clSetKernelArg(kernel1, 2, sizeof(cl_int), &K);
    ret |= clSetKernelArg(kernel1, 3, sizeof(cl_mem), (void *)&memobjA1);
    ret |= clSetKernelArg(kernel1, 4, sizeof(cl_mem), (void *)&memobjB1);
    ret |= clSetKernelArg(kernel1, 5, sizeof(cl_mem), (void *)&memobjC1);
    checkErr(ret, "clSetKernelArg-1");
 
    // Thiết lập tham số cho kernel 2
    ret = clSetKernelArg(kernel2, 0, sizeof(cl_int), &MM);
    ret |= clSetKernelArg(kernel2, 1, sizeof(cl_int), &NN);
    ret |= clSetKernelArg(kernel2, 2, sizeof(cl_int), &K);
    ret |= clSetKernelArg(kernel2, 3, sizeof(cl_mem), (void *)&memobjA2);
    ret |= clSetKernelArg(kernel2, 4, sizeof(cl_mem), (void *)&memobjB2);
    ret |= clSetKernelArg(kernel2, 5, sizeof(cl_mem), (void *)&memobjC2);
    checkErr(ret, "clSetKernelArg-2");

    /* Execute OpenCL Kernel */

    size_t global_work_size[2] = {MM, NN};
    ret = clEnqueueNDRangeKernel(command_queue1, kernel1, 2, NULL, global_work_size, NULL, 0, NULL, NULL);
    checkErr(ret, "clEnqueueNDRangeKernel 1");
    ret = clEnqueueNDRangeKernel(command_queue2, kernel2, 2, NULL, global_work_size, NULL, 0, NULL, NULL);
    checkErr(ret, "clEnqueueNDRangeKernel 2");
    clFlush(command_queue1);
    clFlush(command_queue2);

    ret = clEnqueueReadBuffer(command_queue1, memobjC1, CL_TRUE, 0, sizeof(half) * M * N/2, C1, 0, NULL, NULL);
    ret = clEnqueueReadBuffer(command_queue2, memobjC2, CL_TRUE, 0, sizeof(half) * M * N/2, C2, 0, NULL, NULL);
    
    // NonBlocking Task
    if (get_Nonblocking_launch())
        return;

    clFinish(command_queue1);
    clFinish(command_queue2);
    // Giải phóng bộ nhớ
    ret = clReleaseMemObject(memobjA1);
    ret = clReleaseMemObject(memobjA2);
    ret = clReleaseMemObject(memobjB1);
    ret = clReleaseMemObject(memobjB2);
    ret = clReleaseMemObject(memobjC1);
    ret = clReleaseMemObject(memobjC2);
}

#endif

#endif

void gemm_ntt_fpga(int M, int N, int K, float ALPHA,
                   float *A, int lda,
                   float *B, int ldb,
                   float *C, int ldc)
{
}
void gemm_nn_fpga(int M, int N, int K, float ALPHA,
                  half *A, int lda,
                  half *B, int ldb,
                  half *C, int ldc)
{
}
void gemm_nn_fpga_half(int M, int N, int K, float ALPHA,
                       half *A, int lda,
                       half *B, int ldb,
                       half *C, int ldc)
{
}
void gemm_fpga_finalize()
{
    cl_int ret;
    /* Finalization */
    ret = clFlush(command_queue);
    ret = clFinish(command_queue);
    ret = clReleaseKernel(kernel);
    ret = clReleaseProgram(program);
    ret = clReleaseCommandQueue(command_queue);
    ret = clReleaseContext(context);

    // free ((void*)source_str);

    if (ret == CL_SUCCESS)
        fprintf(stderr, "gemm fpga finalized.\n");
    return;
}
#endif
