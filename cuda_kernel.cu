#include <cuda_runtime.h>

__global__ void xor_decrypt_kernel(unsigned char* input, unsigned char* output, unsigned char key, size_t size) {
    int idx = blockIdx.x * blockDim.x + threadIdx.x;
    if (idx < size) {
        output[idx] = input[idx] ^ key;
    }
}
