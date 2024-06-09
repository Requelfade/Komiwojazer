#pragma once
#include "point.h"


// Struktury dla algorytmu Kruskala
typedef struct Subset {
	point parent;
	int rank;
} subset;

int kruskalMST(pathMatrix matrix, arrayPath* resultMST);
