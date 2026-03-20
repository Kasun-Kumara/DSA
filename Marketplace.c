#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

// --- Data Structures ---

struct User {
    int user_id;
    char username[50];
    int coins;
    struct CardList* inventory;
};

struct PlayerCard {
    int card_id;
    char name[50];
    char position[10];
    int rating;
    int price;
    struct User* owner;  // Pointer to the current owner/seller
    struct PlayerCard* next;
    struct PlayerCard* prev;
};

struct CardList {
    struct PlayerCard* head;
    struct PlayerCard* tail;
};

// --- Linked List Utility Functions ---

void initialize_list(struct CardList* list) {
    list->head = NULL;
    list->tail = NULL;
}

struct PlayerCard* create_card(int card_id, const char* name, const char* position, int rating) {
    struct PlayerCard* newCard = (struct PlayerCard*)malloc(sizeof(struct PlayerCard));
    
    newCard->card_id = card_id;
    strcpy(newCard->name, name);
    strcpy(newCard->position, position);
    newCard->rating = rating;
    newCard->price = 0;
    newCard->owner = NULL;
    
    newCard->next = newCard;  // circular
    newCard->prev = newCard;  // circular
    
    return newCard;
}

bool is_empty(struct CardList* list) {
    return (list->head == NULL);
}

void insert_back(struct CardList* list, struct PlayerCard* newCard) {
    if (is_empty(list)) {
        list->head = list->tail = newCard;
        newCard->next = newCard;
        newCard->prev = newCard;
    } else {
        newCard->prev = list->tail;
        newCard->next = list->head;

        list->tail->next = newCard;
        list->head->prev = newCard;

        list->tail = newCard;
    }
}

// Remove a specific node from the list
void remove_node(struct CardList* list, struct PlayerCard* cardToRemove) {
    if (is_empty(list) || cardToRemove == NULL) return;

    if (list->head == list->tail && list->head == cardToRemove) {
        // Only one item in the list
        list->head = list->tail = NULL;
    } else {
        cardToRemove->prev->next = cardToRemove->next;
        cardToRemove->next->prev = cardToRemove->prev;

        if (list->head == cardToRemove) {
            list->head = cardToRemove->next;
        }
        if (list->tail == cardToRemove) {
            list->tail = cardToRemove->prev;
        }
    }
    
    // Reset pointers so it can be added to another list cleanly
    cardToRemove->next = cardToRemove;
    cardToRemove->prev = cardToRemove;
}

// --- Marketplace Functions ---

// 1. Search Marketplace
void search_marketplace(struct CardList* marketplace, const char* name, const char* position, int min_rating) {
    if (is_empty(marketplace)) {
        printf("Marketplace is empty.\n");
        return;
    }

    printf("\n--- Search Results ---\n");
    struct PlayerCard* temp = marketplace->head;
    bool found = false;

    do {
        bool match = true;
        if (name != NULL && strlen(name) > 0 && strcmp(temp->name, name) != 0) {
            match = false;
        }
        if (position != NULL && strlen(position) > 0 && strcmp(temp->position, position) != 0) {
            match = false;
        }
        if (temp->rating < min_rating) {
            match = false;
        }

        if (match) {
            printf("[ID: %d] %s (%s) - Rating: %d - Price: %d coins - Seller: %s\n", 
                temp->card_id, temp->name, temp->position, temp->rating, temp->price, temp->owner->username);
            found = true;
        }
        temp = temp->next;
    } while (temp != marketplace->head);

    if (!found) {
        printf("No players found matching your criteria.\n");
    }
    printf("----------------------\n");
}

// 2. Sell a Card (Move from inventory to marketplace)
void sell_card(struct User* user, struct CardList* marketplace, int card_id, int sell_price) {
    if (is_empty(user->inventory)) {
        printf("Your inventory is empty.\n");
        return;
    }

    struct PlayerCard* temp = user->inventory->head;
    struct PlayerCard* cardToSell = NULL;

    do {
        if (temp->card_id == card_id) {
            cardToSell = temp;
            break;
        }
        temp = temp->next;
    } while (temp != user->inventory->head);

    if (cardToSell != NULL) {
        // Remove from inventory
        remove_node(user->inventory, cardToSell);
        
        // Set price and ensure owner is correct
        cardToSell->price = sell_price;
        cardToSell->owner = user;

        // Add to marketplace
        insert_back(marketplace, cardToSell);
        
        printf("%s listed %s on the marketplace for %d coins.\n", user->username, cardToSell->name, sell_price);
    } else {
        printf("Card with ID %d not found in %s's inventory.\n", card_id, user->username);
    }
}

