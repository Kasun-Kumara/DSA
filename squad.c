#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
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

// Helper function for sorting
static int compare_players(const void *a, const void *b) {
    const struct Node_inventory *p1 = *(const struct Node_inventory **)a;
    const struct Node_inventory *p2 = *(const struct Node_inventory **)b;
    // Sort descending by rating
    return p2->player_rating - p1->player_rating;
}

// Case-insensitive string comparison helper
static int strings_equal_ignore_case(const char *s1, const char *s2) {
    while (*s1 && *s2) {
        if (tolower((unsigned char)*s1) != tolower((unsigned char)*s2))
            return 0;
        s1++;
        s2++;
    }
    return (tolower((unsigned char)*s1) == tolower((unsigned char)*s2));
}

static void add_best_players_of_type(Squad *s, struct player_inventory *inv, const char *type, int needed) {
    // Count matching players first
    int match_count = 0;
    struct Node_inventory *curr = inv->head;
    while (curr != NULL) {
        if (strings_equal_ignore_case(curr->player_type, type)) {
            match_count++;
        }
        curr = curr->next;
    }

    if (match_count == 0) {
        printf("No players of type '%s' found in inventory.\n", type);
        return;
    }

    // Allocate array
    struct Node_inventory **candidates = (struct Node_inventory **)malloc(match_count * sizeof(struct Node_inventory*));
    if (!candidates) {
        printf("Memory allocation failed during auto selection.\n");
        return;
    }

    // Fill array
    int idx = 0;
    curr = inv->head;
    while (curr != NULL) {
        if (strings_equal_ignore_case(curr->player_type, type)) {
            candidates[idx++] = curr;
        }
        curr = curr->next;
    }

    // Sort
    qsort(candidates, match_count, sizeof(struct Node_inventory*), compare_players);

    // Add top N to squad
    int added = 0;
    for (int i = 0; i < match_count && added < needed; i++) {
        // Check if already in squad to avoid duplicates
        int already_in = 0;
        for (int k = 0; k < s->count; k++) {
            if (strcmp(s->players[k].name, candidates[i]->player_name) == 0) {
                already_in = 1;
                break;
            }
        }
        
        if (!already_in) {
            if (s->count < SQUAD_SIZE) {
                SquadPlayer *sp = &s->players[s->count];
                
                // Copy name
                strncpy(sp->name, candidates[i]->player_name, sizeof(sp->name) - 1);
                sp->name[sizeof(sp->name) - 1] = '\0';
                
                sp->rating = candidates[i]->player_rating;
                
                // Copy type
                strncpy(sp->type, candidates[i]->player_type, sizeof(sp->type) - 1);
                sp->type[sizeof(sp->type) - 1] = '\0';
                
                sp->price = candidates[i]->player_price;

                s->count++;
                added++;
            } else {
                printf("Squad is full! Cannot add more players.\n");
                break;
            }
        }
    }
    
    if (added < needed) {
        printf("Warning: Could only add %d players of type '%s' (needed %d).\n", added, type, needed);
    }
    
    free(candidates);
}

static void add_remaining_best_players(Squad *s, struct player_inventory *inv) {
    if (s->count >= SQUAD_SIZE) return;
    
    int remaining = SQUAD_SIZE - s->count;
    printf("Filling %d remaining slots with best available players...\n", remaining);

    // Count all players
    int total_count = 0;
    struct Node_inventory *curr = inv->head;
    while (curr != NULL) {
        total_count++;
        curr = curr->next;
    }
    
    if (total_count == 0) return;

    // Allocate array
    struct Node_inventory **candidates = (struct Node_inventory **)malloc(total_count * sizeof(struct Node_inventory*));
    if (!candidates) {
        printf("Memory allocation failed during fallback selection.\n");
        return;
    }

    // Fill array
    int idx = 0;
    curr = inv->head;
    while (curr != NULL) {
        candidates[idx++] = curr;
        curr = curr->next;
    }

    // Sort all by rating
    qsort(candidates, total_count, sizeof(struct Node_inventory*), compare_players);

    // Add until full
    for (int i = 0; i < total_count && s->count < SQUAD_SIZE; i++) {
        // Check duplicate
        int already_in = 0;
        for (int k = 0; k < s->count; k++) {
            if (strcmp(s->players[k].name, candidates[i]->player_name) == 0) {
                already_in = 1;
                break;
            }
        }
        
        if (!already_in) {
            SquadPlayer *sp = &s->players[s->count];
            strncpy(sp->name, candidates[i]->player_name, sizeof(sp->name) - 1);
            sp->name[sizeof(sp->name) - 1] = '\0';
            sp->rating = candidates[i]->player_rating;
            strncpy(sp->type, candidates[i]->player_type, sizeof(sp->type) - 1);
            sp->type[sizeof(sp->type) - 1] = '\0';
            sp->price = candidates[i]->player_price;
            s->count++;
        }
    }
    free(candidates);
}

void auto_select_squad(Squad *s, struct player_inventory *inv) {
    if (is_empty(inv)) {
        printf("Inventory is empty!\n");
        return;
    }
    
    // Clear squad for fresh selection
    s->count = 0;
    
    printf("\nAuto-selecting best squad...\n");
    // 3 FWD
    add_best_players_of_type(s, inv, "FWD", 3);
    // 4 DEF
    add_best_players_of_type(s, inv, "DEF", 4);
    // 3 MID
    add_best_players_of_type(s, inv, "MID", 3);
    // 1 GK
    add_best_players_of_type(s, inv, "GK", 1);
    
    // Fill remaining slots if any
    if (s->count < SQUAD_SIZE) {
        add_remaining_best_players(s, inv);
    }
    
    printf("Squad selection complete. Total players: %d/11\n", s->count);
}

