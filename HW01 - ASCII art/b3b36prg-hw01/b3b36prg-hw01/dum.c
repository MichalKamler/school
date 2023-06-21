#include <stdio.h>

int main() {
    int width, height, row, col;
    printf("Enter the width and height of the box:\n");
    scanf("%d %d", &width, &height);

    for (row = 1; row <= height; row++) {
        for (col = 1; col <= width; col++) {
            if (col == row || col == width - row + 1 || row == 1 || row == height || col == 1 || col == width) {
                printf("X");
            } else {
                printf(" ");
            }
        }
        printf("\n");
    }

    return 0;
}