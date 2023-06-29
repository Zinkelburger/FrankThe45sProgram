#include "deck.c"

//spaghetti code time! Really should implement opaque object design, lol
//returns the stronger of two values both in the same suit, assuming that suit is the trump. Supports the ace of hearts if input as -1.
int _45s_better_value_within_trump(int value1, int value2, int trump){
    int H[] = {5, 11, 1, 13, 12, 10, 9, 8, 7, 6, 4, 3, 2};
    int D[] = {5, 11, -1, 1, 13, 12, 10, 9, 8, 7, 6, 4, 3, 2};
    int CS[] = {5, 11, -1, 1, 13, 12, 2, 3, 4, 6, 7, 8, 9, 10};
    int* toUse;
    int size = 14;
    Boolean changedValue1 = FALSE;
    Boolean changedValue2 = FALSE;
    if(trump == 1){
        toUse = H;
        size = 13;
        //if one of the cards is the ace of hearts, set the value to 1 bc in hearts the ace of hearts is just a heart
        if(value1 == -1){
            value1 = 1;
            changedValue1 = TRUE;
        } else if(value2 == -1){
            value2 = 1;
            changedValue2 = TRUE;
        }

    } else if (trump == 2){
        toUse = D;
    } else {
        toUse = CS;
    }
    //returns the one that occurs earlier in the list
    for(int i = 0; i<size; i++){
        if(toUse[i] == value1){
            if(changedValue1){
                return -1;
            }
            return value1;
        } else if(toUse[i] == value2){
            if(changedValue2){
                return -1;
            }
            return value2; 
        }
    }
    return -31337;
}

int player_get_bid(Deck* hDeck){
    //read the bid file to determine what to bid
}
//plays the highest trump card. If it has no trump, it picks a random card.
Card* player_play_trick(Deck* hPlayerDeck, Deck** hDiscardedDeck, int trump){
    int suit = trump;
    int heartsOrder[] = {5, 11, 1, 13, 12, 10, 9, 8, 7, 6, 4, 3, 2};
    //I represent the ace of hearts with a -1
    int clubsNSpadesOrder[] = {5, 11, -1, 1, 13, 12, 2, 3, 4, 6, 7, 8, 9, 10};
    int diamondsOrder []= {5, 11, -1, 1, 13, 12, 10, 9, 8, 7, 6, 4, 3, 2};
        
    int* toUse;
    int size;
    if(trump == 1){
        toUse = heartsOrder;
        size = sizeof(heartsOrder) / sizeof(heartsOrder[0]);
    } else if(trump == 2){
        toUse = diamondsOrder;
        size = sizeof(diamondsOrder) / sizeof(diamondsOrder[0]);
    } else if(trump == 3 || trump == 4){
        toUse = clubsNSpadesOrder;
        size = sizeof(clubsNSpadesOrder) / sizeof(clubsNSpadesOrder[0]);
    }
    //plays the largest card
    Card* playedCard = NULL;
    for(int i = 0; i<deck_get_size(hPlayerDeck); i++){
        //tries each toUse
        for(int j = 0; j<size; j++){
            //use playedCard like it is the largest card currently found, and make it a different card if a bigger one is found
            if(toUse[j] == -1){
                //if the deck has the ace of hearts
                if(deck_contains_value(hPlayerDeck, 1, 1)){
                    if(playedCard != NULL && toUse[j] == _45s_better_value_within_trump(playedCard->value, -1, trump)){
                            card_destroy(&playedCard);
                            playedCard = card_init_value(1, 1);
                    } else if (playedCard == NULL){
                        playedCard = card_init_value(toUse[j], trump);
                    }
                }
            } else if(deck_contains_value(hPlayerDeck, toUse[j], trump)){
                if(playedCard != NULL && toUse[j] == _45s_better_value_within_trump(playedCard->value, toUse[j], trump)){
                            card_destroy(&playedCard);
                            playedCard = card_init_value(toUse[j], trump);
                } else if (playedCard == NULL){
                    playedCard = card_init_value(toUse[j], trump);
                }
            }
        }
    }
    //if there are no trump cards, just take the one at index 0
    if(playedCard == NULL){
        //printf("No trump cards in deck. Playing card at the 0th index\n");
        //want a deep copy of played card? Doing deck at 0 gives bugs, possible that the card is changing as the playedCard changes
        playedCard = deck_at(hPlayerDeck, 0);
        playedCard = card_init_value(playedCard->value, playedCard->suit);
    } 

    deck_push_back_card(hDiscardedDeck, *playedCard);
    deck_remove_card(hPlayerDeck, playedCard);
    //printf("Card: ");
    //card_print(playedCard);
    return playedCard;
}

//discards all non trump cards, except it has to hold onto one card
Deck* player_discard(Deck* hPlayerDeck, int trump){
    for(int i = 0; i < deck_get_size(hPlayerDeck); i++){
        //don't discard the ace of hearts either
        if(deck_at(hPlayerDeck, i)->suit != trump && deck_get_size(hPlayerDeck) > 1 && deck_at(hPlayerDeck, i)->suit != 1 && deck_at(hPlayerDeck, i)->value != 1){
            deck_remove_card(hPlayerDeck, deck_at(hPlayerDeck, i));
            //because size decreases and i is incremented, decrement i. Like if the 0th element is removed, i is now 1. But we need to check the new 0th element again, so i should be decremented.
            i--;
        }
    }
}