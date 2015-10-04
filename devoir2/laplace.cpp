#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <chrono>
#include <random>
#include <algorithm>


static const int MAT_SIZE = 10;

static const double MAT_BORD = -1.0;
int master(MPI_Comm);
int slave(MPI_Comm);
int done = 2;

namespace rdm{
  std::mt19937 rng;
  void init(){
    rng.seed((unsigned int)time(NULL));
  }
  std::uniform_int_distribution<int> int_dist(1,100);
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

  if(proc != MAT_SIZE+1)
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
  
  double mat1[MAT_SIZE+2][MAT_SIZE+2];
  double res[MAT_SIZE+2][MAT_SIZE+2];
  
  // initialisation des matrices
  for(i=0; i<MAT_SIZE+2; ++i){
    for(j=0; j<MAT_SIZE+2; ++j){
      mat1[i][j] = rdm::int_dist(rdm::rng)*1.0;
      res[i][j] = 0;
    }
  }
  for(i=0; i<MAT_SIZE+2; ++i){
    mat1[i][0] = MAT_BORD;
    mat1[0][i] = MAT_BORD;
    mat1[MAT_SIZE+1][i] = MAT_BORD;
    mat1[i][MAT_SIZE+1] = MAT_BORD;
  }
  if(MAT_SIZE <= 10){
    printf("Mat1\n");
    printf("[\n");
    for(i=0; i<MAT_SIZE+2; ++i){
      printf("[");
      for(j=0; j<MAT_SIZE+2; ++j){
        printf("%2.2f,", mat1[i][j]);
      }
      printf("]\n");
    }
    printf("]\n");
  }
  for(;;){
    for(i=1; i<=MAT_SIZE; ++i){
      MPI::COMM_WORLD.Send(&(mat1[i-1]), MAT_SIZE+2, MPI::DOUBLE, i, 0);
      MPI::COMM_WORLD.Send(&(mat1[i][0]), MAT_SIZE+2, MPI::DOUBLE, i, 0);
      MPI::COMM_WORLD.Send(&(mat1[i+1][0]), MAT_SIZE+2, MPI::DOUBLE, i, 0);
    }
    double ttreshold = 100000000.0;
    double resi[MAT_SIZE];
    for(i=1; i<=MAT_SIZE; ++i){
      MPI::COMM_WORLD.Recv(&(resi[0]), MAT_SIZE, MPI::DOUBLE, i, 0);
      for(j=0;j<MAT_SIZE;++j){
        //        res[i][j+1] = resi[j];
        mat1[i][j+1] = resi[j];
        ttreshold = std::min<double>(resi[j] - MAT_BORD, ttreshold);
      }
    }
    if(MAT_SIZE <= 10){
      printf("treshold %2.6f.\n", ttreshold);
    }
    if(ttreshold <= 0.01){
      for(i=1; i<=MAT_SIZE; ++i){
        double d = 1.0;
        MPI::COMM_WORLD.Send(&d, 1, MPI::DOUBLE, i, 0);
      }
      break;
    }
  }
  printf("Res\n");
  printf("[\n");
  for(i=1; i<=MAT_SIZE; ++i){
    printf("[");
    for(j=1; j<=MAT_SIZE; ++j){
      printf("%2.8f, ", mat1[i][j]);
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
  double v1[MAT_SIZE+2], v2[MAT_SIZE+2], v3[MAT_SIZE+2];
  double res[MAT_SIZE];
  MPI::Status status;
  MPI_Comm_rank( comm, &id );
  for(;;){
    MPI::COMM_WORLD.Recv(v1, MAT_SIZE+2, MPI::DOUBLE, 0, 0, status);
    if(status.Get_count(MPI::INT) == done){
      break;
    }
    MPI::COMM_WORLD.Recv(v2, MAT_SIZE+2, MPI::DOUBLE, 0, 0);
    MPI::COMM_WORLD.Recv(v3, MAT_SIZE+2, MPI::DOUBLE, 0, 0);
    for(i=1; i<=MAT_SIZE; ++i){
      //    printf("%d, %d res : %2.5f\n", id+1, i, (v1[i]+v2[i-1]+v2[i+1]+v3[i])/4.0);
      res[i-1] = (v1[i]+v2[i-1]+v2[i+1]+v3[i])/4.0;
    }
    // printf("pid %d | Debug res : \n", id);
    // for(i=0; i<MAT_SIZE; ++i){
    //   printf("res pid %d | %2.2f, ", id, res[i]);
    // }
    // printf("\n");
    MPI::COMM_WORLD.Send(&res, MAT_SIZE, MPI::DOUBLE, 0, 0);
  }
  return 0;
}
