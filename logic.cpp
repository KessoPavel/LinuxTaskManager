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
#include <pthread.h>
#include <string.h>

/**
 * @brief The inform struct
 */
struct inform {
  int pid;
  } inf;

/**
 * @brief memoryToString
 * @param memory - сontains the amount of memory in kb
 * @return - string containing the amount of memory in the format xx GB xx MB xx KB
 */
char* memoryToString(long int memory) {
    char * str = (char*)malloc(10);

    if(memory == -1) {
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

/**
 * @brief getPriority
 * @param prioruty - contains priority
 * @return - string containing process priority
 */
const char* getPriority(int prioruty) {
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

/**
 * @brief getPidNumber
 * @param proc
 * @param n
 * @param pid
 * @return
 */
int getPidNumber(process ** proc, int n, int pid) {
    for(int i = 0; i < n; i++) {
	if(proc[i] == NULL)
	  continue;
	if(proc[i]->pid == pid)
	  return i;
      }
    return -1;
  }

/**
 * @brief sort
 * @param proc
 * @param n
 * @param coll
 * @param less
 */
void sort(process ** proc, int n, int coll, int less) {
    switch(coll) {
      case 0: { //name
	  for(int i = 0; i < n; i++) {
	      if(proc[i] == NULL) {
		  continue;
		}
	      for(int j = i; j < n; j++) {
		  if(proc[j] == NULL) {
		      continue;
		    }
		  if(less == 1) {
		      if(strcmp(proc[i]->name,proc[j]->name) >= 0) {
			  process * temp = proc[i];
			  proc[i] = proc[j];
			  proc[j] = temp;
			}
		    } else {
		      if(strcmp(proc[j]->name,proc[i]->name) >= 0) {
			  process * temp = proc[i];
			  proc[i] = proc[j];
			  proc[j] = temp;
			}
		    }
		}
	    }
	  break;
	}
      case 1: { //pid
	  for(int i = 0; i < n; i++) {
	      if(proc[i] == NULL) {
		  continue;
		}
	      for(int j = i; j < n; j++) {
		  if(proc[j] == NULL) {
		      continue;
		    }
		  if(less == 1) {
		      if(proc[i]->pid >= proc[j]->pid) {
			  process * temp = proc[i];
			  proc[i] = proc[j];
			  proc[j] = temp;
			}
		    } else {
		      if(proc[j]->pid >= proc[j]->pid) {
			  process * temp = proc[i];
			  proc[i] = proc[j];
			  proc[j] = temp;
			}
		    }
		}
	    }
	  break;
	}
      case 2: { //ppid
	  for(int i = 0; i < n; i++){
	      if(proc[i] == NULL){
		  continue;
		}
	      for(int j = i; j < n; j++){
		  if(proc[j] == NULL){
		      continue;
		    }
		  if(less == 1){
		      if(proc[i]->ppid >= proc[j]->ppid){
			  process * temp = proc[i];
			  proc[i] = proc[j];
			  proc[j] = temp;
			}
		    } else {
		      if(proc[j]->ppid >= proc[i]->ppid){
			  process * temp = proc[i];
			  proc[i] = proc[j];
			  proc[j] = temp;
			}
		    }
		}
	    }
	  break;
	}
      case 3: { //priority
	  for(int i = 0; i < n; i++){
	      if(proc[i] == NULL){
		  continue;
		}
	      for(int j = i; j < n; j++){
		  if(proc[j] == NULL){
		      continue;
		    }
		  if(less == 1){
		      if(proc[i]->priority >= proc[j]->priority){
			  process * temp = proc[i];
			  proc[i] = proc[j];
			  proc[j] = temp;
			}
		    } else {
		      if(proc[j]->priority >= proc[i]->priority){
			  process * temp = proc[i];
			  proc[i] = proc[j];
			  proc[j] = temp;
			}
		    }
		}
	    }
	  break;
	}
      case 4:{ //cpu
	  for(int i = 0; i < n; i++){
	      if(proc[i] == NULL){
		  continue;
		}
	      for(int j = i; j < n; j++){
		  if(proc[j] == NULL){
		      continue;
		    }
		  if(less == 1){
		      if(proc[i]->cpu >= proc[j]->cpu){
			  process * temp = proc[i];
			  proc[i] = proc[j];
			  proc[j] = temp;
			}
		    } else {
		      if(proc[j]->cpu >= proc[i]->cpu){
			  process * temp = proc[i];
			  proc[i] = proc[j];
			  proc[j] = temp;
			}
		    }
		}
	    }
	  break;
	}
      case 5:{//state
	  for(int i = 0; i < n; i++){
	      if(proc[i] == NULL){
		  continue;
		}
	      for(int j = i; j < n; j++){
		  if(proc[j] == NULL){
		      continue;
		    }
		  if(less == 1){
		      if(strcmp(proc[i]->state,proc[j]->state) >= 0){
			  process * temp = proc[i];
			  proc[i] = proc[j];
			  proc[j] = temp;
			}
		    } else {
		      if(strcmp(proc[j]->state,proc[i]->state) >= 0){
			  process * temp = proc[i];
			  proc[i] = proc[j];
			  proc[j] = temp;
			}
		    }
		}
	    }
	  break;
	}
      case 6:{ //mem
	  for(int i = 0; i < n; i++){
	      if(proc[i] == NULL){
		  continue;
		}
	      for(int j = i; j < n; j++){
		  if(proc[j] == NULL){
		      continue;
		    }
		  if(less == 1){
		      if(proc[i]->memory >= proc[j]->memory){
			  process * temp = proc[i];
			  proc[i] = proc[j];
			  proc[j] = temp;
			}
		    } else {
		      if(proc[j]->memory >= proc[i]->memory){
			  process * temp = proc[i];
			  proc[i] = proc[j];
			  proc[j] = temp;
			}
		    }
		}
	    }
	  break;
	}
      case 7:{
	  for(int i = 0; i < n; i++){
	      if(proc[i] == NULL){
		  continue;
		}
	      for(int j = i; j < n; j++){
		  if(proc[j] == NULL){
		      continue;
		    }
		  if(less == 1){
		      if(strcmp(proc[i]->owner,proc[j]->owner) >= 0){
			  process * temp = proc[i];
			  proc[i] = proc[j];
			  proc[j] = temp;
			}
		    } else {
		      if(strcmp(proc[j]->owner,proc[i]->owner) >= 0){
			  process * temp = proc[i];
			  proc[i] = proc[j];
			  proc[j] = temp;
			}
		    }
		}
	    }
	  break;
	}
      }
    return;
  }

/**
 * @brief setFilter
 * @param proc
 * @param size
 * @param u_filter
 * @param s_filter
 * @return
 */
process **setFilter(process ** proc, int * size, USER_FILTER u_filter, STATE_FILTER s_filter) {
    int n = *size;
    if(size == 0){
	return proc;
      }
    process ** newProc = (process**)malloc(n*sizeof(process*));
    int newN = 0;
    switch (u_filter) {
      case THIS_USER: {
	  char user[100];
	  strcpy(user,getProces(proc, n, inf.pid)->owner);
	  for(int i = 0; i < n; i++){
	      if(proc[i] == NULL)
		continue;
	      if(strcmp(proc[i]->owner, user) == 0){
		  newProc[newN] = proc[i];
		  newN++;
		}
	    }
	  break;
	}
      case ALL_USERS : {
	  for(int i = 0; i < n; i++) {
	      newProc[newN] = proc[i];
	      newN++;
	    }
	  break;
	}
      case ROOT: {
	  for(int i = 0; i < n; i++) {
	      if(proc[i] == NULL)
		continue;
	      if(strcmp(proc[i]->owner, "root") == 0) {
		  newProc[newN] = proc[i];
		  newN++;
		}
	    }
	  break;
	}
      default:
	break;
      }
    process ** finalProcess = (process**)malloc(newN*sizeof(process*));
    int finalN = 0;

    switch (s_filter) {
      case RUNNING: {
	  for(int i = 0; i < newN; i++){
	      if(proc[i] == NULL)
		continue;
	      if(strcmp(newProc[i]->state, "running") == 0) {
		  finalProcess[finalN] = newProc[i];
		  finalN++;
		}
	    }
	  break;
	}
      case SLEEPING: {
	  for(int i = 0; i < newN; i++){
	      if(proc[i] == NULL)
		continue;
	      if(strcmp(newProc[i]->state, "sleeping") == 0) {
		  finalProcess[finalN] = newProc[i];
		  finalN++;
		}
	    }
	  break;
	}
      case ALL: {
	  for(int i = 0; i < newN; i++){
	      finalProcess[finalN] = newProc[i];
	      finalN++;
	    }
	  break;
	}
      default:
	break;
      }
    free(newProc);
    free(proc);
    *size = finalN;
    process ** answer = (process**)malloc((*size)*sizeof(process*));
    for(int i = 0; i < *size; i++){
	answer[i] = finalProcess[i];
      }
    free(finalProcess);
    return answer;
  }

/**
 * @brief filter
 * @param proc
 * @param size
 * @param filter
 * @return
 */
process ** filter(process ** proc, int * size, char * filter) {
    int filterSize = strlen(filter);
    if(filterSize == 0){
	return proc;
      }

    process ** newProc = (process**)malloc(*(size)*sizeof(process*));
    int newSize = 0;

    for(int i = 0; i < *size; i++){
	if(strncasecmp(proc[i]->name,filter,filterSize) == 0) {
	    newProc[newSize] = proc[i];
	    newSize++;
	  }
      }

    process ** answer = (process**)malloc(newSize * sizeof(process*));
    *size = newSize;

    for(int i =0; i < *size; i++) {
	answer[i] = newProc[i];
      }

    free(newProc);
    free(proc);

    return answer;
  }

/**
 * @brief init
 */
void init() {
    inf.pid = getpid();
  }
