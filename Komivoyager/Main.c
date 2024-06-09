#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
	printf_s("\n---------------------\nRozwiazania metoda 1:\n---------------------");
	int s1 = Method1(paths);
	printf_s("\nNajkrotsza sciezka metoda 1: %d", s1);
	printf_s("  ");

	printf_s("\n---------------------\nRozwiazania metoda 2:\n---------------------\n");
	
	printf_s("\n");
    arrayPath mst;
    int totalWeight = kruskalMST(paths, &mst);

    printf("Edges in the MST:\n");
    for (int i = 0; i < mst.size; ++i) {
        printf("%s - %s: %d\n", mst.array[i].point1.name, mst.array[i].point2.name, mst.array[i].weight);
    }
    printf("Total weight of MST: %d\n", totalWeight);

    
    return 0;
}




