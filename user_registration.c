#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

struct user_registry {
    struct userNode *head;
    struct userNode *tail;
};

struct userNode {
    char user_name[25];
    int user_id;
    char rank;
    int score;
    struct userNode *next;
    struct userNode *prev;
};

void initialize_list (struct user_registry *list){
    list -> head = NULL;
    list -> tail = NULL;
}

struct userNode* create_node(char name[25], int id){
    struct userNode *newNode;
    newNode = (struct userNode*)malloc(sizeof(struct userNode));
    
    strcpy(newNode -> user_name, name);
    newNode -> user_id = id;
    newNode -> rank = 'E';
    newNode -> score = 0;
    newNode -> next = NULL;
    newNode -> prev = NULL;
    return newNode;
}

void traverse_users(struct user_registry *list){
    struct userNode *temp = list->head;
    printf("  User Name |  Rank |   Score \n");
    printf("-------------------------------------\n");
    int i = 1;
    
    while (temp != NULL){
        printf(" %10s | %5c | %5d \n",temp->user_name, temp->rank, temp->score);
        temp = temp -> next;
        i++;
    }
    printf("-------------------------------------\n");
}

bool is_empty(struct user_registry* list){
    if (list->head == NULL)
        return true;
    else
        return false;
}

int new_ID_provider(struct user_registry* list) {
    if (is_empty(list)) {
        return 1;
    }
    return list -> tail -> user_id+1;
}


void add_user(struct user_registry* list, char name[25]){//insert_back
    int id = new_ID_provider(list);
    struct userNode* newNode = create_node(name, id);
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

void searchUser(struct user_registry* list, int id){
    if (is_empty(list)) {
        printf("List is Empty!!!\n");
    }
    else{
        struct userNode* temp = list -> head;
        while (temp != NULL) {
            if (temp->user_id == id) {
                printf("\nUser Found...\n");
                printf("_____________________________\n");
                printf("|        |       |          |\n");
                printf("| %6s | %5d | %8d |\n",temp->user_name, temp->rank, temp->score);
                printf("|________|_______|__________|\n\n\n");
            }
            temp = temp -> next;
        }
    }
}

void delete_account (struct user_registry* list, int id){ //delete_mid
    if (is_empty(list)){
        printf("User does not exists.\n");
    }
    else if (list->head == list->tail){
        list -> head = NULL;
        list -> tail = NULL;
    }
    else {
        struct userNode* temp = list -> head;
        while (temp != NULL && temp->user_id != id) {
            temp = temp->next;
        }
        if (temp == NULL){
            printf("No user with the id : %d in the user space.\n", id);
        }
        else if (temp == list -> tail) {
            struct userNode* temp = list -> tail;
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
    struct user_registry users;
    initialize_list(&users);
    add_user(&users, "senuka");
    add_user(&users, "deneth");
    add_user(&users, "nimsara");
    searchUser(&users, 2);
    
    traverse_users(&users);

    return 0;
}
