#include <stdlib.h>
#include <stdio.h>
#include "deck.c"
#include "player.c"
//functions useful for playing the gmae 45s

void _45s_bidding(Deck* players[]){
    //ask each player for their bid
    int bid;
    for(int i = 0; i < 4; i++){
        bid = player_get_bid(players[i]);
        printf("Player %d bids %d ", i + 1, bid);
    }
    
}

void _45s_deal_player(Deck** hPlayer, Deck* hDeck){
    for(int j = deck_get_size(*hPlayer); j < 5; j++){
        deck_push_back_card(hPlayer, *deck_front(hDeck));
        deck_service(hDeck);
    }
}


//the unicode goes heart, diamond, club, spade so I will too
Card* _45s_trick_evaluate(int trump, int suitLed, Card* card1, Card* card2, Card* card3, Card* card4){
    Card* largestCard = NULL;
    //none of the cards are on suit, evaluate the off suite order of cards
    if(card1->suit != trump && card2->suit != trump && card3->suit != trump && card4->suit != trump){
        int HD[] = {13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1};
        int CS[] = {13, 12, 11, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        int* toUse = NULL;
        //hearts or diamonds
        if(suitLed == 1 || suitLed == 2){
            toUse = HD;
        //clubs or spades
        } else {
            toUse = CS;
        }
        for(int i = 0; i<13; i++){
            if(card1->suit == suitLed && card1->value == toUse[i]){
                largestCard = card1;
                break;
            } 
            else if(card2->suit == suitLed && card2->value == toUse[i]){
                largestCard = card2;
                break;
            }
            else if(card3->suit == suitLed && card3->value == toUse[i]){
                largestCard = card3;
                break;
            }
            else if(card4->suit == suitLed && card4->value == toUse[i]){
                largestCard = card4;
                break;
            }
        }
        
    } else {
        int heartsOrder[] = {5, 11, 1, 13, 12, 10, 9, 8, 7, 6, 4, 3, 2};
        //I represent the ace of hearts with a -1
        int clubsNSpadesOrder[] = {5, 11, -1, 1, 13, 12, 2, 3, 4, 6, 7, 8, 9, 10};
        int diamondsOrder[] = {5, 11, -1, 1, 13, 12, 10, 9, 8, 7, 6, 4, 3, 2};
        
        int* toUse = NULL;
        int size = 0;
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
        for(int i = 0; i<size; i++){
            //special case for ace of hearts
            if(toUse[i] == -1){
                if(card1->suit == 1 && card1->value == 1){
                    largestCard = card1;
                    break;
                }
                if(card2->suit == 1 && card2->value == 1){
                    largestCard = card2;
                    break;
                }
                if(card3->suit == 1 && card3->value == 1){
                    largestCard = card3;
                    break;
                }
                if(card4->suit == 1 && card4->value == 1){
                    largestCard = card4;
                    break;
                }
            } else {
                if(card1->suit == trump && card1->value == toUse[i]){
                    largestCard = card1;
                    break;
                }
                if(card2->suit == trump && card2->value == toUse[i]){
                    largestCard = card2;
                    break;
                }
                if(card3->suit == trump && card3->value == toUse[i]){
                    largestCard = card3;
                    break;
                }
                if(card4->suit == trump && card4->value == toUse[i]){
                    largestCard = card4;
                    break;
                }
        
            }
        }
    }
    if(largestCard == NULL){
        printf("There was a severe error with 45s evaluate hand\n Trump: %d Suit led: %d", trump, suitLed);
        printf("\nCard1:");
        card_print(card1);
        printf("\nCard2:");
        card_print(card2);
        printf("\nCard3:");
        card_print(card3);
        printf("\nCard4:");
        card_print(card4);
        //just return a card to avoid failure
        largestCard = card4;
    }
    return largestCard;
}