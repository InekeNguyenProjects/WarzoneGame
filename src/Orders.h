/*
 * Orders.h
 *
 *  Created on: Apr. 26, 2024
 *      Author: mikenkie
 */
#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <iterator>
#include <map>
#include <typeinfo>

class Map;
class Territory;
class Player;
class Deck;

using namespace std;

#ifndef ORDERS_H_
#define ORDERS_H_

class Orders {
public:
	Orders();
	Orders(string status);
	Orders(const Orders& oo);
	virtual Orders& operator = (const Orders& oo);
	~Orders();

	void description(int value);
	void afterExecution(int value);

	friend ostream& operator<< (ostream& out, const Orders& copy);

	virtual Territory* getTargetTerritory() = 0;
	virtual void setTargetTerritory(Territory* changeTarget) = 0;
	virtual void setSourceTerritory(Territory* changeSource) = 0;
	virtual void execute() = 0;
	virtual int getValue() = 0;
	virtual bool validate() = 0;
	virtual string getOrderName() = 0;

private:
	string success;

};

class Advance : public Orders {
public:
	Advance();
	Advance(const Advance& advance);
	Advance(Territory* from, Territory* to, int amount, vector<Territory*> playerTerritory, Player* myPlayer, Deck* gameDeck);
	Advance& operator = (const Advance& advance);
	~Advance();

	Territory* getTargetTerritory();
	void setTargetTerritory(Territory* changeTarget);
	void setSourceTerritory(Territory* changeSource);

	int getValue();
	bool validate();
	void execute();
	string getOrderName();


	friend ostream& operator<< (ostream& out, const Advance& copy);


private:
	const string orderName = "Advance";
	const int value1 = 4;

	int armyAmount;
	Territory* playerTerritory;
	Territory* targetTerritory;
	vector <Territory*> playerTerritories;
	Player* myPlayer;
	Deck* gameDeck; //the GameEngine's deck
};

class Airlift : public Orders {
public:
	Airlift();
	Airlift(Territory* targetTerritory1, Territory* sourceTerritory1, vector<Territory*> playerTerritoryList, int armies);
	Airlift(const Airlift& airlift);
	Airlift& operator = (const Airlift& airlift);
	~Airlift();

	Territory* getTargetTerritory();
	void setTargetTerritory(Territory* changeTarget);
	void setSourceTerritory(Territory* changeSource);
	int getValue();
	bool validate();
	void execute();
	string getOrderName();
	friend ostream& operator<< (ostream& out, const Airlift& copy);

private:
	const string orderName = "Airlift";
	const int value2 = 2;

	int numArmies;
	Territory* sourceTerritory;
	Territory* targetTerritory;
	vector <Territory*> playerTerritories;
};

class Blockade : public Orders {
public:
	Blockade();
	Blockade(Territory* target, vector<Territory*> listOfPlayerTerritory);
	Blockade(const Blockade& blockade);
	Blockade& operator = (const Blockade& bloackde);
	~Blockade();

	Territory* getTargetTerritory();
	void setTargetTerritory(Territory* changeTarget);
	void setSourceTerritory(Territory* changeSource);
	int getValue();
	bool validate();
	void execute();
	string getOrderName();

	friend ostream& operator<< (ostream& out, const Blockade& copy);

private:
	const string orderName = "Blockade";
	const int value3 = 3;

	Territory* targetTerritory;
	vector <Territory*> playerTerritories;
};

class Bomb : public Orders {
public:
	Bomb();
	Bomb(Territory* target, vector<Territory*> listOfplayerTerritory);
	Bomb(const Bomb& bomb);
	Bomb& operator = (const Bomb& bomb);
	~Bomb();

	Territory* getTargetTerritory();
	void setTargetTerritory(Territory* changeTarget);
	void setSourceTerritory(Territory* changeSource);
	int getValue();
	bool validate();
	void execute();
	string getOrderName();

	friend ostream& operator<< (ostream& out, const Bomb& copy);

private:
	const string orderName = "Bomb";
	const int value4 = 4;

	Territory* targetTerritory;
	vector <Territory*> playerTerritories;
};

class Deploy : public Orders {
public:
	Deploy();
	Deploy(Territory* target, int amount, vector<Territory*> listOfPlayerTerritory);
	Deploy(const Deploy& deploy);
	Deploy& operator = (const Deploy& deploy);
	~Deploy();

	Territory* getTargetTerritory();
	void setTargetTerritory(Territory* changeTarget);
	void setSourceTerritory(Territory* changeSource);
	int getValue();
	bool validate();
	void execute();
	string getOrderName();
	friend ostream& operator<< (ostream& out, const Deploy& copy);

private:
	const string orderName = "Deploy";
	const int value5 = 1;

	int amountDeploy;
	Territory* targetTerritory;
	vector <Territory*> listOfplayerTerritories;
};

class Negotiate : public Orders {
public:
	Negotiate();
	Negotiate(Player* myPlayer, Player* enemyPlayer);
	Negotiate(const Negotiate& negotiate);
	Negotiate& operator = (const Negotiate& negotiate);
	~Negotiate();

	Territory* getTargetTerritory();
	void setTargetTerritory(Territory* changeTarget);
	void setSourceTerritory(Territory* changeSource);
	int getValue();
	bool validate();
	void execute();
	string getOrderName();
	friend ostream& operator<< (ostream& out, const Negotiate& copy);

private:
	const string orderName = "Negotiate";
	const int value6 = 4;

    Player* myPlayer;
	Player* enemyPlayer;
};

class Orderlist {
public:
	Orderlist();
	Orderlist(const Orderlist& orderList);
	Orderlist& operator = (const Orderlist& orderlist);
	~Orderlist();

	void add_OrderToList(Orders& orderlist);
	void remove(int index);
	void move(int one, int two);
	void displayOrders();

	friend ostream& operator<<(ostream& out, const Orderlist& copy);

	vector <Orders*> getList();

private:
	vector<Orders*> list;
};




#endif /* ORDERS_H_ */
