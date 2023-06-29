#ifndef DECK_C
#define DECK_C
#include "deck.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//cards use 1-13
struct card{
    int value;
    int suit;
};

typedef struct card Card;
//deck is an array of cards
struct deck{    
    Card* cards;
    int size;
    int capacity;
};

typedef struct deck Deck;

Card* card_init_value(int value, int suit){
    Card* card = malloc(sizeof(Card));
    if(card != NULL){
        card->value = value;
        card->suit = suit;
    }
    return card;
}
int card_get_suit(Card* hCard){
    Card* pCard = (Card*) hCard;
    return pCard->suit;
}
int card_get_value(Card* hCard){
    Card* pCard = (Card*) hCard;
    return pCard->value;
}
//the unicode goes heart, diamond, club, spade so I will too
Deck* deck_init_deck(void){
    Deck* deck = malloc(sizeof(Deck));
    if(deck != NULL){
        deck->cards = malloc(sizeof(Card) * 52);
        if(deck->cards == NULL){
            return NULL;
        }
        deck->capacity = 52;
        deck->size = 52;
        int index = 0;
        for(int j = 0; j < 4; j++){
            for(int i = 0; i<13; i++){
                deck->cards[index].value = i+1;
                deck->cards[index].suit = j+1;
                index++;
            }
        }
    }
    return deck;
}

Deck* deck_init_default(void){
    Deck* deck = malloc(sizeof(Deck));
    if(deck != NULL){
        deck->cards = malloc(sizeof(Card));
        if(deck->cards == NULL){
            return NULL;
        }
        deck->capacity = 1;
        deck->size = 0;
    }
    return deck;
}
void deck_print(Deck* hDeck){
    Deck* pDeck = (Deck*) hDeck;
    for(int i = 0; i<pDeck->size; i++){
            printf("%d of %d\n", pDeck->cards[i].value, pDeck->cards[i].suit);
    }
}
void card_print(Card* hCard){
    Card* pCard = (Card*) hCard;
    printf("%d of %d\n", pCard->value, pCard->suit);
}

void card_destroy(Card** hCard){
    Card* pCard = (Card*) *hCard;
    free(pCard);
    *hCard = NULL;
}

int deck_get_size(Deck* hDeck){
    Deck* deck = (Deck*) hDeck;
    return deck->size;
}

Boolean deck_contains_card(Deck* hDeck, Card* card){
    Deck* deck = (Deck*) hDeck;
    for(int i = 0; i< deck->size; i++){
        if(deck->cards[i].suit == card->suit && deck->cards[i].value == card->value){
            return TRUE;
        }
    }
    return FALSE;
}
Boolean deck_contains_value(Deck* hDeck, int value, int suit){
    Deck* deck = (Deck*) hDeck;
    for(int i = 0; i< deck->size; i++){
        if(deck->cards[i].suit == suit && deck->cards[i].value == value){
            return TRUE;
        }
    }
    return FALSE;
}

void deck_shuffle(Deck* hDeck){
    Deck* deck = (Deck*) hDeck;
    srand(time(NULL));
    for(int i = 0; i<deck->size; i++){
        int randomIndex = rand() % deck->size;
        Card temp = deck->cards[i];
        deck->cards[i] = deck->cards[randomIndex];
        deck->cards[randomIndex] = temp;
    }
}
void deck_shuffle_seed(Deck* hDeck, int seed){
    Deck* deck = (Deck*) hDeck;
    srand(seed);
    for(int i = 0; i<deck->size; i++){
        int randomIndex = rand() % deck->size;
        Card temp = deck->cards[i];
        deck->cards[i] = deck->cards[randomIndex];
        deck->cards[randomIndex] = temp;
    }
}
void deck_remove_card(Deck* hDeck, Card* card){
    Deck* deck = (Deck*) hDeck;
    for(int i = 0; i<deck->size; i++){
        if(deck->cards[i].suit == card->suit && deck->cards[i].value == card->value){
            Card temp = deck->cards[i];
            //printf("Removed card: ");
            //card_print(&temp);
            deck->cards[i] = deck->cards[deck->size-1];
            deck->cards[deck_get_size(deck)-1] = temp;
            deck->size--;
            break;
        }
    }
}

void deck_remove_value(Deck* hDeck, int value, int suit){
    Deck* deck = (Deck*) hDeck;
    for(int i = 0; i<deck->size; i++){
        if(deck->cards[i].suit == suit && deck->cards[i].value == value){
            //swap to the back is way faster than shifting everything down
            Card temp = deck->cards[i];
            //printf("Removed card: ");
            //card_print(&temp);
            //printf("\n");
            deck->cards[i] = deck->cards[deck->size-1];
            deck->cards[deck->size-1] = temp;
            deck->size--;
            break;
        }
    }
}

Boolean deck_is_empty(Deck* deck){
    if(deck == NULL){
        return TRUE;
    }
    return FALSE;
}

Status deck_push_back_card(Deck** hDeck, Card card){
    Deck* deck = (Deck*) *hDeck;
    if(deck == NULL){
        deck = deck_init_default();
        if(deck == NULL){
            return FAILURE;
        }
    }
    if(deck->size >= deck->capacity){
        Card* temp = deck->cards;
        deck->cards = malloc(sizeof(Card) * deck->capacity * 2);
        if(deck == NULL){
            return FAILURE;
        }
        for(int i = 0; i< deck->size; i++){
            deck->cards[i] = temp[i];
        }
        *hDeck = deck;
        deck->capacity *= 2;
    }
    deck->cards[deck->size] = card;
    deck->size++;
    return SUCCESS;
}
Status deck_push_back_value(Deck** hDeck, int value, int suit){
    Deck* deck = (Deck*) *hDeck;
    if(deck == NULL){
        deck = deck_init_default();
        if(deck == NULL){
            return FAILURE;
        }
    }
    if(deck->size >= deck->capacity){
        Card* temp = deck->cards;
        deck->cards = malloc(sizeof(Card) * deck->capacity * 2);
        if(deck == NULL){
            return FAILURE;
        }
        for(int i = 0; i< deck->size; i++){
            deck->cards[i] = temp[i];
        }
        *hDeck = deck;
        deck->capacity *= 2;
    }
    Card* newCard = card_init_value(value, suit);
    deck->cards[deck->size] = *newCard;
    deck->size++;
    return SUCCESS;
}
Status deck_service(Deck* hDeck){
    Deck* deck = (Deck*) hDeck;
    if(deck_is_empty(deck)){
        return FAILURE;
    }
    deck->size--;
    return SUCCESS;
}
Card* deck_at(Deck* hDeck, int index){
    Deck* deck = (Deck*) hDeck;
    if(index >= 0 && index < deck->size){
        return &(deck->cards[index]);
    }
    return NULL;
    
}
Card* deck_front(Deck* hDeck){
    Deck* deck = (Deck*) hDeck;
    if(deck_is_empty(deck)){
        printf("Failed to view the front of an empty deck\n");
    }
    return &(deck->cards[deck->size-1]);
}

void deck_destroy(Deck** phDeck){
    Deck* deck = (Deck*) *phDeck;
    free(deck->cards);
    free(deck);
    *phDeck = NULL;
}


#endif