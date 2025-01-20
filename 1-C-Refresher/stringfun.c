#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#define BUFFER_SZ 50

//prototypes
void usage(char *);
void print_buff(char *, int);
int  setup_buff(char *, char *, int);

//prototypes for functions to handle required functionality
int  count_words(char *, int, int);
//add additional prototypes here


int setup_buff(char *buff, char *user_str, int len) {
	
   int index = 0;
	int length = 0;
	int chars = 0;
   char prev = ' ';

   while (user_str[index] != '\0') {
       if (length >= BUFFER_SZ) {
            return -1;
       }
       if ((user_str[index] == ' ' || user_str[index] == '\t') && prev == ' ') {
		 //checking if previous char was a tab or space
           index++;
          continue;
       } else if (user_str[index] == '\t') {
           buff[length] = ' ';
       } else {
           buff[length] = user_str[index];
       }
       prev = buff[length];
       length++;
		 index++;
		 chars++;
   }
	
	if (prev == ' ' && length > 0) {
		length--;
	}
		
    while (length < len) {
        buff[length++] = '.';
    }

    return chars;
}

void print_buff(char *buff, int len){
	
    printf("Buffer:  [");
    for (int i=0; i<len; i++){
        putchar(*(buff+i));
    }
	 printf("]\n");
   // putchar('\n');
}

void usage(char *exename){
    printf("usage: %s [-h|c|r|w|x] \"string\" [other args]\n", exename);

}

int count_words(char *buff, int len, int str_len){
	if (str_len > len){
		return -1;
	}

	int index = 0;
	int total = 0;
	int word = 0;

    while(index < str_len){
	 	if (buff[index] == ' '){
			if (word){
			word = 0;
			}
		} else {
			if (!word){
				word = 1;
				total++;
			}
		}
		index++;
	} 	

    return total;
}

int reverse_words(char *buff, int len, int str_len) {
   if (str_len > len) {
        return -1;  
   }

   char* temp = malloc(len);
	memcpy(temp,buff,len);	


   for (int i = 0; i < str_len; i++) {
       buff[i] = temp[str_len - i -1 ];

   }


   free(temp);

   return 0;
}

//im so good at this! so good i waited until the last minute and ran out of time! 
/*
int word_print(char *buff, int len, int str_len){
	int start = 1;
	int space = 1;
   if (str_len > len){
      return -1;
   }
	printf("Word Print\n----------");
	for (int i = 0; i < len; i++){
	if(space){
		printf("\n%d.kk");	
}
*/


//ADD OTHER HELPER FUNCTIONS HERE FOR OTHER REQUIRED PROGRAM OPTIONS

int main(int argc, char *argv[]){

    char *buff;             //placehoder for the internal buffer
    char *input_string;     //holds the string provided by the user on cmd line
    char opt;               //used to capture user option from cmd line
    int  rc;                //used for return codes
    int  user_str_len;      //length of user supplied string

    //TODO:  #1. WHY IS THIS SAFE, aka what if arv[1] does not exist?
    //This is safe because it checks if there are any arguments. it also references the pointer of the second argument,
	 //not the value of it. 
    if ((argc < 2) || (*argv[1] != '-')){
        usage(argv[0]);
        exit(1);
    }

    opt = (char)*(argv[1]+1);   //get the option flag

    //handle the help flag and then exit normally
    if (opt == 'h'){
        usage(argv[0]);
        exit(0);
    }

    //WE NOW WILL HANDLE THE REQUIRED OPERATIONS

    //TODO:  #2 Document the purpose of the if statement below
    // it exits with an error code if there are to few arugments 
    if (argc < 3){
        usage(argv[0]);
        exit(1);
    }

    input_string = argv[2]; //capture the user input string

    //TODO:  #3 Allocate space for the buffer using malloc and
    //          handle error if malloc fails by exiting with a 
    //          return code of 99
    // CODE GOES HERE FOR #3
	 buff = malloc(BUFFER_SZ);
	 if (buff == NULL){
	 	exit(99);
	}


    user_str_len = setup_buff(buff, input_string, BUFFER_SZ);     //see todos
    if (user_str_len < 0){
        printf("Error setting up buffer, error = %d", user_str_len);
        exit(2);
    }

    switch (opt){
        case 'c':
            rc = count_words(buff, BUFFER_SZ, user_str_len);  //you need to implement
            if (rc < 0){
                printf("Error counting words, rc = %d", rc);
                exit(2);
            }
            printf("Word Count: %d\n", rc);
            break;
			
        //TODO:  #5 Implement the other cases for 'r' and 'w' by extending
        //       the case statement options
		  case 'r':
		  		rc = reverse_words(buff, BUFFER_SZ, user_str_len);
				if (rc < 0){
                printf("Error reversing words, rc = %d", rc);
                exit(2);
            }	
				break;
		  	
        default:
            usage(argv[0]);
            exit(1);
    }

    //TODO:  #6 Dont forget to free your buffer before exiting
    print_buff(buff,BUFFER_SZ);
    exit(0);
}

//TODO:  #7  Notice all of the helper functions provided in the 
//          starter take both the buffer as well as the length.  Why
//          do you think providing both the pointer and the length
//          is a good practice, after all we know from main() that 
//          the buff variable will have exactly 50 bytes?
//  
//          PLACE YOUR ANSWER HERE
