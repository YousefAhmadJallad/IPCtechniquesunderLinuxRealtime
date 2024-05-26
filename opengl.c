#include "header.h" // Include the GLUT library
#include "constants.h"

#define MAX_LENGTH 50
#define MAX_RCV_LENGTH 100
int collected = 0;
int collected2 = 0;

int threshold;
char receiverWins[MAX_LENGTH]={0};
char masterWins[MAX_LENGTH]={0};

int receiver_wins_round = 0 ;
int spy_wins_round = 0 ;

boolean done = false;

void display();
void readData();
void timer(int);
// Keyboard(keyboard);
char text_collected[MAX_LENGTH]={0};
char text2_collected[MAX_LENGTH]={0};
const char* text = "# columns:";
char columns[MAX_LENGTH]={0};

const char resultText[BUFSIZ] = "waiting for someone to win ...";

const char ReceiverText[BUFSIZ] = "Receiver Win the game";

const char SpyText[BUFSIZ] = "Spy Win the game";

void display(){
// {   int x = 0;
//     while(x);
    glClear(GL_COLOR_BUFFER_BIT); // Clear the color buffer

    // Draw the first rectangle (Red)
    glColor3f(1.0, 0.0, 0.0); // Set the color to red
    glLineWidth(2.0); // Set the line width to 2 pixels
    glBegin(GL_LINE_LOOP);
    glVertex2f(-0.9, 0.9);
    glVertex2f(0, 0.9);
    glVertex2f(0, 0.4);
    glVertex2f(-0.9, 0.4);
    glEnd();
    
    glColor3f(1.0, 1.0, 1.0); // Set the color to white
    glRasterPos2f(-0.85, 0.7); // Set the position of the text inside the red rectangle

    sprintf(text_collected, "receiver has collected: %d", collected); // Convert collected to string
    for (int i = 0; i < strlen(text_collected); i++) {
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, text_collected[i]); // Render each character of the text
     }


    // Draw the second rectangle (Green)
    glColor3f(0.0, 1.0, 0.0); // Set the color to green
    glBegin(GL_LINE_LOOP);
    glVertex2f(0.4, 0.9);
    glVertex2f(0.9, 0.9);
    glVertex2f(0.9, 0.4);
    glVertex2f(0.4, 0.4);
    glEnd();
    
   
    glColor3f(1.0, 1.0, 1.0); // Set the color to white
    glRasterPos2f(0.5, 0.7); // Set the position of the text inside the green rectangle

    for (int i = 0; i < strlen(text); i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, text[i]); // Render each character of the text
    }
    
    sprintf(columns, "%d", max_columns); // Convert max_columns to string
    for (int i = 0; i < strlen(columns); i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, columns[i]); // Render each character of the text
    }
 





    // Draw the third rectangle (Blue)
    glColor3f(0.0, 0.0, 1.0); // Set the color to blue
    glBegin(GL_LINE_LOOP);
    glVertex2f(-0.9, 0.2);
    glVertex2f(0, 0.2);
    glVertex2f(0, -0.3);
    glVertex2f(-0.9, -0.3);
    glEnd();
    
    glColor3f(1.0, 1.0, 1.0); // Set the color to white
    glRasterPos2f(-0.85, 0.1); // Set the position of the text inside the red rectangle

    sprintf(text2_collected, "Spies has collected: %d", collected2); // Convert collected to string
    for (int i = 0; i < strlen(text2_collected); i++) {
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, text2_collected[i]); // Render each character of the text
     }
    
    //glColor3f(1.0, 1.0, 1.0); // Set the color to white for the text
    //glRasterPos2f(0.3, 0.0); // Set the position of the text inside the yellow rectangle
    
     
    // Draw the fourth rectangle (Yellow)
    glColor3f(1.0, 1.0, 0.0); // Set the color to yellow
    glBegin(GL_LINE_LOOP);
    glVertex2f(0.2, 0.2);
    glVertex2f(0.9, 0.2);
    glVertex2f(0.9, -0.3);
    glVertex2f(0.2, -0.3);
    glEnd();
    
     glColor3f(1.0, 1.0, 1.0); // Set the color to black for the text
    glRasterPos2f(0.3, 0.1); // Set the position of the text inside the yellow rectangle

    char thresholdString[MAX_LENGTH] = {0}; 
    sprintf(thresholdString, "Threshold: %d", threshold); // Convert collected to string
    for (int i = 0; i < strlen(thresholdString); i++) {
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,thresholdString[i]); // Render each character of the text
     }
    
    //sprintf(receivedText, "Received Value: %d", someVariable); // Convert the received value to string

   
   
  
     
     
    // Draw the fifth rectangle (Pink)
    glColor3f(1.0, 0.6, 0.8); // Set the color to pink
    glBegin(GL_LINE_LOOP);
    glVertex2f(-0.9, -0.4);
    glVertex2f(0.9, -0.4);
    glVertex2f(0.9, -0.9);
    glVertex2f(-0.9, -0.9);
    glEnd();
    
    
    glColor3f(1.0, 1.0, 1.0); // Set the color to black for the text
    glRasterPos2f(-0.8, -0.6); // Set the position of the text inside the pink rectangle

    if(collected == max_columns) {
        //strcpy(resultText , "Receiver won the round!");
        receiver_wins_round++;
        sleep(2);
    } else if(collected2 == max_columns) {
        //strcpy(resultText, "Spies won the round!");
        spy_wins_round++;
        sleep(2);
    }
    
    if(receiver_wins_round == threshold) {
        for (int i = 0; i < strlen(ReceiverText); i++) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, ReceiverText[i]); // Render each character of the text
        }
        done = true;
        ++collected;
        // ++receiver_wins_round;
        // sleep(100);
    } 
    
    else if(spy_wins_round == threshold) {
        for (int i = 0; i < strlen(SpyText); i++) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, SpyText[i]); // Render each character of the text
        }
        done = true;
        ++collected2;
        // ++spy_wins_round;
        // sleep(100);
    } 
    

    else{
        for (int i = 0; i < strlen(resultText); i++) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, resultText[i]); // Render each character of the text
        }
        
    }


  
    glColor3f(1.0, 1.0, 1.0); // Set the color to black for the text
    glRasterPos2f(0.3, 0.0); // Set the position of the text inside the yellow rectangle
    
    sprintf(receiverWins, "receiver points: %d", receiver_wins_round); // Convert collected to string
    for (int i = 0; i < strlen(receiverWins); i++) {
     glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,receiverWins[i]); // Render each character of the text
     }
     
     glColor3f(1.0, 1.0, 1.0); // Set the color to black for the text
    glRasterPos2f(0.3, -0.1); // Set the position of the text inside the yellow rectangle
    sprintf(masterWins,  "Spy points: %d", spy_wins_round); // Convert collected to string
    for (int i = 0; i < strlen(masterWins); i++) {
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, masterWins[i]); // Render each character of the text
     }



    glFlush(); // Flush the rendering pipeline
}


