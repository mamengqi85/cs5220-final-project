#include<math.h>
#include<stdlib.h>
#include<stdio.h>
#include<time.h>
#include"params.h"
#include"utility.h"

int main()
{
	//max = 6
	int testing = 6;
	int i, j;
	srand((int)time(0));
	if (testing == 0) {
		printf("round 1.1 = %f\n", round(1.1));
		printf("round 0.0 = %f\n", round(0.0));
		printf("round 0.1 = %f\n", round(0.1));
		printf("round 0.9 = %f\n", round(0.9));
		printf("round -0.1 = %f\n", round(-0.1));
		printf("round -0.9 = %f\n", round(-0.9));
		printf("round -1.1 = %f\n", round(-1.1));
	}
	if (testing == 1) {
		printf("rnd(0,1):");
		for (i = 0; i < 5; ++i) {
			printf("%d,",rnd(0, 1));
		}
		printf("\n");
		printf("rnd(0,0):");
		for (i = 0; i < 5; ++i) {
			printf("%d,",rnd(0, 0));
		}
		printf("\n");
		printf("rnd(-2,5):");
		for (i = 0; i < 10; ++i) {
			printf("%d,",rnd(-2, 5));
		}
		printf("\n");
	}
	if (testing == 2) {
		printf("randper(1):");
		int* perm = randperm(1);
		for (i = 0; i < 1; ++i) {
			printf("%d,", perm[i]);
		}
		printf("\n");
		printf("randper(10):");
		perm = randperm(10);
		for (i = 0; i < 10; ++i) {
			printf("%d,", perm[i]);
		}
		printf("\n");
	}
	int** array = (int**) calloc(5, sizeof(int*));
	for (i = 0; i < 5; ++i) {
		array[i] = (int*) calloc(2, sizeof(int));
	}
	array[0][0] = 21;
	array[0][1] = 61;
	array[1][0] = 62;
	array[1][1] = 2;
	array[2][0] = 163;
	array[2][1] = 23;
	array[3][0] = 4;
	array[3][1] = 84;
	array[4][0] = -105;
	array[4][1] = 75;
	printf("ori array:");
	for (i = 0; i < 5; ++i) {
			for (j = 0; j < 2; ++j) {
				printf("%d,", array[i][j]);
			}
			printf("\n");
		}
	if (testing == 3) {
		printf("randper(5):");
		int* perm = randperm(5);
		for (i = 0; i < 5; ++i) {
			printf("%d,", perm[i]);
		}
		printf("\n");
		printf("array after reorder:\n");
		param_t params;
		params.popsize = 5;
		params.len = 2;
		reorder(&params, array, perm);
		for (i = 0; i < 5; ++i) {
			for (j = 0; j < 2; ++j) {
				printf("%d,", array[i][j]);
			}
			printf("\n");
		}
	}
	if (testing == 4) {
		printf("sort:");
		int cost[5] = {71,42, -103, 84, 55};
		int* order = sort(cost, 5);
		for (i = 0; i < 5; ++i) {
			printf("%d,", cost[i]);
		}
		printf("\n");
		printf("array after reorder:\n");
		param_t params;
		params.popsize = 5;
		params.len = 2;
		reorder(&params, array, order);
		for (i = 0; i < 5; ++i) {
			for (j = 0; j < 2; ++j) {
				printf("%d,", array[i][j]);
			}
			printf("\n");
		}
	}
	if (testing ==5)
	{
		int array[5] = {32, -893, 31, 1, 0};
		printf("min: %d\n", min(array, 5));
		printf("max: %d\n", max(array, 5));
		printf("mean: %f\n", mean(array, 5));
	}

	if (testing == 6)
	{
		int* new_a = de_d(array, 5, 2);
		print_matrix(&new_a, 1, 10, "1d.txt");
		int** newnew_a = in_d(new_a, 5, 2);
		print_matrix(newnew_a, 5, 2, "2d.txt");
	}
		
	return 0;
}


