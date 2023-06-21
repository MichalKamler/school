#include <stdio.h>
#include <stdlib.h>
#include <string.h>


enum{
  ERROR_OK = EXIT_SUCCESS,
  ERROR_WRONG_INPUT = 100, 
};

static const char*E_WRONG_INPUT = "Error: Chybny vstup!";
#define MIN_VALUE 0
#define CAL_UP_TO 1000000

void reportError(int error);
void calculatePrimes(int *primesArray); //finds all primes up to CAL_UP_TO
int readInput(int *numArray[101]);
int checkIfNum(int ret, char curNumAsChar);

int main(int argc, char *argv[]){
  int ret = ERROR_OK;
  int primesArray[CAL_UP_TO];
  int numArray[101];

  calculatePrimes(primesArray);

  ret = readInput(&numArray[101]);
  for(int i = 0; i<5; i++){
    printf("%d\n", numArray[i]);
  }
  
  return ret;
}

int readInput(int *numArray[101]){
  int ret = ERROR_OK;
  int r;
  int i = 0;
  char curNumAsChar;
  while(1){
    r = scanf("%c", &curNumAsChar);
    ret = checkIfNum(ret, curNumAsChar);
    if(r==1 && ret==ERROR_OK){
      *numArray[i++] = atoi(&curNumAsChar);
    }else if(r==EOF){
      break;
    }else{
      ret = ERROR_WRONG_INPUT;
      break;
    }
  }
  return ret;
}

int checkIfNum(int ret, char curNumAsChar){ //checks if the string is really a number
  char numArr[] = "0123456789"; 
  int numIsNum = 0;
  for(int i=0; i<10; ++i){
    if(curNumAsChar==numArr[i]){
      numIsNum = 1;
      break;
    }
  }
  if(numIsNum != 1){
    ret = ERROR_WRONG_INPUT;
  }
  return ret;
}

void calculatePrimes(int *primesArray){
  int i, curNum, multiplicants;
  for(i = 0; i < CAL_UP_TO;  ++i){
    *(primesArray+i) = 1;
  }
  *primesArray = *(primesArray+1) = 0; 
  for(curNum = 2; curNum*curNum < CAL_UP_TO; ++curNum){   //till sqrt(numOfPrimes)
    if(*(primesArray+curNum)){
      //excludes multiplicants of curNum, bcs curNum is prime
      for(multiplicants = curNum*curNum; multiplicants <= CAL_UP_TO; multiplicants += curNum){
        *(primesArray+multiplicants) = 0;
      }
    }
  }
}

void reportError(int error){  //reports error
  const char *e = NULL;
  switch(error){
    case ERROR_WRONG_INPUT:
      e=E_WRONG_INPUT;
      break;
  }
  if(e){
    fprintf(stderr, "%s\n", e);
  }
}
