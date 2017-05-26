#include "process.h"
#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <pwd.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdlib.h>
#include <unistd.h>


float _proc(const char*);
int _cpu();


process* initialization(const char * pid){
    process * proc = (process*)malloc(sizeof(process));
    char path[100] = "/proc/";
    strcat(path,pid);
    strcat(path,"/status");
    FILE * file = fopen(path,"r");
    if(file == NULL){
        return NULL;
    }

    char buff[100];

    proc->pid = atoi(pid);

    while(true){
        if(fscanf(file,"%s",buff)==EOF){
            proc->memory = -1;
            break;
        }

        if(!strcmp(buff,"Name:")){
            if(fscanf(file,"%s",buff)==EOF){
                return NULL;
            }
	    strncpy(proc->name,buff,sizeof(proc->name));
            continue;
        }
        if(!strcmp(buff,"State:")){
            if(fscanf(file,"%s%s",buff,buff)==EOF){
                return NULL;
            }
            strncpy(proc->state,&buff[1],strlen(buff)-2);
            proc->state[strlen(buff) - 2] = '\0';
            continue;
        }

        if(!strcmp(buff,"PPid:")){
            if(fscanf(file,"%s",buff)==EOF){
                return NULL;
            }
            proc->ppid = atoi(buff);
            continue;
        }
        if(!strcmp(buff,"Uid:")){
            if(fscanf(file,"%s",buff)==EOF){
                return NULL;
            }
            uid_t uid = atoi(buff);
            struct passwd * pswd = getpwuid(uid);
            if(pswd == NULL){
                proc->owner[0] = '\0';
            }
            else{
                strcpy(proc->owner,pswd->pw_name);
            }
            continue;
        }
        if(!strcmp(buff,"VmSize:")){
            if(fscanf(file,"%s",buff)==EOF){
                return NULL;
            }
            proc->memory = strtol(buff,NULL,10);
            break;
        }

    }
    fclose(file);

    strcpy(path,"/proc/");
    strcat(path,pid);
    strcat(path,"/stat");
    file = fopen(path,"r");
    if(file == NULL){
        return NULL;
    }
    for(int i = 0; i < 19;i++){
        if(fscanf(file,"%s",buff)==EOF){
            return NULL;
        }
    }
    fclose(file);
    proc->priority = atoi(buff);

    proc->procTime = _proc(pid);

    proc->coreTime = _cpu();

    proc->cpu = (proc->procTime * 100)/(proc->coreTime);

    return proc;
}

void update(process * proc){
    char buff[100];
    sprintf(buff,"%d",proc->pid);
    char path[20] = "/proc/";
    strcat(path,buff);
    strcat(path,"/status");
    FILE * file = fopen(path,"r");
    if(file == NULL){
        return;
    }

    while(true){
        if(fscanf(file,"%s",buff)==EOF){
            proc->memory = -1;
            break;
        }
        if(!strcmp(buff,"State:")){
            if(fscanf(file,"%s%s",buff,buff)==EOF){
                return;
            }
            strncpy(proc->state,&buff[1],strlen(buff)-2);
            proc->state[strlen(buff) - 2] = '\0';
            continue;
        }
        if(!strcmp(buff,"VmSize:")){
            if(fscanf(file,"%s",buff)==EOF){
                return;
            }
            proc->memory = strtol(buff,NULL,10);
            break;
        }

    }
    fclose(file);


    sprintf(buff,"%d",proc->pid);
    strcpy(path,"/proc/");
    strcat(path,buff);
    strcat(path,"/stat");
    file = fopen(path,"r");
    if(file == NULL){
        return;
    }
    for(int i = 0; i < 19;i++){
        if(i == 18){
            if(fscanf(file,"%s",buff)==EOF){
                return;
            }
            proc->priority = atoi(buff);
            break;
        }
        if(fscanf(file,"%s",buff)==EOF){
            return;
        }
    }
    fclose(file);

    sprintf(buff,"%d",proc->pid);

    float prev_procTime = proc->procTime;
    float prev_coreTime = proc->coreTime;

    char pid[6];
    sprintf(pid,"%d",proc->pid);

    proc->procTime = _proc(pid);
    proc->coreTime = _cpu();

    proc->cpu = ((prev_procTime - proc->procTime) *100)/(prev_coreTime - proc->coreTime);
}

int* getPids(int * n){
    DIR * proc = opendir("/proc");
    int temp;
    int size = 20;
    int m = 0;
    struct dirent * dir;
    int * pids = (int*)malloc(size*sizeof(int));
    while(true){
        dir = readdir(proc);
        if(dir != NULL){
            temp = atoi(dir->d_name);
            if(temp != 0){
                pids[m] = temp;
                m++;
            }
        }
        else{
            *n = m;
            return pids;

        }
        if(m == size){
            size += 10;
            pids = (int*)realloc(pids,size*sizeof(int));
        }
    }
}

process ** getProcess(int * pids, int n ){
    if(pids == NULL || n < 1)
        return NULL;
    process ** processList = (process**)malloc((n + 1) * sizeof(process*));
    char pid[6];

    for(int i = 0; i < n; i++){
        processList[i] = (process*)malloc(sizeof(process));
        sprintf(pid,"%d",pids[i]);
        processList[i] = initialization(pid);
    }
    return processList;
}

process ** updateProcess(process** proc, int n, int * pids, int nPids){
    process ** answer = (process**)malloc(nPids*sizeof(process*));

    for(int i = 0; i < nPids; i++){
        process * temp = getProces(proc, n, pids[i]);
        if(temp == NULL) {
            char pid[6];
            sprintf(pid,"%d",pids[i]);
            temp = initialization(pid);
            answer[i] = temp;
        }
        else {
            update(temp);
            answer[i] = temp;
        }
    }
    free(proc);
    return answer;
}

float _proc(const char* pid){
    char buff[100];
    char path[20] = "/proc/";
    strcat(path,pid);
    strcat(path,"/stat");
    FILE * file = fopen(path,"r");
    if(file == NULL){
        return 0;
    }
    int utime = 0, stime = 0;
    for(int i = 0; i < 17;i++){
        if(i == 13){
            if(fscanf(file,"%s",buff)==EOF){
                return 0;
            }
            utime = atoi(buff);
            continue;
        }
        if(i == 14){
            if(fscanf(file,"%s",buff)==EOF){
                return 0;
            }
            stime = atoi(buff);
            break;
        }
        if(fscanf(file,"%s",buff)==EOF){
            return 0;
        }
    }
    fclose(file);
    return (float)(utime + stime);

}

int _cpu(){
    char buff[100];
    FILE * file = fopen("/proc/stat","r");
    if(file == NULL){
        return 0;
    }

    if(fscanf(file,"%s",buff)==EOF){
        return 0;
    }

    int cputime = 0;

    while(true){
        if(fscanf(file,"%s",buff)==EOF){
            break;
        }
        if(!strcmp(buff,"cpu0"))
            break;
        cputime += atoi(buff);
    }
    fclose(file);
    return cputime;
}

process * getProces(process ** proc, int n, int pid) {
    for(int i = 0; i < n; i++){
        if(proc[i]->pid == pid)
            return proc[i];
    }
    return NULL;
}
