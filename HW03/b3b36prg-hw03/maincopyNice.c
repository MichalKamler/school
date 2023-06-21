#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum{
  ERROR_OK = EXIT_SUCCESS,
  ERROR_INPUT = 100,
  ERROR_LEN = 101,
  ERROR_MEMORY = 69 //not required
};

#define MIN_MES_LEN 10
#define a_z_A_Z 52

static const char*E_INPUT = "Error: Chybny vstup!";
static const char*E_LEN = "Error: Chybna delka vstupu!";
static const char*E_MEM = "Error u me - malloc";

void reportError(int error);
char *readInput(int *lenOfMes, int *error);
char rotate(char original, int offset);
void shift(char *original, int len, int offset);
int compare(char *shifted, int shifLen, char *listened);
int checkInput(char *mess, int len);
int checkIfCorrect(char *message, int messageLen, char *listenMes, int listenMesLen);
void decomposeMess(char *message, int messageLen, char *listenMes, int listenMesLen);

int main(int argc, char *argv[]) {
  int ret = ERROR_OK;
  int messageLen, listenMesLen;
  char *message, *listenMes;
  message = readInput(&messageLen, &ret);
  if(ret==ERROR_OK){
    listenMes = readInput(&listenMesLen, &ret);
  } 
  if(ret==ERROR_OK){
    ret = checkIfCorrect(message, messageLen, listenMes, listenMesLen);
  }    
  if (ret == ERROR_OK) {
    decomposeMess(message, messageLen, listenMes, listenMesLen);
    free(message);
    free(listenMes);
  }
  reportError(ret);
  return ret;
}

//decomposes message only if ret is error_ok
void decomposeMess(char *message, int messageLen, char *listenMes, int listenMesLen){ 
  int curCompa, bestCompa = 0, bestIndex = 0;
  for (int i = 1; i <= a_z_A_Z; ++i) {
    shift(message, messageLen, 1);
    curCompa = compare(message, messageLen, listenMes);
    if (curCompa > bestCompa) {
      bestCompa = curCompa;
      bestIndex = i;
    }
  }
  shift(message, messageLen, bestIndex);
  printf("%s\n", message);
}
//checks both inputs
int checkIfCorrect(char *message, int messageLen, char *listenMes, int listenMesLen){ 
  int ret = ERROR_OK;
  if (message == NULL || listenMes == NULL) {
    ret = ERROR_INPUT;
  } else if (messageLen != listenMesLen) {
    ret = ERROR_LEN;
  } else {
    ret = checkInput(message, messageLen);
    if (ret == ERROR_OK) {
      ret = checkInput(listenMes, listenMesLen);
    }
  }
  return ret;
}
//checks if input has correct char
int checkInput(char *mess, int len){ 
  int ret = ERROR_OK;
  int OK;
  for(int i=0; i<len; ++i){
    OK = 0;
    for(int j='A'; j<='z'; ++j){
      if(mess[i]==j){
        OK = 1;
        break;
      }
    }
    if(!OK){
      ret = ERROR_INPUT;
      break;
    }
  }
  return ret;
}
//shifts string by an offset
void shift(char *original, int len, int offset){
  for(int i=0; i<len; ++i){
    original[i] = rotate(original[i], offset);
  }
}
//compares 2 strings returns how alike are they
int compare(char *shifted, int shifLen, char *listened){
  int ret = 0;
  for(int i=0; i<shifLen; ++i){
    if(shifted[i]==listened[i]){
      ret += 1;
    }
  }
  return ret;
}
//reads inputs, if couldnt allocate mem error is 69, else changes len of input and returns char input
char *readInput(int *lenOfMes, int *error) {  
  int len = 0;
  int curMemAl = MIN_MES_LEN;
  char *ret = malloc(MIN_MES_LEN);
  if (ret == NULL) {
    *error = ERROR_MEMORY;
  } else {
    int curChar = 0;
    while ((curChar = getchar()) != EOF && curChar != '\n') {
      if (curMemAl == len) {
        curMemAl *= 2;
        char *succRealloc = realloc(ret, curMemAl);
        if (succRealloc == NULL) {
          *error = ERROR_MEMORY;
          free(ret);
          ret = NULL;
          break;
        }
        ret = succRealloc;
      }
      if (curChar != 13) {
        ret[len++] = curChar;
      }
    }
  }
  char *succRealloc = realloc(ret, len + 1);
  if (succRealloc != NULL) {
    ret = succRealloc;
  }
  ret[len] = '\0';
  *lenOfMes = len;
  return ret;
}
//returns charakter that has been offseted
char rotate(char original, int offset){
  for(int i=0; i<offset; ++i){
    original +=1;
    if(original==91){
      original=97;
    } 
    if(original==123){
      original = 65;
    }
  }
  return original;
}
//reports errros that occurs in program
void reportError(int error){
  const char *e = NULL;
  switch(error){
    case ERROR_INPUT:
      e=E_INPUT;
      break;
    case ERROR_LEN:
      e=E_LEN;
      break;
    case ERROR_MEMORY:
      e=E_MEM;
      break;
  }
  if(e){
    fprintf(stderr, "%s\n", e);
  }
}
