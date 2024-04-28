/*
 * Cards.cpp
 *
 *  Created on: Apr. 26, 2024
 *      Author: mikenkie
 */

#include "Cards.h"

Cards::Cards()
{
    //constr
}

Cards::Cards(string action) // consr with parameter
{
    cardType = action;
    cardOrder = nullptr;
}

Cards::Cards(const Cards &tempCards)//copy constr
{
    cardType = tempCards.cardType;
    cardOrder = tempCards.cardOrder;
}

Cards& Cards::operator=(const Cards&tempCards)//assignment Operators
{
    this->cardType = tempCards.cardType;
    this->cardOrder = tempCards.cardOrder;
    return *this;
}

ostream & operator << (ostream &out, const Cards &tempCards) //stream insertion
{
    out << "This Cards is :";
    out << tempCards.cardType << endl;
    return out;
}

string Cards::getCardType()
{
    return cardType;
}

Orders* Cards::play()
{
    if(cardType == "Airlift")
    {
        cardOrder = new Airlift();
    }
    else
    {
        if(cardType == "Blockade")
        {
            cardOrder = new Blockade();
        }
        else
        {
            if(cardType == "Bomb")
            {
                cardOrder = new Bomb();
            }
            else
            {
                if(cardType == "Negotiate")
                {
                    cardOrder = new Negotiate();
                }
                else
                {
                    cout<<"error there have been a problem in the card implementation"<<endl;
                }
            }
        }
    }
    return cardOrder;
}

Cards::~Cards()
{
    //dtor
}
//--------------------------------------

//-------------Deck implementation-------

Deck::Deck()
{
    //default constr
}

Deck::Deck(int inum)// initiate for max number of cards in a deck/hands
{
    maxCards = inum;
    remainingCards = 0;
    setCards.reserve(maxCards);
}

Deck::Deck(const Deck &tempDeck)
{
    this->maxCards = tempDeck.maxCards;
    this->setCards.reserve(tempDeck.maxCards);
    for(int inum = 0 ; inum < tempDeck.setCards.size(); inum++)
    {
        this->addCard(*tempDeck.setCards[inum]);
    }
}

Deck& Deck::operator=(const Deck&tempDeck)//assignment Operators
{
    this->maxCards = tempDeck.maxCards;
    this->remainingCards = tempDeck.remainingCards;
    this->setCards.reserve(tempDeck.maxCards);
    for(int inum = 0 ; inum < tempDeck.setCards.size(); inum++)
    {
        this->addCard(*tempDeck.setCards[inum]);
    }
    return *this;
}

ostream & operator << (ostream &out, const Deck &tempDeck) //stream insertion
{
    out << "This Deck Contain is : ";
    out << tempDeck.remainingCards <<" Cards."<<endl;
    return out;
}

void Deck::addCard(Cards tempCard) // add a card to the deck/hands
{
    if(this->remainingCards == this->maxCards)
    {
        cout<<"Max Cards added"<<endl;
    }
    else
    {
        setCards.push_back(new Cards(tempCard));// added to the Deck
        remainingCards++;
    }

}

Cards Deck::draw() // return the top most card
{
    if(this->remainingCards == 0 )
    {
        cout<<"No cards remaining"<<endl;
    }
    else
    {
        Cards tempCard = *setCards.back();
        setCards.pop_back();
        remainingCards--;
        return tempCard;
    }

    Cards noCard;
    return noCard;
}

void Deck::shuffle()
{
    auto rng = std::default_random_engine ();
    cout<<"Shuffling the Deck"<<endl;
    std::shuffle(setCards.begin(), setCards.end(),rng);
}

Deck::~Deck()
{
    for (auto p : setCards)
    {
        delete p;
    }
    setCards.clear();
}
//--------------------------------------

//--------Hand implementation------------
Hand::Hand()
{

}
Hand::Hand(int inum)
{
    currentCards = 0;
    maxCards = inum;
    setCards.reserve(maxCards);
}

Hand::Hand(const Hand &tempDeck) // copy constr
{
    this->maxCards = tempDeck.maxCards;
    this->setCards.reserve(tempDeck.maxCards);
    for(int inum = 0 ; inum < tempDeck.setCards.size(); inum++)
    {
        this->addCard(*tempDeck.setCards[inum]);
    }
}

Hand& Hand::operator =(const Hand& tempDeck) // assign operator
{
    this->maxCards = tempDeck.maxCards;
    this->setCards.reserve(tempDeck.maxCards);
    for(int inum = 0 ; inum < tempDeck.setCards.size(); inum++)
    {
        this->addCard(*tempDeck.setCards[inum]);
    }
    return *this;
}

ostream & operator << (ostream &out, const Hand &tempDeck) //stream insertion
{
    for(int inum = 0; inum < tempDeck.currentCards ; inum++)
    {
        out<<inum<<". "<<tempDeck.setCards.at(inum)->getCardType()<<endl;
    }
    out<<endl;
    return out;
}

void Hand::addCard(Cards tempCard)
{
    if( currentCards == maxCards)
    {
        int cardIndex;
        cout<<"Which card you want to remove?:"<<endl;
        showCard(); // display card to choose
        cin>>cardIndex;
        removeCards(cardIndex);
    }

    setCards.push_back(new Cards(tempCard));// added to the hand
    cout<<"Card added to hand: "<<setCards.back()->getCardType()<<endl;
    currentCards++;
}

Cards Hand::playCard(int cardIndex)
{

    Cards tempCard = *setCards.at(cardIndex);
    *setCards.at(cardIndex) = *setCards.back();
    setCards.pop_back();
    cout<<"Card play is: "<<tempCard.getCardType()<<endl;
    currentCards--;
    return tempCard;
}


void Hand::removeCards(int inum)
{
    Cards tempCard = *setCards.at(inum);
    *setCards.at(inum) = *setCards.back();
    setCards.pop_back();
    cout<<"Card remove is: "<<tempCard.getCardType()<<endl;
    currentCards--;

}

void Hand::showCard()
{
    cout<<"Cards you possess: "<<endl;
    int inum = 0;
    for(auto p: setCards)
    {
        cout<<inum<<". "<<p->getCardType()<<endl;
        inum++;
    }
    cout<<endl;
}

Hand::~Hand()
{
   for (auto p : setCards)
    {
        delete p;
    }
    setCards.clear();
}

vector<Cards*> Hand::getAllCards()
{
    return this->setCards;
}



