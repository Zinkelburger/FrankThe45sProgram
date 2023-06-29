// Copyright Andrew Bernal 2023
#include "tableGenerator.hpp"
#include "x45s/x45s.hpp"
#include <vector>
#include <string>
#include <utility>
#include <algorithm>

class Computer : public x45s::Player {
    // the player must keep at least 1 card
    virtual void discard() = 0;
    // pair is bidAmount, suit
    virtual std::pair<int, Suit::Suit> getBid(const std::vector<int>& bidHistory) = 0;
    // the player is forced to bid
    virtual Suit::Suit bagged() = 0;
    // should return the card you want to play and remove it from your hand
    virtual x45s::Card playCard(std::vector<x45s::Card> cardsPlayedThisHand) = 0;
};

// the player must keep at least 1 card
void TableGenerator::discard() {
    // keep all trump. If you have extra space, keep kings.
    // 1. Sort your hand according to the trump
    // 2. Discard all of your worst cards
    std::sort(hand.begin(), hand.end());

    bool trumpInHand = false;
    bool kingInHand = false;

    for (int i = 0; i < hand.size(); i++) {
        if (hand.at(i).getSuit() == trump) {
            trumpInHand = true;
        // Ace of hearts counts as trump
        } else if (hand.at(i).getSuit() == Suit::ACE_OF_HEARTS) {
            trumpInHand = true;
        } else if (hand.at(i).getValue() == 13) {
            kingInHand = true;
        }
    }

    // keep the trump and kings
    if (trumpInHand && kingInHand) {
        for (int i = 0; i < hand.size(); i++) {
            // if the card is not trump, the ace of hearts, or a king, remove it from the hand
            if (hand.at(i).getSuit() != trump && hand.at(i).getSuit() == Suit::ACE_OF_HEARTS
            && hand.at(i).getValue() == 13) {
                hand.erase(hand.begin() + i);
            }
        }
    // keep the trump
    } else if (trumpInHand && !kingInHand) {
        for (int i = 0; i < hand.size(); i++) {
            // if the card is not trump, the ace of hearts, or a king, remove it from the hand
            if (hand.at(i).getSuit() != trump && hand.at(i).getSuit() == Suit::ACE_OF_HEARTS) {
                hand.erase(hand.begin() + i);
            }
        }
    // discard everything besides the king
    } else if (!trumpInHand && kingInHand) {
        for (int i = 0; i < hand.size(); i++) {
            // if the card is not trump, the ace of hearts, or a king, remove it from the hand
            if (hand.at(i).getValue() == 13) {
                hand.erase(hand.begin() + i);
            }
        }
    // somehow have to pick the best offsuite. Pick the card with the highest value
    } else {
        // get the card with the highest value (not the best method, but better than random)
        int maxValue = -1;
        for (int i = 0; i < hand.size(); i++) {
            if (hand.at(i).getValue() >= maxValue) {
                maxValue = hand.at(i).getValue();
            }
        }
        // remove everything except this card
        for (int i = 0; i < hand.size(); i++) {
            if (hand.at(i).getValue() != maxValue) {
                hand.erase(hand.begin() + i);
            }
        }
    }
}

// pair is bidAmount, suit
std::pair<int, Suit::Suit> TableGenerator::getBid(const std::vector<int>& bidHistory) {
    // bid whatever it was told to bid
    return std::make_pair(bidAmount, Suit::DIAMONDS);
}

// the player is forced to bid
Suit::Suit TableGenerator::bagged() {
    return Suit::DIAMONDS;
}

// should return the card you want to play and remove it from your hand
x45s::Card TableGenerator::playCard(const std::vector<x45s::Card>& cardsPlayedThisHand) {
    // if no cards played, play its best card
    if (cardsPlayedThisHand.size() <= 0) {
        x45s::Card returnCard;
        // can't use the > operator for non-trump as suitLed is not defined
        for(int i = 0; i < hand.size(); i++) {
            if (hand.at(i).getSuit() == trump)
        }
 
        return returnCard;
    }

    // if it can win, play its best card. Otherwise play the worst card it legally can.
    x45s::Card bestCardPlayed = *std::max_element(cardsPlayedThisHand.begin(), cardsPlayedThisHand.end());
    x45s::Card bestCardInHand = *std::max_element(hand.begin(), hand.end());

    if (bestCardInHand > bestCardPlayed) {

    }
}
