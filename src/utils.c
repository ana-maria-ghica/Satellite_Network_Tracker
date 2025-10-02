/*
    Copyright (c) 2025 Ghica Ana-Maria
    All rights reserved

    Facultatea de Automatica si Calculatoare
    Seria CD, grupa 314
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"

#define MAX_CONCATENATED_NAME 250
#define ASCII_0 48

// generale
Satellite createSatellite(int frequency, char *name) {
    Satellite satellite = malloc(sizeof(struct satellite));
    if (!satellite) {
        printf("Eroare la alocarea dinamica a unui satelit.\n");
        return NULL;
    }
    satellite->frequency = frequency;
    satellite->name = strdup(name);
    if (!satellite->name) {
        printf("Eroare la alocarea dinamica a numelui satelitului.\n");
        free(satellite);
        return NULL;
    }

    satellite->left = NULL;
    satellite->right = NULL;
    satellite->parent = NULL;

    return satellite;
}

void deleteSatellite(Satellite* satellite) {
    if (*(satellite) == NULL)
        return;

    free((*satellite)->name);

    (*satellite)->left = NULL;
    (*satellite)->right = NULL;
    (*satellite)->parent = NULL;

    free(*satellite);
    *satellite = NULL;
    return;
}

int compare(const void *a, const void *b) {
    Satellite first = (Satellite) a;
    Satellite second = (Satellite) b;
    if (first->frequency != second->frequency)
        return (first->frequency - second->frequency);
    return strcmp(first->name, second->name);
}

MinHeap createHeap(int capacity, int (*compare)(const void*, const void*)) {
    MinHeap heap = malloc(sizeof(struct minHeap));
    if (!heap) {
        printf("Eroare la alocarea dinamica a min-heap-ului.\n");
        return NULL;
    }
    heap->size = 0;
    heap->capacity = capacity;

    heap->satellites = calloc(heap->capacity,
        sizeof(struct satellite*));  // le si initializeaza cu NULL

    if (!heap->satellites) {
        printf("Eroare la alocarea dinamica a array-ului de sateliti.\n");
        free(heap);
        return NULL;
    }
    heap->compare = compare;

    return heap;
}

void deleteHeap(Satellite root) {
    if (root == NULL)
        return;

    deleteHeap(root->left);
    deleteHeap(root->right);

    deleteSatellite(&root);
}

MinHeap ascending_sort(MinHeap heap, int nr_sateliti) {
    for (int i = 0; i < nr_sateliti - 1; i++) {
        for (int j = i+1; j < nr_sateliti; j++) {
            if (heap->satellites[i]->frequency >
                    heap->satellites[j]->frequency) {
                Satellite aux = heap->satellites[i];
                heap->satellites[i] = heap->satellites[j];
                heap->satellites[j] = aux;
            }
        }
    }

    // sortez si in ordine alfabetica
    for (int i = 0; i < nr_sateliti - 1; i++) {
        for (int j = i+1; j < nr_sateliti; j++) {
            if (heap->satellites[i]->frequency ==
                    heap->satellites[j]->frequency) {
                if (strcmp(heap->satellites[i]->name,
                        heap->satellites[j]->name) > 0) {
                    Satellite aux = heap->satellites[i];
                    heap->satellites[i] = heap->satellites[j];
                    heap->satellites[j] = aux;
                }
            }
        }
    }
    return heap;
}

MinHeap siftDown(MinHeap heap, int index) {
    int max_index = index;
    int left = 2*index + 1;
    int right = 2*index + 2;

    if (left < heap->size &&
        heap->compare(heap->satellites[left], heap->satellites[max_index]) < 0)
        max_index = left;

    if (right < heap->size &&
        heap->compare(heap->satellites[right], heap->satellites[max_index]) < 0)
        max_index = right;

    if (index != max_index) {
        Satellite aux = heap->satellites[index];
        heap->satellites[index] = heap->satellites[max_index];
        heap->satellites[max_index] = aux;

        siftDown(heap, max_index);
    }

    return heap;
}

Satellite extractMin(MinHeap heap) {
    if (heap->size == 0) {
        return NULL;
    }

    Satellite min = heap->satellites[0];
    heap->satellites[0] = heap->satellites[heap->size - 1];
    heap->size--;

    siftDown(heap, 0);

    return min;
}

Satellite concatenateSatellites(Satellite left, Satellite right) {
    int new_frequency = left->frequency + right->frequency;
    char *new_name = malloc(MAX_CONCATENATED_NAME * sizeof(char));
    // strcpy(new_name, left->name);
    // strcat(new_name, right->name);
    snprintf(new_name, MAX_CONCATENATED_NAME, "%s%s", left->name, right->name);

    Satellite new_satellite = createSatellite(new_frequency, new_name);

    free(new_name);

    new_satellite->left = left;
    new_satellite->right = right;

    left->parent = right->parent = new_satellite;

    return new_satellite;
}

MinHeap siftUp(MinHeap heap, int index) {
    while (index > 0 &&
        (heap->compare(heap->satellites[index],
            heap->satellites[(index - 1) / 2]) < 0)) {
        Satellite aux = heap->satellites[(index - 1) / 2];
        heap->satellites[(index - 1) / 2] = heap->satellites[index];
        heap->satellites[index] = aux;
        index = (index - 1) / 2;
    }
    return heap;
}

MinHeap insertHeap(MinHeap heap, Satellite new_satellite) {
    if (heap->size == heap->capacity) {
        heap->capacity *= 2;
        Satellite* aux_array =
            realloc(heap->satellites, heap->capacity * sizeof(Satellite));
        if (!aux_array) {
            printf("Eroare la realocarea noului array de sateliti.\n");
            return NULL;
        }
        heap->satellites = aux_array;
    }
    heap->satellites[heap->size] = new_satellite;
    heap->size++;

    siftUp(heap, heap->size - 1);

    return heap;
}

// taskul 1
void printLevel(Satellite root, int level, FILE* output_file) {
    if (root == NULL)
        return;
    if (level == 0) {
        fprintf(output_file, "%d-%s ", root->frequency, root->name);
        return;
    }
    printLevel(root->left, level - 1, output_file);
    printLevel(root->right, level - 1, output_file);
}

int height(Satellite root) {
    if (root == NULL || (root->left == NULL && root->right == NULL) )
        return 0;

    int left, right;
    left = height(root->left);
    right = height(root->right);

    if (left < right)
        return right + 1;

    return left + 1;
}

void printLevelOrder(Satellite root, FILE* output_file) {
    int i, h = height(root);

    for (i = 0; i <= h; i++) {
        printLevel(root, i, output_file);
        fprintf(output_file, "\n");
    }
}

//  taskul 2
void decode_satellites(Satellite root, char *codificare, FILE* output_file) {
    Satellite copy_root = root;

    int len = strlen(codificare);

    for (int i = 0; i < len; i++) {
        // ca sa ajung la valoarea de 0 sau 1
        int current_bit = codificare[i] - ASCII_0;

        if (current_bit == 0)
            root = root->left;
        else
            root = root->right;

        if (root->left == NULL && root->right == NULL) {
            fprintf(output_file, "%s ", root->name);
            root = copy_root;
        }
    }
}

// taskul 3
void encode_satellites(FILE* output_file, Satellite root,
    char *satellite_name, char *path, int index) {
    if (root == NULL)
        return;

    if (root->left == NULL && root->right == NULL) {
        if (strcmp(root->name, satellite_name) == 0) {
            path[index] = '\0';
            fprintf(output_file, "%s", path);
        } else {
            return;
        }
    }

    if (root->left != NULL) {
        path[index] = '0';
        encode_satellites(output_file,
            root->left, satellite_name, path, index+1);
    }

    if (root->right != NULL) {
        path[index] = '1';
        encode_satellites(output_file,
            root->right, satellite_name, path, index+1);
    }
}

// taskul 4
Satellite find_satellite(Satellite root, char* name) {
    if (root == NULL)
        return NULL;

    if (strcmp(root->name, name) == 0)
        return root;

    Satellite left = find_satellite(root->left, name);
    if (left != NULL)
        return left;

    Satellite right = find_satellite(root->right, name);
    if (right != NULL)
        return right;

    return NULL;
}

Satellite lowest_common_ancestor
    (Satellite root, Satellite first, Satellite second) {
    if (root == NULL)
        return NULL;

    // daca unull din sateliti e radacina
    // atunci trb sa returnez din start radacina
    if (first == root || second == root)
        return root;

    // caut in subarborele stang acum
    Satellite left = lowest_common_ancestor(root->left, first, second);

    // caut si in subarborele drept
    Satellite right = lowest_common_ancestor(root->right, first, second);

    if (left != NULL && right != NULL)
        return root;

    if (left != NULL)
        return left;
    else
        return right;
}
