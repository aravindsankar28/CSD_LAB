#include <iostream>
#include <stdio.h>
#include <string>
#include <cstdlib>
#include <time.h>

using namespace std;

int** square_matrix_multiply( int **a, int **b, int n){
  int** c = (int**) malloc(n*sizeof(int*));
  for(int i = 0; i < n; i++){
    c[i] = (int*) malloc(n*sizeof(int));
  }
  
  for(int i = 0; i < n; i ++){
    for(int j = 0; j < n; j ++){
      c[i][j] = 0;
      for(int k = 0; k < n; k ++){
	c[i][j] += a[i][k] + b[k][j];
      }
    }
  }
  
  return c;
}

int main( int argc, char** argv){
  if(argc != 2){
    cout<<"Invalid command. \n Command format : "<<argv[0]<< " <N>";
    return -1;
  }
  
  //read n
  int n = atoi(argv[1]);
  
  //Initialization like array of int* or as a single array?
  int **matrix1 = (int**) malloc(n*sizeof(int*));
  int **matrix2 = (int**) malloc(n*sizeof(int*));
    
  for(int i = 0; i < n; i++){
    matrix1[i] = (int*) malloc(n*sizeof(int));
    matrix2[i] = (int*) malloc(n*sizeof(int));
  }
  
  //reseed random number generator
  srand( time(NULL));
  
  //Populate matrices with random ints
  for(int i = 0; i < n; i++){
    for(int j = 0; j < n; j++){
      matrix1[i][j] = rand();
      matrix2[i][j] = rand();
    }
  }
  
  //call square_matrix_multiply
  int** prod = square_matrix_multiply(matrix1, matrix2, n);
  
/*
 *Commenting out this poart coz this printing will fuck up the output
 *We need only output of the pin tool in stdout.
 *Since we must not read from a file, we will be reading from stdin to do our processing
 *
 *  
  //print out product matrix
  //is this required?
  for(int i = 0; i < n; i++){
    for(int j = 0; j < n; j++){
      cout << prod[i][j] << "  ";
    }
    cout<<endl;
  }
*/  
}
 