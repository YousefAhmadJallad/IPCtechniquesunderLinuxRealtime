#include "header.h"
#include "messageQueus.h"
#include "sharedMemory.h"
// #include "variables.h"

boolean finish = false;

void handler(int sigNum){
    finish = true;
}

int main(int argc, char *argv[]) {
    
    if(argc < 2){
        perror("wrong number of arguments in spies.c");
        exit(25);
    }
    max_columns = atoi(argv[1]);

    signal(SIGINT, handler);
    srand(getpid());
    // sleep(1);
    while(!finish){	
        char str[BUFSIZ];
        int index = rand() % max_columns;
        getColumn(str, index);  // get column from shared memory
        writeOnQueue(str); // write column to message queue
        sleep(spiesDelay);
    }

    return 0;
}
