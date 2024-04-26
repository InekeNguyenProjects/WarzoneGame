/*
 * Cards.h
 *
 *  Created on: Apr. 26, 2024
 *      Author: mikenkie
 */

#ifndef CARDS_H_
#define CARDS_H_

#include<iostream>
#include<string>
#include<vector>
#include<algorithm>
#include<random>

#include "Orders.h"

using namespace std;

class Cards
{
    private:
        string cardType; // the effect of the card
        Orders* cardOrder;

    public:
        Cards(); // constr
        Cards(string);
        Cards(const Cards& );//copy constr
        Cards& operator =(const Cards&); // assign operator
        friend ostream & operator << (ostream &, const Cards &); //stream insertion operator
        string getCardType(); // return the type of order as a string
        Orders* play();//  create an order according to the cardtype and return it
        ~Cards(); //destr

};


class Deck
{
    private:
        vector<Cards*> setCards; // spy, bomb, reinforcement, blockade, airlift, diplomacy
        int maxCards, remainingCards;

    public:
        Deck();
        Deck(int); // constr
        Deck(const Deck&);// copy constr
        Deck& operator =(const Deck&); // assign operator
        friend ostream & operator << (ostream &, const Deck &); //stream insertion operator
        void addCard(Cards); // add a card to the deck
        void shuffle(); //mix the card
        Cards draw(); // draw the top most card and return a cards in the deck
        ~Deck(); // destr

};

class Hand
{
    private:
        vector<Cards*> setCards; // spy, bomb, reinforcement, blockade, airlift, diplomacy
        int maxCards, currentCards;
        void removeCards(int); // private since the player cant throw away cards only if it has the max amount of card

    public:
        Hand(); // default constr
        Hand(int); // constr
        Hand(const Hand&); // copy constr
        Hand& operator =(const Hand&); // assign operator
        friend ostream & operator << (ostream &, const Hand &); //stream insertion operator
        void addCard(Cards); // add a card to hand
        void showCard(); // show all the cards on hand
        Cards playCard(int cardIndex = 0); // return a card in setcards
        ~Hand();// destr

        vector<Cards*> getAllCards();

};




#endif /* CARDS_H_ */
