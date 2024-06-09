#pragma once

typedef struct Point {
	char name[50];
	int n;
} point;

typedef struct ArrayPoint {
	point* array;
	int size;
} arrayPoint;

#define ComparePoints(a, b) (!strcmp(a.name, b.name) && a.n == b.n ? 1 : 0)

typedef struct Path {
	point point1;
	point point2;
	int weight;
} path;

typedef struct ArrayPath {
	path* array;
	int size;
} arraypath;

typedef struct PathMatrix {
	arrayPoint headers;
	int* values;
} pathMatrix;

#define ComparePaths(a, b) (((ComparePoints(a.point1, b.point1) || ComparePoints(a.point1, b.point2)) && (ComparePoints(a.point2, b.point2) || ComparePoints(a.point2, b.point1))) ? 0 : a.weight > b.weight ? 1 : -1)

int Method1(pathMatrix p);
int FindPoint(point* p, char* f, int size);
pathMatrix GetPathsFromFile(char* name);