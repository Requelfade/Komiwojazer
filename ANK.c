#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "point.h"
#include "ANK.h"

// Funkcje pomocnicze do sortowania krawêdzi
int compare(const void* a, const void* b) {
    path pathA = *(path*)a;
    path pathB = *(path*)b;
    return ComparePaths(pathA, pathB);
}

// Znajdowanie korzenia zestawu z kompresj¹ œcie¿ki
point find(subset subsets[], point p) {
    if (!ComparePoints(subsets[p.n].parent, p)) {
        subsets[p.n].parent = find(subsets, subsets[p.n].parent);
    }
    return subsets[p.n].parent;
}

// £¹czenie dwóch zestawów
void Union(subset subsets[], point x, point y) {
    point rootX = find(subsets, x);
    point rootY = find(subsets, y);

    if (subsets[rootX.n].rank < subsets[rootY.n].rank) {
        subsets[rootX.n].parent = rootY;
    }
    else if (subsets[rootX.n].rank > subsets[rootY.n].rank) {
        subsets[rootY.n].parent = rootX;
    }
    else {
        subsets[rootY.n].parent = rootX;
        subsets[rootX.n].rank++;
    }
}

// Konwersja PathMatrix na ArrayPath
arrayPath convertPathMatrixToArrayPath(pathMatrix matrix) {
    int size = matrix.headers.size;
    int numEdges = 0;

    // Zliczanie liczby krawêdzi
    for (int i = 0; i < size; ++i) {
        for (int j = i + 1; j < size; ++j) {
            if (matrix.values[i * size + j] > 0) {
                numEdges++;
            }
        }
    }

    arrayPath paths;
    paths.size = numEdges;
    paths.array = (path*)malloc(numEdges * sizeof(path));

    int index = 0;
    for (int i = 0; i < size; ++i) {
        for (int j = i + 1; j < size; ++j) {
            if (matrix.values[i * size + j] > 0) {
                paths.array[index].point1 = matrix.headers.array[i];
                paths.array[index].point2 = matrix.headers.array[j];
                paths.array[index].weight = matrix.values[i * size + j];
                index++;
            }
        }
    }

    return paths;
}

// Implementacja algorytmu Kruskala dla TSP
int kruskalMST(pathMatrix matrix, arrayPath* resultMST) {
    arrayPath paths = convertPathMatrixToArrayPath(matrix);
    int totalWeight = 0;
    resultMST->size = 0;
    resultMST->array = (path*)malloc(paths.size * sizeof(path));

    // Sortowanie wszystkich krawêdzi
    qsort(paths.array, paths.size, sizeof(path), compare);

    // Alokowanie pamiêci dla zestawów
    subset* subsets = (subset*)malloc(matrix.headers.size * sizeof(subset));
    for (int i = 0; i < matrix.headers.size; ++i) {
        subsets[i].parent = matrix.headers.array[i];
        subsets[i].rank = 0;
    }

    // Licznik krawêdzi dla ka¿dego punktu
    int* edgeCount = (int*)calloc(matrix.headers.size, sizeof(int));

    // Wybieranie krawêdzi
    for (int i = 0; i < paths.size; ++i) {
        path nextPath = paths.array[i];

        point x = find(subsets, nextPath.point1);
        point y = find(subsets, nextPath.point2);

        // Sprawdzanie, czy dodanie tej krawêdzi nie spowoduje, ¿e którykolwiek punkt bêdzie mia³ wiêcej ni¿ 2 krawêdzie
        if (!ComparePoints(x, y) && edgeCount[nextPath.point1.n] < 2 && edgeCount[nextPath.point2.n] < 2) {
            resultMST->array[resultMST->size++] = nextPath;
            totalWeight += nextPath.weight;
            Union(subsets, x, y);
            edgeCount[nextPath.point1.n]++;
            edgeCount[nextPath.point2.n]++;
        }
    }

    // Dodanie krawêdzi zamykaj¹cej obieg
    for (int i = 0; i < matrix.headers.size; ++i) {
        if (edgeCount[i] < 2) {
            for (int j = 0; j < matrix.headers.size; ++j) {
                if (i != j && edgeCount[j] < 2 && matrix.values[i * matrix.headers.size + j] > 0) {
                    path closingEdge = { matrix.headers.array[i], matrix.headers.array[j], matrix.values[i * matrix.headers.size + j] };
                    resultMST->array[resultMST->size++] = closingEdge;
                    totalWeight += closingEdge.weight;
                    edgeCount[i]++;
                    edgeCount[j]++;
                    break;
                }
            }
        }
    }

    free(subsets);
    free(paths.array);
    free(edgeCount);
    return totalWeight;
}

//int main() {
//    // Przyk³adowe dane wejœciowe
//    point pointsArray[] = {
//        {"A", 0}, {"B", 1}, {"C", 2}, {"D", 3}, {"E", 4}
//    };
//    arrayPoint points;
//    points.array = pointsArray;
//    points.size = 5;
//
//    int values[] = {
//        0, 10, 6, 5, 0,
//        10, 0, 0, 15, 0,
//        6, 0, 0, 4, 0,
//        5, 15, 4, 0, 0,
//        0, 0, 0, 0, 0
//    };
//    pathMatrix matrix;
//    matrix.headers = points;
//    matrix.values = values;
//
//    arrayPath mst;
//    int totalWeight = kruskalMST(matrix, &mst);
//
//    printf("Edges in the MST:\n");
//    for (int i = 0; i < mst.size; ++i) {
//        printf("%s - %s: %d\n", mst.array[i].point1.name, mst.array[i].point2.name, mst.array[i].weight);
//    }
//    printf("Total weight of MST: %d\n", totalWeight);
//
//    free(mst.array);
//    return 0;
//}