#include "header.h"
#include "variables.h"
#include "constants.h"
#include "sharedMemory.h"
#include "functions.h"
#include "decoders.h"

boolean finish = false;
boolean reset = false;
int collected = 0;
int loses = 0;

void writeColumns(char words[][BUFSIZ], int numOfColumns) {

    char rows[1000][BUFSIZ]= {0};
    int numOfRows = 0;
    
    for (int i = 0; i < numOfColumns; i++) {
        char *token = strtok(words[i], " ");    
        if(i != 0)
            strcat(rows[0], " ");
        strcat(rows[0], token);
        for(int j = 1; (token = strtok(NULL, " ")) != NULL; j++){
            boolean alright = false;
            if(strcmp(token, "alright") == 0)
                alright = true;
            if(alright == false){
            if(i != 0)
                strcat(rows[j], " ");

            strcat(rows[j], token);
            }
            numOfRows = j+1;
        }
        }
        char toWrite [BUFSIZ*10] = "";
        for(int i = 0; i < numOfRows; i++){
            strcat(toWrite, rows[i]);
            if(i+1<numOfRows)
                strcat(toWrite, "\n");
        }

    FILE *f = fopen("receiver.txt","w");
    if(f == NULL){
        perror("opening output file");
        exit(-28);
        }
    fprintf(f, "%s", toWrite);

    fclose(f);
}



boolean notDone(boolean received[]){
     boolean flag = false;
    for(int i = 0; i < max_columns; i++){
        if(received[i] == false){
           flag= true;
           
         }
         else 
         collected++;	
	}

     if(!finish){
         FILE *f_collected = fopen("collected.txt", "w");
         if (f_collected != NULL) {
            fprintf(f_collected, "%d", collected);
            fclose(f_collected);
          }}

    return flag;
}


void handler(int sigNum){
    finish = true;
}

void lost(int sigNum){
    loses++;
    reset = true;
}


void resetFun(boolean received[], char columns[][BUFSIZ]) {
    for(int i = 0; i < max_columns; i++){
        received[i] = false;
        strcpy(columns[i],"");
    }
    collected = 0;
    reset = false;
    sleep(1);
}


int main(int argc, char *argv[]){

    if(argc < 3){
        perror("wrong number of arguments in receiver");
        exit(23);
    }

    int winCounter = 0;

    FILE *f_collected = fopen("collected.txt", "w");
    if (f_collected != NULL) 
         fprintf(f_collected, "%d", collected);
    fclose(f_collected);

    max_columns = atoi(argv[1]);
    int threshold = atoi(argv[2]);

    signal(SIGINT,handler);
    signal(SIGUSR1, lost);

    char columns[max_columns][BUFSIZ];
    boolean received[max_columns];



    for(int i = 0; i < max_columns; ++i)
        received[i] = false;


	srand(time(NULL));	


    while(!finish && winCounter < threshold && loses < threshold){
        collected = 0;
        if(notDone(received)){
            char str[BUFSIZ] = "";
            char output[BUFSIZ] = "";
        
            int index = rand() % max_columns;
            getColumn(str, index);  // get column from shared memory
            int rcv = Decoder(str, output); // decode column        


            if(!received[rcv]){ // check if the column was received before
                received[rcv] = true;
                strcpy(columns[rcv], output); 
            }
        }
        else if(!reset){
            printf("receiver won the round!!!\n");
            writeColumns(columns, max_columns);
            kill(getppid(),SIGUSR1);
            winCounter++;
            reset = true;
        }

        if(reset)
            resetFun(received, columns);
        else
            sleep(receiverDelay);
        }

    printf("receiver ended work!\n");
    return 0;
}
