#include <stdio.h>
#include <stdlib.h>
#include "deck.c"
#include "45s.c"

Boolean play_game(int bid, int seed);

int main(void){
    FILE* fp;
    fp = fopen("BiddingPercentages.txt", "w");
    int bid = 15;
    int madeBids = 0;
    int seed = time(NULL);
    int numTimes = 10;
    for(int j = 0; j < 4; j++){
        for(int i = 0; i<numTimes; i++){
            if(play_game(bid, seed + i)){
                madeBids++;
            }
        }
        fputs("5 made the bid ", fp);
        fprintf(fp, "%d", bid);
        //32 is space
        fputc(32, fp);
        fprintf(fp, "%.2f", (float) madeBids/ numTimes * 100);
        fputs("\% of the time\n", fp);
        madeBids = 0;
        bid += 5;
    }
    
    return 0;
}
Boolean play_game(int bid, int seed){
    Deck* hDeck = deck_init_deck();
    //shuffle the deck 4 times to get a better shuffle than just one.
    deck_shuffle_seed(hDeck, seed);
    deck_shuffle_seed(hDeck, seed);
    deck_shuffle_seed(hDeck, seed);
    deck_shuffle(hDeck);
    
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
    int suitLed = -1;
    Card* cardPointerArray[4];
    Card* largestCard;
    //will need to initalize previous winner to the bidder, in this case it is always player 1
    int previousWinner = 0;
    int largestCardTeam = -1;
    
    for(int i = 0; i<5; i++){
        Card* card1 = player_play_trick(hPlayer1, &discardDeck, trump);
        Card* card2 = player_play_trick(hPlayer2, &discardDeck, trump);
        Card* card3 = player_play_trick(hPlayer3, &discardDeck, trump);
        Card* card4 = player_play_trick(hPlayer4, &discardDeck, trump);

        cardPointerArray[0] = card1;
        cardPointerArray[1] = card2;
        cardPointerArray[2] = card3;
        cardPointerArray[3] = card4;

        if(i == 0){
            //need to initalize it to a random card, just use the first one
            largestCard = card1;
        }
        suitLed = card_get_suit(cardPointerArray[previousWinner]);

        Card* winner = _45s_trick_evaluate(trump, suitLed, card1, card2, card3, card4);
        
        for(int j = 0; j<4; j++){
            if(cardPointerArray[j]->suit == winner->suit && cardPointerArray[j]->value == winner->value){
                previousWinner = j;
                break;
            }
        }
        
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

    return team1 >= bid;
}

