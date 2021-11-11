/*

Tempo sequencial

real    0m0,178s
user    0m0,085s
sys     0m0,092s

real    0m0,164s
user    0m0,073s
sys     0m0,090s

real    0m0,163s
user    0m0,083s
sys     0m0,080s

real    0m0,167s
user    0m0,062s
sys     0m0,102s

real    0m0,166s
user    0m0,071s
sys     0m0,094s

Tempo paralelo

real    0m0,219s
user    0m0,030s
sys     0m0,169s

real    0m0,211s
user    0m0,027s
sys     0m0,163s

real    0m0,214s
user    0m0,027s
sys     0m0,167s

real    0m0,202s
user    0m0,023s
sys     0m0,160s

real    0m0,229s
user    0m0,036s
sys     0m0,182s

*/

#include <stdio.h>
#include <stdlib.h>

__global__ void scan_cuda(double* a, double *s, int width) {
 int t = threadIdx.x;
 int b = blockIdx.x*blockDim.x; 
 double x;

 __shared__ double p[1024];

 if(b+t < width)
     p[t] = a[b+t];

 __syncthreads();

 for(int i = 1; i < blockDim.x; i *= 2) {
      if(t >= i)
          x = p[t] + p[t-i];

      __syncthreads();

      if(t >= i)
          p[t] = x;

      __syncthreads();
  }

 if(b + t < width)
     a[b+t] = p[t];

 if(t == blockDim.x-1)
     s[blockIdx.x+1] = a[b+t];

} 

__global__ void add_cuda(double *a, double *s, int width) {
  int t = threadIdx.x;                                                                                                           
  int b = blockIdx.x*blockDim.x; 
 
  if(b+t < width) 
      a[b+t] += s[blockIdx.x];
}




int main()
{
  int width = 40000000;
  int size = width * sizeof(double);

  int block_size = 1024;
  int num_blocks = (width-1)/block_size+1;
  int s_size = (num_blocks * sizeof(double));  
 
  double *a = (double*) malloc (size);
  double *s = (double*) malloc (s_size);

  for(int i = 0; i < width; i++)
    a[i] = i;

  double *d_a, *d_s;

  s[0] = 0;
  for (int i = 1; i < num_blocks; i++)
    s[i] += s[i-1];

  printf("\na[%d] = %f\n",width-1,a[width-1]);
  
  cudaFree(d_a);
  cudaFree(d_s);
}