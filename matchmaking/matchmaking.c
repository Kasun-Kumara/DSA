#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

// User structure
typedef struct User {
    int user_id;
    char username[50];
    int coins;
    int level; // 1 to 9 representing levels from Amateur 3 to Legendary
} User;

// Level constants for readability
#define AMATEUR_3 1
#define AMATEUR_2 2
#define AMATEUR_1 3
#define PRO_3 4
#define PRO_2 5
#define PRO_1 6
#define WORLD_CLASS_2 7
#define WORLD_CLASS_1 8
#define LEGENDARY 9

#define NUM_LEVELS 9

const char* get_level_name(int level) {
    switch(level) {
        case AMATEUR_3: return "Amateur 3";
        case AMATEUR_2: return "Amateur 2";
        case AMATEUR_1: return "Amateur 1";
        case PRO_3: return "Pro 3";
        case PRO_2: return "Pro 2";
        case PRO_1: return "Pro 1";
        case WORLD_CLASS_2: return "World Class 2";
        case WORLD_CLASS_1: return "World Class 1";
        case LEGENDARY: return "Legendary";
        default: return "Unknown";
    }
}

int get_win_reward(int level) {
    switch(level) {
        case AMATEUR_3: return 100;
        case AMATEUR_2: return 150;
        case AMATEUR_1: return 200;
        case PRO_3: return 300;
        case PRO_2: return 400;
        case PRO_1: return 500;
        case WORLD_CLASS_2: return 750;
        case WORLD_CLASS_1: return 1000;
        case LEGENDARY: return 2000;
        default: return 50;
    }
}

// Queue Node
typedef struct Node {
    User* user;
    struct Node* next;
} Node;

// Queue
typedef struct Queue {
    Node* front;
    Node* rear;
} Queue;

void initializeQueue(Queue* queue) {
    queue->front = NULL;
    queue->rear = NULL;
}

Node* createNode(User* user) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL) {
        printf("Memory allocation failed\n");
        exit(1);
    }
    newNode->user = user;
    newNode->next = NULL;
    return newNode;
}

bool isEmpty(Queue* queue) {
    return (queue->front == NULL);
}

void enqueue(Queue* queue, User* user) {
    Node* newNode = createNode(user);
    if (isEmpty(queue)) {
        queue->front = newNode;
        queue->rear = newNode;
    } else {
        queue->rear->next = newNode;
        queue->rear = newNode;
    }
}

User* dequeue(Queue* queue) {
    if (isEmpty(queue)) {
        return NULL;
    }
    Node* temp = queue->front;
    User* user = temp->user;

    if (queue->front == queue->rear) {
        queue->front = NULL;
        queue->rear = NULL;
    } else {
        queue->front = queue->front->next;
    }
    free(temp);
    return user;
}

// Matchmaking System
typedef struct MatchmakingSystem {
    Queue queues[NUM_LEVELS];
} MatchmakingSystem;

void initializeMatchmakingSystem(MatchmakingSystem* system) {
    for (int i = 0; i < NUM_LEVELS; i++) {
        initializeQueue(&system->queues[i]);
    }
}

void findMatch(MatchmakingSystem* system, User* player) {
    if (player->level < 1 || player->level > NUM_LEVELS) {
        printf("Invalid level for player %s.\n", player->username);
        return;
    }

    int queueIndex = player->level - 1;
    Queue* targetQueue = &system->queues[queueIndex];

    if (isEmpty(targetQueue)) {
        // No one in queue, wait for a match
        printf("Player %s joined the %s queue. Waiting for an opponent...\n", player->username, get_level_name(player->level));
        enqueue(targetQueue, player);
    } else {
        // Match found!
        User* opponent = dequeue(targetQueue);
        printf("\n*** MATCH FOUND! ***\n");
        printf("%s (Level: %s) VS %s (Level: %s)\n", player->username, get_level_name(player->level), opponent->username, get_level_name(opponent->level));
        
        // Simulate a match result randomly
        int winner = rand() % 2; // 0 for player, 1 for opponent
        int reward = get_win_reward(player->level);

        printf("Match is being played...\n");
        if (winner == 0) {
            printf("%s wins the match!\n", player->username);
            player->coins += reward;
            printf("%s receives %d coins. New Balance: %d\n", player->username, reward, player->coins);
        } else {
            printf("%s wins the match!\n", opponent->username);
            opponent->coins += reward;
            printf("%s receives %d coins. New Balance: %d\n", opponent->username, reward, opponent->coins);
        }
        printf("********************\n\n");
    }
}

int main() {
    srand(time(NULL)); // Seed for random match results
    
    MatchmakingSystem ms;
    initializeMatchmakingSystem(&ms);

    User p1 = {1, "PlayerOne", 1000, AMATEUR_2};
    User p2 = {2, "PlayerTwo", 1200, LEGENDARY};
    User p3 = {3, "PlayerThree", 500, AMATEUR_2};
    User p4 = {4, "PlayerFour", 3000, LEGENDARY};

    findMatch(&ms, &p1);
    findMatch(&ms, &p2);
    
    // P3 triggers a match with P1
    findMatch(&ms, &p3);
    
    // P4 triggers a match with P2
    findMatch(&ms, &p4);

    return 0;
}