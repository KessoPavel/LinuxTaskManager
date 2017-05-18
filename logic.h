#ifndef LOGIC
#define LOGIC
#include "process.h"

enum USER_FILTER {THIS_USER,ROOT,ALL_USERS};
enum STATE_FILTER {RUNNING,SLEEPING,ALL};

char* memoryToString(long int);
const char* getPriority(int);
bool equals(int *, int *, int, int);
int getPidNumber(int *, int, int);
int getPidNumber(process **, int, int);
void sort(process **, int, int, int);
process ** setFilter(process **, int*, USER_FILTER, STATE_FILTER);

#endif // LOGIC

