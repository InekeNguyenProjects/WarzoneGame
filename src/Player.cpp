/*
 * Player.cpp
 *
 *  Created on: Apr. 26, 2024
 *      Author: mikenkie
 */

#include "Player.h"

// Player default constructor
Player::Player()
{

}


// Parameterized constructor that accpets a Player Id integer as an argument
Player::Player(int Id)
{
    playerId = Id;
    reinformentPool = 0;
    playerHand = new Hand(5);
    playerOrders = new Orderlist();
    cardUsed = {};
    strategyType = "";
    strategy = NULL;

}

// Player constructor with the player Order list, Territory list, Hand and player Id as arguments
Player::Player(int aPlayerId, Hand* aPlayerHand, Orderlist* aPlayerOrders, vector<Territory*> aPlayerTerritories, string typeOfStrategy )
{
    playerId = aPlayerId;
    playerHand = aPlayerHand;
    playerOrders = aPlayerOrders;
    playerTerritories = aPlayerTerritories;
    reinformentPool = 0;
    cardUsed = {};
    strategyType = typeOfStrategy;

    if(strategyType == "human")
    {
        strategy = new HumanPlayerStrategy();
    }
    else
    {
        if(strategyType == "aggressive")
        {
            strategy = new AggressivePlayerStrategy();
        }
        else
        {
            if(strategyType == "benevolent")
            {
                strategy = new BenevolentPlayerStrategy();
            }
            else
            {
                strategy = new NeutralPlayerStrategy();
            }

        }
    }
}

//A copy constructor for a Player object
Player::Player(const Player& obj)
{
    playerId = obj.playerId;
    playerHand = new Hand(*obj.playerHand);
    playerOrders = new Orderlist(*obj.playerOrders);
    playerTerritories = obj.playerTerritories;
    reinformentPool = obj.reinformentPool;
    cardUsed = obj.cardUsed;
    strategyType = obj.strategyType;

    if(strategyType == "human")
    {
        strategy = new HumanPlayerStrategy();
    }
    else
    {
        if(strategyType == "aggressive")
        {
            strategy = new AggressivePlayerStrategy();
        }
        else
        {
            if(strategyType == "benevolent")
            {
                strategy = new BenevolentPlayerStrategy();
            }
            else
            {
                strategy = new NeutralPlayerStrategy();
            }

        }
    }
    for(auto p : obj.cardUsed)
    {
        cardUsed.push_back(p);
    }

}

Player& Player::operator=(const Player& obj)//assignment Operators
{
    playerId = obj.playerId;
    playerHand = new Hand(*obj.playerHand);
    playerOrders = new Orderlist(*obj.playerOrders);
    playerTerritories = obj.playerTerritories;
    reinformentPool = obj.reinformentPool;
    cardUsed = obj.cardUsed;
    strategyType = obj.strategyType;

    if(strategyType == "human")
    {
        strategy = new HumanPlayerStrategy();
    }
    else
    {
        if(strategyType == "aggressive")
        {
            strategy = new AggressivePlayerStrategy();
        }
        else
        {
            if(strategyType == "benevolent")
            {
                strategy = new BenevolentPlayerStrategy();
            }
            else
            {
                strategy = new NeutralPlayerStrategy();
            }

        }
    }
    for(auto p : obj.cardUsed)
    {
        cardUsed.push_back(p);
    }

    return *this;
}

ostream& operator << (ostream& out, const Player& obj) //stream insertion
{
    out << "The player id is: ";
    out << obj.playerId << endl;
    return out;
}

// deconstructor
Player::~Player()
{
    delete playerHand;
    playerHand = NULL;
    delete playerOrders;
    playerOrders = NULL;

    for (auto p : playerTerritories)
    {
        delete p;
        p = NULL;
    }
    playerTerritories.clear();
}

//returns players id number
int Player::getPlayerId()
{
    return playerId;
}

//sets players id number
void Player::setPlayerId(int id)
{
    playerId = id;
}

// returns players hand
Hand Player::getPlayerHand()
{
    return *playerHand;
}

//set players hand
void Player::setPlayerHand(Hand hand)
{
    playerHand = &hand;
}

void Player::addTerritory(Territory* territory)
{
    territory->setOwnerId(this->getPlayerId());
    playerTerritories.push_back(territory);
}

//set armies in player reforment pool
void Player::setReinformentPool(int amount)
{
    reinformentPool = amount;
}

int Player::getReinformentPool() {
    return reinformentPool;
}

vector<Territory*> Player::getAllTerritories()
{
    return playerTerritories;
}

void Player::issueOrder(Map* gameMap, vector<Player*> listOfPlayer, Deck *deck )
{
    strategy->issueOrder(this,gameMap,listOfPlayer,deck);
}


void Player::addReinforment(int inum)
{
    this->reinformentPool = reinformentPool + inum;
}

void Player::subReinforment(int inum)
{
    this->reinformentPool = reinformentPool - inum;
}

void Player::clearCardUse()
{
    cardUsed.clear();
}

vector<Cards*> Player::getCardUsed()
{
    return cardUsed;
}

Orderlist* Player::getPlayerOrderList()
{
    return playerOrders;
}

vector<Territory*> Player::toAttack()
{
   return strategy->toAttack(this,0);
}
vector<Territory*> Player::toDefend()
{
    return strategy->toDefend(this);
}

void Player::addCardToHand(Cards tempCard)
{
    playerHand->addCard(tempCard);
}

string Player::getStrategy()
{
    return strategyType;
}

void Player::setStrategy(string myStrategy)
{
    strategyType = myStrategy;
    if (strategyType == "human")
    {
        strategy = new HumanPlayerStrategy();
    }
    else
    {
        if (strategyType == "aggressive")
        {
            strategy = new AggressivePlayerStrategy();
        }
        else
        {
            if (strategyType == "benevolent")
            {
                strategy = new BenevolentPlayerStrategy();
            }
            else
            {
                strategy = new NeutralPlayerStrategy();
            }

        }
    }
}




