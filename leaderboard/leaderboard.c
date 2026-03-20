#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define the Player structure
typedef struct {
    int id;
    char name[50];
    int score;
} Player;

// Define the Circular Linked List Node
typedef struct Node {
    Player data;
    struct Node* next;
} Node;

// Function to create a new node
Node* createNode(Player player) {
    Node* newNode = (Node*)malloc(sizeof(Node));
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

// Main function to demonstrate the console application
int main() {
    Node* leaderboard = NULL;
    int choice;
    Player tempPlayer;
    int searchId;

    while (1) {
        printf("\n--- FC Mobile Manager Console (FCMC) ---\n");
        printf("1. Add/Update Player Score\n");
        printf("2. View Leaderboard\n");
        printf("3. Search Player Rank\n");
        printf("4. Exit\n");
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
            case 4:
                printf("Exiting FCMC Leaderboard System...\n");
                exit(0);
            default:
                printf("[!] Invalid choice. Please try again.\n");
        }
    }

    return 0;
}
