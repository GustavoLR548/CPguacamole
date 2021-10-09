/*

Tempo sequencial

real	0m4.492s
user	0m4.488s
sys	0m0.000s

real	0m4.405s
user	0m4.402s
sys	0m0.000s

real	0m4.431s
user	0m4.385s
sys	0m0.000s

real	0m4.651s
user	0m4.385s
sys	0m0.004s

real	0m4.616s
user	0m4.415s
sys	0m0.008s

Tempo paralelo com scheduler(static)

real	0m2.648s
user	0m4.920s
sys	0m0.000s

real	0m2.641s
user	0m4.909s
sys	0m0.000s

real	0m2.647s
user	0m4.917s
sys	0m0.004s

real	0m2.638s
user	0m4.895s
sys	0m0.000s

real	0m2.684s
user	0m4.900s
sys	0m0.000s

Tempo paralelo com scheduler(guided) (Melhor política)

test passed

real	0m2.637s
user	0m4.895s
sys	0m0.000s

real	0m2.632s
user	0m4.881s
sys	0m0.008s

real	0m2.643s
user	0m4.905s
sys	0m0.000s

real	0m2.639s
user	0m4.897s
sys	0m0.008s

real	0m2.643s
user	0m4.901s
sys	0m0.004s

Henrique, bom dia, tentamos testar no servidor da PUC usando a
política de scheduler dynamic, porém, o programa não terminava de rodar,
e quando testavamos em nossos computadores, o programa terminava com
"test failed"
*/

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
 
int main() 
{
   omp_set_num_threads(2);   

   int i, j, n = 30000; 
 
   // Allocate input, output and position arrays
   int *in = (int*) calloc(n, sizeof(int));
   int *pos = (int*) calloc(n, sizeof(int));   
   int *out = (int*) calloc(n, sizeof(int));   
 
   #pragma omp parallel for 
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
 
   #pragma omp parallel for 
   for(i=0; i < n; i++) 
      out[pos[i]] = in[i];
 
   // print output array
   //   for(i=0; i < n; i++) 
   //      printf("%d ",out[i]);
 
   #pragma omp parallel for  
   for(i=0; i < n; i++)
      if(i+1 != out[i]) 
      {
         printf("test failed\n");
         exit(0);
      }
 
   printf("test passed\n"); 
} 