#include <stdio.h>
#include <stdlib.h>
#include <string.h>


enum{
  ERROR_OK = EXIT_SUCCESS,
  ERROR_WRONG_INPUT = 100, 
};
static const char*E_WRONG_INPUT = "Error: Chybny vstup!";
#define MIN_VALUE 0

void reportError(int error);
int divide(char *numStr[101], char numStrCopy[101], long long int dividor);
void primeDecomposition(char numStr[101]); 
int toThePowerOf(int, int);
int readInput(char *numStr);
int checkIfNum(int ret, char numStr[101]);


int main(int argc, char *argv[]){
  int ret = ERROR_OK;
  char numStr[101];
  numStr[0] = '1';

  while(ret == ERROR_OK && atoi(&numStr[0]) != 0){
    ret = readInput(numStr);
    if(ret == ERROR_OK && atoi(&numStr[0]) != 0){
      primeDecomposition(numStr);
    }
  }
  if(ret != ERROR_OK){
    reportError(ret);
  }  
  return ret;
}

void primeDecomposition(char numStr[101]){
  printf("Prvociselny rozklad cisla %s je:\n", numStr);
  int howManyTimes = 0;
  int devisionSuc = 0;
  int curPrime = 1;
  if(strcmp(numStr, "1")==0){
    printf("1\n");
  }else{
    while(1){
      curPrime += 1;
      devisionSuc = divide(&numStr, numStr, curPrime);
      if(devisionSuc==1){
        howManyTimes = 0;
        while(devisionSuc==1){
          howManyTimes += 1;
          devisionSuc = divide(&numStr, numStr, curPrime);
        }
        printf("%d", curPrime);
        if(howManyTimes>1){
          printf("^%d", howManyTimes);
        }
        if(strcmp(numStr, "1")==0){
          printf("\n");
          break;
        }else{
          printf(" x ");
        }
        //printf("cur prime %d, how many times %d, strin %s\n", curPrime, howManyTimes, numStr);
      }

      //break;
    }
  }



  //smazat zbytek po tomto
  // for(long long int i = 2;;++i){
  //   divide(&numStr, numStr, i); //returns 0/1 depending on if the devision was succ and change the val of numstr to result 
  //   printf("printim devko %s\n", numStr);
  //   break;
  // }
}
int divide(char *numStr[101], char numStrCopy[101], long long int dividor){
  int index = 0;
  char result[101];
  int startDivi = 0;
  int remain = 0;
  int power = 0;
  
  for(int i = 0; i < strlen(numStrCopy); ++i) {
    if(dividor == 29){
      printf("i: %d, remain: %d result: %s, numstrcopy[i]: %c\n", i, remain, result, numStrCopy[i]);
    }
    power = strlen(numStrCopy) - (1+i);
    if(dividor == 29){
      printf("10*remain = %d, atoi(&numStrCopy[i]) = %d /toThePowerOf(10, power) = %d\n", 10*remain, 10*remain + atoi(numStrCopy[i]), toThePowerOf(10, power));
    }
    remain = 10*remain + atoi(&numStrCopy[i])/toThePowerOf(10, power);
    if(remain>=dividor){
      startDivi = 1;
    }
    if(startDivi==1){
      result[index] = (remain/dividor)+'0';
      index++;
      remain %= dividor;
    }
  }
  result[index] = '\0';
  if(remain == 0){
    printf("jsem tu a dividor je: %lld a result je: %s a remain je: %d\n", dividor, result, remain);
    strcpy(*numStr, result);
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

int readInput(char *numStr){  //reads input, also returns error, if
  int ret = ERROR_OK;
  if(scanf("%s", &*numStr)!=1){
    ret = ERROR_WRONG_INPUT; 
  }
  if(*numStr < MIN_VALUE){
    ret = ERROR_WRONG_INPUT; 
  }
  return ret;
}

int checkIfNum(int ret, char numStr[101]){ //checks if the string is really a number
  char numArr[] = "0123456789"; 
  int numIsNum = 1;
  if(ret == ERROR_OK){
    for(int i = 0; i < strlen(numStr); ++i){
      for(int j = 0; j < 10; ++j){
        if(numStr[i]==numArr[j]){
          numIsNum = 1;
          break;
        }else{
          numIsNum = 0;
        }
      }
      if(numIsNum != 1){
        ret = ERROR_WRONG_INPUT;
        break;
      }
    }
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

