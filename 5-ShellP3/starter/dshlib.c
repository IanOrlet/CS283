#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

#include "dshlib.h"

/*
 * Implement your exec_local_cmd_loop function by building a loop that prompts the 
 * user for input.  Use the SH_PROMPT constant from dshlib.h and then
 * use fgets to accept user input.
 * 
 *      while(1){
 *        printf("%s", SH_PROMPT);
 *        if (fgets(cmd_buff, ARG_MAX, stdin) == NULL){
 *           printf("\n");
 *           break;
 *        }
 *        //remove the trailing \n from cmd_buff
 *        cmd_buff[strcspn(cmd_buff,"\n")] = '\0';
 * 
 *        //IMPLEMENT THE REST OF THE REQUIREMENTS
 *      }
 * 
 *   Also, use the constants in the dshlib.h in this code.  
 *      SH_CMD_MAX              maximum buffer size for user input
 *      EXIT_CMD                constant that terminates the dsh program
 *      SH_PROMPT               the shell prompt
 *      OK                      the command was parsed properly
 *      WARN_NO_CMDS            the user command was empty
 *      ERR_TOO_MANY_COMMANDS   too many pipes used
 *      ERR_MEMORY              dynamic memory management failure
 * 
 *   errors returned
 *      OK                     No error
 *      ERR_MEMORY             Dynamic memory management failure
 *      WARN_NO_CMDS           No commands parsed
 *      ERR_TOO_MANY_COMMANDS  too many pipes used
 *   
 *   console messages
 *      CMD_WARN_NO_CMD        print on WARN_NO_CMDS
 *      CMD_ERR_PIPE_LIMIT     print on ERR_TOO_MANY_COMMANDS
 *      CMD_ERR_EXECUTE        print on execution failure of external command
 * 
 *  Standard Library Functions You Might Want To Consider Using (assignment 1+)
 *      malloc(), free(), strlen(), fgets(), strcspn(), printf()
 * 
 *  Standard Library Functions You Might Want To Consider Using (assignment 2+)
 *      fork(), execvp(), exit(), chdir()
 */
int execute_pipeline(command_list_t* cmd_list) {
    int num_commands = cmd_list->num;
    int pipes[num_commands - 1][2]; 
    pid_t pids[num_commands];        

  
    for (int i = 0; i < num_commands - 1; i++) {
        if (pipe(pipes[i]) == -1) {
            perror("pipe");
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < num_commands; i++) {
        pids[i] = fork();
        if (pids[i] == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        }

        if (pids[i] == 0) {  
         
            if (i > 0) {
                dup2(pipes[i - 1][0], STDIN_FILENO);
            }

            
            if (i < num_commands - 1) {
                dup2(pipes[i][1], STDOUT_FILENO);
            }

            
            for (int j = 0; j < num_commands - 1; j++) {
                close(pipes[j][0]);
                close(pipes[j][1]);
            }
            execvp(cmd_list->commands[i].argv[0], cmd_list->commands[i].argv);

					      
        }


    }

   
    for (int i = 0; i < num_commands - 1; i++) {
        close(pipes[i][0]);
        close(pipes[i][1]);
    }
    for (int i = 0; i < num_commands; i++) {
        waitpid(pids[i], NULL, 0);
    }
	 return 0;
}



int exec_local_cmd_loop()
{
   char* cmd_buff = malloc(SH_CMD_MAX * sizeof(char));
	if (cmd_buff == NULL){
		return ERR_MEMORY;
	}

    cmd_buff_t cmd;
    command_list_t cmd_list;


    while (1) {
        printf("%s", SH_PROMPT);
        if (fgets(cmd_buff, SH_CMD_MAX, stdin) == NULL) {
            printf("\n");
            break;
        }

        cmd_buff[strcspn(cmd_buff, "\n")] = '\0';

		if (strlen(cmd_buff) == 0) {
            return(WARN_NO_CMDS);

        }	
        if (strcmp(cmd_buff, EXIT_CMD) == 0) {

        return OK_EXIT;

        }






		  if (strlen(cmd_buff) >= SH_CMD_MAX){
		  		return ERR_CMD_OR_ARGS_TOO_BIG;
			}
		



	
	memset(&cmd_list, 0, sizeof(command_list_t));
    cmd_list.num = 0;
	char *line = strtok(cmd_buff, PIPE_STRING);
    while (line != NULL) {
	 	while (*line == ' '){
			line++;
		}
	if (strlen(line) == 0) {
	   return(WARN_NO_CMDS);

		}
//	 printf("cmds:%d\n",cmd_list.num);
	if (cmd_list.num == CMD_MAX - 1){
         return(ERR_TOO_MANY_COMMANDS);
      } 
	//	printf("processsing command: %s\n", line);
        memset(&cmd, 0, sizeof(cmd_buff_t));
        cmd.argc = 0;

        char *temp = line;
        int quote = 0;
			
    
        while (*temp) {
            if (*temp == '\"') {
                quote = !quote;
            }
            if (!quote && isspace(*temp)) {
                *temp = '"'; 
            }
            temp++;
        }
			char *saveptr;
			char *token = strtok_r(line, "\"", &saveptr); 
			if (!token) {
            return ERR_CMD_OR_ARGS_TOO_BIG;
         }
			if (strcmp(token, "cd") == 0){	
				token = strtok_r(NULL, "\"", &saveptr);
				chdir(token);
				line = strtok(NULL, PIPE_STRING);
				cmd_list.num++;
				continue;
			}

			if (strcmp(line, EXIT_CMD) == 0) {

        return OK_EXIT;

        }


			cmd._cmd_buffer = line;  


		//  printf("command: %s\n",  cmd._cmd_buffer);
        while (token) {
        
            if (cmd.argc == CMD_ARGV_MAX) {
                return ERR_CMD_OR_ARGS_TOO_BIG;
            }

            cmd.argv[cmd.argc] = malloc(strlen(token) + 1);
            strcpy(cmd.argv[cmd.argc], token);
			//	printf("arg: %s\n",  cmd.argv[cmd.argc]);
            cmd.argc++;
            token = strtok_r(NULL, "\"", &saveptr);
        }

		  cmd.argv[cmd.argc] = NULL;
        cmd_list.commands[cmd_list.num] = cmd;
        cmd_list.num++;
        line = strtok(NULL, PIPE_STRING);
    }	
 		
//		printf("\n");

		//printf("i got here\n");
		execute_pipeline(&cmd_list);		


   } 


	return 0;	
}
