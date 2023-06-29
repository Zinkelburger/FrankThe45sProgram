#include <stdio.h>
#include <stdlib.h>
#include "deck.c"
#include "45s.c"
Boolean play_game(int bid, int seed);
int main(void){
    int bid = 15;
    int madeBids = 0;
    int seed = time(NULL);
    for(int j = 0; j < 4; j++){
        for(int i = 0; i<2; i++){
            if(play_game(bid, seed + i)){
                madeBids++;
                printf("Made bid\n");
            } else {
                printf("Failed to make bid\n");
            }
        }
        printf("Made bids %d\n", madeBids);

        //32 is space
        madeBids = 0;
        bid += 5;
    }
    
    return 0;

}
//this main is for debuging _45ss trick evaluate and the program as a whole. There are more prints statements and stuff
Boolean play_game(int bid, int seed){
    Deck* hDeck = deck_init_deck();
    //shuffle the deck 4 times to get a better shuffle than just one.
    deck_shuffle_seed(hDeck, seed);
    deck_shuffle_seed(hDeck, seed);
    deck_shuffle_seed(hDeck, seed);
    deck_shuffle(hDeck);
    //deck_print(hDeck);
    /*
    Deck* hPlayer1 = deck_init_default();
    Deck* hPlayer2 = deck_init_default();
    Deck* hPlayer3 = deck_init_default();
    Deck* hPlayer4 = deck_init_default();
    Deck* hKiddie = deck_init_default();

    
    _45s_deal_player(&hPlayer1, hDeck);
    _45s_deal_player(&hPlayer2, hDeck);
    _45s_deal_player(&hPlayer3, hDeck);
    _45s_deal_player(&hPlayer4, hDeck);

    //deals the kiddie
    for(int j = 0; j < 3; j++){
        deck_push_back(&hKiddie, *deck_front(hDeck));
        deck_service(hDeck);
    }
    printf("Player 1:\n");
    //deck_print(hPlayer1);
    printf("Player 2:\n");
    //deck_print(hPlayer2);
    printf("Player 3:\n");
    //deck_print(hPlayer3);
    printf("Player 4:\n");
    //deck_print(hPlayer4);
    printf("Kiddie:\n");
    //deck_print(hKiddie);
    Deck* players[] = {hPlayer1, hPlayer2, hPlayer3, hPlayer4};

    */
    //This is for testing and generating the lookup table 
    Deck* hPlayer1 = deck_init_default();
    Deck* hPlayer2 = deck_init_default();
    Deck* hPlayer3 = deck_init_default();
    Deck* hPlayer4 = deck_init_default();
    Deck* hKiddie = deck_init_default();
    //to start off with we'll do all non-heart suits. We can kill 3 birds with just diamonds.
    //5's
    //player 1 gets all the testing cards & the kiddie

    deck_remove_value(hDeck, 5, 2);    
    deck_push_back_value(&hPlayer1, 5, 2);
    
    for(int i = 0; i<deck_get_size(hKiddie); i++){
        deck_push_back_card(&hPlayer1, *deck_front(hKiddie));
        deck_service(hKiddie);
    }
    //player1 should discard all non trump from the kiddie
    player_discard(hPlayer1, 2);

    _45s_deal_player(&hPlayer1, hDeck);
    _45s_deal_player(&hPlayer2, hDeck);
    _45s_deal_player(&hPlayer3, hDeck);
    _45s_deal_player(&hPlayer4, hDeck);

    player_discard(hPlayer1, 2);
    player_discard(hPlayer2, 2);
    player_discard(hPlayer3, 2);
    player_discard(hPlayer4, 2);

    _45s_deal_player(&hPlayer1, hDeck);
    _45s_deal_player(&hPlayer2, hDeck);
    _45s_deal_player(&hPlayer3, hDeck);
    _45s_deal_player(&hPlayer4, hDeck);

    Deck* discardDeck = deck_init_default();
    int team1 = 0;
    int team2 = 0;
    int trump = 2;
    int suitLed;
    Card* cardPointerArray[4];
    Card* largestCard;
    int previousWinner;
    int largestCardTeam = -1;

    printf("Player 1:\n");
    deck_print(hPlayer1);
    printf("Player 2:\n");
    deck_print(hPlayer2);
    printf("Player 3:\n");
    deck_print(hPlayer3);
    printf("Player 4:\n");
    deck_print(hPlayer4);

    for(int i = 0; i<5; i++){
        printf("------------\nRound %d\n------------\n", i+1);

        //printf("Player 1:\n");
        Card* card1 = player_play_trick(hPlayer1, &discardDeck, trump);
        //printf("Player 2:\n");
        Card* card2 = player_play_trick(hPlayer2, &discardDeck, trump);
        //printf("Player 3:\n");
        Card* card3 = player_play_trick(hPlayer3, &discardDeck, trump);
        //printf("Player 4:\n");
        Card* card4 = player_play_trick(hPlayer4, &discardDeck, trump);

        printf("Cards :\n");
        printf("Player 1:\n");
        card_print(card1);
        printf("Player 2:\n");
        card_print(card2);
        printf("Player 3:\n");
        card_print(card3);
        printf("Player 4:\n");
        card_print(card4);
        //printf("Discard deck:\n");
        //deck_print(discardDeck);

        cardPointerArray[0] = card1;
        cardPointerArray[1] = card2;
        cardPointerArray[2] = card3;
        cardPointerArray[3] = card4;

        if(i == 0){
            //need to initalize it to a random card, just use the first one
            largestCard = card1;
        }
        if(i != 0)
            suitLed = cardPointerArray[previousWinner]->suit;
        else
            suitLed = cardPointerArray[0]->suit;
        
        Card* winner = _45s_trick_evaluate(trump, suitLed, card1, card2, card3, card4);
        
        for(int j = 0; j<4; j++){
            if(cardPointerArray[j]->suit == winner->suit && cardPointerArray[j]->value == winner->value){
                previousWinner = j;
                break;
            }
        }
        /*
        printf("Player %d won with card ", previousWinner + 1);
        card_print(winner);
        printf("\n");

        printf("Decks:\n");
        printf("Player 1:\n");
        deck_print(hPlayer1);
        printf("Player 2:\n");
        deck_print(hPlayer2);
        printf("Player 3:\n");
        deck_print(hPlayer3);
        printf("Player 4:\n");
        deck_print(hPlayer4);
        */
        if(winner == card1 || winner == card3){
            //I am going to assume that the largest value is always trump
            if(winner->suit == trump){
                if(largestCard->value == _45s_better_value_within_trump(largestCard->value, winner->value, trump)){
                    largestCard = winner;
                    largestCardTeam = 0;
                }
            //_45s better value within trump has a special case for the ace of hearts
            } else if (winner->suit == 1 && winner->value == 1){
                if(largestCard->value == _45s_better_value_within_trump(largestCard->value, -1, trump)){
                    largestCard = winner;
                    largestCardTeam = 0;
                }
            }
            team1 += 5;
        } else {
            if(winner->suit == trump){
                if(largestCard->value == _45s_better_value_within_trump(largestCard->value, winner->value, trump)){
                    largestCard = winner;
                    largestCardTeam = 1;
                }
            //_45s better value within trump has a special case for the ace of hearts
            } else if (winner->suit == 1 && winner->value == 1){
                if(largestCard->value == _45s_better_value_within_trump(largestCard->value, -1, trump)){
                    largestCard = winner;
                    largestCardTeam = 1;
                }
            }
            team2 += 5;
        }

    }
    if(largestCardTeam == 0){
        team1 += 5;
    } else {
        team2 += 5;
    }
    //printf("Bid was %d. Team 1 made %d. Team 2 made %d", bid, team1, team2);


    //_45s_bidding(players);
    //player who won the bid plays first
    //did they make the bid? Add or subtract bid from score
    //if their score is greater than 120, they win

    deck_destroy(&hPlayer1);
    deck_destroy(&hPlayer2);
    deck_destroy(&hPlayer3);
    deck_destroy(&hPlayer4);
    deck_destroy(&discardDeck);
    deck_destroy(&hDeck);
    printf("Team1: %d Team2: %d\n", team1, team2);
    if(team1 >= bid){
        return TRUE;
    } else {
        return FALSE;
    }
}

