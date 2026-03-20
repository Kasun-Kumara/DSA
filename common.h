#ifndef COMMON_H
#define COMMON_H

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

#endif // COMMON_H
