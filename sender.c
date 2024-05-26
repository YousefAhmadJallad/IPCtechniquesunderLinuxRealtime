#include "header.h"
#include "variables.h"
#define MAX_LINE_LENGTH 1024
#define MAX_LINES 256
#define MAX_COLUMNS 256


void create_all_shared_memories();
void create_shared_memory(int key, int memory_struct_size);

void clean_all_shared_memories();
void clean_shared_memory(int key, int memory_struct_size);


void create_all_semaphores();
void create_semaphore(char* key);

void clean_all_semaphores();
void clean_semaphore(char* key);



void get_column(char *words[][MAX_COLUMNS], int line_count, int column_index, char result[]) {
    // printf("Column %d: ", column_index + 1);
    for (int i = 0; i < line_count; i++) {
        if (words[i][column_index]) {
            strcat(result, words[i][column_index]);
        } else {
            strcat(result, "alright");
        }
        if(i+1 < line_count)
            strcat(result," ");
    }
}
void WritetoFile(){
	// Write the maximum number to a file
	FILE *max_columns_file = fopen("max_columns_file.txt", "w");
	if (max_columns_file)
	{
	    fprintf(max_columns_file, "%d\n", max_columns);
	    fclose(max_columns_file);
	}
	else
	{
	    perror("Error opening output file");
	    return 1;
	}


}

int main(int argc, char *argv[]) {
    // argv[0] is the executable name
    // argv[1] is the send pipe address
    
    
    
    if(argc < 2){
        perror("wrong number of arguments in sender.c");
        exit(12);
    }

    int send = atoi(argv[1]);

    FILE *file = fopen("sender.txt", "r");
    if (!file) {
        perror("Error opening file");
        return 1;
    }

    char line[MAX_LINE_LENGTH];
    char *words[MAX_LINES][MAX_COLUMNS] = {0};
    int line_count = 0;
    // int max_columns = 0;

    while (fgets(line, MAX_LINE_LENGTH, file)) {
        int column_count = 0;
        char *token = strtok(line, " \n");
        while (token) {
            words[line_count][column_count++] = strdup(token);
            token = strtok(NULL, " \n");
        }
        if (column_count > max_columns) {
            max_columns = column_count;
        }
        line_count++;
    }
    fclose(file);
    

    create_all_semaphores();
    create_all_shared_memories();
    WritetoFile();
    int *column_order = malloc(max_columns * sizeof(int));
    for (int i = 0; i < max_columns; i++) {
        column_order[i] = i;
    }

    for (int i = 0; i < max_columns; i++) {
        pid_t pid = fork();
        if (pid == 0) {
            char column[BUFSIZ]="";
            get_column(words, line_count, column_order[i], column);
            char columnNum[5];
            sprintf(columnNum, "%d", i+1);
            execl("./encoder", "./encoder", column, columnNum);
            exit(0);
        } else if (pid < 0) {
            perror("Error creating child process");
            return 1;
        }
    }
    int status;
    for (int i = 0; i < max_columns; i++) {
        wait(&status);
        // printf("Process ended with status %d\n", status);
    }

    free(column_order);

    // sleep(1);
    // char mem[max_columns][BUFSIZ];
    // displayAll(mem, max_columns);
    // printf("\n\n\n");
    // for(int i = 0; i < max_columns; i++)
    //     printf("mem[%d] : (%s)\n", i, mem[i]);

    //printf("sending signal to parent\n");
    // while(kill(getppid(), SIGINT)==-1);

    char msg[100];
    sprintf(msg, "%d", max_columns);

    if (write(send, msg, sizeof(msg)) == -1) // send # of columns to parent
            {
                perror("write");
                exit(31);
            }
    if(close(send) == -1) {
        perror("closing in sender");
        exit(-10);
    }

    return 0;
}


////////////////////////

/////////////////////////







void create_all_semaphores(){
    create_semaphore(COLUMNS_SEMAPHORE);
}
void create_semaphore(char* key){

    sem_t* sem = sem_open(key, O_CREAT, 0777, 0);
    if(sem == SEM_FAILED){
        perror("Semaphore Creation Error\n");
        exit(-1);
    }

    // When return -1 then wrong issue happened
    if (sem_post(sem) < 0){
        perror("Semaphore Release Error\n");
        exit(-1);
    }

    sem_close(sem);

}

void clean_all_semaphores(){
    clean_semaphore(COLUMNS_SEMAPHORE);
}
void clean_semaphore(char* key){
    if(sem_unlink(key) < 0){
        perror("Semaphore Unlink Error\n");
        exit(-1);
    }
}




void create_all_shared_memories(){
    create_shared_memory(CSM_KEY, sizeof(struct shared_memory));

}
void create_shared_memory(int key, int memory_struct_size){

    // For create the memory
    int sm_id = shmget(key, memory_struct_size, IPC_CREAT | 0777);
    if (sm_id == -1){
        perror("Memory Create Error\n");
        exit(-1);
    }

    // For access the CSM
    CSM csm_mem =  shmat(sm_id, NULL, 0);


    if (csm_mem == (void *) -1) {
        perror("Memory Attached Error\n");
        exit(-1);
    }
    for(int i = 0 ; i < 200; ++i)
        strcpy(csm_mem->columns[i] , "");

    // csm_mem->timer = 0;


    if (shmdt(csm_mem) == -1) {
        perror("Memory Detached Error\n");
        exit(-1);
    }



}

void clean_all_shared_memories(){
    clean_shared_memory(CSM_KEY, sizeof(struct shared_memory));
}
void clean_shared_memory(int key, int memory_struct_size){

    int sm_id = get_shared_memory(key, memory_struct_size);

    // clean the memory
    if(shmctl(sm_id,IPC_RMID, 0) == -1){
        perror("SM Remove Error\n");
        exit(-1);
    }

}

