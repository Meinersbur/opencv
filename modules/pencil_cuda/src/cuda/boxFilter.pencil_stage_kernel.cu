#include <stdio.h>
#include <assert.h>
__global__ void __ppcg_applyBoxFilter_kernel0(unsigned char *dst, unsigned char *src, int srcCols, int rows, int dstCols, int cols, int se_dim, int filterSizeY, int filterSizeX, int filterAnchorX, int filterAnchorY)
{
    int b0 = blockIdx.y, b1 = blockIdx.x;
    int t0 = threadIdx.y, t1 = threadIdx.x;
    int private_avg_sum;
    int private_curr_y;
    int private_curr_x;

    #define ppcg_min(x,y)    ({ __typeof__(x) _x = (x); __typeof__(y) _y = (y); _x < _y ? _x : _y; })
    for (int c0 = 32 * b0; c0 < rows; c0 += 8192)
      if (rows >= t0 + c0 + 1)
        for (int c1 = 32 * b1; c1 < cols; c1 += 8192)
          for (int c3 = t1; c3 <= ppcg_min(31, cols - c1 - 1); c3 += 16) {
            private_avg_sum = 0;
            for (int c4 = 0; c4 < filterSizeY; c4 += 1) {
              private_curr_y = (-filterAnchorY + t0 + c0 + c4);
              if ((-filterAnchorY + t0 + c0 + c4) < 0) {
                private_curr_y = (-(-filterAnchorY + t0 + c0 + c4));
              } else {
                if ((-filterAnchorY + t0 + c0 + c4) >= (rows)) {
                  private_curr_y = ((2 * ((rows) - 1)) - (-filterAnchorY + t0 + c0 + c4));
                }
              }
              for (int c5 = 0; c5 < filterSizeX; c5 += 1) {
                private_curr_x = (-filterAnchorX + c1 + c3 + c5);
                if ((-filterAnchorX + c1 + c3 + c5) < 0) {
                  private_curr_x = (-(-filterAnchorX + c1 + c3 + c5));
                } else {
                  if ((-filterAnchorX + c1 + c3 + c5) >= (cols)) {
                    private_curr_x = ((2 * ((cols) - 1)) - (-filterAnchorX + c1 + c3 + c5));
                  }
                }
                private_avg_sum += src[private_curr_y * srcCols + private_curr_x];
              }
            }
            dst[(t0 + c0) * dstCols + (c1 + c3)] = (private_avg_sum / (se_dim));
          }
}

extern "C" void __ppcg_applyBoxFilter_prog0(int srcCols, int rows, int dstCols, int cols, int se_dim, int filterSizeY, int filterSizeX, int filterAnchorX, int filterAnchorY, unsigned char *dst, unsigned char *src)
{
  #define ppcg_fdiv_q(n,d) (((n)<0) ? -((-(n)+(d)-1)/(d)) : (n)/(d))
  #define ppcg_min(x,y)    ({ __typeof__(x) _x = (x); __typeof__(y) _y = (y); _x < _y ? _x : _y; })
  {
#define cudaCheckReturn(ret) \
  do { \
    cudaError_t cudaCheckReturn_e = (ret); \
    if (cudaCheckReturn_e != cudaSuccess) { \
      fprintf(stderr, "CUDA error: %s\n", cudaGetErrorString(cudaCheckReturn_e)); \
      fflush(stderr); \
    } \
    assert(cudaCheckReturn_e == cudaSuccess); \
  } while(0)
#define cudaCheckKernel() \
  do { \
    cudaCheckReturn(cudaGetLastError()); \
  } while(0)
/*
    unsigned char *dev_dst;
    unsigned char *dev_src;
    
    cudaCheckReturn(cudaMalloc((void **) &dev_dst, (rows) * (dstCols) * sizeof(unsigned char)));
    cudaCheckReturn(cudaMalloc((void **) &dev_src, (rows) * (srcCols) * sizeof(unsigned char)));
    
    cudaCheckReturn(cudaMemcpy(dev_dst, dst, (rows) * (dstCols) * sizeof(unsigned char), cudaMemcpyHostToDevice));
    cudaCheckReturn(cudaMemcpy(dev_src, src, (rows) * (srcCols) * sizeof(unsigned char), cudaMemcpyHostToDevice));*/
    {
      dim3 k0_dimBlock(16, 32);
      dim3 k0_dimGrid(ppcg_min((cols + 31) / 32, 256), ppcg_min(256, (rows + 31) / 32));
      __ppcg_applyBoxFilter_kernel0 <<<k0_dimGrid, k0_dimBlock>>> (dst, src, srcCols, rows, dstCols, cols, se_dim, filterSizeY, filterSizeX, filterAnchorX, filterAnchorY);
      cudaCheckKernel();
    }
    /*
    cudaCheckReturn(cudaMemcpy(dst, dev_dst, (rows) * (dstCols) * sizeof(unsigned char), cudaMemcpyDeviceToHost));
    cudaCheckReturn(cudaFree(dev_dst));
    cudaCheckReturn(cudaFree(dev_src));*/
  }
}
