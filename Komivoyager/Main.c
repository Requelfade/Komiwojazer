#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "point.h"

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
	int s = Method1(paths);
	printf_s("\nNajkrotsza sciezka metoda 1: %d", s);
}