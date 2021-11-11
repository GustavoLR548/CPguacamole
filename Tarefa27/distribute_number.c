#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

void main(int argc, char* argv[]) {
  int i, rank, val, numProcs;
  MPI_Status status;

  MPI_Init(&argc, &argv) ;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &numProcs);
  if (rank == 0) {
    val = 51;

    MPI_Send(&val, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
    printf("Process %d sends a message to %d\n",rank,i);

  } else {

    printf("Processor %d receives a message from %d\n", rank,i);
    MPI_Recv(&val, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
    printf("Process %d receives a message from %d with value %d\n",rank,0,val);
  }
  MPI_Finalize();
}