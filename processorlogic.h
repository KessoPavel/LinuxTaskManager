#ifndef PROCESSORLOGIC
#define PROCESSORLOGIC

struct CPU {
    unsigned long long busy;
    unsigned long long work;
    CPU * cpux;
};

void processorInfinit();
char * getProcessorName();
int getProcessorNumberOfCore();
void cpuInit(CPU *);
int* CPU_usage(CPU *);

#endif // PROCESSORLOGIC

