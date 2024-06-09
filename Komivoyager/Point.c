#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "point.h"
#define _countof(array) (sizeof(array) / sizeof(array[0]))


// Algorytm najbli¿szego s¹siada
int Method1(pathMatrix pm) {
	if (pm.headers.size <= 2) return;

	int lowest = -1;
	
	for (int startingRow = 0; startingRow < pm.headers.size; startingRow++) {
		int row = startingRow;
		int shortest = 0;
		point* usedHeaders = (point*)malloc(sizeof(point) * pm.headers.size);
		usedHeaders[0] = pm.headers.array[row];

		for (int i = 1; i < pm.headers.size; i++) {
			int currentMin = -1;
			int minIndex = -1;
			for (int j = 0; j < pm.headers.size; j++) {
				int curIndex = row * pm.headers.size + j;
				if (pm.values[curIndex] != 0) {
					int found = 0;
					if (FindPoint(usedHeaders, pm.headers.array[j].name, pm.headers.size) != -1) {
						found = 1;
					}
					if (found == 0) {
						if (currentMin == -1 || currentMin > pm.values[curIndex]) {
							currentMin = pm.values[curIndex];
							minIndex = j;
						}
					}
				}
			}
			shortest += currentMin;
			usedHeaders[i] = pm.headers.array[minIndex];
			printf_s("\nSciezka z %s do %s: %d", pm.headers.array[row].name, pm.headers.array[minIndex].name, currentMin);
			row = minIndex;
			if (i + 1 == pm.headers.size) {
				int sum = pm.values[row * pm.headers.size + FindPoint(pm.headers.array, usedHeaders[0].name, pm.headers.size)];
				printf_s("\nSciezka z %s do %s: %d", pm.headers.array[row].name, usedHeaders[0].name, sum);
				shortest += sum;
			}
		}
		free(usedHeaders);
		printf_s("\nLacznie: %d\n------------", shortest);

		if (lowest == -1 || lowest > shortest) {
			lowest = shortest;
		}
	}

	return lowest;
}

int FindPoint(point* p, char* f, int size) {
	for (int i = 0; i < size; i++) {
		if (strcmp(p[i].name, f) == 0) {			
			return i;
		}
	}
	return -1;
}

pathMatrix GetPathsFromFile(char* name) {
	FILE* ptr;
	errno_t err;
	
	if ((err = fopen_s(&ptr, name, "r")) != 0) {
		fprintf_s(stderr, "cannot open file '%s'\n",
			name);
		return;
	}
	
	int bufsize = 255;
	char buffer[255];
	int i = 0;
	int charsToRead = 0;


	int sizeofPath = 0;
	char ch;
	while ((ch = fgetc(ptr)) != EOF) {
		if (ch == '\n') sizeofPath++;
	}
	fseek(ptr, 0L, SEEK_SET);
	fgets(buffer, bufsize, ptr);
	sscanf_s(buffer, "%d", &charsToRead);
	sizeofPath -= charsToRead;

	path* paths = (path*)malloc(sizeof(path) * sizeofPath);
	pathMatrix matrix;
	matrix.headers.array = (point*)malloc(sizeof(point) * charsToRead);
	matrix.headers.size = charsToRead;
	matrix.values = (int*)malloc(sizeof(int) * charsToRead * charsToRead);

	for (int i = 0; i < charsToRead; i++) {
		matrix.values[i * charsToRead + i] = 0;
	}
	
	while (fgets(buffer, bufsize, ptr)) {
		if (i < charsToRead) {					
			char temp[50];
			sscanf_s(buffer, "%s", &temp, (unsigned)_countof(temp));
			point p;
			memcpy(p.name, temp, (unsigned)_countof(temp));
			p.n = i;
			matrix.headers.array[i] = p;
			i++;
		}
		else {
			char p1[50], p2[50];
			int w = 0;
			sscanf_s(buffer, "%s %s %d", &p1, (unsigned)_countof(p1), &p2, (unsigned)_countof(p2), &w);
			int f1 = FindPoint(matrix.headers.array, p1, charsToRead);
			int f2 = FindPoint(matrix.headers.array, p2, charsToRead);
			if (f1 > -1 && f2 > -1) {
				matrix.values[f1 + charsToRead * f2] = w;
				matrix.values[f2 + charsToRead * f1] = w;
			}		
			i++;
		}
	}

	fclose(ptr);

	return matrix;
}