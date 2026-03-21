#ifndef LEADERBOARD_H
#define LEADERBOARD_H

#include "../common.h"

// Define the Circular Linked List Node
typedef struct Node {
    Player data;
    struct Node* next;
} Node;

// Leaderboard Function Prototypes
Node* createNode(Player player);
void insertPlayer(Node** head_ref, Player new_player);
void bubbleSortLeaderboard(Node* head);
void displayLeaderboard(Node* head);
void getPlayerRankAndDetails(Node* head, int playerId);
Node* findPlayer(Node* head, int playerId); // Helper inside leaderboard to find player pointer

#endif // LEADERBOARD_H
