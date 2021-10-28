/*

1)

real    0m0,235s
user    0m0,059s
sys     0m0,155s

real    0m0,253s
user    0m0,067s
sys     0m0,167s

real    0m0,270s
user    0m0,075s
sys     0m0,166s

real    0m0,266s
user    0m0,091s
sys     0m0,152s

real    0m0,239s
user    0m0,073s
sys     0m0,143s

2)

==123303== NVPROF is profiling process 123303, command: ./a.out

Sum = 799999980000000.000000
==123303== Profiling application: ./a.out
==123303== Profiling result:
            Type  Time(%)      Time     Calls       Avg       Min       Max  Name
 GPU activities:   82.35%  41.041ms         1  41.041ms  41.041ms  41.041ms  [CUDA memcpy HtoD]
                   17.60%  8.7735ms         1  8.7735ms  8.7735ms  8.7735ms  sum_cuda(double*, double*, int)

3)

real    0m0,068s
user    0m0,067s
sys     0m0,001s

real    0m0,062s
user    0m0,058s
sys     0m0,004s

real    0m0,043s
user    0m0,042s
sys     0m0,000s

real    0m0,082s
user    0m0,078s
sys     0m0,001s

real    0m0,063s
user    0m0,060s
sys     0m0,000s

4)

real    0m0,041s
user    0m0,187s
sys     0m0,004s

real    0m0,029s
user    0m0,152s
sys     0m0,000s

real    0m0,044s
user    0m0,139s
sys     0m0,004s

real    0m0,009s
user    0m0,048s
sys     0m0,004s

real    0m0,057s
user    0m0,296s
sys     0m0,004s

5)

real    0m0,032s
user    0m0,142s
sys     0m0,004s

real    0m0,065s
user    0m0,207s
sys     0m0,004s

real    0m0,042s
user    0m0,205s
sys     0m0,004s

real    0m0,058s
user    0m0,277s
sys     0m0,003s

real    0m0,070s
user    0m0,323s
sys     0m0,003s

6)

real    0m0,288s
user    0m0,101s
sys     0m0,163s

real    0m0,284s
user    0m0,089s
sys     0m0,171s

real    0m0,265s
user    0m0,090s
sys     0m0,154s

real    0m0,280s
user    0m0,074s
sys     0m0,186s

real    0m0,273s
user    0m0,106s
sys     0m0,146s

==125067== NVPROF is profiling process 125067, command: ./a.out

Sum = 799999980000000.000000
==125067== Profiling application: ./a.out
==125067== Profiling result:
            Type  Time(%)      Time     Calls       Avg       Min       Max  Name
 GPU activities:   83.24%  49.618ms         1  49.618ms  49.618ms  49.618ms  [CUDA memcpy HtoD]
                   16.71%  9.9626ms         1  9.9626ms  9.9626ms  9.9626ms  sum_cuda(double*, double*, int)

Comentários:

Henrique, nós tentamos usar o servidor da PUC para pegar as métricas do CUDA com o comando nvprof, mas ele não listou tudo
que a gente precisava. Acreditamos que tem a ver com a versão do driver, porque testamos no PC de um dos integrantes do grupo
que tem uma GPU Nvidia com a versão mais nova do driver e funcionou. A GPU dele é uma Nvidia GTX 1060 6GB e a versão 
do driver é 470.74-10. Por causa disso, acreditamos que seria mais justo testar a versão OpenMP para GPU na mesma placa também.

A versão em CUDA rodou bem pior que as outras versões, e o grupo acredita que isso se deve à complexidade do código comparada 
à outras versões. Enquanto o código em CUDA precisa chamar uma função, rodar diversos loops for e fazer várias operações, as
outras versões são bem mais simples, tendo apenas 1 loop for.

Dentre as versões com e sem __shared__, podemos notar uma melhora considerável. A versão com uso do __shared__ tem uma melhora
enorme, pois o uso dessa flag faz uso da memória cache interna dos blocos da GPU, que é bem mais rápida que a memória global.

Entre as duas versões do código em CUDA, o maior overhead é causado entre a transferência dos dados entre a memória global da GPU
para as threads individuais. Como o uso da flag __shared__ favorece o uso da cache dos blocos de thread da GPU, esse overhead é
diminuído.

Obs: deixamos todos os códigos utilizados neste arquivo.

*/
#include <stdio.h>
#include <stdlib.h>

