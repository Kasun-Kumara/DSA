#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Match History Stack Node (LIFO)
typedef struct MatchNode {
    char result[10]; // "Win", "Loss", "Draw"
    int pointsChange;
    struct MatchNode* next;
} MatchNode;

// Define the Player structure
typedef struct {
    int id;
    char name[50];
    int score;
    MatchNode* historyTop; // Stack top pointer
} Player;

// Define the Circular Linked List Node
typedef struct Node {
    Player data;
    struct Node* next;
} Node;

// Function to create a new node
Node* createNode(Player player) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    player.historyTop = NULL; // Initialize match history stack for new player
    newNode->data = player;
    newNode->next = newNode; // Points to itself initially
    return newNode;
}

// Function to swap data of two nodes
void swapData(Node* a, Node* b) {
    Player temp = a->data;
    a->data = b->data;
    b->data = temp;
}

// Function to perform Bubble Sort on the Circular Linked List
void bubbleSortLeaderboard(Node* head) {
    if (head == NULL || head->next == head) {
        return;
    }

    int swapped;
    Node* current;
    Node* last_ptr = NULL;

    do {
        swapped = 0;
        current = head;

        while (current->next != head && current->next != last_ptr) {
            // Sort in descending order
            if (current->data.score < current->next->data.score) { 
                swapData(current, current->next);
                swapped = 1;
            }
            current = current->next;
        }
        last_ptr = current;
    } while (swapped);
}

// Function to insert a player into the leaderboard
void insertPlayer(Node** head_ref, Player new_player) {
    Node* new_node = createNode(new_player);

    // Case 1: Empty List
    if (*head_ref == NULL) {
        *head_ref = new_node;
    } else {
        Node* current = *head_ref;
        // Find the last node
        while (current->next != *head_ref) {
            current = current->next;
        }
        // Insert at the end
        current->next = new_node;
        new_node->next = *head_ref;
    }

    // Sort the updated leaderboard using Bubble Sort
    bubbleSortLeaderboard(*head_ref);
}

// Function to display the leaderboard
void displayLeaderboard(Node* head) {
    if (head == NULL) {
        printf("\n[!] Leaderboard is empty.\n");
        return;
    }

    Node* temp = head;
    int rank = 1;

    printf("\n======================================================\n");
    printf("               🏆 GLOBAL LEADERBOARD 🏆               \n");
    printf("======================================================\n");
    printf("Rank  | ID    | Player Name          | Score\n");
    printf("------------------------------------------------------\n");

    do {
        printf("%-5d | %-5d | %-20s | %d\n", rank++, temp->data.id, temp->data.name, temp->data.score);
        temp = temp->next;
    } while (temp != head);
    printf("======================================================\n");
}

// ADDITIONAL CRITERIA: Meaningful Function
// Function to find the exact rank and details of a specific player by ID
void getPlayerRankAndDetails(Node* head, int playerId) {
    if (head == NULL) {
        printf("\n[!] Leaderboard is empty. Player not found.\n");
        return;
    }

    Node* temp = head;
    int rank = 1;
    int found = 0;

    do {
        if (temp->data.id == playerId) {
            printf("\n--- Player Found ---\n");
            printf("Rank: %d\n", rank);
            printf("Name: %s\n", temp->data.name);
            printf("Score: %d\n", temp->data.score);
            found = 1;
            break;
        }
        rank++;
        temp = temp->next;
    } while (temp != head);

    if (!found) {
        printf("\n[!] Player with ID %d not found on the leaderboard.\n", playerId);
    }
}

// ================= MATCH HISTORY (STACK - LIFO) ================= 

// Helper: Find player node by ID
Node* findPlayer(Node* head, int playerId) {
    if (head == NULL) return NULL;
    Node* temp = head;
    do {
        if (temp->data.id == playerId) return temp;
        temp = temp->next;
    } while (temp != head);
    return NULL;
}

// Function to record a new match (Push to Stack)
void pushMatch(Node** head_ref, int playerId, const char* result, int points) {
    Node* playerNode = findPlayer(*head_ref, playerId);
    if (playerNode == NULL) {
        printf("\n[!] Player with ID %d not found. Cannot record match.\n", playerId);
        return;
    }

    MatchNode* newMatch = (MatchNode*)malloc(sizeof(MatchNode));
    strcpy(newMatch->result, result);
    newMatch->pointsChange = points;
    
    // Push operation: new node points to current top, then becomes new top
    newMatch->next = playerNode->data.historyTop;
    playerNode->data.historyTop = newMatch;

    // Update the player's overall score
    playerNode->data.score += points;
    
    // Re-sort the leaderboard since score changed
    bubbleSortLeaderboard(*head_ref);
    printf("\n[+] Match recorded! %s (%s, %+d points). New Score: %d\n", 
           playerNode->data.name, result, points, playerNode->data.score);
}

