
#include "header.h"
#include "constants.h"
#include "sharedMemory.h"
#include "messageQueus.h"
#include "functions.h"
#include "decoders.h"

boolean finish = false; // it will be false after the first signal
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

    FILE *f = fopen("spy.txt","w");
    if(f == NULL){
        perror("opening output file");
        exit(-28);
    }

    fprintf(f, "%s", toWrite);
    fclose(f);
}


void resetFun(boolean received[], char columns[][BUFSIZ]) {
    for (int i = 0; i < max_columns; i++){
        received[i] = false;
        strcpy(columns[i],"");
    }
    collected = 0;
    reset = false;
    sleep(1);
}

// extern int max_columns;
// int max_columns;

void spiesKiller(int sigNum){
    finish = true;
}

void lost(int sigNum){
    collected = 0;
    loses++;
    reset = true;
}



boolean notDone(boolean received[]){
    boolean flag = false;
    for(int i = 0; i < max_columns; i++)
        if(received[i] == false)
           flag= true;         
         else 
            collected++;

    //Opengl
    if(!finish){
        FILE *M_collected = fopen("Mastercollected.txt", "w");
        if (M_collected != NULL) {
            fprintf(M_collected, "%d", collected);
            fclose(M_collected);
        }
    }        
	
    return flag;
}



//===========================================================================//
//                              MAIN PROGRAM                                 //
//===========================================================================//

int main(int argc, char *argv[]){

    if(argc < 3){
        perror("wrong number of arguments in master");
        exit(22);
    }
    
    int winCounter = 0;

    FILE *M_collected = fopen("Mastercollected.txt", "w");
         if (M_collected != NULL) 
         fprintf(M_collected, "%d", collected);
        fclose(M_collected);

    max_columns = atoi(argv[1]);
    int threshold = atoi(argv[2]);


    signal(SIGINT, spiesKiller);
    signal(SIGUSR2, lost);

    char columns[max_columns][BUFSIZ];
    boolean received[max_columns];


    // while(1);
    for(int i = 0; i < max_columns; ++i)
        received[i] = false;

    while(!finish && winCounter < threshold && loses < threshold){
        collected = 0;
        if(notDone(received)){
            char str[BUFSIZ] = "";
            char output[BUFSIZ] = "";
            readFromQueue(str);
            int rcv = Decoder(str, output);
            if(!received[rcv]){
                received[rcv] = true;
                strcpy(columns[rcv], output); 
            }
        }
        else if(!reset){
            ++winCounter;
            printf("\nreceived all columns in master spy!\n");
            writeColumns(columns, max_columns);
            kill(getppid(), SIGUSR2);
            reset = true;
        }
        if(reset)
            resetFun(received, columns);
        else
            sleep(receiverDelay);
        

    }
    
    
    printf("receiver return 0\n");
    return 0;
}

//===============================================================//
//                         END MAIN PROGRAM                      //
//===============================================================//

