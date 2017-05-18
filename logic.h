#ifndef LOGIC
#define LOGIC
#include "process.h"



char* memoryToString(long int);
const char* getPriority(int);
bool equals(int *, int *, int, int);
int getPidNumber(int *, int, int);
int getPidNumber(process **, int, int);
void sort(process **, int, int, int);


#endif // LOGIC

