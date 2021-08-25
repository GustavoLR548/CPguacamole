#include <stdio.h>
#include <stdlib.h>
/* PC da PUC
* Programa paralelizado
*
*Teste1
*real	0m0.463s
*user	0m1.800s
*sys	0m0.004s
*
*Teste2
*real	0m0.463s
*user	0m1.803s
*sys	0m0.000s
*
*Teste3
*real	0m0.469s
*user	0m1.803s
*sys	0m0.004s
*
*Teste4
*real	0m0.465s
*user	0m1.808s
*sys	0m0.000s
*
* Programa sequencial
*
*Teste1
*real	0m1.448s
*user	0m1.442s
*sys	0m0.004s
*
*Teste2
*real	0m1.448s
*user	0m1.434s
*sys	0m0.012s
*
*Teste3
*real	0m1.466s
*user	0m1.462s
*sys	0m0.000s
*
*Teste4
*real	0m1.459s
*user	0m1.449s
*sys	0m0.008s
*
*/
//Meu PC:
// Linux version 5.13.12-arch1-1 (linux@archlinux) (gcc (GCC) 11.1.0
// CPU: Intel i7-7700 (8) @ 4.200GHz 
//
// Paralelizado
// 101,36s user 2,77s system 432% cpu 24,073 total
//
// Não paralelizado
// 63,21s user 0,03s system 99% cpu 1:03,36 total

void mm(double* a, double* b, double* c, int width) 
{
  #pragma omp parallel for collapse(2)
  for (int i = 0; i < width; i++) {
    for (int j = 0; j < width; j++) {
      double sum = 0;
      #pragma omp parallel for
      for (int k = 0; k < width; k++) {
        double x = a[i * width + k];
        double y = b[k * width + j];
        sum += x * y;
      }
      c[i * width + j] = sum;
    }
  }
}

int main()
{
  int width = 500;
  double *a = (double*) malloc (width * width * sizeof(double));
  double *b = (double*) malloc (width * width * sizeof(double));
  double *c = (double*) malloc (width * width * sizeof(double));

  #pragma omp parallel for collapse(2)
  for(int i = 0; i < width; i++) {
    for(int j = 0; j < width; j++) {
      a[i*width+j] = i;
      b[i*width+j] = j;
      c[i*width+j] = 0;
    }
  }

  mm(a,b,c,width);

  //for(int i = 0; i < width; i++) {
  //  for(int j = 0; j < width; j++) {
  //    printf("\n c[%d][%d] = %f",i,j,c[i*width+j]);
  //  }
  //}
}
