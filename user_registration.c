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

void delete_account(struct user_registry* list, int id);

static void initialize_list(struct user_registry *list){
    list -> head = NULL;
    list -> tail = NULL;
}

static struct userNode* create_node(const char *name, int id){
    struct userNode *newNode = (struct userNode*)malloc(sizeof(struct userNode));
    if (newNode == NULL) {
        return NULL;
    }

    if (name == NULL) {
        newNode->user_name[0] = '\0';
    }
    else {
        snprintf(newNode->user_name, sizeof(newNode->user_name), "%s", name);
    }
    newNode -> user_id = id;
    newNode -> rank = 'E';
    newNode -> score = 0;
    newNode -> next = NULL;
    newNode -> prev = NULL;
    return newNode;
}

static void traverse_users(struct user_registry *list){
    struct userNode *temp = list->head;
    printf("  User Name |  Rank |   Score \n");
    printf("-------------------------------------\n");

    while (temp != NULL){
        printf(" %10s | %5c | %5d \n",temp->user_name, temp->rank, temp->score);
        temp = temp -> next;
    }
    printf("-------------------------------------\n");
}

static bool is_empty(struct user_registry* list){
    return list->head == NULL;
}

static int new_ID_provider(struct user_registry* list) {
    if (is_empty(list)) {
        return 1;
    }
    return list -> tail -> user_id + 1;
}


static void add_user(struct user_registry* list, const char *name){//insert_back
    int id = new_ID_provider(list);
    struct userNode* newNode = create_node(name, id);
    if (newNode == NULL) {
        fprintf(stderr, "Failed to allocate memory for new user.\n");
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

static void searchUser(struct user_registry* list, int id){
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
                printf("| %6s | %5c | %8d |\n",temp->user_name, temp->rank, temp->score);
                printf("|________|_______|__________|\n\n\n");
            }
            temp = temp -> next;
        }
    }
}

void delete_account(struct user_registry* list, int id){ //delete_mid
    if (is_empty(list)){
        printf("User does not exist.\n");
    }
    else {
        struct userNode* temp = list -> head;
        while (temp != NULL && temp->user_id != id) {
            temp = temp->next;
        }
        if (temp == NULL){
            printf("No user with the id : %d in the user space.\n", id);
        }
        else {
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

int main(void) {
    return 0;
}
