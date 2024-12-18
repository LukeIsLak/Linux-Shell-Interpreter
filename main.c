/**
 * @brief A Simple Shell Interpreter that which primary function handles foreground execution, background execution, and change in directory
 * @brief Can handle additional things, refer to readme.txt
 * @author Luke Kuligowicz
 */

#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

//P1 Custom Libraries
#include "linkedlists.h"
#include "command.h"
#include "token.h"
#include "background.h"

//Buffer Sizes
#define CWD_BUFFER_SIZE 1024
#define HOST_BUFFER_SIZE 1024
#define PROMT_BUFFER_SIZE 1024
#define ARGV_BUFFER_SIZE 1024

//Define CTRL-C Handler
#define SIGINT 2


//Functions declared in this file
void initialize(char* cwdAddr, char* hostNameAdd);
void handleChangeInDirectory(char* argv[], char* cwdAddr);
void exitProgram();
void updatePrompt(char* prompt, char* username, char* hostname, char* cwd);

//Library functions that need to declaration
int gethostname(char *name, size_t len);
char* readline (const char *prompt);
int execvp(const char *file, char *const argv[]);
pid_t fork(void);
pid_t waitpid(pid_t pid, int *wstatus, int options);

//Global Variables and Roots
id_linked_node* bg_root;
int bg_list_size = 0;
double_linked_node* commandHistory;

int main() {

    /*Initialize the ssi*/
    char* username = getlogin();
    char cwd[CWD_BUFFER_SIZE];
    char hostname[HOST_BUFFER_SIZE];
    char prompt[PROMT_BUFFER_SIZE] = "\0";

    /*Initialize the ssi*/
    initialize(cwd, hostname);
    updatePrompt(prompt, username, hostname, cwd);

    /*Handle Ctrl-C*/
    signal(SIGINT, handleRootCTRLC);
    

    /*Do on a loop*/
    while (1) {

        /*Get the user input*/
        char* input = readline(prompt);

        /*Otherwise, add input to command history*/
        double_linked_node* new_command = create_double_node(input);
        if (commandHistory != NULL) {
            while (commandHistory->next != NULL) {
                commandHistory = commandHistory->next;
            }
        }
        if (commandHistory != NULL) {
            new_command->prev = commandHistory;
            commandHistory->next = new_command;
        }
        commandHistory = new_command;
        char* argv[ARGV_BUFFER_SIZE];
        int argc;

        /*Tokenize the input*/
        tokenizeString(input, argv, &argc);

        if (argv[0]) {
            /*If the input is exit - to exit the program*/
            if(strcmp(argv[0], "exit") == 0) {
                free(input);
                exitProgram();
                break;
            }
            /*If input is cd - handle change in directory*/
            if(strcmp(argv[0], "cd") == 0) {
                handleChangeInDirectory(argv, cwd);
                updatePrompt(prompt, username, hostname, cwd);
                updateBackground();
                free(input);
                continue;
            }
            /*If the input is bg - handle background processes*/
            else if(strcmp(argv[0], "bg") == 0) {
                handleBackground(argv, argc);
                updateBackground();
                free(input);
                continue;
            }
            /*If the input is bglist - handle print background*/
            else if (strcmp(argv[0], "bglist") == 0) {
                printBackgroundProcesses(bg_root);
                updateBackground();
                free(input);
                continue;
            }
            /*If the input is bgkill - handle killing child process*/
            else if (strcmp(argv[0], "bgkill") == 0) {
                if (argc > 1) {
                    int pid = atoi(argv[1]);
                    /*If invalid input (not an int)*/
                    if (pid == 0) {
                        printf("ERROR: Expected integer value for bgkill\n");

                    }
                    else {
                        killBackgroundProcess(pid);
                        sleep(1);
                    }

                }
                /*If no input*/
                else {
                    printf("ERROR: Cannot execute: %s, expecting process ID\n", argv[0]);
                }
                    updateBackground();
                    free(input);
                    continue;
            }
        }


        /*Create child process*/
        pid_t p = fork();

        /*If we are in the child process*/
        if (p == 0) {
            signal(SIGINT, handleChildCTRLC);
            if (execvp(argv[0], argv) == -1) {
                printf("%s: No such file or directory\n", argv[0]);
            }
            exit(0);
        }

        /*If we are in the parent process*/
        if (p > 0) {
            waitpid(p, NULL, 0);
        }

        free(input);
        updateBackground();
    }
    return 0;
}


/**
 * @brief Handles all functions relating to change in directory (cd)
 * @param argv a list of all arguments passed through
 * @param cwdAddr pointer to the cwdAddr to be changed
 */
void handleChangeInDirectory(char* argv[], char* cwdAddr) {
    if (argv[1] == NULL || strcmp(argv[1], "~") == 0) {
        chdir(getenv("HOME"));
    }
    else if (argv[1] == NULL || argv[1][0] == '~') {
        chdir(getenv("HOME"));
        if (strlen(argv[1]+2) == 0) {
            return;
        }
        if(chdir(argv[1]+2)==-1) {
            printf("<%s> No such directory\n", argv[1]+2);
            return;
        }
    }
    else {
        if (chdir(argv[1]) == -1) {
            printf("<%s> No such directory\n", argv[1]);
            return;
        }
    }

    getcwd(cwdAddr, CWD_BUFFER_SIZE);
}

/**
* @brief Clears the current prompt string and replaces it with an updated prompt of with updated variables
* @param prompt is a refrence to the destination
* @param username a string of the username of the system
* @param hostname a string of the hostname of the system
* @param cwd a string of the current working directory of the process
*/
void updatePrompt(char* prompt, char* username, char* hostname, char* cwd) {
    strcpy(prompt, "");
    strcat(strcat(strcat(strcat(strcat(strcat(prompt, username), "@"), hostname), ": "), cwd), " > ");
}

/**
* @brief initialzes the SSI with certain key inputs and variables
* @param cwdAddr reference to the cwd variable, gets updated to a string of the current working directory of the process
* @param hostNameAddr refrences the hostname variable, gets updates to a string of the hostname
*/
void initialize(char* cwdAddr, char* hostNameAddr) {
    rl_bind_keyseq("\033[A", upArrowFunction);
    rl_bind_keyseq("\033[B", downArrowFunction);

    getcwd(cwdAddr, CWD_BUFFER_SIZE);
    gethostname(hostNameAddr, HOST_BUFFER_SIZE);
}

/**
* @brief a function to handle the exit of a program and to handle freeing any allocated memory
*/
void exitProgram() {
    /*Free command history*/
    while (commandHistory != NULL) {
        free(commandHistory->data);
        double_linked_node* temp = commandHistory->prev;
        free(commandHistory);
        commandHistory = temp;
    }
    /*Free background history*/
    while (bg_root != NULL) {
        killBackgroundProcess(bg_root->id);
        free(bg_root->data);
        id_linked_node* temp = bg_root->next;
        free(bg_root);
        bg_root = temp;
    }
}