#include <stdio.h>
#include <stdlib.h>

enum{
  ERROR_OK=EXIT_SUCCESS,
  ERROR_INPUT=100
};

#define CAL_UP_TO 1000000

int load_num(int *ret, int *numList);
int check_if_num(char c);
void calculatePrimes(int *primesArray); //finds all primes up to CAL_UP_TO
void print_num(int *numList);
void primeDecomposition(int *numList, int *primesArray);
int toThePowerOf(int x, int y);
int divide(int *numList, int dividor);

int main(int argc, char *argv[]){
  int ret = ERROR_OK;
  int numList[101];
  int primesArray[CAL_UP_TO];

  calculatePrimes(primesArray);

  numList[0]=1;
  while(ret == ERROR_OK && numList[0] != 0){
    load_num(&ret, numList);
    if(ret == ERROR_OK && numList[0] != 0){
      primeDecomposition(numList, primesArray);
    }
  }

  return ret;
}

void primeDecomposition(int *numList, int *primesArray){
  printf("Prvociselny rozklad cisla ");
  print_num(numList);
  printf(" je:\n");
  int howManyTimes = 0;
  if(numList[0]==1 && numList[1]==-1){
    printf("1\n");
  }else{
    for(unsigned int curPrime=0; curPrime<CAL_UP_TO; curPrime++){
      if(primesArray[curPrime]){        
        if(divide(numList, curPrime)){
          howManyTimes=1;
          while(divide(numList, curPrime)){
            howManyTimes+=1;
          }
          printf("%d", curPrime);
          if(howManyTimes>1){
            printf("^%d", howManyTimes);
          }
          if(numList[0]==1 && numList[1]==-1){
            printf("\n");
            break;
          }else{
            printf(" x ");
          }
        }
      }
    }   
  }
}

int list_len(int *numList){
  int index = 0;
  int len = 0;
  while(numList[index++]!=-1){
    len+=1;
  }
  return len;
}

int divide(int *numList, int dividor){
  int index = 0;
  int result[101];
  int remain = 0;
  int power = 0;
  int startDiv = 0;
  for(unsigned int i = 0; i<101; i++){
    if(numList[i]==-1){
      break;
    }
    power = list_len(numList)-(1+i);
    remain = 10*remain + numList[i];
    if(remain>=dividor){
      startDiv = 1;
    }
    if(startDiv==1){
      result[index++]=(remain/dividor);
      remain %= dividor;
    }
  }
  result[index]=-1;
  if(remain==0){
    for(unsigned int i = 0; i<101; i++){
      numList[i]=result[i];
      if(result[i]==-1){
        break;
      }
    }
    return 1;
  }
  return 0;
}

int toThePowerOf(int x, int y){ //x^y
  int z = x;
  if(y==0){
    return 1;
  }
  for(int i = 1; i < y; ++i){
    z *= x;
  }
  return z;
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

void print_num(int *numList){
  for(unsigned int i = 0; i<101; i++){
    if(numList[i]==-1){
      break;
    }
    printf("%d", numList[i]);
  }
}

int check_if_num(char c){
char listOfNums[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
  for(unsigned int i = 0; i<10; i++){
    if(listOfNums[i]==c){
      return 1;
    }
  }
  return 0;
}

int load_num(int *ret, int *numList){
  char c;
  int index = 0;
  while((c=getchar())!=EOF){
    if(check_if_num(c)){
      numList[index++]=c-'0';
    }else if(c!=10){
      *ret=ERROR_INPUT;
      break;
    }else{
      break;
    }
  }
  if(*ret==ERROR_OK){
    numList[index]=-1;
  }
  return *ret;
}
