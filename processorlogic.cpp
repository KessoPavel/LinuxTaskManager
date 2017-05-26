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
    int numberOfCores;
}processor;

void processorInfinit()
{

    processor.numberOfCores = 0;
    char buff[100];
    FILE * file = fopen("/proc/cpuinfo","r");
    if(file == NULL){
        return;
    }

    for(int i = 0; i < 16;i++) {
        if(fscanf(file,"%s",buff)==EOF){
            strcpy(processor.name,"");
            processor.numberOfCores = 0;
            return;
        }
    }

    if(fgets(buff,100,file) == NULL){
        fclose(file);
        return;
    }
    strncpy(processor.name,buff,strlen(buff) - 1);

    fclose(file);


    file = fopen("/proc/stat", "r");

    char cpuX[10];
    sprintf(cpuX,"cpu%d",processor.numberOfCores);

    while(1){
        if(fscanf(file,"%s",buff)==EOF){
            fclose(file);
            return;
        }
        if(!strcmp(buff,cpuX)){
            processor.numberOfCores++;
            sprintf(cpuX,"cpu%d",processor.numberOfCores);
        }
    }

}

char *getProcessorName() {
    return processor.name;
}

int getProcessorNumberOfCore() {
    return processor.numberOfCores;
}

int* CPU_usage(CPU * mem) {
    int * answer = (int*)malloc((processor.numberOfCores + 1) * sizeof(int));
    unsigned long long cpu = 0;
    unsigned long long nice = 0;
    unsigned long long system = 0;
    unsigned long long idle = 0;

    FILE *f = fopen("/proc/stat", "r");
    if (fscanf(f, "cpu %llu %llu %llu %llu", &cpu, &nice, &system, &idle) < 4)
    {
        return NULL;
    }

    unsigned long long new_busy = cpu + nice + system;
    unsigned long long new_work = new_busy + idle;

    double usage = 100.0 * (new_busy - mem->busy ) / (double) (new_work - mem->work);

    answer[0] = (int)usage;

    mem->busy = new_busy;
    mem->work = new_work;

    char buff[100];
    for(int i = 0; i < processor.numberOfCores; i++ ){
        char cpuX[10];
        sprintf(cpuX,"cpu%d",i);
        while(1){
            if(fscanf(f,"%s",buff)==EOF){
                fclose(f);
                return answer;
            }
            if(!strcmp(buff,cpuX)){
                if (fscanf(f, "%llu %llu %llu %llu", &cpu, &nice, &system, &idle) < 4){
                    fclose(f);
                    return answer;
                }
                new_busy = cpu + nice + system;
                new_work = new_busy + idle;

                usage = 100.0 * (new_busy - mem->cpux[i].busy ) / (double) (new_work - mem->cpux[i].work);

                mem->cpux[i].busy = new_busy;
                mem->cpux[i].work = new_work;

                answer[i + 1] = (int)usage;

                break;
            }
        }
    }
    fclose(f);

    return answer;
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

    unsigned long long new_busy = cpu + nice + system;
    unsigned long long new_work = new_busy + idle;

    mem->busy = new_busy;
    mem->work = new_work;

    mem->cpux = (CPU*)malloc(processor.numberOfCores * sizeof(CPU));

    char buff[100];
    for(int i = 0; i < processor.numberOfCores; i++ ){
        char cpuX[10];
        sprintf(cpuX,"cpu%d",i);
        while(1){
            if(fscanf(f,"%s",buff)==EOF){
                fclose(f);
                return;
            }
            if(!strcmp(buff,cpuX)){
                if (fscanf(f, "%llu %llu %llu %llu", &cpu, &nice, &system, &idle) < 4){
                    fclose(f);
                    return;
                }
                new_busy = cpu + nice + system;
                new_work = new_busy + idle;
                mem->cpux[i].busy = new_busy;
                mem->cpux[i].work = new_work;
                mem->cpux[i].cpux = NULL;
                break;
            }
        }
    }
    fclose(f);
}


