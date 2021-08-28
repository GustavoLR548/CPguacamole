/*
* Tempo Sequencial
*
*real	0m4.047s
*user	0m3.960s
*sys	0m0.080s
*
*real	0m4.053s
*user	0m3.967s
*sys	0m0.080s
*
*real	0m4.042s
*user	0m3.962s
*sys	0m0.072s
*
*real	0m4.044s
*user	0m3.953s
*sys	0m0.084s
*
*real	0m4.045s
*user	0m3.967s
*sys	0m0.072s
*
* Tempo paralelo (Atividade 03)
*
*real	0m3.700s
*user	0m6.857s
*sys	0m0.104s
*
*real	0m3.661s
*user	0m6.813s
*sys	0m0.072s
*
*real	0m3.797s
*user	0m6.955s
*sys	0m0.096s 
*
*real	0m3.647s
*user	0m6.817s
*sys	0m0.076s
*
*real	0m3.890s
*user	0m7.107s
*sys	0m0.088s
*
* Tempo paralelo(Atividade 07)
*
*real	0m2.682s
*user	0m10.331s
*sys	0m0.080s
*
*real	0m2.684s
*user	0m10.331s
*sys	0m0.080s
*
*real	0m2.689s
*user	0m10.309s
*sys	0m0.083s
*
*real	0m2.659s
*user	0m9.956s
*sys	0m0.103s
*
*real	0m2.698s
*user	0m10.333s
*sys	0m0.084s
*/

     
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
     
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
   
   #pragma omp parallel for schedule (dynamic,100)
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
   printf("%d\n",sieveOfEratosthenes(n));
   return 0;
} 
