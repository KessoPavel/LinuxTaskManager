#include "raminfo.h"
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

int total;

void initMem(){
    char buff[100];
    char path[] = "/proc/meminfo";
    FILE * file = fopen(path,"r");
    if(file == NULL){
        total = -1;
    }
    if(fscanf(file,"%s",buff)==EOF){
        total = -1;
    }

    if(fscanf(file,"%s",buff)==EOF){
        total = -1;
    } else {
        total = atoi(buff);
    }
}

int getMemTotal() {
    return total;
}

int getMemFree() {
    char buff[100];
    char path[] = "/proc/meminfo";
    FILE * file = fopen(path,"r");
    if(file == NULL){
        return -1;
    }
    for(int i = 0; i < 4; i++){
        if(fscanf(file,"%s",buff)==EOF){
            return -1;
        }
    }

    if(fscanf(file,"%s",buff)==EOF){
        return -1;
    } else {
        return atoi(buff);
    }
}
