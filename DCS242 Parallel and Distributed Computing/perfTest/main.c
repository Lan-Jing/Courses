#include <time.h>
#include "MatrixOp.h"

int main() {
    srand(time(0));
    
    struct Matrix *mata = NULL, *matb = NULL, *resMat = NULL;
    int m1, n1, m2, n2;
    printf("Enter lengths for the first 2d matrix.\n");
    scanf("%d %d", &m1, &n1);
    printf("Enter lengths for the second.\n");
    scanf("%d %d", &m2, &n2);

    mata = new_mat(m1, n1, 0);
    // printf("The first random %d*%d matrix:\n", m1, n1);
    // print_mat(mata);
    matb = new_mat(m2, n2, 0);
    // printf("The second random %d*%d matrix:\n", m2, n2);
    // print_mat(matb);

    clock_t startTime = clock();
    resMat = BrutalMul(mata, matb);
    clock_t endTime = clock();
    printf("Plain alogirthm took %lfs to complete.\n", (double)(endTime - startTime)/CLOCKS_PER_SEC);

    // clock_t startTime = clock();
    // resMat = StrassenMul(mata, matb);
    // clock_t endTime = clock();
    // printf("Strassen alogirthm took %lfs to complete.\n", (double)(endTime - startTime)/CLOCKS_PER_SEC);

    free_mat(mata);
    free_mat(matb);
    free_mat(resMat);

    return 0;
}