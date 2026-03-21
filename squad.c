#include <stdio.h>
#include <string.h>
#include "squad.h"
#include "inventory.h"

void initialize_squad(Squad *s) {
    s->count = 0;
}

void display_squad(Squad *s) {
    if (s->count == 0) {
        printf("Squad is empty.\n");
        return;
    }
    printf("\n--- Current Squad (%d/%d) ---\n", s->count, SQUAD_SIZE);
    printf("    Player Name   | Rating |  Type  |   Price   \n");
    printf("-----------------------------------------------\n");
    for (int i = 0; i < s->count; i++) {
         printf(" %16s | %5d  |%5s   | %8d \n", 
            s->players[i].name, 
            s->players[i].rating, 
            s->players[i].type, 
            s->players[i].price);
    }
    printf("-----------------------------------------------\n");
}

void add_player_to_squad(Squad *s, struct player_inventory *inv, const char *name) {
    if (s->count >= SQUAD_SIZE) {
        printf("Squad is full! Cannot add more players.\n");
        return;
    }

    struct Node_inventory *found = find_player_by_name(inv, name);
    if (found == NULL) {
        printf("Player '%s' not found in inventory.\n", name);
        return;
    }

    // Check for duplicates
    for (int i = 0; i < s->count; i++) {
        if (strcmp(s->players[i].name, name) == 0) {
            printf("Player '%s' is already in the squad.\n", name);
            return;
        }
    }

    // Copy to squad array
    SquadPlayer *sp = &s->players[s->count];
    if (sizeof(sp->name) <= strlen(found->player_name)) {
        strncpy(sp->name, found->player_name, sizeof(sp->name) - 1);
        sp->name[sizeof(sp->name) - 1] = '\0';
    } else {
        strcpy(sp->name, found->player_name);
    }
    
    sp->rating = found->player_rating;
    
    if (sizeof(sp->type) <= strlen(found->player_type)) {
        strncpy(sp->type, found->player_type, sizeof(sp->type) - 1);
        sp->type[sizeof(sp->type) - 1] = '\0';
    } else {
        strcpy(sp->type, found->player_type);
    }
    
    sp->price = found->player_price;

    s->count++;
    printf("Player '%s' added to squad successfully.\n", name);
}
