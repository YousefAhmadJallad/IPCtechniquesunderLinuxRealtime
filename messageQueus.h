#ifndef QUEUES
#define QUEUES

#include "header.h"

struct queue{
    long mtype;
    char column[BUFSIZ]
};

typedef struct queue MessagesQueue;

#define MQ_KEY 0x1234



/* For access the Queue */
int get_queue(int key){

    int queue;

    if ( (queue = msgget(key, IPC_CREAT | 0777)) == -1 ) {
        perror("Queue Access Error\n");
        exit(-1);
    }

    return queue;
}

void writeOnQueue(char str[]){
    int queue = get_queue(MQ_KEY);
    MessagesQueue data;
    data.mtype = 1;   // Add this line
    strcpy(data.column, str);

    if (msgsnd(queue, &data,  sizeof(data) - sizeof(long), 0) == -1)
    {
        perror("Queue Send Error in write\n");
        exit(-1);
    }  
}



void readFromQueue(char str[]){
    int queue = get_queue(MQ_KEY);
    MessagesQueue data;

    while (msgrcv(queue, &data, sizeof(data) - sizeof(long),1, 0) == -1)
    {
        perror("Queue Receive Error in read\n");
        // exit(-1);
    }
    strcpy(str, data.column);  
}




#endif
