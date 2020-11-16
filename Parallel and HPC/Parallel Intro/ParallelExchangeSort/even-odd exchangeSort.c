#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

void swap(int* a, int* b) {
	int c = *a;
	*a = *b;
	*b = c;
}

int main() {
	int vecLength;
	printf("Enter the length of the unsorted array:");
	scanf("%d", &vecLength);
	
	int* vec = (int*)malloc(vecLength * sizeof(int));
	if (vec == NULL) {
		printf("Memory allocation failed.\n");
		return 0;
	}

	srand((unsigned int)time(NULL));
	for (int i = 0; i < vecLength; i++) {
		vec[i] = rand();
		printf("%d ", vec[i]);
	}
	printf("\n");

	for (int i = 0; i < vecLength; i++) {
		// O(n^2) for exchangeSort
		for (int j = i % 2; j + 1 < vecLength; j += 2) {
			if (vec[j] <= vec[j + 1]) continue;
			swap(&vec[j], &vec[j + 1]);
		}
	} // amazing! Very short and easy to implement.
	
	for (int i = 0; i < vecLength; i++) {
		printf("%d ", vec[i]);
	}
	printf("\n");
	return 0;
}