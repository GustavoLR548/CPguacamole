#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define N 10
#define MAX 100

void main(int argc, char* argv[]) {
    int p, rank, maior_parcial, maior_final, numProcs;
    int buffer[N];
    MPI_Status status;

    MPI_Init(&argc, &argv) ;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &numProcs);

    if (rank == 0) {
        srand(time(NULL));
        for (int i = 0; i < N; i ++) 
            buffer[i] = rand() % MAX;        
    } 

    MPI_Bcast(buffer, N, MPI_INT, 0, MPI_COMM_WORLD);
    
    maior_parcial = maior_final = 0;
    for (int i = rank; i < N ; i+= numProcs) {
        if (buffer[i] > maior_parcial)
            maior_parcial = buffer[i];
    }

    MPI_Reduce(&maior_parcial, &maior_final, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("Valores no array: \n");
        for(int i = 0; i < N; i++) 
            printf("%d, ", buffer[i]);
        printf("\n");

        printf("\nMaior elemento: %d\n", maior_final);
    }
  
    MPI_Finalize();
}