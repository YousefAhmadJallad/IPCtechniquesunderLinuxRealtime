#include "header.h"

boolean rcv = false;
boolean spy = false;
pid_t receiver;
pid_t masterSpy;
int receiverWins = 0;
int masterWins = 0;

void Receiver(int sigNum){
    receiverWins++;
    kill(masterSpy, SIGUSR2);
}

void Spy(int sigNum){
    masterWins++;
    kill(receiver, SIGUSR1);
}

void Sender(int sigNum){
    printf("sender signal was received\n");
}

boolean isNumber(char s[])
{
    for (int i = 0; s[i]!= '\0'; i++)
    {
        if (isdigit(s[i]) == false)
              return false;
    }
    return true;
}




void create_all_message_queues(){
    create_Q(MQ_KEY);
}


void create_Q(int key){

    if ( msgget(key, IPC_CREAT | 0777) == -1 ) {
        perror("Queue Creation Error\n");
        exit(-1);
    }

}

void clean_all_message_queues(){
    clean_Q(MQ_KEY);
}


void clean_Q(int key){
    int queue = get_queue(key);
    msgctl(queue, IPC_RMID, 0);/* remove */
}


void clean_all_semaphores();

void clean_all_shared_memories();

int main(int argc, char*argv[]){

    if(argc < 4) {
        perror("wrong number of arguments\npass the helpers number then spies number then the wins threshold");
        exit(1);
    }
    else if(!isNumber(argv[1]) || !isNumber(argv[2]) || !isNumber(argv[3])){
        perror("These arguments should be integers");
        exit(2);
    }

    create_all_message_queues();

    int newChildPid = 0;

    

    signal(SIGUSR1, Receiver);
    signal(SIGUSR2, Spy);
    signal(SIGINT, Sender);



    int helpersNum;
    int spiesNum;
    int threshold;


    

    // int sleepTime =1;     // for debugging
    // while(sleepTime);

    


    helpersNum = atoi(argv[1]);
    spiesNum = atoi(argv[2]);
    threshold = atoi(argv[3]);
    

    char thresholdString [10];
    sprintf(thresholdString, "%d", threshold);
    
    int pid = fork();
    if (pid == 0)
    {
        execlp("./opengl", "./opengl", thresholdString,  (char *)NULL);
        printf("\nError with execlp in pid = [%d]\n", getpid());
        fflush(stdout);

    }

   
    pid_t sender ;
    pid_t helpers[helpersNum];
    pid_t spies[spiesNum];

    int fd[2];

    if(pipe(fd) == -1){
        perror("pipe creation failed");
        exit(78);
    }
    // sleep(1);
    if((sender = fork()) == -1){
        perror("forking sender error");
        exit(3);
    }
    else if(sender == 0){
        if (close(fd[0]) == -1){
            perror("close in forking sender");
            exit(-11);
        }
        char sendPipe[12];
        sprintf(sendPipe,"%d",fd[1]);
        execl("./sender", "./sender", sendPipe, NULL);
        perror("exec error");
        exit(13);
    }

    if(close(fd[1]) == -1){
        perror("close error in parent 1");
        exit(-13);
    }

    char columnsNum[100];
    if (read(fd[0], columnsNum, 100) == -1)
            { // read from pipe
                perror("read");
                exit(3);
            }

    if(close(fd[0]) == -1){
        perror("close error in parent 1");
        exit(-13);
    }


    for(int i = 0; i <helpersNum; ++i){
        if((helpers[i] = fork()) == -1){
            perror("forking helpers error");
            exit(6);
        }
        else if(helpers[i] == 0){
            execl("./helpers", "helpers", columnsNum, NULL);
            perror("exec error");
            exit(16);
        }
    }

    // int x = 1;
    // while(x);
    
    if((receiver = fork()) == -1){
        perror("forking receiver error");
        exit(4);
    }
    else if(receiver == 0){
        execl("./receiver", "receiver", columnsNum, thresholdString, NULL);
        perror("exec error");
        exit(14);
    }




    for(int i = 0; i <spiesNum; ++i){
         if((spies[i] = fork()) == -1){
            perror("forking spies error");
            exit(7);
        }
        else if(spies[i] == 0){
            execl("./spies", "spies", columnsNum, NULL);
             perror("exec error");
             exit(17);
         }
    }

      if((masterSpy = fork()) == -1){
          perror("forking masterSpy error");
          exit(5);
     }
      else if(masterSpy == 0){
         execl("./masterSpy", "masterSpy", columnsNum, thresholdString, NULL);
         perror("exec error");
          exit(15);
     }
    int status;
    // waitpid(receiver, &status, 0);
    // printf("receiver terminated with status %d\n", status);
    // waitpid(masterSpy, &status, 0);
    // printf("masterSpy terminated with status %d\n", status);
    //waitpid(pid, &status, 0);
    //printf("opengl terminated with status %d\n", status);

    while(receiverWins < threshold && masterWins < threshold);
    
    
    
    printf("==================================================\n\n\n");
    if(receiverWins >= threshold){
        kill(masterSpy, SIGKILL);
        printf("\nparent : receiver wins!\n\n");
    }
    else if(masterWins >= threshold){
        kill(receiver, SIGKILL);
        printf("\nparent : Spies win!\n\n");
    }
    

printf("================================================\n\n\n");

for(int i = 0; i < helpersNum; ++i)
        kill(helpers[i], SIGINT);

for(int i = 0; i < spiesNum; ++i)
        kill(spies[i], SIGKILL);

    printf("++++++++++++++++++++++++++++\n\n\n\n\nkilling opnegl in 25 sec...\n\n\n+++++++++++++++++++++++++++++\n");
    sleep(25);
    kill(pid, SIGKILL);

    clean_all_message_queues();
    clean_all_semaphores();
    clean_all_shared_memories();
    
    return 0;
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
