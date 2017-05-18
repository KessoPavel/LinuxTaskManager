#ifndef PROCESS
#define PROCESS

struct process{
    int pid;
    int ppid;
    char name[150];
    char owner[100];
    int priority;
    int cpu;
    char state[20];
    long int memory;
    float procTime;
    float coreTime;
};

process* initialization(const char*);

void update(process*);

int* getPids(int*);

process ** getProcess(int *, int);

process** updateProcess(process**, int, int*, int);

process * getProces(process **, int, int);

#endif // PROCESS

