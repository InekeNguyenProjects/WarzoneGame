/*
 * GameEngine.h
 *
 *  Created on: Apr. 26, 2024
 *      Author: mikenkie
 */

#pragma once

#include "Map.h"
#include "Player.h"
#include "MapLoader.h"
#include "Cards.h"
#include "GameObservers.h"

#include <vector>
#include <string>
#include <sstream>
#include <set>
#include <map>
#include <fstream>
#include <iostream>
#include <math.h>


class GameEngine : public Observable {
public:

    GameEngine();
    GameEngine(const string mapDirectory);
    GameEngine(const GameEngine& obj);
    ~GameEngine();
    friend ostream& operator<<(ostream&, const GameEngine&);
    GameEngine& operator=(const GameEngine& copy);
    void observerSettings();

    Deck* getDeck();
    vector<Player*> getPlayers();
    Map* getMap();

    enum class State { COND0, COND1, COND2, COND3, CONDSTAT };
    void setState(State state);
    State getState();

private:

    Map* gameMap;
    Deck* gameDeck;
    vector<Player*> players; //vector of all players that has been created

    MapLoader *maploader;


    void startupPhase();
    void createPlayers();
    void createGameMap(const string mapDirectory);
    void createDeck();

    void setPlayerOrder(); //return vector of randomly ordered players
    void assignArmies(); //assign armies to each country round robin style
    void assignTerritories(); //assign countries to each player round robin styles

    int getUserInputInteger(string, int, int);
    string getUserInputString(string, string, string);

    void mainGameLoop();

    void sortListByPriority(Player* player);

    void reinforcementPhase();
    void issueOrdersPhase();
    void executeOrdersPhase();
    vector<Player*> getGamePlayer();
    State state;
};