// 3. Buy a Card (Move from marketplace to user inventory, handle coins)
void buy_card(struct User* buyer, struct CardList* marketplace, int card_id) {
    if (is_empty(marketplace)) {
        printf("Marketplace is empty.\n");
        return;
    }

    struct PlayerCard* temp = marketplace->head;
    struct PlayerCard* cardToBuy = NULL;

    do {
        if (temp->card_id == card_id) {
            cardToBuy = temp;
            break;
        }
        temp = temp->next;
    } while (temp != marketplace->head);

    if (cardToBuy != NULL) {
        if (buyer->coins >= cardToBuy->price) {
            struct User* seller = cardToBuy->owner;
            
            // Prevent buying own card
            if (seller->user_id == buyer->user_id) {
                printf("You cannot buy your own card.\n");
                return;
            }

            // Perform transaction
            buyer->coins -= cardToBuy->price;
            seller->coins += cardToBuy->price;

            // Remove from marketplace
            remove_node(marketplace, cardToBuy);

            printf("Success! %s bought %s for %d coins from %s.\n", 
                buyer->username, cardToBuy->name, cardToBuy->price, seller->username);

            // Add to buyer's inventory
            cardToBuy->owner = buyer;
            cardToBuy->price = 0; // Reset price
            insert_back(buyer->inventory, cardToBuy);
            printf("%s's New Balance: %d coins\n", buyer->username, buyer->coins);
            printf("%s's New Balance: %d coins\n", seller->username, seller->coins);
        } else {
            printf("Not enough coins! You need %d more coins.\n", cardToBuy->price - buyer->coins);
        }
    } else {
        printf("Card with ID %d not found in the marketplace.\n", card_id);
    }
}

// --- Example Usage ---
int main() {
    // Initialize marketplace
    struct CardList marketplace;
    initialize_list(&marketplace);

    // Create users
    struct User user1 = {1, "PlayerOne", 1000, (struct CardList*)malloc(sizeof(struct CardList))};
    initialize_list(user1.inventory);
    
    struct User user2 = {2, "PlayerTwo", 500, (struct CardList*)malloc(sizeof(struct CardList))};
    initialize_list(user2.inventory);

    // Give user1 some cards initially
    struct PlayerCard* card1 = create_card(101, "Lionel Messi", "RW", 94);
    card1->owner = &user1;
    insert_back(user1.inventory, card1);

    struct PlayerCard* card2 = create_card(102, "Cristiano Ronaldo", "ST", 92);
    card2->owner = &user1;
    insert_back(user1.inventory, card2);

    // 1. User1 sells Messi on the marketplace
    sell_card(&user1, &marketplace, 101, 800);

    // 2. User1 sells Ronaldo on the marketplace
    sell_card(&user1, &marketplace, 102, 600);

    // 3. Search marketplace 
    // Search by name
    search_marketplace(&marketplace, "Lionel Messi", "", 0);
    // Search by position
    search_marketplace(&marketplace, "", "ST", 0);
    // Search by rating
    search_marketplace(&marketplace, "", "", 93);

    // 4. User2 tries to buy Ronaldo (Has 500 coins, costs 600)
    printf("\nUser2 trying to buy Ronaldo...\n");
    buy_card(&user2, &marketplace, 102); 

    // 4. User2 tries to buy Messi (not enough coins) and fails, so let's try someone cheaper
    sell_card(&user1, &marketplace, 102, 450); // sell cheaper - wait, it is already in marketplace.
    
    // Create a cheaper card for user1 and sell it
    struct PlayerCard* card3 = create_card(103, "Pedri", "CM", 85);
    card3->owner = &user1;
    insert_back(user1.inventory, card3);
    sell_card(&user1, &marketplace, 103, 300);

    printf("\nUser2 trying to buy Pedri...\n");
    buy_card(&user2, &marketplace, 103);

    return 0;
}
