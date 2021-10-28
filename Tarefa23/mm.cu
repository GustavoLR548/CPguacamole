
/*

Tempo sequencial:

real    0m31,842s
user    0m31,713s
sys     0m0,083s

real    0m30,929s
user    0m30,789s
sys     0m0,074s

real    0m31,569s
user    0m31,408s
sys     0m0,077s

real    0m30,880s
user    0m30,749s
sys     0m0,067s

real    0m31,120s
user    0m30,967s
sys     0m0,087s

Tempo Paralelo - Multicore:

real    0m9,663s
user    1m1,498s
sys     0m2,830s

real    0m9,897s
user    1m1,072s
sys     0m2,809s

real    0m9,767s
user    1m0,906s
sys     0m2,798s

real    0m9,857s
user    1m1,442s
sys     0m2,972s

real    0m9,709s
user    1m1,580s
sys     0m3,002s

Tempo paralelo - GPU 

distribute 

real    0m30,728s
user    0m30,683s
sys     0m0,184s

real    0m30,883s
user    0m30,952s
sys     0m0,122s

real    0m31,138s
user    0m31,304s
sys     0m0,087s

real    0m31,675s
user    0m31,759s
sys     0m0,050s

real    0m31,450s
user    0m31,600s
sys     0m0,043s

distribute parallel for

real    0m9,370s
user    0m54,341s
sys     0m0,196s

real    0m9,421s
user    0m55,248s
sys     0m0,227s

real    0m8,649s
user    0m59,349s
sys     0m0,220s

real    0m8,653s
user    0m59,911s
sys     0m0,234s

real    0m8,644s
user    0m59,108s
sys     0m0,195s

distribute parallel for simd

real    0m9,144s
user    0m56,485s
sys     0m0,878s

real    0m9,119s
user    0m58,920s
sys     0m0,247s

real    0m8,835s
user    0m57,115s
sys     0m0,208s

real    0m8,842s
user    0m57,008s
sys     0m0,170s

real    0m8,984s
user    0m56,533s
sys     0m0,232s

Tempo paralelo do código em cuda:

real    0m0,803s
user    0m0,580s
sys     0m0,196s

real    0m0,789s
user    0m0,586s
sys     0m0,180s

real    0m0,720s
user    0m0,575s
sys     0m0,120s

real    0m0,693s
user    0m0,559s
sys     0m0,114s

real    0m0,752s
user    0m0,588s
sys     0m0,138s

Dados do nvprof

Invocations                                Event Name         Min         Max         Avg       Total
Device "GeForce GT 1030 (0)"
    Kernel: mm(double*, double*, double*, int)
          1                            warps_launched      277699      277699      277699      277699

==21368== Metric result:
Invocations                               Metric Name                        Metric Description         Min         Max         Avg
Device "GeForce GT 1030 (0)"
    Kernel: mm(double*, double*, double*, int)
          1                 warp_execution_efficiency                 Warp Execution Efficiency      99.21%      99.21%      99.21%


*/

#include <stdio.h>
#include <stdlib.h>
#include <cuda.h>

__global__ void mm(double* a, double* b, double* c, int width) 
{
	int j = blockIdx.y*blockDim.y+threadIdx.y;
	int i = blockIdx.x*blockDim.x+threadIdx.x;

	if((i < width) && (j < width))
	{	
		double sum = 0;
		for (int k = 0; k < width; k++) {
			double x = a[i * width + k];
			double y = b[k * width + j];
			sum += x * y;
		}
		c[i * width + j] = sum;
	}
}

int main()
{
	int width = 2000;
	double *a = (double*) malloc (width * width * sizeof(double));
	double *b = (double*) malloc (width * width * sizeof(double));
	double *c = (double*) malloc (width * width * sizeof(double));

	for(int i = 0; i < width; i++) {	  
		for(int j = 0; j < width; j++) {
			a[i*width+j] = i;
			b[i*width+j] = j;
			c[i*width+j] = 0;
		}
	}

	int size = width*width*sizeof(double);
	double *d_a, *d_b, *d_c;

	cudaMalloc((void **) &d_a, size);
	cudaMemcpy(d_a, a, size, cudaMemcpyHostToDevice);

	cudaMalloc((void **) &d_b, size);
	cudaMemcpy(d_b, b, size, cudaMemcpyHostToDevice);

	cudaMalloc((void **) &d_c, size);

	int block_size = 32;
	dim3 dimGrid((width-1)/block_size+1, (width-1)/block_size+1, 1);
	dim3 dimBlock(block_size, block_size, 1);

	mm<<<dimGrid, dimBlock>>>(d_a, d_b, d_c, width);

	cudaMemcpy(c, d_c, size, cudaMemcpyDeviceToHost);

	/*
	for(int i = 0; i < width; i++) {
		for(int j = 0; j < width; j++) {
			printf("\n c[%d][%d] = %lf",i,j,c[i*width+j]);
		}
	}
	*/

	cudaFree(d_a);
	cudaFree(d_b);
	cudaFree(d_c);
}