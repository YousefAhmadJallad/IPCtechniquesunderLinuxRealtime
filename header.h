
#ifndef LIBRARIES
#define LIBRARIES

typedef int boolean;
#define true 1
#define false 0


#include <stdio.h>
#include <stdarg.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <limits.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <time.h>
#include <sys/sem.h>
#include <semaphore.h>
#include <pthread.h>
#include<sys/shm.h>
#include <errno.h>
#include <GL/glut.h>
#include <stdbool.h>
#include <math.h>
#include <ctype.h>
// #include "variables.h"
//#include <GLFW/glfw3.h>
// #include "glad.h" 
void display();
#include "constants.h"
#include "semaphore.h"
#include "sharedMemory.h"
#include "messageQueus.h"
#include "functions.h"
#include "decoders.h"
#include "sharedMemory.h"
// char L_sum1[10];
// char L_sum2[10];
// char value1_T1[10];
// char value2_T1[10];
// char value1_T2[10];
// char value2_T2[10];

// char result[10];
// char number_of_Rounds[10];


#endif
