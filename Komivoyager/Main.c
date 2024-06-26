#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "point.h"
#include "ANK.h"

int main() {
	pathMatrix paths = GetPathsFromFile("dane.txt");
	printf_s("  ");
	for (int i = 0; i < paths.headers.size; i++) {
		printf_s(" %3s", paths.headers.array[i].name);
	}
	for (int y = 0; y < paths.headers.size; y++) {
		printf_s("\n%3s ", paths.headers.array[y].name);
		for (int x = 0; x < paths.headers.size; x++) {
			printf_s("%3d ", paths.values[x + y * paths.headers.size]);
		}
	}
	printf_s("\n---------------------\nRozwiazania metoda 1:\n---------------------\n");
	// Ten pomiar trzeba by zaimplementowaæ w Twojej funkcji - 
	// problem w tym, ¿e nie zmie¿ymy czasu realizacji, bo funkcja traci czas na printy

	clock_t start = clock();
	int s1 = Method1(paths);
	clock_t end = clock();
	long double  time_spent = (long double)(end - start);
	printf("Time taken in Method1: %f ms\n", time_spent);
	printf_s("\nNajkrotsza sciezka metoda 1: %d", s1);
	

	printf_s("\n---------------------\nRozwiazania metoda 2:\n---------------------\n");
	
	printf_s("\n");
    arrayPath mst;
	clock_t start2 = clock();
    int totalWeight = kruskalMST(paths, &mst);
	clock_t end2 = clock(); 
	long double  time_spent2 = (long double)(end2 - start2);


    printf("Edges in the MST:\n");
    for (int i = 0; i < mst.size; ++i) {
        printf("%s - %s: %d\n", mst.array[i].point1.name, mst.array[i].point2.name, mst.array[i].weight);
    }
	printf("Time taken in Method2: %f ms\n", time_spent2);
    printf("Total weight of MST: %d\n", totalWeight);

	drukuj_porownanie(time_spent, time_spent2, s1, totalWeight);

    return 0;
}




