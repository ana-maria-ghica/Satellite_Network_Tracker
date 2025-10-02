/*
    Copyright (c) 2025 Ghica Ana-Maria
    All rights reserved

    Facultatea de Automatica si Calculatoare
    Seria CD, grupa 314
*/

/*
main.c -> main
tema2.c -> utils.c
tema2.h -> utils.h
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"

#define BUFFER_SIZE 20
#define MAX_NAME_SIZE 15
#define MAX_CODIFICARE 300
#define ENCODED_PATH_CAPACITY 250

int main(int argc, char* argv[]) {
    if (argc != 4) {
        printf("Introduceti in linia de comanda astfel:\n");
        printf("./main [-c1 |-c2 |-c3 |-c4 |-c5] [input] [output]\n");
        return 0;
    }

    int cerinta = 0;
    if (strstr(argv[1], "1") != NULL) {
        cerinta = 1;
    } else if (strstr(argv[1], "2") != NULL) {
        cerinta = 2;
    } else if (strstr(argv[1], "3") != NULL) {
        cerinta = 3;
    } else if (strstr(argv[1], "4") != NULL) {
        cerinta = 4;
    } else if (strstr(argv[1], "5") != NULL) {
        cerinta = 5;
    }

    // prelucrez datele de intrare generale
    // apoi impart cazurile in functie de taskurile propriu zise

    FILE* input_file = fopen(argv[2], "r");
    FILE* output_file = fopen(argv[3], "w");

    int nr_sateliti;
    fscanf(input_file, "%d", &nr_sateliti);
    fgetc(input_file);

    MinHeap heap = createHeap(nr_sateliti, compare);
    if (!heap) {
        printf("Eroare la alocarea memoriei pentru heap-ul din main.\n");
        fclose(input_file);
        fclose(output_file);
        return 0;
    }

    for (int i = 0; i < nr_sateliti; i++) {
        // citesc din fisier urmatoarele linii
        // cu datele despre fiecare satelit in parte
        int frequency;
        char name[MAX_NAME_SIZE];
        fscanf(input_file, "%d %s", &frequency, name);
        fgetc(input_file);

        // le atribui pentru fiecare satelit alocat dinamic
        heap->satellites[i] = createSatellite(frequency, name);
    }

    // sortez array-ul initial ca sa pot sa obtin frecventele cele mai mici
    heap = ascending_sort(heap, nr_sateliti);

    heap->size = nr_sateliti;

    // extrag din heap, si formez noul nod
    while (heap->size > 1) {
        // extrag minimul pt left
        Satellite left_satellite = extractMin(heap);

        // extrag minimul pt right
        Satellite right_satellite = extractMin(heap);

        // concatenez nodurile
        Satellite new_satellite =
            concatenateSatellites(left_satellite, right_satellite);

        // plasez rezultatul inapoi in heap
        heap = insertHeap(heap, new_satellite);
    }

    // consider root-ul pentru arborele creat
    Satellite root = heap->satellites[0];

    if (cerinta == 1) {
        // afisarea pe nivel
        printLevelOrder(root, output_file);

    } else if (cerinta == 2) {
        // continui de citit datele din fisier
        int nr_codificari;
        fscanf(input_file, "%d", &nr_codificari);
        fgetc(input_file);

        // algoritmul de dcodificare pt fiecare input primit
        for (int i = 0; i < nr_codificari; i++) {
            char codificare[MAX_CODIFICARE];
            fscanf(input_file, "%s", codificare);
            fgetc(input_file);

            decode_satellites(root, codificare, output_file);

            fprintf(output_file, "\n");
        }

    } else if (cerinta == 3) {
        // continui sa citesc datele din fisier
        int nr_satellites_name;
        fscanf(input_file, "%d", &nr_satellites_name);
        fgetc(input_file);

        for (int i = 0; i < nr_satellites_name; i++) {
            char satellite_name[20];
            fscanf(input_file, "%s", satellite_name);
            fgetc(input_file);

            char *path = malloc(10 * sizeof(char));
            if (!path) {
                printf("Eroare la alocarea dinamica a path-ului.\n");
                deleteHeap(root);
                free(heap->satellites);
                free(heap);
                fclose(input_file);
                fclose(output_file);
            }

            encode_satellites(output_file, root, satellite_name, path, 0);

            free(path);
        }
    } else if (cerinta == 4) {
        // continui sa citesc datele din fisier
        int nr_satellites_name;
        fscanf(input_file, "%d", &nr_satellites_name);
        fgetc(input_file);

        // identific din lista de input satelitul cu numele dat
        char name[10];
        fscanf(input_file, "%s", name);
        fgetc(input_file);

        Satellite sat1 = find_satellite(root, name);

        // identific din lista de input al doilea satelit cu numele dat
        fscanf(input_file, "%s", name);
        fgetc(input_file);

        Satellite sat2 = find_satellite(root, name);

        // calculez stramosul comun pentru cei 2 sateliti initiali
        Satellite ancestor = lowest_common_ancestor(root, sat1, sat2);

        // ca sa nu mai iau in calcul satelitii deja analizati
        nr_satellites_name -= 2;

        for (int i = 0; i < nr_satellites_name; i++) {
            // iterez prin lista de sateliti si determin urmatorul satelit
            fscanf(input_file, "%s", name);
            fgetc(input_file);

            Satellite sat3 = find_satellite(root, name);

            // calculez stramosul in mod "recursiv" pentru urmatorii sateliti
            // pe care ii consider 2 cate 2
            Satellite new_ancestor =
                lowest_common_ancestor(root, ancestor, sat3);

            // atribui noul stramos determinat, iar cautarea continua
            ancestor = new_ancestor;
        }
        fprintf(output_file, "%s", ancestor->name);
    }

    // eliberarea memoriei
    deleteHeap(root);

    free(heap->satellites);
    free(heap);

    // inchid fisierele
    fclose(input_file);
    fclose(output_file);

    return 0;
}
