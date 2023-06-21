#include <stdio.h>
#include <stdlib.h>

enum{
  ERROR_OK = EXIT_SUCCESS,
  NO_MATCH_FOUND = 1,
  ERROR_FILE = 100,
  ERROR_READING_FILE = 101
};

#define MIN_MEM_ALLOC 10

static const char*E_FILE = "Error: Nepodarilo se nacist soubor!";
static const char*E_READING_FILE = "Error: Nepodarilo se precist soubor!";

void readInput(int *ret, char *pattern, char *filename);
int lenString(char *line);
void comparison(int *ret, char *pattern, char *line);
void reportError(int error);

int main(int argc, char **argv){
  int ret = NO_MATCH_FOUND;

  readInput(&ret, argv[1], argv[2]);

  reportError(ret);
  return ret;
}

//calculates the len of string
int lenString(char *line){
  int len = 0;
  for(unsigned int i=0; line[i]!='\0' ;i++){
    len += 1;
  }
  return len;
}
//looks for the pattern in line
void comparison(int *ret, char *pattern, char *line){
  int lenLine = lenString(line);
  int lenPattern = lenString(pattern);
  int patternMatch = 0, printLine = 0;
  
  for(unsigned int i=0; i<lenLine; i++){
    patternMatch = 1;
    for(unsigned int j=0; j<lenPattern; j++){
      if(pattern[j]!=line[i+j]){
        patternMatch = 0;
        break;
      }
    }
    if(patternMatch==1){
      printLine = 1;      
    }
  }
  if(printLine==1){
    *ret = EXIT_SUCCESS;
    printf("%s\n", line);
  }
}
//opens file and passes each line to other function
void readInput(int *ret, char *pattern, char *filename){
  FILE *fp;
  fp = fopen(filename, "r");
  if (fp == NULL) {
    *ret = ERROR_FILE;
  }
  if(*ret==ERROR_OK || *ret==NO_MATCH_FOUND){    
    while(*ret==ERROR_OK || *ret==NO_MATCH_FOUND){
      char *line = NULL;
      line = (char*)malloc(MIN_MEM_ALLOC*sizeof(char));
      int c, index = 0, sizeOfStr = MIN_MEM_ALLOC;
      while((c=fgetc(fp))!=EOF && c!='\n'){
        if(index>=sizeOfStr-1){
          sizeOfStr *= 2;
          line = (char*)realloc(line, sizeOfStr*sizeof(char));
        }
        line[index++] = c;
      }
      line[index] = '\0';      
      comparison(&*ret, pattern, line);
      free(line);
      if(c==EOF){
        break;
      }
    }
    fclose(fp);
  }
}
//reports error that occurs when program is running
void reportError(int error){
  const char *e = NULL;
  switch(error){
    case ERROR_FILE:
      e=E_FILE;
      break;
    case ERROR_READING_FILE:
      e=E_READING_FILE;
      break;
  }
  if(e){
    fprintf(stderr, "%s\n", e);
  }
}

