#ifndef PROCESSORLOGIC
#define PROCESSORLOGIC

struct CPU {
    unsigned long long busy;
    unsigned long long work;
};

void processorInfinit();
char * getProcessorName();
int getProcessorNumberOfCore();
char * getProcessorFrequency();
void cpuInit(CPU *);
int CPU_usage(CPU *);

#endif // PROCESSORLOGIC

