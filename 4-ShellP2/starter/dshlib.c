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
int exec_local_cmd_loop()
{
   char* cmd_buff = malloc(SH_CMD_MAX * sizeof(char));
	if (cmd_buff == NULL){
		return ERR_MEMORY;
	}

    cmd_buff_t cmd;


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
		}



		memset(&cmd, 0, sizeof(cmd_buff_t));
	cmd.argc = 0;
	char *temp = cmd_buff;
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


	char* token = strtok(cmd_buff, "\"");

	cmd._cmd_buffer = token;

	while (token){
		if(cmd.argc == CMD_ARGV_MAX){
			return ERR_CMD_OR_ARGS_TOO_BIG;
		}
		cmd.argv[cmd.argc] = malloc(strlen(token) + 1);
		strcpy(cmd.argv[cmd.argc], token);
		cmd.argv[cmd.argc][strlen(token)] = '\0';
		cmd.argc++;
		token = strtok(NULL, "\"");

    
	}
	


      if (strcmp(cmd._cmd_buffer, "cd") == 0){

            if (cmd.argc == 1){
                chdir(cmd.argv[0]);
            }

		} else {
		int f_result, c_result;
	f_result = fork();
    if (f_result < 0){
     
        exit(ERR_EXEC_CMD);
    }

    if (f_result == 0){
        
       int rc;
        rc = execvp(cmd._cmd_buffer, cmd.argv);
        if (rc < 0){
           
            exit(ERR_EXEC_CMD); 
        }
    } else{
     		wait(&c_result);
		   if (WIFEXITED(c_result) && WEXITSTATUS(c_result) != 0) {
				return WEXITSTATUS(c_result);  
	}

}	


	}

	return 0;	
}

