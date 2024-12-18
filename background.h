#define ARGV_BUFFER_SIZE 1024

#include "helperfunctions.h"

extern int bg_list_size;
extern id_linked_node* bg_root;

int kill(pid_t pid, int siq);

/**
 * @brief general function to handle background processes
 * @param argv list of arguements for the background execution
 * @param argc number of arguements
 */
int handleBackground(char* argv[], int argc) {
    //remove the first arguement (remove bg from our arguements)
    shiftTokenizedString(argv, argc);
    char command[ARGV_BUFFER_SIZE];
    unTokenizeString(command, argv, argc-1);

    pid_t process_num = fork();

    /*If process number is = 0, we are the child process*/
    if (process_num == 0) {
        signal(SIGINT, handleRootCTRLC);
        freopen("/dev/null", "w", stdout);
        execvp(argv[0], argv);
        fclose(stdout);
        exit(0);

    }
    
    /*If process number is > 0, we are the parent process*/
    if (process_num > 0) {
        id_linked_node* new_child = create_id_node(command, process_num);
        if (bg_root == NULL) {
            bg_root = new_child;
        }
        else {
            append_node(bg_root, new_child);
        }
        bg_list_size ++;
    }
    return 0;

}

/**
 * @brief Handles finished background processses
 */
void updateBackground() {
    if (bg_list_size > 0) {
        pid_t ter = waitpid(0, NULL, 1);
        while (ter > 0) {
            //If the finished process is referenced in the root node
            if (bg_root->id == ter) {
                id_linked_node* temp = bg_root;
                printf("%d: %s has terminated.\n", ter, temp->data);
                bg_root = bg_root->next;
                free(temp->data);
                free(temp);
                bg_list_size--;
            }
            //If the finished process is not referenced in the root node
            else {
                id_linked_node* curr = bg_root;
                while(curr->next != NULL && curr->next->id != ter) {
                    curr = curr->next;
                }
                if (curr->next == NULL) {
                    printf("ERROR, CHILD PROCESS NOT RECORDED\n");
                }
                else {
                    id_linked_node* temp = curr->next;
                    printf("%d: %s has terminated.\n", ter, temp->data);
                    curr->next = curr->next->next;
                    free(temp->data);
                    free(temp);
                    bg_list_size--;
                }
            }

            ter = waitpid(0, NULL, 1);
        }
    }
}

/**
 * @brief prints out a list of all current background processses that are still running/executing
 */
void printBackgroundProcesses() {
    if (bg_root == NULL) {
        printf("There are no background jobs running\n");
        return;
    }
    id_linked_node* curr = bg_root;
    int count = 1;
    while (curr != NULL) {
        printf("%d: %s %d\n", curr->id, curr->data, count);
        curr = curr->next;
        count++;
    }
    printf("Total Background Jobs: %d\n", count-1);
}

void killBackgroundProcess(int pid) {
    /*If no processes to kill*/
    if (bg_root == NULL) {
        printf("ERROR: No background proccesses to terminate\n");
        return;
    }

    /*Check each element in list to see if pid matches*/
    id_linked_node* curr = bg_root;
    while (curr != NULL) {
        if (curr->id == pid) {
            kill(pid, SIGKILL);
            break;
        }
        else {
            curr = curr->next;
        }
    }
    /*If did not find, print error*/
    if (curr == NULL) {
        printf("ERROR: No background proccess running with an ID of: %d\n", pid);
    }
}