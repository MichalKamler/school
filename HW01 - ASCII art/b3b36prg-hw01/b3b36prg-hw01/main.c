#include <stdio.h>
#include <stdlib.h>

enum{
  ERROR_OK = EXIT_SUCCESS,
  ERROR_WRONG_INPUT = 100, //no input or wrong input
  ERROR_RANGE = 101,
  ERROR_NOT_ODD = 102, //first num is width - has to be odd
  ERROR_FENCE = 103
};

#define MAX_VALUE 69
#define MIN_VALUE 3

static const char*E_WRONG_INPUT = "Error: Chybny vstup!";
static const char*E_RANGE = "Error: Vstup mimo interval!";
static const char*E_NOT_ODD = "Error: Sirka neni liche cislo!";
static const char*E_FENCE = "Error: Neplatna velikost plotu!";

void reportError(int error);
void printHouseFence(int width, int height, int fence, int fenceTrue);
void printRoof(int width);
int conditions(int ret, int width, int height);
int readInputFence(int ret, int width, int height, int *fence, int *fenceTrue);
int readWidhtHeight(int ret, int *width, int *height);

int main(int argc, char *argv[]){
  int ret = ERROR_OK;
  int width, height, fence;
  int fenceTrue = 0;

  ret = readWidhtHeight(ret, &width, &height);
  ret = conditions(ret, width, height); 

  if(ret == ERROR_OK){
    ret = readInputFence(ret, width, height, &fence, &fenceTrue);
  }

  if(ret==ERROR_OK){
    printHouseFence(width, height, fence, fenceTrue);
  }else{
    reportError(ret);
  }
  return ret;
}

int readWidhtHeight(int ret, int *width, int *height){
  if(scanf("%d%d", &*width, &*height)!=2){  //scan of width and height
    ret = ERROR_WRONG_INPUT;
  }
  return ret;
}
int readInputFence(int ret, int width, int height, int *fence, int *fenceTrue){
  if(ret==ERROR_OK && width==height){ //fence load that also handles end of input(no fence)
    int r = scanf("%d", &*fence);
    if(r==1){
      *fenceTrue = 1;
    }else if(r != EOF){
      ret = ERROR_WRONG_INPUT;
    }
  }
  if(*fenceTrue && (*fence<=0 || *fence>=height)){ //fence has to be lower than height
    ret = ERROR_FENCE;
  }
  return ret;
}
int conditions(int ret, int width, int height){
  if(ret==ERROR_OK){
    if(width<MIN_VALUE || width>MAX_VALUE  || height<MIN_VALUE || height>MAX_VALUE){
      ret = ERROR_RANGE;
    }
  }
  if(ret==ERROR_OK && width%2==0){
    ret = ERROR_NOT_ODD;
  }
  return ret;
}
void printHouseFence(int width, int height, int fence, int fenceTrue){
  int row, col, len; //cur place in fence 
  printRoof(width);
  for(row = 1; row <= height; row++){
    for(col = 1; col <= width; col++){
      if(row == 1 || row == height || col == 1 || col == width){  //printing house
        printf("X");  
      }else if(!fenceTrue){ //house with no fence
        printf(" ");
      }else if((row%2 == 0 && col%2 == 0) || (row%2 == 1 && col%2 == 1)){ //house with fence
        printf("o");
      }else{
        printf("*");
      }
    }
    if((fence+row > height) && fenceTrue){
      for(len = 1; len <= fence; len++){
        if((len%2 == 1 && fence%2 == 1) || (len%2 == 0 && fence%2 == 0)){
          printf("|");
        }else if(fence+row == height+1 || row == height){
          printf("-");
        }else{
          printf(" ");
        }
      }
    }
    printf("\n");
  }
}
void printRoof(int width){
  int row, col, spaces;
  spaces = (width+1)/2;
  for(row = 0; row < width/2; row++){
    spaces--; //spaces before first X
    for(col = 0; col < width/2+row; col++){     
      if(spaces != col){
        printf(" ");
      }else{
        printf("X");  //first X
      }
    }
    printf("X\n"); //last X
  }
}
void reportError(int error){
  const char *e = NULL;
  switch(error){
    case ERROR_WRONG_INPUT:
      e=E_WRONG_INPUT;
      break;
    case ERROR_RANGE:
      e=E_RANGE;
      break;
    case ERROR_NOT_ODD:
      e=E_NOT_ODD;
      break;
    case ERROR_FENCE:
      e=E_FENCE;
      break;
  }
  if(e){
    fprintf(stderr, "%s\n", e);
  }
}
