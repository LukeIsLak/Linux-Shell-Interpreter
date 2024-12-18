extern double_linked_node* commandHistory;

/**
* @brief a function to handle when the up arrow is pressed
*/
int upArrowFunction() { 
    if (commandHistory != NULL) {
        rl_replace_line("", 0);
        rl_redisplay(); 
        rl_insert_text(commandHistory->data);
        if (commandHistory->prev != NULL) {
            commandHistory = commandHistory->prev;
        }
        return 0;
    }
    return 1;
}

/**
* @brief a function to handle when the down arrow is pressed
*/
int downArrowFunction() {
    if (commandHistory != NULL && commandHistory->next != NULL) {
        rl_replace_line("", 0);
        rl_redisplay(); 
        rl_insert_text(commandHistory->next->data);
        if (commandHistory->next != NULL) {
            commandHistory = commandHistory->next;
        }
        return 0;
    }
    else if ((commandHistory != NULL && commandHistory->next == NULL) || commandHistory == NULL) {
        rl_replace_line("", 0);
        rl_redisplay(); 
        return 0;
    }
    else {
        return 1;
    }
}