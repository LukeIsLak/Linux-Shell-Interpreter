#include <stdio.h>

/**
 * @brief converts a a string and tokenizes it at " " and "\n"
 * @param input the input string
 * @param argv a list of strings to fill tokens into
 * @param argc the size of the list
 */

void tokenizeString(char* input, char* argv[], int* argc) {
    argv[0] = strtok(input, " \n");

    int i = 0;

    while (argv[i] != NULL) {
        argv[i+1] = strtok(NULL, " \n");
        i++;
    }
    *argc = i;
}

/**
 * @brief converts a list of strings into a single string
 * @param dest the location of the string
 * @param argv the list of strings
 * @param argc the size of the list
 */

void unTokenizeString(char* dest, char* argv[], int argc) {
    int i = 0;
    while (i < argc) {
        strcat(dest, argv[i]);

        if (i+1 < argc) {
            strcat(dest, " ");
        }
        i++;
    }
    strcat(dest, "\0");
}

/**
 * @brief shifts the array to the left, the extra variable is dropped
 * @param argv the list to be shifted
 * @param argc number of arguements
 */

void shiftTokenizedString(char* argv[], int argc) {
    int i = 1;
    while (i <= argc) {
        argv[i-1] = argv[i];
        i++;
    }
    argv[i] = NULL;
}