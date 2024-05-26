#include "header.h"


void encoder(int base, char* column, char result[]);
char characterEncoder(char c, int num);



void prefix(char base[], char result[]);
void numberHandler(word, result, i , base);

int main (int argc, char *args[]) {
	// args[0] is the executable name
	// args[1] is the column
	// args[2] is the column number
	
	if(argc < 3){
		perror("Wrong number of arguments");
		exit(-31);
	}
	char column[BUFSIZ] = "";
	int base = atoi(args[2]); 
	strcpy(column, args[1]);


	char result[BUFSIZ] = "";


	encoder(base, column, result);


	writeInMemory(result, base-1);

	
	return 0;
	}

void encoder(int base, char column[], char result[]){
	strcpy(result, "");
	char b[BUFSIZ];
	sprintf(b, "%d", base);
	prefix(b,result);
	char word [BUFSIZ];
	char * token = strtok(column, " ");

	while (token != NULL)
	{	
		strcpy(word, token);
		boolean isNumber = false;

		char wordKeeper[BUFSIZ];
		strcpy(wordKeeper, word);
		
			for(int i = 1; i <= strlen(word); i++){
				if(isdigit(word[i-1])){
					numberHandler(wordKeeper, result, base);
					isNumber = true;
					break;
				}
				else
					word[i-1] = characterEncoder(word[i-1], i*base);
			}

		token = strtok(NULL, " ");
		if(!isNumber)
			strcat(result, word);
		if(token != NULL)	
			strcat(result, " ");
	}
	// printf("encoder result = %s\n\n", result);
}


///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////

char characterEncoder (char c, int num){

	if(isalpha(c) && isupper(c))
		c = ((c-'A'+num) %26 ) + 'A' ; // for capital letters
	else if(isalpha(c))
		c = ((c-'a'+num) %26 ) + 'a' ; // for small letters
	else {
		switch(c) {
			case '!':
				c = '1';
				break;
			
			case '?':
				c = '2';
				break;

			case ',':
				c = '3';
				break;

			case ';':
				c = '4';
				break;

			case ':':
				c = '5';
				break;
			

			case '%':
				c = '6';
				break;
				
			default:
				printf("unknown character {%c}\n", c);
		}
	}
	return c;
}




void numberHandler(char word[], char result[], int base){


	for(int i = 0; i <= strlen(word); i++){

		if(isdigit(word[i])){
						int temp = 1000000 -( word[i] - '0');
						char str[BUFSIZ];
						sprintf(str, "%d", temp);
						strcat(result, str);	
					}


		else{
			char c;
			c = characterEncoder(word[i], (i+1)*base);
			char temp [1];
			sprintf(temp, "%c", c);
			strcat(result, temp);
		}
	}
}

void prefix(char base[], char result[]){
	for(int i = 0; i <strlen(base); i++){
		char c = base[i];

		switch(c){
			case '1':
				c = '!';
				break;

			case '2':
				c = '@';
				break;

			case '3':
				c = '#';
				break;

			case '4':
				c = '$';
				break;

			case '5':
				c = '%';
				break;

			case '6':
				c = '^';
				break;

			case '7':
				c = '&';
				break;

			case '8':
				c = '*';
				break;

			case '9':
				c = '(';
				break;

			case '0':
				c = ')';
				break;
		}
		char s [6];
		sprintf(s, "%c", c);
		strcat(result, s);
	}

	strcat(result," ");
}
