#ifndef decoders
#define decoders


#include "header.h"

int Decoder(char input[], char output[]){ 
    strcpy(output, "");
    char * token = strtok(input," ");
    int base = prefixReader(token);

    for(int i = 0; (token = strtok(NULL," ") ) != NULL; ++i){
        
        if(i != 0)
        strcat(output, " ");
        
        char word[BUFSIZ]="";
        strcpy(word, token);
            if(isdigit(word[0]) && strlen(word)>2 && isdigit(word[1])){
                numbersHandler(word);
            }
            else{ // word or special character
                wordsHandler(word, base);
            }
        strcat(output,word);
    }
    return base-1; // the index of the column
}


//================================================================

int prefixReader(char input[]){
    int result = 0;
    for(int i = 0; i <strlen(input); i++){
		char c = input[i];
		switch(c){
			case '!':
				c = '1';
				break;

			case '@':
				c = '2';
				break;

			case '#':
				c = '3';
				break;

			case '$':
				c = '4';
				break;

			case '%':
				c = '5';
				break;

			case '^':
				c = '6';
				break;

			case '&':
				c = '7';
				break;

			case '*':
				c = '8';
				break;

			case '(':
				c = '9';
				break;

			case ')':
				c = '0';
				break;
		}
        result =result * 10 + (c-'0');
	}
	return result;
}


//============================================================================


void numbersHandler(char word[]){

    char specialCharacter(char);
    char result [BUFSIZ] = "";

    for(int i = 0; i < strlen(word); i++){
        // sleep(1);

        if((word[i] - '0') < 7 && (word[i] - '0') > 0 && (i == strlen(word)-1  || i == 0)){
            result[i] = specialCharacter(word[i]);

            if(i+2 < strlen(word) && isdigit(word[i+1]) && isdigit(word[i+2])){
                numberDecoder(word, i+1);
                strcat(result, word);
                break;
            }
            else if(i+1 < strlen(word) && isdigit(word[i+1])){
                char c = specialCharacter(word[i+1]);
                strncat(result, &c, 1);
            }
        }
        else {
            numberDecoder(word, i);
            strcat(result, word);
            break;
        }
        
    }
    strcpy(word, result);

}


char specialCharacter(char c){
    switch(c) {
        case '1':
            return '!';
        case '2':
            return '?';
        case '3':
            return ',';
        case '4':
            return ';';
        case '5':
            return ':';
        case '6':
            return '%';
        default:
            return c;
    }
}


//==========================================================================


void numberDecoder(char word[], int start) { // 999999100000010000001000000
    int number = 0;
    boolean flag = false;
    
    for(int i = start; i < strlen(word); i++){
        if(word[i] == '9'){ // 999998
            number = number *10 + (10 - (word[i+5] - '0') ) ;
            // printf("number = %d\n\n\n", number);
            i += 5;
        }
        else if(word[i] == '1' && (i+6) < strlen(word) && word[i+1] == '0')// word[i] = 1 => 1000000
        {
            number *= 10;
            // printf("number here = %d\n\n", number);
            i += 6;
        }
        else{
            char c = specialCharacter(word[i]);
            sprintf(word, "%d", number);
            strncat(word, &c, 1); // append character to string
            flag = true;
            break;
        }
    }

    if(!flag)
        sprintf(word, "%d", number);
}



//============================================================================

void wordsHandler(char word[], int base) {
    for(int i = 0; i < strlen(word); i++){
        char c = word[i];
        if(isalpha(c) && isupper(c)){
            // printf("char : %c is upper\n", c);
            int num = (i+1)*base;
            word[i] = ((c-'A'-num)) ; // for capital letters
            while(word[i] < 0)
                word[i] = 26 + word[i];
            word[i] += 'A';
        }
        else if(isalpha(c)){
            int num = (i+1)*base;
            word[i] = ((c-'a'-num)); // for small letters
            while(word[i] < 0)
                word[i] = 26 + word[i] ;
            word[i] += 'a';
            
        }
        else word[i] = specialCharacter(word[i]);
    }
}




#endif
