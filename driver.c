/* 
 * The command used to compile the executable is
 * `gcc -Wall driver.c fcfs.c sjsf.c priority.c segment.c -o memMan`
 * The compiler used is gcc 9.4.0 on a Ubuntu 20.04.1 system
 * 
 * DOCUMENTATION
 * -----------------
 * The binary takes three commandline arguments in the format:
 * `./memMan -{algorithmFlag} {process_file_path} {total_memory}`
 * 
 * 
 * There are 4 algorithmFlags:
 * -f   will call the FCFS algorithm
 * -s   will call the SJSF algorithm
 * -S   will call the Segmentation memory management algorithm
 * -p   will call the Priority memory management algorithm
 * 
 * The flags are case-sensitive.
 * 
 * 
 * 
 * All process files are .txt files
 * The format of the process file is constant across all the flags,
 * HOWEVER, 
 * there are differences in the way they are interpreted, depending on the algorithmFlag set.
 * 
 * -f   {process_id} {process_name} {process_size} {dummy_values}\n...  
 * -s   {process_id} {process_name} {process_size} {dummy_values}\n...
 * 
 * -S   {process_id} {process_name} {process_size} {{segment1},{segment2},...{segment8}}\n...
 * 
 * -p   {process_id} {process_name} {process_size} {priority}\n...
 * 
 * 
 * 
 * -f, -s, -S, -p
 * -> The process file is a text file with each process described on *newlines*.
 * -> There are NO headers. All the fields for an indivdual process are *space* seperated. 
 * -> There can be NO space character in the process_name or process_id.
 * -> process_size MUST be a non-negative integer.
 * -> No field must exceed 32 characters (including the segments field).
 * 
 * -f, -s
 * It is recommended for the dummy values to be 0, however they can be any string as the field will not be read
 * by FCFS and SJSF algorithms.
 * 
 * -s
 * If a program has the same process_size, it will be allocated on a FCFS basis.
 * 
 * -S
 * There can be 8 or less segments in the current implementation of the algorithm.
 * The segments MUST add up to the process_size. If it doesn't, an error message will be shown and the program will exit.
 * 32 character limit is inclusive of the ',' that seperate the segments.
 * 
 * --> If this flag is appended with a v => `./memMan -Sv {process_file_path} {total_mem}`
 * this will enter the verbose mode of the Segmentation Algorithm.
 * It will print out extra details about how the process segments are laid out in memory and how much of the
 * process is allocated.
 * 
 * -p
 * -> priority field MUST be a non-negative integer.
 * -> if a process has the same priority then it will be allocated on a FCFS basis.
 * -> a lower value means a higher priority. the value 0 is given the highest priority.
 * ^ the above can be reversed easily,, this decision is arbitrary.
 * 
 * 
 * 
 * total_mem must be a non-negative integer.
 */



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// helper function
int IsInteger(char[]);

// Allocator Function
int SingleProgramAllocator(int);



// Initialisation of default sets
int Occupied[2] = {0, 0};
int Free[2] = {0, 0};


// decalaration of algorithms
extern void FCFSMemManage(char[][4][32], int);
extern void SJSFMemManage(char[][4][32], int);
extern void SegMemManage(char[][4][32], int, int);
extern void PriorityMemManage(char[][4][32], int);


int main(int argc, char *argv[]) {

    if (argc != 3 + 1) {
        printf("Invalid number of arguments\n");
        return 1;
    }

    if (argv[1][0] != '-') {
// not setting any default flags,, exits program
        printf("No flags set\n");
        return 1;
    }


// checking if the inputs are integers (not floats)
    if (!(IsInteger(argv[3]))) {
        printf("Total Memory should be a non-negative integer\n");
        return 0;
    }


    int total_mem = atoi(argv[3]);

// // checking if inputs are negative
//     if (total_mem < 0) {
//         printf("Total Memory should be non-negative\n");
//         return 0;    
//     }


    Free[1] = total_mem;



// an array of array of strings;
// the below array can hold 256 processes
// a process is made up of 3 strings, and those strings cant be
// longer than 32 characters 
    char processes[256][4][32];     // this is like 25 kB

    FILE *file = fopen(argv[2], "rt");
    char line[101];
    char pid[32];
    char pname[32];
    char psize[32];
// can be either/or
    char PriorSeg[32];

    // char process[3][32];
    int cur_line = 0;

    do {
    // no spaces allowed in process name
        fscanf(file, "%s %s %s %s", pid, pname, psize, PriorSeg);

// important distinction,,,, pid: [0]; pname: [1]; psize: [2]
        strcpy(processes[cur_line][0], pid);
        strcpy(processes[cur_line][1], pname);
        strcpy(processes[cur_line][2], psize);
        strcpy(processes[cur_line][3], PriorSeg);

        cur_line++;

    } while (fgets(line, 101, file) != NULL && cur_line != 256);

    fclose(file);

    
    int NumProcess = cur_line - 1;



    switch (argv[1][1]) {
    case 'f':
    case 'F':
        FCFSMemManage(processes, NumProcess);
        break;
    
    case 's':
        SJSFMemManage(processes, NumProcess);
        break;
    
    case 'S':
        ;
        int verbose = (argv[1][2] != '\0' && argv[1][2] == 'v') ? 1 : 0; 
        SegMemManage(processes, NumProcess, verbose);
        break;

    case 'p':
    case 'P':
// lower number is higher priority
// if priority same, then first come first serve
        PriorityMemManage(processes, NumProcess);
        break;

    default:
        printf("Invalid flag\n");
        break;
    }

    return 0;

}


int SingleProgramAllocator(int program_size) {
// boolean value

    int availability = Free[1] - Free[0] >= program_size;

    if (availability) {

        Free[0] += program_size;
        Occupied[1] += program_size;
    }

    return availability;
}


// helper function
int IsInteger(char arg[]) {

    for (int i = 0; arg[i] != 0; i++) {
        if (!isdigit(arg[i])) { return 0; }
    }
    return 1;
}
// the above can be refactored to include the non-negative check
// i thought it would be useful to keep the negative integer thing
// and now i don't want to refactor the code around that change