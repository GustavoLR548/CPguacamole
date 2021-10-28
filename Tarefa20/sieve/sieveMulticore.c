/*

Tempos de execução:

Versão sequencial:

real	0m3.005s
user	0m2.919s
sys	0m0.080s

real	0m3.006s
user	0m2.930s
sys	0m0.068s

real	0m3.028s
user	0m2.951s
sys	0m0.064s

real	0m3.007s
user	0m2.926s
sys	0m0.076s

real	0m3.006s
user	0m2.920s
sys	0m0.080s

Versão paralela multicore:

real	0m2.579s
user	0m9.820s
sys	0m0.080s

real	0m2.573s
user	0m9.845s
sys	0m0.068s

real	0m2.579s
user	0m9.838s
sys	0m0.080s

real	0m2.576s
user	0m9.830s
sys	0m0.080s

real	0m2.580s
user	0m9.840s
sys	0m0.084s

Versão paralela para GPU

*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <omp.h>
     
int sieveOfEratosthenes(int n)
{
   // Create a boolean array "prime[0..n]" and initialize
   // all entries it as true. A value in prime[i] will
   // finally be false if i is Not a prime, else true.
   int primes = 0; 
   bool *prime = (bool*) malloc((n+1)*sizeof(bool));
   int sqrt_n = sqrt(n);
     
   memset(prime, true,(n+1)*sizeof(bool));
   
   int i, p;
   
   #pragma omp parallel for schedule (guided)
   for (p=2; p <= sqrt_n; p++)
   {
       // If prime[p] is not changed, then it is a prime
       if (prime[p] == true)
       {
         // Update all multiples of p
   		#pragma omp parallel for 
		   for(i=p*2; i<=n; i += p)
	           prime[i] = false;
       }
   }
   
   // count prime numbers
	#pragma omp parallel for reduction(+:primes)   
	for (int p=2; p<=n; p++)
       if (prime[p])
         primes++;
 
      return(primes);
   }
     
int main()
{
   int n = 100000000;
   double start; 
   double end; 
   start = omp_get_wtime(); 
   printf("%d\n",sieveOfEratosthenes(n));
    end = omp_get_wtime(); 
    printf("Total time: %f seconds\n", end - start);
   return 0;
} 
