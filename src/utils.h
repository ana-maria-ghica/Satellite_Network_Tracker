/*
    Copyright (c) 2025 Ghica Ana-Maria
    All rights reserved

    Facultatea de Automatica si Calculatoare
    Seria CD, grupa 314
*/

#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// ---Structuri--------------------------------------

typedef struct satellite {
    int frequency;
    char *name;
    struct satellite *left;
    struct satellite *right;
    struct satellite *parent;
} *Satellite;

typedef struct minHeap {
    Satellite* satellites;
    int size;
    int capacity;
    int (*compare)(const void*, const void*);
} *MinHeap;

// ---Functii--------------------------------------

// generale
Satellite createSatellite(int frequency, char *name);
void deleteSatellite(Satellite* satellite);

int compare(const void *a, const void *b);
MinHeap createHeap(int capacity, int (*compare)(const void*, const void*));
void deleteHeap(Satellite root);

MinHeap ascending_sort(MinHeap heap, int nr_sateliti);
MinHeap siftDown(MinHeap heap, int index);
Satellite extractMin(MinHeap heap);
Satellite concatenateSatellites(Satellite left, Satellite right);
MinHeap siftUp(MinHeap heap, int index);
MinHeap insertHeap(MinHeap heap, Satellite new_satellite);

// taskul 1
void printLevel(Satellite root, int level, FILE* output_file);
int height(Satellite root);
void printLevelOrder(Satellite root, FILE* output_file);

// taskul 2
void decode_satellites(Satellite root, char *codificare, FILE* output_file);

// taskul 3
void encode_satellites(FILE* output_file,
    Satellite root, char *satellite_name, char *path, int index);

// taskul 4
Satellite find_satellite(Satellite root, char* name);
Satellite lowest_common_ancestor(Satellite root,
    Satellite first, Satellite second);

#endif
