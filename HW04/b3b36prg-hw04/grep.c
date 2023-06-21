#include <stdio.h>
#include <stdlib.h>

enum{
  ERROR_OK = EXIT_SUCCESS,
  NO_MATCH_FOUND = 1,
  ERROR_FILE = 100,
  ERROR_READING_FILE = 101
};

#define MIN_MEM_ALLOC 10
#define REG_EXP "-E"
#define COLOR_PLS_MLS "--color=always"

static const char*E_FILE = "Error: Nepodarilo se nacist soubor!";
static const char*E_READING_FILE = "Error: Nepodarilo se precist soubor!";

void readInput(int *ret, char *pattern, char *filename, char *opt);
int lenString(char *line);
void comparison(int *ret, char *pattern, char *line, char *opt);
void reportError(int error);
int stringCmp(char *str1, char *str2);
void printColor(char *pattern, char*line);

int main(int argc, char **argv){
  int ret = NO_MATCH_FOUND;
  if(argc==3){
    readInput(&ret, argv[1], argv[2], NULL);
  }else if(argc==4){
    readInput(&ret, argv[2], argv[3], argv[1]);
  }
  reportError(ret);
  return ret;
}
//prints line witch colors
void printColor(char *pattern, char*line){
  int lenPattern = lenString(pattern);
  int lenLine = lenString(line);
  int fullMatch;
  int offset = 0;
  for(unsigned int i = 0; i<lenLine; i++){
    fullMatch = 0;
    if(offset+i>=lenLine){
      break;
    }
    if(line[i+offset]!=pattern[0]){
      printf("%c", line[i+offset]);
    }else{
      fullMatch = 1;
      for(unsigned int j = 0; j<lenPattern; j++){
        if(pattern[j]!=line[i+j+offset]){
          fullMatch = 0;
        }        
      }
      if(fullMatch){
        offset += lenPattern-1;           
        printf("\033[01;31m\033[K%s\033[m\033[K", pattern);
      }else{
        printf("%c", line[i]);
      }
    }    
  }
  printf("\n");
}

//compares 2 strings and returns 0 or 1 depending if they are the same
int stringCmp(char *str1, char *str2){
  int lenStr1 = lenString(str1);
  int lenStr2 = lenString(str2);
  if(str1==NULL || str2==NULL){
    return 0;
  }
  if(lenStr1!=lenStr2){
    return 0;
  }else{
    for(unsigned int i=0; i<lenStr1; i++){
      if(str1[i]!=str2[i]){
        return 0;
      }
    }
  }
  return 1;
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
void comparison(int *ret, char *pattern, char *line, char *opt){
  int lenLine = lenString(line);
  int lenPattern = lenString(pattern);
  int patternMatch = 0, printLine = 0, offset = 0, patOffset = 0;
  for(unsigned int i=0; i<lenLine; i++){
    patternMatch = 1;
    offset = 0;
    patOffset = 0;
    for(unsigned int j=0; j<lenPattern; j++){
      if(opt!=NULL){
        if(stringCmp(REG_EXP, opt)){
          if(j>=lenPattern){
            break;
          }
          if(j+1<lenPattern){
            if(pattern[j+1]=='*'){
              patOffset += 2;
              while(pattern[j]==line[i+j+offset]){
                offset += 1;
              }
            }
            if(pattern[j+1]=='+'){
              patOffset +=2;
              offset += 1;
              while(pattern[j]==line[i+j+offset]){
                offset += 1;
              }
            }
            if(pattern[j]!=line[i+j] && pattern[j+1]=='?'){
              offset -= 2;
              continue;
            }
            if(pattern[j]==line[i+j] && pattern[j+1]=='?'){
              offset -= 1;
              continue;
            }
            if(pattern[j]=='?'){
              continue;          
            }
          }        
        }
      }       
      if(pattern[j+patOffset]!=line[i+j+offset]){
        patternMatch = 0;
        break;
      }
      if(lenPattern==j+patOffset+1){
        break;
      }
    }
    if(patternMatch==1){
      printLine = 1;      
    }
  }
  if(printLine==1){
    if(opt!=NULL){
      if(stringCmp(opt, REG_EXP)){
        *ret = EXIT_SUCCESS;
        printf("%s\n", line);
      }
      if(stringCmp(opt, COLOR_PLS_MLS)){      
        printColor(pattern, line);
        *ret = EXIT_SUCCESS;
      }
    }else{
      *ret = EXIT_SUCCESS;
      printf("%s\n", line);
    }    
  }
}
//opens file and passes each line to other function
void readInput(int *ret, char *pattern, char *filename, char *opt){
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
      comparison(&*ret, pattern, line, opt);
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

