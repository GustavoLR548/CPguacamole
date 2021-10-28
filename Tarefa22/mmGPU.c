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

*/
#include <stdio.h>
#include <stdlib.h>

void mm(double *a, double *b, double *c, int width)
{
#pragma omp target map(to                                                       \
                       : a [0:(width * width)], b [0:(width * width)]) map(from \
                                                                           : c [0:(width * width)])
#pragma omp teams distribute parallel for
  for (int i = 0; i < width; i++)
  {
    for (int j = 0; j < width; j++)
    {
      double sum = 0;
      for (int k = 0; k < width; k++)
      {
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
  int width = 2000;
  double *a = (double *)malloc(width * width * sizeof(double));
  double *b = (double *)malloc(width * width * sizeof(double));
  double *c = (double *)malloc(width * width * sizeof(double));

#pragma omp parallel for collapse(2)
  for (int i = 0; i < width; i++)
  {
    for (int j = 0; j < width; j++)
    {
      a[i * width + j] = i;
      b[i * width + j] = j;
      c[i * width + j] = 0;
    }
  }

  mm(a, b, c, width);

    
  
}