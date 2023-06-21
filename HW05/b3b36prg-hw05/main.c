#include <stdio.h>
#include <stdlib.h>

enum{
  ERROR_OK = EXIT_SUCCESS,
  ERROR_INPUT = 100,
};

#define END_OF_INPUT -1

static const char*E_INPUT = "Error: Chybny vstup!";

void reportError(int error);
void printMatrix(int rows, int cols, int **matrix);
int readMatrix(int rows, int cols, int **matrix);
int readOperation(char *operation);
void addSubMatrices(int rows, int cols, int **mat1, int **mat2, int **result, char operation);
void multiplyMatrices(int m, int n, int p, int **mat1, int **mat2, int **result);
int **performOperation(int *ret, int rows1, int cols1, int **m1, int rows2, int cols2, int **m2, char operation);
void deleteMatrix(int** matrix, int rows);
int **createMatrix(int rows, int cols);
int goThroughAllMatrices();
int **loadDimAndMat();
int readDim(int **r, int **c);
int readMatrixUpdated(int rows, int cols, int **matrix);

int main(int argc, char *argv[]){
  int ret = goThroughAllMatrices();
  reportError(ret);
  return ret;
}

//combines two readInputs - readDim and readMatrix(also creates matrix for reading)
int **loadDimAndMat(int *ret, int *row, int *col){
  if(!readDim(&row, &col)){
    *ret = ERROR_INPUT;
  }
  int  **m = createMatrix(*row, *col); 
  if(*ret==ERROR_OK && !readMatrix(*row, *col, m)){
    deleteMatrix(m, *row);
    *ret = ERROR_INPUT;
  }
  return m;
}
//goes through all the matrices that can scanf
int goThroughAllMatrices(){
  int ret = ERROR_OK;
  int row1, row2, row3, col1, col2, col3, repeat;
  row1 = row2 = row3 = col1 = col2 = col3 = repeat = 1;
  char operation1, operation2;
  int **m1 = loadDimAndMat(&ret, &row1, &col1);
  if(ret!=ERROR_OK){
    return ret;
  }
  if(ret==ERROR_OK && !readOperation(&operation1)){
    ret = ERROR_INPUT;
    return ret;
  }
  int **m2 = loadDimAndMat(&ret, &row2, &col2);
  if(ret!=ERROR_OK){
    return ret;
  }
  int **m3;
  int check = 1;
  while(repeat!=END_OF_INPUT){
    if(ret!=ERROR_OK){
      break;
    }
    if(!(check=readOperation(&operation2))){
      ret = ERROR_INPUT;
      return ret;
    }
    if(check==END_OF_INPUT && ret==ERROR_OK){//EOF
      m1 = performOperation(&ret, row1, col1, m1, row2, col2, m2, operation1);
      if(operation1=='*'){
        col1 = col2;
      }
      if(ret==ERROR_OK){
        printMatrix(row1, col1, m1);
      }else{
        return ret;
      }
      deleteMatrix(m1, row1);        
      repeat = END_OF_INPUT;
      continue;
    }
    if(operation1=='*' && ret==ERROR_OK){
      m1 = performOperation(&ret, row1, col1, m1, row2, col2, m2, operation1);
      col1 = col2;
      operation1 = operation2;
      m2 = loadDimAndMat(&ret, &row2, &col2);
      continue;
    }
    if((operation1=='+'|| operation1=='-') && operation2=='*' && ret==ERROR_OK){
      m3 = loadDimAndMat(&ret, &row3, &col3);
      m2 = performOperation(&ret, row2, col2, m2, row3, col3, m3, operation2);
      col2 = col3;
    }
    if((operation1=='+' || operation1=='-') && (operation2=='+' || operation2=='-') && ret==ERROR_OK){
      m1 = performOperation(&ret, row1, col1, m1, row2, col2, m2, operation1);
      m2 = loadDimAndMat(&ret, &row2, &col2);
      operation1 = operation2;
    }    
  }
  return ret;
}
//performs operation on 2 matrices
int **performOperation(int *ret, int rows1, int cols1, int **m1, int rows2, int cols2, int **m2, char operation){
  if(operation=='-' || operation=='+'){
    if(rows1==rows2 && cols1==cols2){
      int **result = createMatrix(rows1, cols1);
      addSubMatrices(rows1, cols2, m1, m2, result, operation);
      deleteMatrix(m1, rows1);
      deleteMatrix(m2, rows2);
      return result;
    }else{
      *ret = ERROR_INPUT;
      deleteMatrix(m1, rows1);
      deleteMatrix(m2, rows2);      
    }
  }
  if(operation=='*'){
    if(cols1!=rows2){
      *ret = ERROR_INPUT;
      deleteMatrix(m1, rows1);
      deleteMatrix(m2, rows2);
    }else{
      int **result = createMatrix(rows1, cols2);
      multiplyMatrices(rows1, cols1, cols2, m1, m2, result);
      deleteMatrix(m1, rows1);
      deleteMatrix(m2, rows2);
      return result;
    }
  }
  return NULL;
}
//allocates memory for matrix
int **createMatrix(int rows, int cols){
  int **matrix = malloc(rows * sizeof(int*));
  if (!matrix) {
    return NULL;
  }
  for(unsigned int i = 0; i < rows; i++) {
    matrix[i] = malloc(cols * sizeof(int));
    if (!matrix[i]) {
      for(unsigned int j = 0; j < i; j++) {
        free(matrix[j]);
      }
      free(matrix);
      return NULL;
    }
  }
  return matrix;
}
//delets matrix from memory
void deleteMatrix(int** matrix, int rows){
  for(unsigned int i = 0; i < rows; i++) {
    free(matrix[i]);
  }
  free(matrix);
}
//multiplys 2 matrices also checks if it is possible
void multiplyMatrices(int m, int n, int p, int **mat1, int **mat2, int **result){
  for (int i = 0; i < m; i++) {
    for (int j = 0; j < p; j++) {
      int sum = 0;
      for (int k = 0; k < n; k++) {
        sum += mat1[i][k] * mat2[k][j];
      }
      result[i][j] = sum;
    }
  }
}
//sums or substracts two matrices also checks if the dimension are alright
void addSubMatrices(int rows, int cols, int **mat1, int **mat2, int **result, char operation){
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      if(operation=='+'){
        result[i][j] = mat1[i][j] + mat2[i][j];
      }else if(operation=='-'){
        result[i][j] = mat1[i][j] - mat2[i][j];
      }            
    }
  }
}
//reads +-*
int readOperation(char *operation){
  char newline;  
  int what = scanf("%c", &newline);
  if(newline==13){
    what = scanf("%c", &newline);
  }
  if(newline=='\n'){
    what = scanf("%c", operation);
    if(what==EOF){
      return -1;
    }
  }
  return 1;
}
//reads whole matrix and stores it into preallocated matrix
int readMatrix(int rows, int cols, int **matrix){
  for(unsigned int r=0; r<rows; r++){
    for(unsigned int c=0; c<cols; c++){
      if(scanf("%d", &matrix[r][c])!=1){        
        return 0;
      }
    }
  }
  return 1;
}
//reads dim of matrix
int readDim(int **r, int **c){
  return scanf("%d %d", *r, *c)==2;
}
//prints the matrix
void printMatrix(int rows, int cols, int **matrix){
  printf("%d %d\n", rows, cols);
  for(unsigned int r=0; r<rows; r++){
    for(unsigned int c=0; c<cols; c++){
      if(c+1==cols){
        printf("%d", matrix[r][c]);
      }else{
        printf("%d ", matrix[r][c]);
      }      
    }
    printf("\n");
  }
}
//reports error
void reportError(int error){
  const char *e = NULL;
  switch(error){
    case ERROR_INPUT:
      e=E_INPUT;
      break;
  }
  if(e){
    fprintf(stderr, "%s\n", e);
  }
}
