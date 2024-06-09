#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "point.h"
#include "ANK.h"

// Funkcje pomocnicze do sortowania kraw�dzi
int compare(const void* a, const void* b) {
    path pathA = *(path*)a;
    path pathB = *(path*)b;
    return ComparePaths(pathA, pathB);
}

// Znajdowanie korzenia zestawu z kompresj� �cie�ki
point find(subset subsets[], point p) {
    if (!ComparePoints(subsets[p.n].parent, p)) {
        subsets[p.n].parent = find(subsets, subsets[p.n].parent);
    }
    return subsets[p.n].parent;
}

// ��czenie dw�ch zestaw�w
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

    // Zliczanie liczby kraw�dzi
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

    // Sortowanie wszystkich kraw�dzi
    qsort(paths.array, paths.size, sizeof(path), compare);

    // Alokowanie pami�ci dla zestaw�w
    subset* subsets = (subset*)malloc(matrix.headers.size * sizeof(subset));
    for (int i = 0; i < matrix.headers.size; ++i) {
        subsets[i].parent = matrix.headers.array[i];
        subsets[i].rank = 0;
    }

    // Licznik kraw�dzi dla ka�dego punktu
    int* edgeCount = (int*)calloc(matrix.headers.size, sizeof(int));

    // Wybieranie kraw�dzi
    for (int i = 0; i < paths.size; ++i) {
        path nextPath = paths.array[i];

        point x = find(subsets, nextPath.point1);
        point y = find(subsets, nextPath.point2);

        // Sprawdzanie, czy dodanie tej kraw�dzi nie spowoduje, �e kt�rykolwiek punkt b�dzie mia� wi�cej ni� 2 kraw�dzie
        if (!ComparePoints(x, y) && edgeCount[nextPath.point1.n] < 2 && edgeCount[nextPath.point2.n] < 2) {
            resultMST->array[resultMST->size++] = nextPath;
            totalWeight += nextPath.weight;
            Union(subsets, x, y);
            edgeCount[nextPath.point1.n]++;
            edgeCount[nextPath.point2.n]++;
        }
    }

    // Dodanie kraw�dzi zamykaj�cej obieg
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

void drukuj_porownanie(long double time_1, long double  time_2, int score_1, int score_2) {
    printf_s("\n\n*******Porownanie Metod Najblizszego Sasiada i Algorytmu Najkrotszych Krawedzi*******\n\n");

    char szybsza_metoda[50]; 
    char dokladna_metoda[50];
    long double  roznica_czasu;
    int roznica_wynik;
    long double  procent_czasu;
    long double  procent_wyniku;

    // Wyb�r szybszej metody
    if (time_1 < time_2) {
        strcpy_s(szybsza_metoda, sizeof(szybsza_metoda), "Algorytm Najblizszego Sasiada");
        roznica_czasu = time_2 - time_1;
        procent_czasu = (long double) roznica_czasu / time_2 * 100;
    }
    else {
        strcpy_s(szybsza_metoda, sizeof(szybsza_metoda), "Algorytm Najkrotszych Krawedzi");
        roznica_czasu = time_1 - time_2;
        procent_czasu = (long double) roznica_czasu / time_1 * 100;
    }

    // Wyb�r dok�adniejszej metody
    if (score_1 < score_2) {
        strcpy_s(dokladna_metoda, sizeof(dokladna_metoda), "Algorytm Najblizszego Sasiada");
        roznica_wynik = score_2 - score_1;
        procent_wyniku = (double) roznica_wynik / score_2 *100;
    }
    else {
        strcpy_s(dokladna_metoda, sizeof(dokladna_metoda), "Algorytm Najkrotszych Krawedzi");
        roznica_wynik = score_1 - score_2;
        procent_wyniku = (double) roznica_wynik / score_1 * 100;
    }
    
    // Wnioski
    printf_s("Szybsza metoda jest: %s\n", szybsza_metoda);
    printf_s("Roznica w czasie wykonania wyniosla %lf milisekund\n", roznica_czasu);
    printf_s("Czyli %lf procent dluzszego procesu\n", procent_czasu);
    printf_s("dokladnosc pomiaru czasu jest ograniczona czestotliwoscia tickera fuckji *colck()*\n");

    printf_s("\nDokladniejsza metoda jest: %s\n", dokladna_metoda);
    printf_s("Roznica w punktacji rozwiazania wyniosla %d jednostek\n", roznica_wynik);
    printf_s("Czyli %lf procent nizszego wyniku\n", procent_wyniku);

   

    return;
}

