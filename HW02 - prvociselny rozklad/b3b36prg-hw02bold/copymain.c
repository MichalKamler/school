#include <stdio.h>
#include <stdlib.h>

enum{
  ERROR_OK = EXIT_SUCCESS,
  ERROR_WRONG_INPUT = 100, 
};

static const char*E_WRONG_INPUT = "Error: Chybny vstup!";
#define MIN_VALUE 0
#define CAL_UP_TO 1000000

void reportError(int error);
int readInput(long long int *num);
int howManyPrimes();
void calculatePrimes(int *primesArray); //finds all primes up to CAL_UP_TO
void primeDecomposition(long long int num, int *primesArray);

int main(int argc, char *argv[]){
  int ret = ERROR_OK;
  long long int num;
  int primesArray[CAL_UP_TO];

  calculatePrimes(primesArray);

  num = 1;
  while(ret == ERROR_OK && num != 0){
    ret = readInput(&num);
    if(ret == ERROR_OK && num != 0){
      primeDecomposition(num, primesArray);
    }
  }
  if(ret != ERROR_OK){
    reportError(ret);
  }
  
  return ret;
}

void primeDecomposition(long long int num, int *primesArray){
  printf("Prvociselny rozklad cisla %lld je:\n", num);
  int curPrime = 1;
  int howManyTimes;
  if(num == 1){
    printf("1\n");
  }else{
    while(1){
      curPrime += 1;
      if(*(primesArray+curPrime) && num%curPrime == 0){  //if num is prime and devides
        howManyTimes = 0;
        while(num%curPrime == 0){
          howManyTimes += 1;
          num /= curPrime;
        }
        printf("%d", curPrime);
        if(howManyTimes>1){
          printf("^%d", howManyTimes);
        }
        if(num==1){
          printf("\n");
          break;
        }else{
          printf(" x ");
        }
      }
    }
  }
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

int readInput(long long int *num){  //reads input, also returns error
  int ret = ERROR_OK;
  if(scanf("%lld", &*num)!=1){
    ret = ERROR_WRONG_INPUT; 
  }
  if(*num < MIN_VALUE){
    ret = ERROR_WRONG_INPUT; 
  }
  return ret;
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

