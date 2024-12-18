#include <stdio.h>

typedef struct double_linked_node {
    char* data;
    struct double_linked_node* next;
    struct double_linked_node* prev;
} double_linked_node;

typedef struct id_linked_node {
    char* data;
    int id;
    struct id_linked_node* next;
} id_linked_node;

/**
 * @brief allocates a space in memory for a node for linked lists
 * @param input is a string that the double node will store
 * @return returns a pointer to the created node
 */
double_linked_node* create_double_node(char* input) {
    double_linked_node* new_node = (double_linked_node *)malloc(sizeof(double_linked_node));
    new_node->data = (char *)malloc(sizeof(char) * strlen(input));
    strcpy(new_node->data, input);
    new_node->next = NULL;
    new_node->prev = NULL;
    return new_node;
}

/**
 * @brief allocates a space in memory for a node with an id for linked lists
 * @param input is a string that the double node will store
 * @param id the id to assign to the node
 * @return returns a pointer to the created node
 */
id_linked_node* create_id_node(char* input, int id) {
    id_linked_node* new_node = (id_linked_node *)malloc(sizeof(id_linked_node));
    new_node->data = (char *)malloc(sizeof(char) * strlen(input));
    strcpy(new_node->data, input);
    new_node->id = id;
    new_node->next = NULL;
    return new_node;
}

/**
 * @brief appends a node to the end of a double linked list
 * @param root is the node to start off at
 * @param node is the node to append
 */
void append_double_node(double_linked_node* root, double_linked_node* node) {
    double_linked_node* end = root;
    while(end->next != NULL) {
        end = end->next;
    }
    end->next = node;
}

/**
 * @brief appends a node to the end of a linked list
 * @param root is the node to start off at
 * @param node is the node to append
 */
void append_node(id_linked_node* root, id_linked_node* node) {
    id_linked_node* temp = root;
    while(temp->next != NULL) {
        temp = temp->next;
    }
    temp->next = node;
    return;
}