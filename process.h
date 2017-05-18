#ifndef PROCESS
#define PROCESS

struct process{
    int pid;
    int ppid;
    char name[100];
    char owner[100];
    int priority;
    int cpu;
    char state[20];
    long int memory;
};

process* initialization(const char*);

void update(process*);

int* getPids(int*);

process ** getProcess(int *, int);

void updateProcess(process**, int);

process * getProces(process **, int, int);

#endif // PROCESS

