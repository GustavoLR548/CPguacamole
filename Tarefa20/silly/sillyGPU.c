/*
Tempo sequencial

real    0m9,743s
user    0m9,674s
sys     0m0,030s

real    0m9,695s
user    0m9,673s
sys     0m0,004s

real    0m9,692s
user    0m9,669s
sys     0m0,011s

real    0m9,649s
user    0m9,631s
sys     0m0,004s

real    0m9,694s
user    0m9,682s
sys     0m0,000s

Tempo paralelo - multicore

real    0m2,720s
user    0m15,143s
sys     0m0,056s

real    0m2,822s
user    0m15,280s
sys     0m0,040s

real    0m2,624s
user    0m15,217s
sys     0m0,044s

real    0m2,800s
user    0m15,143s
sys     0m0,056s

real    0m2,680s
user    0m15,143s
sys     0m0,056s

Tempo paralelo - GPU

real    0m2,360s
user    0m14,952s
sys     0m0,033s

real    0m2,465s
user    0m14,981s
sys     0m0,037s

real    0m2,300s
user    0m15,118s
sys     0m0,046s

real    0m2,279s
user    0m15,029s
sys     0m0,030s

real    0m2,381s
user    0m15,603s
sys     0m0,091s

*/

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
 
int main() 
{

   int i, j, n = 100000; 
 
   // Allocate input, output and position arrays
   int *in = (int*) calloc(n, sizeof(int));
   int *pos = (int*) calloc(n, sizeof(int));   
   int *out = (int*) calloc(n, sizeof(int));   
 
   #pragma omp target map(from:in[0:n])
   #pragma omp teams distribute parallel for simd
   for(i=0; i < n; i++)
      in[i] = n-i;  
 
   // Print input array
   //   for(i=0; i < n; i++) 
   //      printf("%d ",in[i]);

   #pragma omp parallel for collapse(2) schedule(guided)
   for(i=0; i < n; i++) 
      for(j=0; j < n; j++)
	     if(in[i] > in[j]) 
            pos[i]++;	
 
   #pragma omp target map(from:in[0:n],pos[0:n]) map(to:out[0:n])
   #pragma omp teams distribute parallel for simd
   for(i=0; i < n; i++) 
      out[pos[i]] = in[i];
 
   // print output array
   //   for(i=0; i < n; i++) 
   //      printf("%d ",out[i]);
 
   #pragma omp parallel for schedule(guided)  
   for(i=0; i < n; i++)
      if(i+1 != out[i]) 
      {
         printf("test failed\n");
         exit(0);
      }
 
   printf("test passed\n"); 
} 