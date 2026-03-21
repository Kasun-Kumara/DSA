#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "inventory.h"

// Struct definitions moved to inventory.h

void initialize_inventory(struct player_inventory *list){
    list -> head = NULL;
    list -> tail = NULL;
}

static struct Node_inventory* create_node(const char *name, int rating, const char *type, int price){
    struct Node_inventory *newNode = (struct Node_inventory*)malloc(sizeof(struct Node_inventory));
    if (newNode == NULL) {
        return NULL;
    }

    if (name == NULL) {
        newNode->player_name[0] = '\0';
    }
    else {
        snprintf(newNode->player_name, sizeof(newNode->player_name), "%s", name);
    }


    newNode -> player_rating = rating;
    if (type == NULL) {
        newNode->player_type[0] = '\0';
    }
    else {
        snprintf(newNode->player_type, sizeof(newNode->player_type), "%s", type);
    }
    newNode -> player_price = price;
    newNode -> next = NULL;
    newNode -> prev = NULL;
    return newNode;
}

void traverse_inventory(struct player_inventory *list){
    struct Node_inventory *temp = list->head;
    printf("    Player Name   | Rating |  Type  |   Price   \n");
    printf("-----------------------------------------------\n");

    while (temp != NULL){
        printf(" %16s | %5d  |%5s   | %8d \n",temp->player_name, temp->player_rating, temp->player_type, temp->player_price);
        temp = temp -> next;
    }
    printf("-----------------------------------------------\n");
}

bool is_empty(struct player_inventory* list){
    return list->head == NULL;
}

void add_player(struct player_inventory* list, const char *name, int rating, const char *type, int price){//insert_back
    struct Node_inventory* newNode = create_node(name, rating, type, price);
    if (newNode == NULL) {
        fprintf(stderr, "Failed to allocate memory for player.\n");
        return;
    }

    if (is_empty(list)) {
        list->head = newNode;
        list->tail = newNode;
    }
    else{
        newNode -> prev = list -> tail;
        list -> tail -> next = newNode;
        list -> tail = newNode;
    }
}

//void insert_front(struct player_inventory* list, char name[25], char code[10], int rating, char type[5]){
//    struct Node* newNode = create_node(name, code, rating, type);
//    if (is_empty(list)) {
//        list->head = newNode;
//        list->tail = newNode;
//    }
//    else{
//        newNode -> next = list -> head;
//        list -> head = newNode;
//    }
//}

void search_inventory(struct player_inventory* list, const char *name, int rating){
    if (is_empty(list)) {
        printf("List is Empty!!!\n");
    }
    else{
        struct Node_inventory* temp = list -> head;
        while (temp != NULL) {
            if (strcmp(temp->player_name, name) == 0 && temp->player_rating == rating) {
                printf("\nPlayer Found...\n");
                printf("____________________________\n");
                printf("|                          |\n");
                printf(" %16s | %8d | %5d  |%5s\n",temp->player_name, temp->player_price, temp->player_rating, temp->player_type);
                printf("|__________________________|\n\n\n");
            }
            temp = temp -> next;
        }
    }
}

void sell_player(struct player_inventory* list, const char *name, int rating){ //delete_mid
    if (is_empty(list)){
        printf("Inventory is Empty\n");
    }
    else {
        struct Node_inventory* temp = list -> head;

        while (temp != NULL && (strcmp(temp->player_name, name) != 0 || temp->player_rating != rating)) {
            temp = temp -> next;
        }

        if (temp == NULL){
            printf("No Player with the code : %s in the inventory\n", name);
        }
        else{
            if (temp->prev != NULL) {
                temp->prev->next = temp->next;
            }
            else {
                list->head = temp->next;
            }

            if (temp->next != NULL) {
                temp->next->prev = temp->prev;
            }
            else {
                list->tail = temp->prev;
            }

            free(temp);
        }
    }
}

struct Node_inventory* find_player_by_name(struct player_inventory* list, const char *name) {
    if (is_empty(list)) return NULL;
    struct Node_inventory* temp = list->head;
    while (temp != NULL) {
        if (strcmp(temp->player_name, name) == 0) {
            return temp;
        }
        temp = temp->next;
    }
    return NULL;
}

int main(void) {


    return 0;
}
