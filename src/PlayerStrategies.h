/*
 * PlayerStrategies.h
 *
 *  Created on: Apr. 26, 2024
 *      Author: mikenkie
 */
#pragma once
#ifndef PLAYERSTRATEGIES_H_
#define PLAYERSTRATEGIES_H_

#include "Map.h"
#include "GameEngine.h"

#include <iostream>
#include <string>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

class Player;

using namespace std;

class Strategy {
public:
    Strategy();
    virtual ~Strategy();
    virtual vector<Territory*> toAttack(Player* player, int from) = 0;
    virtual vector<Territory*> toDefend(Player* player) = 0;
    virtual void issueOrder(Player* player, Map* gameMap ,vector <Player*> enemyPlayers, Deck* deck) = 0; //must add deck as the last param
};

class HumanPlayerStrategy : public Strategy {
private:

    int checkInput(int range); // check if intiger and if it in the range of 0 to range
    void showList(vector<Territory*> display);

public:
    HumanPlayerStrategy();
    HumanPlayerStrategy(const HumanPlayerStrategy& temp);
    ~HumanPlayerStrategy();
    HumanPlayerStrategy& operator = (const HumanPlayerStrategy& temp );
    friend ostream& operator<< (ostream& out, const HumanPlayerStrategy& copy);
    vector<Territory*> toAttack(Player* player, int from);
    vector<Territory*> toDefend(Player* player);
    void issueOrder(Player* player, Map* gameMap, vector <Player*> enemyPlayers, Deck* deck);

};

class AggressivePlayerStrategy : public Strategy {
private:
    vector <Territory*> myOrderedTerr;
    vector<Territory *> getOrders();

public:

    AggressivePlayerStrategy();
    AggressivePlayerStrategy(const AggressivePlayerStrategy& temp);
    ~AggressivePlayerStrategy();
    AggressivePlayerStrategy& operator = (const AggressivePlayerStrategy& temp );
    friend ostream& operator<< (ostream& out, const AggressivePlayerStrategy& copy);

    vector<Territory*> toAttack(Player* player,  int from);
    vector<Territory*> toDefend(Player* player);
    void issueOrder(Player* player, Map* gameMap, vector <Player*> enemyPlayers, Deck* deck);

};


class BenevolentPlayerStrategy : public Strategy {

public:
    BenevolentPlayerStrategy();
    BenevolentPlayerStrategy(const BenevolentPlayerStrategy& benevolent);
    ~BenevolentPlayerStrategy();
    BenevolentPlayerStrategy& operator = (const BenevolentPlayerStrategy& benevolent);
    friend ostream& operator<< (ostream& out, const BenevolentPlayerStrategy& copy);
    vector <Territory*> toAttack(Player* player, int x);
    vector <Territory*> toDefend(Player* player);
    void issueOrder(Player* player, Map* gameMap, vector <Player*> enemyPlayers, Deck* deck); //deck is added for advance

private:
    vector <Territory*> myOrderedTerr;
    vector <Territory*> getOrders();

};

class NeutralPlayerStrategy : public Strategy {
public:

    NeutralPlayerStrategy();
    NeutralPlayerStrategy(const NeutralPlayerStrategy& temp);
    ~NeutralPlayerStrategy();
    NeutralPlayerStrategy& operator = (const NeutralPlayerStrategy& temp );
    friend ostream& operator<< (ostream& out, const NeutralPlayerStrategy& copy);
	vector<Territory*> toAttack(Player* player,int);
	vector<Territory*> toDefend(Player* player);
    void issueOrder(Player* player, Map* gameMap, vector <Player*> enemyPlayers, Deck* deck);

};



#endif /* PLAYERSTRATEGIES_H_ */
