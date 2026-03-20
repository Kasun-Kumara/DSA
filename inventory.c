#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

struct player_inventory {
    struct Node_inventory *head;
    struct Node_inventory *tail;
};

struct Node_inventory {
    
    char player_name[25];
    int player_rating;
    char player_type[5];
    int player_price;
    struct Node_inventory *next;
    struct Node_inventory *prev;
};

void initialize_inventory(struct player_inventory *list){
    list -> head = NULL;
    list -> tail = NULL;
}

struct Node_inventory* create_node(char name[25], int rating, char type[5], int price){
    struct Node_inventory *newNode;
    newNode = (struct Node_inventory*)malloc(sizeof(struct Node_inventory));
    strcpy(newNode -> player_name, name);
    newNode -> player_rating = rating;
    strcpy(newNode -> player_type, type);
    newNode -> player_price = price;
    newNode -> next = NULL;
    newNode -> prev = NULL;
    return newNode;
}

void traverse_inventory(struct player_inventory *list){
    struct Node_inventory *temp = list->head;
    printf("    Player Name   | Rating |  Type  |   Price   \n");
    printf("-----------------------------------------------\n");
    int i = 1;
    
    while (temp != NULL){
        printf(" %16s | %5d  |%5s   | %8d \n",temp->player_name, temp->player_rating, temp->player_type, temp->player_price);
        temp = temp -> next;
        i++;
    }
    printf("-----------------------------------------------\n");
}

bool is_empty(struct player_inventory* list){
    if (list->head == NULL)
        return true;
    else
        return false;
}

void add_player(struct player_inventory* list, char name[25], int rating, char type[5], int price){//insert_back
    struct Node_inventory* newNode = create_node(name, rating, type, price);
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

void search_inventory(struct player_inventory* list, char name[25], int rating){
    if (is_empty(list)) {
        printf("List is Empty!!!\n");
    }
    else{
        struct Node_inventory* temp = list -> head;
        while (temp != NULL) {
            if (temp -> player_name == name && temp -> player_rating == rating) {
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

void sell_player (struct player_inventory* list, char name[25], int rating){ //delete_mid
    if (is_empty(list)){
        printf("Inventory is Empty\n");
    }
    else {
        int i = 1;
        struct Node_inventory* temp = list -> head;
        while (temp -> player_name == name && temp -> player_rating == rating) {
            temp = temp -> next;
            i++;
        }
        if (temp == NULL){
            printf("No Player with the code : %s in the inventory\n", name);
        }
        else if (temp == list -> tail) {
            struct Node_inventory* temp = list -> tail;
            list -> tail = list -> tail -> prev;
            list -> tail -> next = NULL;
            free(temp);
        }
        else{
            temp -> next -> prev = temp -> prev;
            temp -> prev -> next = temp -> next;
            free(temp);
        }
    }
}

int main(void) {

    return 0;
}
