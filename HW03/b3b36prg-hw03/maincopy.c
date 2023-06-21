#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum{
  ERROR_OK = EXIT_SUCCESS,
  ERROR_INPUT = 100,
  ERROR_LEN = 101
};

#define MIN_MES_LEN 10
#define a_z_A_Z 52

static const char*E_INPUT = "Error: Chybny vstup!";
static const char*E_LEN = "Error: Chybna delka vstupu!";

void reportError(int error);
char *readInput(int *lenOfMes);
char rotate(char original, int offset);
void shift(char *original, int len, int offset);
int compare(char *shifted, int shifLen, char *listened);
int checkInput(char *mess, int len);

int main(int argc, char *argv[]) {
  int ret = ERROR_OK;
  int messageLen, listenMesLen;
  char *message, *listenMes;
  int curCompa, bestCompa = 0, bestIndex = 0;

  message = readInput(&messageLen);
  listenMes = readInput(&listenMesLen);

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
  
  if (ret == ERROR_OK) {
    //char mesFin[messageLen + 1]; 
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

  free(message);
  free(listenMes);

  reportError(ret);
  return ret;
}

int checkInput(char *mess, int len){ //checks if input has correct char
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

void shift(char *original, int len, int offset){//shifts string by an offset
  for(int i=0; i<len; ++i){
    original[i] = rotate(original[i], offset);
  }
}

int compare(char *shifted, int shifLen, char *listened){//compares 2 stringes returns how alike are them
  int ret = 0;
  for(int i=0; i<shifLen; ++i){
    if(shifted[i]==listened[i]){
      ret += 1;
    }
  }
  return ret;
}

char *readInput(int *lenOfMes) {
    int len = 0;
    int curMemAl = MIN_MES_LEN;
    char *ret = malloc(MIN_MES_LEN);
    if (ret == NULL) {
        fprintf(stderr, "Error u me - malloc\n");
        exit(69);
    } else {
        int curChar = 0;
        while ((curChar = getchar()) != EOF && curChar != '\n') {
            if (curMemAl == len) {
                curMemAl *= 2;
                char *succRealloc = realloc(ret, curMemAl);
                if (succRealloc == NULL) {
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

char rotate(char original, int offset){//returns charakter that has been offseted
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

void reportError(int error){
  const char *e = NULL;
  switch(error){
    case ERROR_INPUT:
      e=E_INPUT;
      break;
    case ERROR_LEN:
      e=E_LEN;
  }
  if(e){
    fprintf(stderr, "%s\n", e);
  }
}
