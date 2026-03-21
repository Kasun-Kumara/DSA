#ifndef MATCH_HISTORY_H
#define MATCH_HISTORY_H

#include "../common.h"
#include "../leaderboard/leaderboard.h" // Needed to resort leaderboard and find player

// Match History Function Prototypes
void pushMatch(Node** head_ref, int playerId, const char* result, int points);
void traverseMatchHistory(Node* head, int playerId);
void popMatch(Node** head_ref, int playerId);

#endif // MATCH_HISTORY_H
