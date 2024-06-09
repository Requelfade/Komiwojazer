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

void drukuj_porownanie(double time_1, double time_2, int  score_1, int score_2) {
    printf_s("\n\n*******Porownanie Metod Najblizszego Sasiada i Algorytmu Najkrotszych Krawedzi*******\n\n");

    char m2 = "ank";
    char m1 = "mns";
    char szybsza_metoda[50];
    char dokladna_metoda[50];
    double roznica_czasu;
    int roznica_wynik;


    if (time_1 > time_2) {
        char szybsza_metoda = m2;
        roznica_czasu = time_1 - time_2;
    }
    if (time_2 > time_1) {
        char szybsza_metoda = m1;
        roznica_czasu = time_2 - time_1;

    }
    if (score_1 > score_2) {
        char dokladna_metoda = m2;
        roznica_wynik = score_1 - score_2;
    }
    if (score_2 > score_1) {
        char dokladna_metoda = m1;
        roznica_wynik = score_2 - score_1;
    }

    printf_s("Szybsza metoda jest: %s \n", szybsza_metoda);
    printf_s("Roznica w czasie wykonania wyniosla %lf milisekund\n\n", roznica_czasu);
    printf_s("Dokladniejsza metoda jest: %s \n", dokladna_metoda);
    printf_s("Roznica w punktacji rozwiazania wyniosla %d jednostek", roznica_wynik);


    return;
}