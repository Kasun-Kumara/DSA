#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "match_history.h"
#include "../leaderboard/leaderboard.h"

// ================= MATCH HISTORY (STACK - LIFO) ================= 

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
    
    // Automatically keep the global leaderboard completely updated
    bubbleSortLeaderboard(*head_ref);
    printf("\n[+] Match recorded! %s (%s, %+d points). New Total Score: %d\n", 
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
    printf(" Current Total Score: %d\n", playerNode->data.score);
    printf("======================================================\n");
}

// Function to undo last match (Pop from Stack) - Meaningful Function
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

    // Automatically keep the global leaderboard completely updated
    bubbleSortLeaderboard(*head_ref);
    printf("\n[-] Last match undone for %s. Reverted %+d points. Restored Score: %d\n", 
           playerNode->data.name, pointsToRevert, playerNode->data.score);
}