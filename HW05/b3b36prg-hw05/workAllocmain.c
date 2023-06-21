#include <stdio.h>
#include <stdlib.h>

enum{
  ERROR_OK = EXIT_SUCCESS,
  ERROR_INPUT = 100,
};

static const char*E_INPUT = "Error: Chybny vstup!";

void reportError(int error);
void printMatrix(int rows, int cols, int **matrix);
int readDim(int *r, int *c);
int readMatrix(int rows, int cols, int **matrix);
int readOperation(char *operation);
void addSubMatrices(int rows, int cols, int **mat1, int **mat2, int **result, char operation);
void multiplyMatrices(int m, int n, int p, int **mat1, int **mat2, int **result);
void deleteMatrix(int** matrix, int rows);
int **createMatrix(int rows, int cols);

int main(int argc, char *argv[]){
  int ret = ERROR_OK;
  int rows1, cols1, rows2, cols2;
  rows1 = cols1 = rows2 = cols2= 1;
  char operation;
  if(!readDim(&rows1, &cols1)){
    ret = ERROR_INPUT;
  }
  int  **m1 = createMatrix(rows1, cols1); 
  if(ret==ERROR_OK && !readMatrix(rows1, cols1, m1)){
    ret = ERROR_INPUT;
  }
  if(ret==ERROR_OK && !readOperation(&operation)){
    ret = ERROR_INPUT;
  }
  if(ret==ERROR_OK && !readDim(&rows2, &cols2)){
    ret = ERROR_INPUT;
  }
  int **m2 = createMatrix(rows2, cols2);
  if(ret==ERROR_OK && !readMatrix(rows2, cols2, m2)){
    ret = ERROR_INPUT;
  }
  if(ret==ERROR_OK){
    if(operation=='-' || operation=='+'){
      if(rows1==rows2 && cols1==cols2){
        int **result = createMatrix(rows1, cols1);
        addSubMatrices(rows1, cols2, m1, m2, result, operation);
        printMatrix(rows1, cols1, result);
        deleteMatrix(m1, rows1);
        deleteMatrix(m2, rows2);
      }else{
        ret = ERROR_INPUT;
      }
    }
    if(operation=='*'){
      if(cols1!=rows2){
        ret = ERROR_INPUT;
      }else{
        int **result = createMatrix(rows1, cols2);
        multiplyMatrices(rows1, cols1, cols2, m1, m2, result);
        printMatrix(rows1, cols2, result);
        deleteMatrix(m1, rows1);
        deleteMatrix(m2, rows2);
      }
    }
  }
  reportError(ret);
  return ret;
}

int** createMatrix(int rows, int cols){
  int** matrix = malloc(rows * sizeof(int*));
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

void deleteMatrix(int** matrix, int rows){
  for(unsigned int i = 0; i < rows; i++) {
    free(matrix[i]);
  }
  free(matrix);
}

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

int readOperation(char *operation){
  char newline;  
  if(scanf("%c", &newline)==1 && newline=='\n'){
    return scanf("%c", operation)==1;
  }else{
    *operation = newline;
  }
  return scanf("%c", operation)==1;
}

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

int readDim(int *r, int *c){
  return scanf("%d %d", r, c)==2;
}

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
