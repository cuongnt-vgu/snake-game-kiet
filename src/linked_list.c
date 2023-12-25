#include "linked_list.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function to get the value at a given index in the linked list

void* get(node_t* head_list, int index){
    node_t* current = head_list;
    int count = 0;
    while (current != NULL){
        if (count == index){
            return current->data;
        }
        count++;
        current = current->next;
    }
    return NULL;
}

// Function to insert a new node at the beginning of the linked list
void insert_first(node_t** head_list, void* to_add, size_t size){
    node_t* new_node = (node_t*)malloc(sizeof(node_t));
    new_node->data = malloc(size);
    memcpy(new_node->data, to_add, size);
    new_node->next = (*head_list);
    (*head_list) = new_node;
}

// Function to remove the last node from the linked list
void* remove_last(node_t** head_list){
    if (*head_list == NULL){
        return NULL;
    }
    else if ((*head_list)->next == NULL){
        void* data = (*head_list)->data;
        free(*head_list);
        *head_list = NULL;
        return data;
    }
    else{
        node_t* current = *head_list;
        while (current->next->next != NULL){
            current = current->next;
        }
        void* data = current->next->data;
        free(current->next);
        current->next = NULL;
        return data;
    }
}
