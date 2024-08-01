#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern int Occupied[2];
extern int Free[2];

int IsInteger(char[]);

// this will now allocate segments
int SingleProgramAllocator(int program_size);


void SegMemManage(char processes[][4][32], int NumProcess, int verbose) {
    clock_t begin = clock();

    int segments[NumProcess][8];
    memset(segments, 0, sizeof(segments));

    for (int i = 0; i < NumProcess; i++) {
        char *token = strtok(processes[i][3], ",");
        int j = 0;
        
        int SegmentSum = 0;

        while (token != NULL) {
            if (!IsInteger(token)) { 
                printf("Invalid Input on line %d, cannot compute forward\n", i+1);
                return;
            }
            
            segments[i][j] = atoi(token);

            SegmentSum += segments[i][j];

            j++;
            token = strtok(NULL, ",");
        }

        if (SegmentSum != atoi(processes[i][2])) {
            printf("Segments don't add up to the program size in line %d, cannot compute forward\n", i+1);
            return;
        }

    }

/*
    for (int i = 0; i < NumProcess; i++) {
        int j = 0;
        do {
            printf("%d ", segments[i][j]);
            j++;
        } while (segments[i][j] != 0);
        printf("\n");
    }
*/
    int MemImage[NumProcess][8][2];
    memset(MemImage, 0, sizeof(MemImage));

    int check = 0;
    for (int j = 0; j < 8; j++) {
        if (check == NumProcess) { break; }
        for (int i = 0; i < NumProcess; i++) {
            if (segments[i][j]) {
                printf("PID: %s;  Name: %s;  Size: %s\n", processes[i][0], processes[i][1], processes[i][2]);
                printf("Segment %d; Size: %d\n", j+1, segments[i][j]);
                if (SingleProgramAllocator(segments[i][j])) {
                    printf("Memory Taken: %d %d\n", (Occupied[1] - segments[i][j]), Occupied[1]);
                    printf("Free Memory: %d %d\n\n", Free[0], Free[1]);

                    MemImage[i][j][0] = Occupied[1] - segments[i][j];
                    MemImage[i][j][1] = Occupied[1];

                } else {
                    printf("Memory Taken: %d %d\n", 0, 0);
                    printf("Free Memory: %d %d\n\n", Free[0], Free[1]);
                }
            } else {
                check++;
                continue;
            }
        }

    }

    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

    printf("Time (in milliseconds) spent allocating all the processes: %f\n", time_spent*1000);

    if (verbose) {
        printf("\nMemory Image:\n");
        for (int i = 0; i < NumProcess; i++) {
            int allocated = 0;

            printf("PID: %s;  Name: %s\n", processes[i][0], processes[i][1]);
            int j = 0;
            while (MemImage[i][j][1] && j != 8) {
                allocated += (MemImage[i][j][1] - MemImage[i][j][0]);

                printf("[%d, %d) ", MemImage[i][j][0], MemImage[i][j][1]);
                j++;
            }
            printf("\nAllocated %d out of %s memory units of the process\n\n", allocated, processes[i][2]);
        }
    }

}