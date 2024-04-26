/*
 * Player.h
 *
 *  Created on: Apr. 26, 2024
 *      Author: mikenkie
 */
#pragma once
#ifndef PLAYER_H_
#define PLAYER_H_

#include <iostream>
#include <string>
#include <map>
#include <vector>

#include "Cards.h"
#include "Map.h"
#include "Orders.h"
#include "PlayerStrategies.h"

using namespace std;

class Card;
class Map;
class Strategy;

class Player {
private:

    int playerId;
    Hand* playerHand;
    vector<Cards*> cardUsed;
    Orderlist* playerOrders;
    vector<Territory*> playerTerritories;
    int reinformentPool;
    Strategy* strategy;
    string strategyType;

public:

    Player();
    Player(int aPlayerId);
    Player(int aPlayerId, Hand* aPlayerHand, Orderlist* aPlayerOrders, vector<Territory*> aPlayerTerritories, string typeOfStrategy );
    Player(const Player& obj);
    Player& operator =(const Player&); // assign operator
    friend ostream& operator << (ostream&, const Player&); //stream insertion operator
    ~Player();

    int getPlayerId();
    void setPlayerId(int id);
    Hand getPlayerHand();
    void setPlayerHand(Hand hand);
    void addTerritory(Territory* territory);
    void setReinformentPool(int amount);
    int getReinformentPool();
    vector<Territory*> getAllTerritories();
    void issueOrder(Map* gameMap, vector<Player*> listOfPlayer, Deck *deck );
    void addReinforment(int);
    void subReinforment(int);

    Orderlist* getPlayerOrderList();

    vector<Territory*> toAttack();
    vector<Territory*> toDefend();

    vector<Cards*> getCardUsed();
    void clearCardUse();

    void addCardToHand(Cards);

    string getStrategy();
    void setStrategy(string myStrategy);

};





#endif /* PLAYER_H_ */
