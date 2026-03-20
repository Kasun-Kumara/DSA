#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ================= MATCH HISTORY (STACK - LIFO) ================= 

// Match History Stack Node (LIFO)
typedef struct MatchNode {
    char result[10]; // "Win", "Loss", "Draw"
    int pointsChange;
    struct MatchNode* next;
} MatchNode;

// Define the Player structure for this specific feature
typedef struct {
    int id;
    char name[50];
    int score;
    MatchNode* historyTop; // Stack top pointer
} Player;

// Function to initialize a new player
Player* createPlayer(int id, const char* name, int initialScore) {
    Player* newPlayer = (Player*)malloc(sizeof(Player));
    newPlayer->id = id;
    strcpy(newPlayer->name, name);
    newPlayer->score = initialScore;
    newPlayer->historyTop = NULL; // Initialize empty stack
    return newPlayer;
}

// Function to record a new match (Push to Stack)
void pushMatch(Player* player, const char* result, int points) {
    if (player == NULL) return;

    MatchNode* newMatch = (MatchNode*)malloc(sizeof(MatchNode));
    strcpy(newMatch->result, result);
    newMatch->pointsChange = points;
    
    // Push operation: new node points to current top, then becomes new top
    newMatch->next = player->historyTop;
    player->historyTop = newMatch;

    // Update the player's overall score
    player->score += points;
    
    printf("\n[+] Match recorded! %s (%s, %+d points). New Total Score: %d\n", 
           player->name, result, points, player->score);
}

// Function to view match history (Traverse Stack)
void traverseMatchHistory(Player* player) {
    if (player == NULL) return;

    MatchNode* currentMatch = player->historyTop;
    printf("\n======================================================\n");
    printf("     Match History for %s (Most Recent First)     \n", player->name);
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
    printf(" Current Total Score: %d\n", player->score);
    printf("======================================================\n");
}

// Function to undo last match (Pop from Stack) - Meaningful Function
void popMatch(Player* player) {
    if (player == NULL) return;

    if (player->historyTop == NULL) {
        printf("\n[!] Match history is empty for %s. Nothing to undo.\n", player->name);
        return;
    }

    MatchNode* topMatch = player->historyTop;
    int pointsToRevert = topMatch->pointsChange;

    // Pop operation: top pointer moves to next, free old top
    player->historyTop = topMatch->next;
    free(topMatch);

    // Revert the player's overall score
    player->score -= pointsToRevert;

    printf("\n[-] Last match undone for %s. Reverted %+d points. Restored Score: %d\n", 
           player->name, pointsToRevert, player->score);
}

// Main function to demonstrate the individual Stack feature
int main() {
    int choice;
    char res[10];
    int pts;

    printf("======================================================\n");
    printf("      FCMC Match History System (Stack Data Structure)  \n");
    printf("======================================================\n");

    // For demonstration, establish a single player session
    Player* currentPlayer = createPlayer(1, "GuestPlayer", 1000);
    printf("\nProfile initialized for: %s | Starting Score: %d\n", currentPlayer->name, currentPlayer->score);

    while (1) {
        printf("\n--- Match History Menu ---\n");
        printf("1. Record Match Result (Push)\n");
        printf("2. View Match History (Traverse)\n");
        printf("3. Undo Last Match (Pop)\n");
        printf("4. Exit profile\n");
        printf("Enter your choice: ");
        
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Exiting...\n");
            break;
        }

        switch (choice) {
            case 1:
                printf("Enter Result (Win/Loss/Draw): ");
                scanf(" %9s", res);
                printf("Enter Points Change: ");
                scanf("%d", &pts);
                pushMatch(currentPlayer, res, pts);
                break;
            case 2:
                traverseMatchHistory(currentPlayer);
                break;
            case 3:
                popMatch(currentPlayer);
                break;
            case 4:
                printf("\nExiting Match History Module... Final Score: %d\n", currentPlayer->score);
                exit(0);
            default:
                printf("[!] Invalid choice. Please try again.\n");
        }
    }

    return 0;
}