// Copyright Andrew Bernal 2023
#pragma once
#include "x45s/x45s.hpp"

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