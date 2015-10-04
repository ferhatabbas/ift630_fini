#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <chrono>
#include <random>


static const int MAT_SIZE = 3;

int master(MPI_Comm);
int slave(MPI_Comm);

namespace rdm{
  std::mt19937 rng;
  void init(){
    rng.seed((unsigned int)time(NULL));
  }
  std::uniform_int_distribution<int> int_dist(1,10);
}

int main(int argc, char *argv[])
{
  int id, proc;
  MPI_Comm ncomm;
  rdm::init();
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &proc);
  MPI_Comm_rank(MPI_COMM_WORLD, &id);
  MPI_Comm_split( MPI_COMM_WORLD, id == 0, 0, &ncomm );

  if(proc != (MAT_SIZE*MAT_SIZE)+1)
    MPI_Abort(MPI_COMM_WORLD, 1);

  
  if(id == 0)
    master(ncomm);
  else
    slave(ncomm);

  MPI_Finalize();
  
  return 0;
}

int master(MPI_Comm comm)
    //    MPI_Comm comm;
{
  int tag=1;
  int i,j;
  
  int mat1[MAT_SIZE][MAT_SIZE];
  int mat2[MAT_SIZE][MAT_SIZE];
  int res[MAT_SIZE][MAT_SIZE];
  
  // initialisation des matrices
  for(i=0; i<MAT_SIZE; ++i){
    for(j=0; j<MAT_SIZE; ++j){
      mat1[i][j] = rdm::int_dist(rdm::rng);
      mat2[i][j] = rdm::int_dist(rdm::rng);
      res[i][j] = 0;
    }
  }
  printf("Mat1\n");
  printf("[");
  for(i=0; i<MAT_SIZE; ++i){
    printf("[");
    for(j=0; j<MAT_SIZE; ++j){
      printf("%2d,", mat1[i][j]);
      //      std::cout << mat1[i][j] << ((j+1<MAT_SIZE)?"\t,":"");
    }
    printf("]\n");
  }
  printf("]\n");
  printf("Mat2.T\n");
  printf("[");
  for(i=0; i<MAT_SIZE; ++i){
    printf("[");
    for(j=0; j<MAT_SIZE; ++j){
      printf("%2d,", mat2[i][j]);
      //      std::cout << mat2[i][j] << ((j+1<MAT_SIZE)?"\t,":"");
    }
    printf("]\n");
  }
  printf("]\n");
  int test = 0;
  for(i=0; i<MAT_SIZE; ++i){
    for(j=0; j<MAT_SIZE; ++j){
      MPI_Send(&(mat1[i][0]), MAT_SIZE, MPI_INT, ((MAT_SIZE*j+i+1)), 0, MPI_COMM_WORLD);
      MPI_Send(&(mat2[j][0]), MAT_SIZE, MPI_INT, ((MAT_SIZE*j+i+1)), 0, MPI_COMM_WORLD);
    }
  }
  for(i=0; i<MAT_SIZE; ++i){
    for(j=0; j<MAT_SIZE; ++j){
      MPI_Recv(&res[i][j], 1, MPI_INT, ((MAT_SIZE*j+i+1)), 0,
               MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }
  }
  printf("Res\n");
  printf("[");
  for(i=0; i<MAT_SIZE; ++i){
    printf("[");
    for(j=0; j<MAT_SIZE; ++j){
      printf("%2d,", res[i][j]);
      //      std::cout << res[i][j] << ((j+1<MAT_SIZE)?"\t,":"");
    }
    printf("]\n");
  }
  printf("]\n");
}

int slave(MPI_Comm comm)
    //    MPI_Comm comm;
{
  int i, id;
  int mat1[MAT_SIZE], mat2[MAT_SIZE];
  int res = 0;
  MPI_Comm_rank( comm, &id );
  MPI_Recv(mat1, MAT_SIZE, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
  MPI_Recv(mat2, MAT_SIZE, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
  for(i=0; i<MAT_SIZE; ++i){
    res += mat1[i]*mat2[i];
  }
  MPI_Send(&res, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
  return 0;
}
