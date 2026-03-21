#ifndef SQUAD_H
#define SQUAD_H

#include "inventory.h"

// Defined max squad size
#define SQUAD_SIZE 11

typedef struct {
    char name[25];
    int rating;
    char type[5];
    int price;
} SquadPlayer;

typedef struct {
    SquadPlayer players[SQUAD_SIZE];
    int count;
} Squad;

void initialize_squad(Squad *s);
void display_squad(Squad *s);
// Pass inventory to search from it
void add_player_to_squad(Squad *s, struct player_inventory *inv, const char *name);
// Automatically select best players for a standard 3-4-3-1 formation
void auto_select_squad(Squad *s, struct player_inventory *inv);
// Display squad with full position names
void display_squad_detailed(Squad *s);

#endif // SQUAD_H
