#include <stdio.h>

int main(){
    int n;
    int r = scanf("%d", &n);
    if(r==0){
        printf("noinput ");
    }else{
        printf("%d ", r);
    }
    
}