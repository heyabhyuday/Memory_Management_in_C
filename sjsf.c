#include <time.h>
#include <stdio.h>
#include <stdlib.h>

extern int Occupied[2];
extern int Free[2];


int IsInteger(char[]);
int SingleProgramAllocator(int program_size);


int Compare(const void *pa, const void *pb) {
    const int *a = pa;
    const int *b = pb;
    return (a[0] == b[0]) ? a[1]-b[1] : a[0]-b[0]; 
}


void SJSFMemManage(char processes[][4][32], int NumProcess) {
    clock_t begin = clock();
    
    int SortIndex[NumProcess][2];

    for (int i = 0; i < NumProcess; i++) {
        if (IsInteger(processes[i][2])) {
            SortIndex[i][0] = atoi(processes[i][2]);
            SortIndex[i][1] = i;
        } else {
            printf("Invalid Input on line %d, cannot compute forward\n", i+1);
            return;
        }
        // printf("%d %d\n", SortIndex[i][0], SortIndex[i][1]);
    }

    qsort(SortIndex, NumProcess, sizeof SortIndex[0], Compare);

    int check = 1;

    for (int i = 0; i < NumProcess; i++) {
        int Process_idx = SortIndex[i][1];

        // printf("%d %d\n", SortIndex[i][0], SortIndex[i][1]);

        int psize = atoi(processes[Process_idx][2]);
        if (!psize) { printf("Invalid Input on line %d\n\n", Process_idx+1); continue; }
            
        printf("PID: %s;  Name: %s;  Size: %s\n", processes[Process_idx][0], processes[Process_idx][1], processes[Process_idx][2]);
// this comparison will be optimised by compiler,, will just look at check first
        if (check && SingleProgramAllocator(psize)) {
            printf("Memory Taken: %d %d\n", (Occupied[1] - psize), Occupied[1]);
            printf("Free Memory: %d %d\n\n", Free[0], Free[1]);
        } else {
            check = 0;
            printf("Memory Taken: %d %d\n", 0, 0);
            printf("Free Memory: %d %d\n\n", Free[0], Free[1]);
        }
    }

    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

    printf("Time (in milliseconds) spent allocating all the processes: %f\n", time_spent*1000);

}