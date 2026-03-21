#ifndef INVENTORY_H
#define INVENTORY_H

#include <stdbool.h>

struct Node_inventory {
    char player_name[25];
    int player_rating;
    char player_type[5];
    int player_price;
    struct Node_inventory *next;
    struct Node_inventory *prev;
};

struct player_inventory {
    struct Node_inventory *head;
    struct Node_inventory *tail;
};

void initialize_inventory(struct player_inventory *list);
void traverse_inventory(struct player_inventory *list);
bool is_empty(struct player_inventory* list);
void add_player(struct player_inventory* list, const char *name, int rating, const char *type, int price);
void search_inventory(struct player_inventory* list, const char *name, int rating);
void sell_player(struct player_inventory* list, const char *name, int rating);
struct Node_inventory* find_player_by_name(struct player_inventory* list, const char *name);

#endif // INVENTORY_H
