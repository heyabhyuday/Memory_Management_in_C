#include <time.h>
#include <stdio.h>
#include <stdlib.h>

extern int Occupied[2];
extern int Free[2];


int IsInteger(char[]);
int SingleProgramAllocator(int program_size);


void FCFSMemManage(char processes[][4][32], int NumProcess) {
    clock_t begin = clock();
    

    for (int i = 0; i < NumProcess; i++) {

        if (IsInteger(processes[i][2])) {
            int psize = atoi(processes[i][2]);
            if (!psize) { printf("Invalid Input on line %d\n\n", i+1); continue; }

            
            printf("PID: %s;  Name: %s;  Size: %s\n", processes[i][0], processes[i][1], processes[i][2]);
            if (SingleProgramAllocator(psize)){
                printf("Memory Taken: %d %d\n", (Occupied[1] - psize), Occupied[1]);
                printf("Free Memory: %d %d\n\n", Free[0], Free[1]);
            } else {
                printf("Memory Taken: %d %d\n", 0, 0);
                printf("Free Memory: %d %d\n\n", Free[0], Free[1]);
            }
        } else {
            printf("Invalid Input on line %d\n\n", i+1);
            continue;
        }
    
    }

    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

    printf("Time (in milliseconds) spent allocating all the processes: %f\n", time_spent*1000);
}