// void keyboard(unsigned char key, int x, int y) {
//     if (key == 27) {  // 27 is the ASCII value of the ESC key
//         exit(-17);
//     }
// }



int main(int argc, char** argv)
{
    if(argc < 2) {
        perror("wrong number of arguments in opengl");
        exit(-98);
    }
    threshold = atoi(argv[1]);  
    glutInit(&argc, argv);                       // Initialize GLUT
    glutInitWindowSize(700, 500);                // Set the window size
    glutCreateWindow("Game 🙂"); // Create a window with a title
    readData();
    glutTimerFunc(0, timer, 0);  // start the timer
    glutPostRedisplay();  // request to redraw the window
    glutDisplayFunc(display);                     // Set the display callback function
    // glutKeyboardFunc(keyboard);
    glutMainLoop();                               // Enter the event-processing loop
    printf("\n\n\nopengl return 0\n\n");
    return 0;
}

void readData()
{

    FILE *fp;
    char buffer[100];
    printf("opening file...\n");
    fp = fopen("max_columns_file.txt", "r");
    if(fp == NULL){
        perror("error opening max_columns_file.txt in opengl");
        exit(190);
    }
    printf("after opening file...\n");
    sleep(1);
	while(fp==NULL)
	{    
	    
		fp = fopen("max_columns_file.txt", "r");

	 }

    fgets(buffer, 100, fp);

    sscanf(buffer, "%d",&max_columns);

    fclose(fp);
}

///////////////
//////////////

void timer(int extra)
     {
        
    if(!done){
        // Read the updated value of 'collected' from the file
        FILE *file_collected = fopen("collected.txt", "r");
        if (file_collected != NULL) {
            fscanf(file_collected, "%d", &collected);
            fclose(file_collected);
        } else {
            perror("collected.txt opengl");
            exit(-15);
        }
        
        FILE *M_collected = fopen("Mastercollected.txt", "r");
        if (M_collected != NULL) {
            fscanf(M_collected, "%d", &collected2);
            fclose(M_collected);
        }
        else {
            perror("collected.txt opengl");
            exit(-16);
        }
        

        glutPostRedisplay();  // request to redraw the window

        // printf("done reading file\n");
        glutTimerFunc(1000, timer, 0);  // reset the timer
    }
}