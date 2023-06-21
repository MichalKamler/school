#include <stdio.h>
#include <stdlib.h>

enum{
  ERROR_OK = EXIT_SUCCESS,
  ERROR_WRONG_INPUT = 100, 
};

static const char*E_WRONG_INPUT = "Error: Chybny vstup!";
#define CAL_UP_TO 1000000
#define MAX_LEN_NUM 101

void calculatePrimes(int *primesArray); //finds all primes up to CAL_UP_TO
int readInput(int myNumArr[]);
void printArr(int arr[]);
void primeDecomposition(int *myNumArr);
void reportError(int error);

int main(int argc, char *argv[]){
  int ret = ERROR_OK;
  int primesArray[CAL_UP_TO];
  calculatePrimes(primesArray);
  int myNumArr[MAX_LEN_NUM];
  myNumArr[0] = 1;
  
  while(ret == ERROR_OK && myNumArr[0] != 0){
    printf("jsem tu\n");
    ret = readInput(myNumArr);
    if(ret == ERROR_OK && myNumArr[0] != 0){
      primeDecomposition(myNumArr);
    }
  }
  if(ret != ERROR_OK){
    reportError(ret);
  }  
  return ret;
}

void primeDecomposition(int *myNumArr){
  printf("Prvociselny rozklad cisla ");
  printArr(myNumArr);
  printf(" je:\n");
  // int howManyTimes = 0;
  // int devisionSuc = 0;
  // int curPrime = 1;
  // if(strcmp(numStr, "1")==0){
  //   printf("1\n");
  // }else{
  //   while(1){
  //     curPrime += 1;
  //     devisionSuc = divide(&numStr, numStr, curPrime);
  //     if(devisionSuc==1){
  //       howManyTimes = 0;
  //       while(devisionSuc==1){
  //         howManyTimes += 1;
  //         devisionSuc = divide(&numStr, numStr, curPrime);
  //       }
  //       printf("%d", curPrime);
  //       if(howManyTimes>1){
  //         printf("^%d", howManyTimes);
  //       }
  //       if(strcmp(numStr, "1")==0){
  //         printf("\n");
  //         break;
  //       }else{
  //         printf(" x ");
  //       }
  //     }
  //   }
  // }
}

void printArr(int arr[]){
  unsigned int i = 0;
  while(1){
    if(arr[i]=='\0'){
      break;
    }
    printf("%d", arr[i++]);
  }
}

int readInput(int myNumArr[]){
  int ret = ERROR_OK;
  char c;
  for(unsigned int i=0; i<MAX_LEN_NUM; i++){
    if((scanf("%c", &c))!=1 && c!='\n'){
      printf("chyba smutek %d\n", c);
      ret = ERROR_WRONG_INPUT;
      return ret;
    }
    if(c=='\n'){
      myNumArr[i] = '\0';
      break;
    }
    myNumArr[i] = c - '0';
  }
  return ret;
}

//calculates Primes and puts them into array - [0, 0, 1, 1, 0] - if 1 then index is prime
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
void reportError(int error){  
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