__global__ void sum_cuda(double* a, double *s, int width) {
  int t = threadIdx.x;
  int b = blockIdx.x*blockDim.x;

  __shared__ double o[1024];  

  if(b+t < width)
    o[t] = a[b+t];

  __syncthreads();
  
  int i;
  for(i = blockDim.x/2; i > 0; i /= 2) {
    if(t < i && b+t+i < width)
      o[t] += o[t+i];
    
    __syncthreads();
  }

  if(t == 0)
    s[blockIdx.x] = o[0];  
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

  // alocação e cópia dos dados
  cudaMalloc((void **) &d_a, size);
  cudaMemcpy(d_a, a, size, cudaMemcpyHostToDevice);

  cudaMalloc((void **) &d_s, s_size);

  // definição do número de blocos e threads
  dim3 dimGrid(num_blocks,1,1);
  dim3 dimBlock(block_size,1,1);

  // chamada do kernel
  sum_cuda<<<dimGrid,dimBlock>>>(d_a, d_s, width);

  // cópia dos resultados para o host
  cudaMemcpy(s, d_s, s_size, cudaMemcpyDeviceToHost);

  // soma das reduções parciais
  for(int i = 1; i < num_blocks; i++) 
    s[0] += s[i];

  printf("\nSum = %f\n",s[0]);
  
  cudaFree(d_a);
  cudaFree(d_s);
}

/* sum-cuda-noshared
#include <stdio.h>
#include <stdlib.h>

__global__ void sum_cuda(double* a, double *s, int width) {
  int t = threadIdx.x;
  int b = blockIdx.x*blockDim.x;
 
  int i;
  for(i = blockDim.x/2; i > 0; i /= 2) {
    if(t < i && b+t+i < width)
      a[t+b] += a[t+b+i];
    
    __syncthreads();
  }
  if(t == 0)
    s[blockIdx.x] = a[t+b];  
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

  // alocação e cópia dos dados
  cudaMalloc((void **) &d_a, size);
  cudaMemcpy(d_a, a, size, cudaMemcpyHostToDevice);

  cudaMalloc((void **) &d_s, s_size);

  // definição do número de blocos e threads
  dim3 dimGrid(num_blocks,1,1);
  dim3 dimBlock(block_size,1,1);

  // chamada do kernel
  sum_cuda<<<dimGrid,dimBlock>>>(d_a, d_s, width);

  // cópia dos resultados para o host
  cudaMemcpy(s, d_s, s_size, cudaMemcpyDeviceToHost);

  // soma das reduções parciais
  for(int i = 1; i < num_blocks; i++) 
    s[0] += s[i];

  printf("\nSum = %f\n",s[0]);
  
  cudaFree(d_a);
  cudaFree(d_s);
}
*/

/* sum sequencial
#include <stdio.h>
#include <stdlib.h>

int main()
{
  int width = 40000000;
  double sum = 0;


  for(int i = 0; i < width; i++)
  {
     sum += i;
  }

  printf("\nSum = %f\n",sum);
}
*/

/* sum paralelo
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>


int main()
{
  double sum   =  0;
  int width    =  40000000;

  #pragma omp parallel for simd reduction(+:sum)
  for(int i = 0; i < width; i++)
  {
     sum += i;
  }

  printf("\nSum = %lf\n",sum);
}
*/

/* sum paralelo gpu
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>


int main()
{
  double sum   =  0;
  int width    =  40000000;

  #pragma omp target teams distribute parallel for simd map(tofrom:sum) map(to:width) reduction(+:sum) 
  for(int i = 0; i < width; i++)
  {
     sum += i;
  }

  printf("\nSum = %lf\n",sum);
}
*/