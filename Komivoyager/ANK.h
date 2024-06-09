#pragma once
#include "point.h"


// Struktury dla algorytmu Kruskala
typedef struct Subset {
	point parent;
	int rank;
} subset;

int kruskalMST(pathMatrix matrix, arrayPath* resultMST);
void drukuj_porownanie(double time_1, double time_2, int  score_1, int score_2);