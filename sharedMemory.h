#ifndef SHARED_MEMORIES
#define SHARED_MEMORIES

#include "header.h"
#include "semaphore.h"
// #include "variables.h"

#define CSM_KEY 0x2477

struct shared_memory {
    char columns[200][BUFSIZ];
};


typedef struct shared_memory* CSM;



int get_shared_memory(int key, int memory_struct_size){


    // For get the memory
    int sm_id = shmget(key, memory_struct_size, IPC_CREAT | 0777);
    if (sm_id == -1){
        perror("Memory Get Error\n");
        exit(-1);
    }

    return sm_id;
}




void swap(int numOfColumns) {

    int sm_id = get_shared_memory(CSM_KEY, sizeof(struct shared_memory));

    sem_t* sm = get_semaphore(COLUMNS_SEMAPHORE);



    // For access the CSM
    CSM CSM_mem =  shmat(sm_id, NULL, 0);


    if (CSM_mem == (void *) -1) {
        perror("Memory Attached Error\n");
        exit(-1);
    }


    attach_semaphore(sm);

    int firstIndex = rand()%numOfColumns;
    int secondIndex = rand()%numOfColumns;

    while(firstIndex == secondIndex)
        secondIndex = rand()%numOfColumns;

    char temp[BUFSIZ];

    // swap
    strcpy(temp,CSM_mem->columns[firstIndex]);
    strcpy(CSM_mem->columns[firstIndex], CSM_mem->columns[secondIndex]);
    strcpy(CSM_mem->columns[secondIndex], temp);


    detach_semaphore(sm);

    if (shmdt(CSM_mem) == -1) {
        perror("Memory Detached Error\n");
        exit(-1);
    }


    sleep(1);
}


void displayAll(char str[][BUFSIZ], int numOfColumns){

    // time_t the_current_time = 0;

    int sm_id = get_shared_memory(CSM_KEY, sizeof(struct shared_memory));

    sem_t* sm = get_semaphore(COLUMNS_SEMAPHORE);



    // For access the CSM
    CSM CSM_mem =  shmat(sm_id, NULL, 0);


    if (CSM_mem == (void *) -1) {
        perror("Memory Attached Error\n");
        exit(-1);
    }


    attach_semaphore(sm); // lock
    for(int i=0; i < numOfColumns; ++i)
        strcpy(str[i], CSM_mem->columns[i]);


    detach_semaphore(sm); //unlock

    if (shmdt(CSM_mem) == -1) {
        perror("Memory Detached Error\n");
        exit(-1);
    }


    // return the_current_time;

}




void writeInMemory(char str[], int index){
    
    int sm_id = get_shared_memory(CSM_KEY, sizeof(struct shared_memory));

    sem_t* sm = get_semaphore(COLUMNS_SEMAPHORE);



    // For access the CSM
    CSM CSM_mem =  shmat(sm_id, NULL, 0);


    if (CSM_mem == (void *) -1) {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    sleep(1);
    
    attach_semaphore(sm);
    
    strcpy(CSM_mem->columns[index],str);

    detach_semaphore(sm);
           
    if (shmdt(CSM_mem) == -1) {
        perror("Memory Detached Error\n");
        exit(-1);
    }

}




void getColumn(char str[], int index){

    // time_t the_current_time = 0;

    int sm_id = get_shared_memory(CSM_KEY, sizeof(struct shared_memory));

    sem_t* sm = get_semaphore(COLUMNS_SEMAPHORE);



    // For access the CSM
    CSM CSM_mem =  shmat(sm_id, NULL, 0);


    if (CSM_mem == (void *) -1) {
        perror("Memory Attached Error\n");
        exit(-1);
    }


    attach_semaphore(sm);
    
    strcpy(str, CSM_mem->columns[index]);

    detach_semaphore(sm);

    if (shmdt(CSM_mem) == -1) {
        perror("Memory Detached Error\n");
        exit(-1);
    }


    // return the_current_time;

}



#endif