// Function to view match history (Traverse Stack)
void traverseMatchHistory(Node* head, int playerId) {
    Node* playerNode = findPlayer(head, playerId);
    if (playerNode == NULL) {
        printf("\n[!] Player with ID %d not found.\n", playerId);
        return;
    }

    MatchNode* currentMatch = playerNode->data.historyTop;
    printf("\n======================================================\n");
    printf("     Match History for %s (Most Recent First)     \n", playerNode->data.name);
    printf("======================================================\n");

    if (currentMatch == NULL) {
        printf(" No matches played yet.\n");
    } else {
        int matchNum = 1;
        while (currentMatch != NULL) {
            printf(" %d. Result: %-5s | Points Change: %+d\n", matchNum++, currentMatch->result, currentMatch->pointsChange);
            currentMatch = currentMatch->next;
        }
    }
    printf("======================================================\n");
}

// Function to undo last match (Pop from Stack)
void popMatch(Node** head_ref, int playerId) {
    Node* playerNode = findPlayer(*head_ref, playerId);
    if (playerNode == NULL) {
        printf("\n[!] Player with ID %d not found.\n", playerId);
        return;
    }

    if (playerNode->data.historyTop == NULL) {
        printf("\n[!] Match history is empty for %s. Nothing to undo.\n", playerNode->data.name);
        return;
    }

    MatchNode* topMatch = playerNode->data.historyTop;
    int pointsToRevert = topMatch->pointsChange;

    // Pop operation: top pointer moves to next, free old top
    playerNode->data.historyTop = topMatch->next;
    free(topMatch);

    // Revert the player's overall score
    playerNode->data.score -= pointsToRevert;

    // Re-sort the leaderboard
    bubbleSortLeaderboard(*head_ref);
    printf("\n[-] Last match undone for %s. Reverted %+d points. Restored Score: %d\n", 
           playerNode->data.name, pointsToRevert, playerNode->data.score);
}

// Main function to demonstrate the console application
int main() {
    Node* leaderboard = NULL;
    int choice;
    Player tempPlayer;
    int searchId;

    while (1) {
        printf("\n--- FC Mobile Manager Console (FCMC) ---\n");
        printf("1. Add/Update Player on Leaderboard\n");
        printf("2. View Leaderboard\n");
        printf("3. Search Player Rank\n");
        printf("4. Record Match Result (Push to Stack)\n");
        printf("5. View Match History (Traverse Stack)\n");
        printf("6. Undo Last Match (Pop from Stack)\n");
        printf("7. Exit\n");
        printf("Enter your choice: ");
        
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Exiting...\n");
            break;
        }

        switch (choice) {
            case 1:
                printf("Enter Player ID: ");
                scanf("%d", &tempPlayer.id);
                printf("Enter Player Name: ");
                scanf(" %49[^\n]", tempPlayer.name); // Read string with spaces
                printf("Enter Player Score: ");
                scanf("%d", &tempPlayer.score);
                
                insertPlayer(&leaderboard, tempPlayer);
                printf("[+] Player %s added successfully!\n", tempPlayer.name);
                break;
            case 2:
                displayLeaderboard(leaderboard);
                break;
            case 3:
                printf("Enter Player ID to search: ");
                scanf("%d", &searchId);
                getPlayerRankAndDetails(leaderboard, searchId);
                break;
            case 4: {
                int pid, pts;
                char res[10];
                printf("Enter Player ID: ");
                scanf("%d", &pid);
                printf("Enter Result (Win/Loss/Draw): ");
                scanf(" %9s", res);
                printf("Enter Points Change: ");
                scanf("%d", &pts);
                pushMatch(&leaderboard, pid, res, pts);
                break;
            }
            case 5:
                printf("Enter Player ID to view match history: ");
                scanf("%d", &searchId);
                traverseMatchHistory(leaderboard, searchId);
                break;
            case 6:
                printf("Enter Player ID to undo last match: ");
                scanf("%d", &searchId);
                popMatch(&leaderboard, searchId);
                break;
            case 7:
                printf("Exiting FCMC Leaderboard System...\n");
                exit(0);
            default:
                printf("[!] Invalid choice. Please try again.\n");
        }
    }

    return 0;
}
