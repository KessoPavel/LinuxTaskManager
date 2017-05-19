#include "processorlogic.h"
#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <pwd.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

struct processoriInf {
    char name[100];
    char frequency[10];
    int numberOfCores;
}processor;

void processorInfinit()
{

    char buff[100];
    FILE * file = fopen("/proc/cpuinfo","r");
    if(file == NULL){
        return;
    }

    for(int i = 0; i < 16;i++) {
        if(fscanf(file,"%s",buff)==EOF){
            strcpy(processor.name,"");
            strcpy(processor.frequency,"");
            processor.numberOfCores = 0;
            return;
        }
    }

    fgets(buff,100,file);
    strncpy(processor.name,buff,strlen(buff) - 1);

    for(int i = 0; i < 9; i++) {
        if(fscanf(file,"%s",buff)==EOF){
            strcpy(processor.frequency,"");
            processor.numberOfCores = 0;
            return;
        }
    }

    if(fscanf(file,"%s",processor.frequency)==EOF){
        strcpy(processor.frequency,"");
        processor.numberOfCores = 0;
        return;
    }

    for(int i = 0; i< 20; i++) {
        if(fscanf(file,"%s",buff)==EOF){
            strcpy(processor.frequency,"");
            processor.numberOfCores = 0;
            return;
        }
    }

    processor.numberOfCores = atoi(buff);
}

char *getProcessorName() {
    return processor.name;
}

int getProcessorNumberOfCore() {
    return processor.numberOfCores;
}

char *getProcessorFrequency() {
    return processor.frequency;
}

int CPU_usage(CPU * mem) {
    unsigned long long cpu = 0;
    unsigned long long nice = 0;
    unsigned long long system = 0;
    unsigned long long idle = 0;

    FILE *f = fopen("/proc/stat", "r");
    if (fscanf(f, "cpu %llu %llu %llu %llu", &cpu, &nice, &system, &idle) < 4)
    {
        fclose(f);
        return -1;
    }
    fclose(f);

    unsigned long long new_busy = cpu + nice + system;
    unsigned long long new_work = new_busy + idle;

    double usage = 100.0 * (new_busy - mem->busy ) / (double) (new_work - mem->work);


    mem->busy = new_busy;
    mem->work = new_work;

    return (int) usage;
}

void cpuInit(CPU * mem) {
    unsigned long long cpu = 0;
    unsigned long long nice = 0;
    unsigned long long system = 0;
    unsigned long long idle = 0;

    FILE *f = fopen("/proc/stat", "r");
    if (fscanf(f, "cpu %llu %llu %llu %llu", &cpu, &nice, &system, &idle) < 4)
    {
        fclose(f);
        return;
    }
    fclose(f);

    unsigned long long new_busy = cpu + nice + system;
    unsigned long long new_work = new_busy + idle;

    mem->busy = new_busy;
    mem->work = new_work;
}
