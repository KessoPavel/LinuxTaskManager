#include "logic.h"
#include <stdio.h>
#include "mainwindow.h"

#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <pwd.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdlib.h>


char* memoryToString(long int memory){
    char * str = (char*)malloc(10);

    if(memory == -1){
        strcpy(str,"0 KB");
        return str;
    }

    const char * size[] = {"KB","MB","GB"};
    int n = 0;

    while (true) {
        if(memory >= 1024){
            n++;
            memory /= 1024;
            if(n == 2)
                break;
        }
        else
            break;
    }
    sprintf(str,"%ld %s",memory, size[n]);
    return str;
}

const char* getPriority(int prioruty){
    if(prioruty < -5)
        return "very high";
    if(prioruty < 0)
        return "high";
    if(prioruty == 0)
        return "normal";
    if(prioruty < 6)
        return "low";
    return "very low";
}

bool equals(int *p1, int *p2, int n1, int n2){
    if(n1 != n2)
        return false;
    for(int i = 0; i < n1; i++){
        if(p1[i]!=p2[i])
            return false;
    }
    return true;
}

