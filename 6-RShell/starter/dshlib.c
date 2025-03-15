#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

#include "dshlib.h"


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

#include "dshlib.h"

/**** 
 **** FOR REMOTE SHELL USE YOUR SOLUTION FROM SHELL PART 3 HERE
 **** THE MAIN FUNCTION CALLS THIS ONE AS ITS ENTRY POINT TO
 **** EXECUTE THE SHELL LOCALLY
 ****
 */

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

void trim_whitespace(char *str) {
    if (!str || *str == '\0') {
        return;
    }
    
    char *start = str;
    while (isspace((unsigned char)*start)) {
        start++;
    }
    
    char *end = start + strlen(start) - 1;
    while (end > start && isspace((unsigned char)*end)) {
        end--;
    }

    *(end + 1) = '\0';

    if (start != str) {
        while ((*str++ = *start++));
    }
}

int build_cmd_list(char *cmd_line, command_list_t *clist) {
    
    memset(clist, 0, sizeof(command_list_t));
    char *line = strtok(cmd_line, PIPE_STRING);
    int i = 0;

    while (line != NULL && i < CMD_MAX) {
		  trim_whitespace(line);
        clist->commands[i]._cmd_buffer = strdup(line);
        if (!clist->commands[i]._cmd_buffer) {
            return ERR_MEMORY;
        }
        
        if (build_cmd_buff(line, &clist->commands[i]) != OK) {
            return ERR_MEMORY;
        }
        
        clist->num++;
        i++;
        line = strtok(NULL, PIPE_STRING);
    }

    if (i >= CMD_MAX) {
        return ERR_TOO_MANY_COMMANDS;
    }
    
    return OK;
}

int free_cmd_list(command_list_t *cmd_lst) {
    if (!cmd_lst) {
        return ERR_MEMORY;
    }

    for (int i = 0; i < cmd_lst->num; i++) {
        for (int j = 0; j < cmd_lst->commands[i].argc; j++) {
            free(cmd_lst->commands[i].argv[j]);
        }
        free(cmd_lst->commands[i]._cmd_buffer);
    }
    memset(cmd_lst, 0, sizeof(command_list_t));
    return OK;
}

Built_In_Cmds match_command(const char *input) {
    
    if (strcmp(input, "exit") == 0) {
        return BI_CMD_EXIT;
    }
    
    if (strcmp(input, "cd") == 0) {
        return BI_CMD_CD;
    }
    
    return BI_NOT_BI;
}

Built_In_Cmds exec_built_in_cmd(cmd_buff_t *cmd) {
    if (strcmp(cmd->argv[0], "exit") == 0) {
        exit(OK_EXIT);
    } else if (strcmp(cmd->argv[0], "cd") == 0) {
        if (cmd->argc < 2) {
            return ERR_CMD_ARGS_BAD;
        }
       
        char *dir = cmd->argv[1];
        trim_whitespace(dir);
        
        if (chdir(dir) != 0) {
            perror("cd");
            return ERR_EXEC_CMD;
        }
        return BI_EXECUTED;
    }
    return BI_NOT_BI;
}


int execute_pipeline(command_list_t *clist) {
    int num_commands = clist->num;
    if (num_commands == 0){
	 	return WARN_NO_CMDS;
	 }
    int pipes[num_commands - 1][2];
    pid_t pids[num_commands];

    for (int i = 0; i < num_commands - 1; i++) {
        if (pipe(pipes[i]) == -1) {
            perror("pipe");
            return ERR_MEMORY;
        }
    }

    for (int i = 0; i < num_commands; i++) {
        pids[i] = fork();
        if (pids[i] == -1) {
            perror("fork");
            return ERR_MEMORY;
        }
        if (pids[i] == 0) {  
            if (i > 0) dup2(pipes[i - 1][0], STDIN_FILENO);
            if (i < num_commands - 1) dup2(pipes[i][1], STDOUT_FILENO);
            for (int j = 0; j < num_commands - 1; j++) {
                close(pipes[j][0]);
                close(pipes[j][1]);
            }
            execvp(clist->commands[i].argv[0], clist->commands[i].argv);
            perror("execvp");  
            _exit(ERR_EXEC_CMD);
        }
    }

    for (int i = 0; i < num_commands - 1; i++) {
        close(pipes[i][0]);
        close(pipes[i][1]);
    }
    for (int i = 0; i < num_commands; i++) {
        waitpid(pids[i], NULL, 0);
    }
    return OK;
}

int build_cmd_buff(char *cmd_line, cmd_buff_t *cmd_buff) {
    if (!cmd_line || !cmd_buff) return ERR_MEMORY;

    memset(cmd_buff, 0, sizeof(cmd_buff_t));
    cmd_buff->_cmd_buffer = strdup(cmd_line);
    if (!cmd_buff->_cmd_buffer) return ERR_MEMORY;

    char *token;
    char *saveptr;
    bool in_quotes = false;

    token = strtok_r(cmd_buff->_cmd_buffer, " ", &saveptr);
    while (token) {
        if (cmd_buff->argc >= CMD_ARGV_MAX - 1) {
            return ERR_CMD_OR_ARGS_TOO_BIG;
        }

        if (token[0] == '"') {
            in_quotes = true;
            token++;
        }
        if (in_quotes) {
            char *end_quote = strchr(token, '"');
            if (end_quote) {
                *end_quote = '\0';
                in_quotes = false;
            }
        }

        cmd_buff->argv[cmd_buff->argc] = strdup(token);
        if (!cmd_buff->argv[cmd_buff->argc]){
		  	return ERR_MEMORY;
		}
        cmd_buff->argc++;
        token = strtok_r(NULL, " ", &saveptr);
    }
    cmd_buff->argv[cmd_buff->argc] = NULL;
    return OK;
}

int exec_local_cmd_loop() {
    char *cmd_buff = malloc(SH_CMD_MAX * sizeof(char));
    if (!cmd_buff){
	 	return ERR_MEMORY;
	 }

    command_list_t cmd_list;

    while (1) {
        printf("%s", SH_PROMPT);
        if (fgets(cmd_buff, SH_CMD_MAX, stdin) == NULL) {
            printf("\n");
            break;
        }
        cmd_buff[strcspn(cmd_buff, "\n")] = '\0';
		trim_whitespace(cmd_buff);		   	



        if (strlen(cmd_buff) == 0) {
            printf(CMD_WARN_NO_CMD);
            continue;
        }
        if (strlen(cmd_buff) >= SH_CMD_MAX) {
            return ERR_TOO_MANY_COMMANDS;
        }

        memset(&cmd_list, 0, sizeof(command_list_t));

        if (build_cmd_list(cmd_buff, &cmd_list) < 0) {
            printf(CMD_ERR_PIPE_LIMIT, CMD_MAX);
            return ERR_TOO_MANY_COMMANDS;
        }


        for (int i = 0; i < cmd_list.num; i++) { 
            Built_In_Cmds cmd_type = match_command(cmd_list.commands[i].argv[0]);
            if (cmd_type != BI_NOT_BI) {
                exec_built_in_cmd(&cmd_list.commands[i]);
                free_cmd_list(&cmd_list); 
                continue;  
            }
        }

        execute_pipeline(&cmd_list);
        free_cmd_list(&cmd_list);
    }

    free(cmd_buff);
    return OK;
}

