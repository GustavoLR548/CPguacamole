#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define TRUE 1
#define FALSE 0
typedef uint8_t bool;

#define MAX_N 100000
#define PIPE_MSG 0  
#define END_MSG 1  

int size;  
int n;     
int rank;  

void reduce() {
	int i, subTotal = 0;
	int primeCount = 3;
	MPI_Status Status;

	for (int i = 2; i < size; i++) {
		MPI_Recv(&subTotal, 1, MPI_INT, i, MPI_ANY_TAG, MPI_COMM_WORLD, &Status);

		primeCount += subTotal;
	}

   printf("number of primes = %d\n", primeCount);
}

void removeMultipleOf3() {  
   int number, i;

   for (i = 1; i <= n/2; i++)  {
      number = 2 * i + 1;
      if (number > n) break;
      if (number % 3 > 0)
         MPI_Send(&number,1,MPI_INT,1,PIPE_MSG,MPI_COMM_WORLD);
   }
   MPI_Send(&number,1,MPI_INT,1,END_MSG,MPI_COMM_WORLD);
}

void removeMultipleOf5() {  	
   int number,i = 0;
   MPI_Status Status; 
	
   while (TRUE)  {
     MPI_Recv(&number,1,MPI_INT,0,MPI_ANY_TAG,MPI_COMM_WORLD,&Status);
     if (Status.MPI_TAG == END_MSG) break;
     if (number % 5 > 0) {
        int nextProcessRank = (i % (size - 2)) + 2;
        i++;

        MPI_Send(&number, 1, MPI_INT, nextProcessRank, PIPE_MSG, MPI_COMM_WORLD);
     }
   }
   for (i = 2; i < size; i++) 
    MPI_Send(&number, 1, MPI_INT, i, END_MSG, MPI_COMM_WORLD);

}

void countOnlyPrimes() {

   int number, primeCount, i;
   bool isComposite;
   MPI_Status Status; 

   primeCount = 0;  

   while (TRUE)  {
      MPI_Recv(&number,1,MPI_INT,1,MPI_ANY_TAG,MPI_COMM_WORLD,&Status);
      if (Status.MPI_TAG == END_MSG) break;

      isComposite = FALSE;
      for (i = 7; i*i <= number; i += 2)
        if (number % i == 0)  {
	      isComposite = TRUE;
	      break;
	    }
      
	  if (!isComposite) primeCount++;  
   }

   MPI_Send(&primeCount, 1, MPI_INT, 0, PIPE_MSG, MPI_COMM_WORLD);
}

void main(int argc, char **argv) {  

   n = atoi(argv[1]); 
   MPI_Init(&argc,&argv);	

   MPI_Comm_size(MPI_COMM_WORLD,&size);
   MPI_Comm_rank(MPI_COMM_WORLD,&rank); 

   switch (rank)  {
      case 0:  removeMultipleOf3();
      reduce();
	  break;
      
	  case 1:  removeMultipleOf5();
      break;
      
	  default:  countOnlyPrimes();
      break;
   };

   MPI_Finalize();
}