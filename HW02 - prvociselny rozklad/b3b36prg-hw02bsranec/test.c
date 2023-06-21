#include <stdio.h>
#include <stdlib.h>

void change(int *a);

int main(){
    int a = 8;
    printf("%d\n", a);
    change(&a);
    printf("%d\n", a);
}
void change(int *a){
    *a = 10;
}