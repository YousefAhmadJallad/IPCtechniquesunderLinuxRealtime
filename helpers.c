#include "header.h"
#include "sharedMemory.h"
// #include "variables.h"
boolean done = false;

void handler (int sigNum){
    done = true;
}

int main(int argc, char *argv[]) {

   if(argc < 2){
        perror("wrong number of arguments in helpers.c");
        exit(26);
    }
    max_columns = atoi(argv[1]);

    srand(getpid());
    signal(SIGINT, handler);

    while (!done)
    {
        swap(max_columns);
        sleep(helperDelay);
    	// char s[max_columns][BUFSIZ];
        // displayAll(s,max_columns);
        // printf("\n\n\n\n");
        // for (int i=0;i<max_columns;i++)
        //   printf("S [%d]: %s\n",i ,s[i]);
    }
    

    return 0;
}
