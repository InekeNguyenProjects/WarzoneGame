/*
 * Orders.cpp
 *
 *  Created on: Apr. 26, 2024
 *      Author: mikenkie
 */

#include "Orders.h"

#include "Map.h"
#include "Player.h"
#include "Cards.h"
#include <cstddef>


/*Orders Class*/
// Default constructor
Orders::Orders()
{
	success = "Not yet executed";
}

// Parameterized constructor
Orders::Orders(string status)
{
	success = status;
}

// Copy constructor
Orders::Orders(const Orders& oo)
{
	Orders::operator = (oo);
	success = oo.success;
}

// Assignment operator for the copy constructor
Orders& Orders::operator = (const Orders& oo)
{
	this->success = oo.success;
	return *this;
}

// Destructor
Orders::~Orders()
{

}

/*
   Prints out the description of the specified subclass of Orders. Takes a constant value
   which is unique to each order and displays the appropriate message. The get_Value() method is to be used
   to get the argument.
 */
void Orders::description(int value)
{
	switch (value)
	{
	case 1: cout << "The Advance Order moves some of your armies from your selected territory to an adjacent territory." << endl;
		cout << "If the target territory belongs to the current player, the armies are moved to the target territory." << endl;
		cout << "If the target territory belongs to another player, an attack happens between the two territories." << endl;
		break;
	case 2: cout << "The Airlift Order moves some of your armies from your selected territory to another territory" << endl;
		break;
	case 3: cout << "The Blockade Order triples the number of armies on one of your current territories and make it a neutral territory" << endl;
		break;
	case 4: cout << "The Bomb Order destroys half of the armies located on an opponent?s territory that is adjacent to one of your currnt territories" << endl;
		break;
	case 5: cout << "The Deploy Order place some armies on one your current territories" << endl;
		break;
	case 6: cout << "The Negotiate Order prevents attacks between the current player and another player until the end of the turn" << endl;
		break;
	default: cout << "Invalid argument" << endl;
		break;
	}

}

/*
   If the execute() method is successful, then it prints out the execution description. The get_Complete()
   and get_Value() methods are to be used to get the arguments
*/
void Orders::afterExecution( int value)
{

		switch (value)
		{
		case 1: success = "Advance: You increased your army.";
			break;
		case 2: success = "Advance: You destroyed enemy armies.";
			break;
		case 3: success = "Advance: You lost armies.";
			break;
		case 4: success = "Airlift: You increased your army";
			break;
		case 5: success = "Airlift: You lost armies";
			break;
		case 6: success = "Airlift: You destroyed your enemy's armies.";
			break;
		case 7: success = "Blockade: Territory now neutral";
			break;
		case 8: success = "Bomb: You deployed bomb. Half of the territory's armies have been wiped out";
			break;
		case 9: success = "Deploy: Territory's armies have increased";
			break;
		case 10: success = "Negotiations successful. Neither territory will attack each other";
			break;
		default: success = "Invalid order.";
			break;
		}
	cout << success << endl;
}

// The afterExecution method must be executed prior using the ostream operator
ostream& operator<< (ostream& out, const Orders& copy)
{
	//out << "Order successful: " << copy.success;
	out << copy.success;
	return out;
}


/*Advance Order*/
// Default constructor
Advance::Advance()
{
	armyAmount = 0;
	playerTerritory = NULL;
	targetTerritory = NULL;
	playerTerritories = vector<Territory*>();
	myPlayer = NULL;
	gameDeck = NULL;
}

Advance::Advance(Territory* from, Territory* to, int amount, vector<Territory*> listPlayerTerritory, Player* myPlay, Deck* deck)
{
	armyAmount = amount;
	playerTerritory = from;
	targetTerritory = to;
	playerTerritories = listPlayerTerritory;
	myPlayer = myPlay;
	gameDeck = deck;
}

// Copy constructor
Advance::Advance(const Advance& advance) : Orders(advance)
{
	Orders::operator = (advance);
	this->playerTerritory = new Territory(*(advance.playerTerritory));
	this->targetTerritory = new Territory(*(advance.targetTerritory));
	for (int i = 0; i < advance.playerTerritories.size(); i++) {

		this->playerTerritories[i] = new Territory(*(advance.playerTerritories[i]));
	}
	this->myPlayer = new Player(*(advance.myPlayer));
	this->gameDeck = new Deck(*(advance.gameDeck));
}

// Assignment operator for the copy constructor
Advance& Advance::operator= (const Advance& advance)
{
	Orders::operator = (advance);
	this->playerTerritory = new Territory(*(advance.playerTerritory));
	this->targetTerritory = new Territory(*(advance.targetTerritory));
	for (int i = 0; i < advance.playerTerritories.size(); i++) {

		this->playerTerritories[i] = new Territory(*(advance.playerTerritories[i]));
	}
	this->myPlayer = new Player(*(advance.myPlayer));
	this->gameDeck = new Deck(*(advance.gameDeck));
	return *this;
}

// Destructor
Advance::~Advance()
{
	delete playerTerritory;
	delete targetTerritory;

	for (auto p : playerTerritories)
	{
		delete p;
		p = NULL;
	}
	playerTerritories.clear();

	delete myPlayer;
	delete gameDeck;
}

// returns the target territory
Territory* Advance::getTargetTerritory()
{
	return targetTerritory;
}

void Advance::setTargetTerritory (Territory* changeTarget)
{
	targetTerritory = changeTarget;
}

void Advance::setSourceTerritory(Territory* changeSource)
{
	playerTerritory = changeSource;
}

// Returns the int value for printing the appropriate description
int Advance::getValue()
{
	return value1;
}
string Advance::getOrderName() {
	return orderName;
}

/*
   Determines if the order is valid by checking the number of armies, the appropriate territories, and their adjacency to
   each other. If it is valid, the data member valid will be true, otherwise, it's false.
*/
bool Advance::validate()
{
	for (auto p : playerTerritories)
	{
		if (p->getName() == playerTerritory->getName())//source is on player territory
		{
			for (auto n : p->getAllNeighbors())
			{
				if (n->getName() == targetTerritory->getName()) // target is adjacent to the source territory
				{
					return true;
				}
			}
		}
	}
	return false;
}

/*
  If valid = true, then the order's actions will be executed and complete data member will be true.
*/
void Advance::execute()
{
	if (this->validate())
	{
		if (targetTerritory->getOwnerId() == playerTerritory->getOwnerId()) // player owns the territory
		{
			playerTerritory->decrementArmiesAmount(armyAmount);
			targetTerritory->incrementArmiesAmount(armyAmount);
			afterExecution(1);
		}
		else // enemy own territory
		{
			playerTerritory->decrementArmiesAmount(armyAmount);
			int enemy = targetTerritory->getArmiesAmount();
			int myarmy = armyAmount;

			int enemyLeft = enemy - (myarmy * 0.60);
			int myarmyLeft = myarmy - (enemy * 0.70);

			if (enemyLeft > 0 && myarmyLeft <= 0) // enemy win the battle
			{
				targetTerritory->decrementArmiesAmount((myarmy * 0.60));
				afterExecution(3);
			}
			else
			{
				if (myarmyLeft >= 0 && enemyLeft <= 0) // player win the battle
				{
					targetTerritory->setArmiesAmount(myarmyLeft);
					targetTerritory->setOwnerId(playerTerritory->getOwnerId());
					myPlayer->addTerritory(targetTerritory);
					afterExecution(2);

					//PLAYER GET A CARD IF IT WINS
					myPlayer->addCardToHand(gameDeck->draw());
				}
			}
		}
	}
	else
	{
		cout << "invalid Order" << endl;
	}
}

// Iostream operator for cout
ostream& operator<< (ostream& out, const Advance& copy)
{
	out <<copy.orderName;
	return out;
}

                                                           // Airlift Order
// Default constructor
Airlift::Airlift()
{
	numArmies = 0;
	sourceTerritory = NULL;
	targetTerritory = NULL;
	playerTerritories = vector<Territory*>();
}

// Parameterized constructor
Airlift::Airlift(Territory* targetTerritory1, Territory* sourceTerritory1, vector<Territory*> playerTerritoryList, int armies)
{
	numArmies = armies;
	sourceTerritory = sourceTerritory1;
	targetTerritory = targetTerritory1;
	playerTerritories = playerTerritoryList;
}

// Copy constructor
Airlift::Airlift(const Airlift& airlift) : Orders(airlift)
{
	this->numArmies = airlift.numArmies;
	this->sourceTerritory = new Territory(*(airlift.sourceTerritory));
	this->targetTerritory = new Territory(*(airlift.targetTerritory));
	for (int i = 0; i < playerTerritories.size(); i++) {

		this->playerTerritories[i] = new Territory(*(airlift.playerTerritories[i]));
	}
}

// Assignment operator for the copy constructor
Airlift& Airlift::operator= (const Airlift& airlift)
{
	Orders::operator = (airlift);
	this->numArmies = airlift.numArmies;
	this->sourceTerritory = new Territory(*(airlift.sourceTerritory));
	this->targetTerritory = new Territory(*(airlift.targetTerritory));

	for (int i = 0; i < playerTerritories.size(); i++)
	{
		this->playerTerritories[i] = new Territory(*(airlift.playerTerritories[i]));
	}
	return *this;
}

// Destructor
Airlift::~Airlift()
{
	delete sourceTerritory;
	delete targetTerritory;

	for (auto p : playerTerritories)
	{
		delete p;
		p = NULL;
	}
	playerTerritories.clear();
}

// returns the target territory
Territory* Airlift::getTargetTerritory()
{
	return targetTerritory;
}

// change the target territory
void Airlift::setTargetTerritory(Territory* changeTarget)
{
	targetTerritory = changeTarget;
}

// change source territory
void Airlift::setSourceTerritory(Territory* changeSource)
{
	sourceTerritory = changeSource;
}

// Returns the value for printing the appropriate description
int Airlift::getValue()
{
	return value2;
}
string Airlift::getOrderName() {
	return orderName;
}


/*
   Determines if the order is valid by checking the number of armies and appropriate territories
   If it is valid, the data member valid will be true, otherwise, it's false.
*/
bool Airlift::validate()
{
	for (auto p : playerTerritories)
	{
		for (int x = 0; x < playerTerritories.size(); x++)
		{
			if (p->getOwnerId() == playerTerritories[x]->getOwnerId())
			{
				return true;
			}
		}
	}

	return false;
}

/*
  If valid = true, then the order's actions will be executed and complete data member will be true.
*/
void Airlift::execute()
{
	if (validate() == true)
	{
		for (auto p : playerTerritories)
		{
			for (int y = 0; y < playerTerritories.size(); y++)
			{
				if (p->getName() == targetTerritory->getName())
				{
					sourceTerritory->decrementArmiesAmount(numArmies);
					targetTerritory->incrementArmiesAmount(numArmies);
					afterExecution(4);
					return;
				}

				if (targetTerritory->getOwnerId() != 0 && targetTerritory->getOwnerId() != sourceTerritory->getOwnerId())
				{
					sourceTerritory->decrementArmiesAmount(numArmies);
					int enemy = targetTerritory->getArmiesAmount();
					int myPlayer = numArmies;

					int enemyLeft = enemy - (myPlayer * 0.6);
					int myPlayerLeft = myPlayer - (enemy * 0.7);

					if (enemyLeft > 0 && myPlayerLeft <= 0) // enemy win the battle
					{
						targetTerritory->decrementArmiesAmount((myPlayer * 0.60));
						afterExecution(5);
						return;
					}
					else
					{
						if (myPlayerLeft >= 0 && enemyLeft <= 0) // player win the battle
						{
							targetTerritory->setArmiesAmount(myPlayerLeft);
							targetTerritory->setOwnerId(sourceTerritory->getOwnerId());
							afterExecution(6);
							return;
						}
						else
						{
							cout << "Invalid Order" << endl;
						}
					}
				}
				else
				{
					cout << "Not a valid target territory" << endl;
				}
			}
		}
	}
}

ostream& operator<< (ostream& out, const Airlift& copy)
{
	out << copy.orderName;
	return out;
}

                                                             /*Blockade Order*/
// Default constructor
Blockade::Blockade()
{
	targetTerritory = NULL;
	playerTerritories = vector<Territory*>();
}

Blockade::Blockade(Territory* target, vector<Territory*> listOfPlayerTerritory)
{
	targetTerritory = target;
	playerTerritories = listOfPlayerTerritory;
}

// Copy constructor
Blockade::Blockade(const Blockade& blockade) : Orders(blockade)
{
	this->targetTerritory = new Territory(*(blockade.targetTerritory));
	for (int i = 0; i < playerTerritories.size(); i++) {

		this->playerTerritories[i] = new Territory(*(blockade.playerTerritories[i]));
	}
}

// Assignment operator for the copy constructor
Blockade& Blockade::operator= (const Blockade& blockade)
{
	Orders::operator = (blockade);
	this->targetTerritory = new Territory(*(blockade.targetTerritory));
	for (int i = 0; i < playerTerritories.size(); i++) {

		this->playerTerritories[i] = new Territory(*(blockade.playerTerritories[i]));
	}
	return *this;
}

// Destructor
Blockade::~Blockade()
{
	delete targetTerritory;

	for (auto p : playerTerritories)
	{
		delete p;
		p = NULL;
	}
	playerTerritories.clear();
}

// returns the target territory
Territory* Blockade::getTargetTerritory()
{
	return targetTerritory;
}

// sets new value of the target territory
void Blockade::setTargetTerritory(Territory* changeTarget)
{
	targetTerritory = changeTarget;
}

// sets the new value of the player's/source territory
void Blockade::setSourceTerritory(Territory* changeSource)
{
	return;
}

// Returns the value for printing the appropriate description
int Blockade::getValue()
{
	return value3;
}
string Blockade::getOrderName() {
	return orderName;
}
/*
   Determines if the order is valid by checking the appropriate territories
   If it is valid, the data member valid will be true, otherwise, it's false.
*/
bool Blockade::validate()
{
	for (auto p : playerTerritories)
	{
		if (p->getName() == targetTerritory->getName())
		{
			return true;
		}
	}
	return false;
}

/*
  If valid = true, then the order's actions will be executed and complete data member will be true.
*/
void Blockade::execute()
{
	if (this->validate())
	{
		targetTerritory->setOwnerId(0); // zero mean that it doesn not belong to anyone
		targetTerritory->setArmiesAmount(targetTerritory->getArmiesAmount() * 2); // double amount of army in territory

		int x = 0;
		for (auto p : playerTerritories)
		{
			if (p->getName() == targetTerritory->getName())
			{
				playerTerritories.erase(playerTerritories.begin()+x);
		        }
			else
			{
				x++;
			}
		}
		afterExecution(7);
	}
}


ostream& operator<< (ostream& out, const Blockade& copy)
{
	out << copy.orderName;
	return out;
}

                                                                  /*Bomb Order*/
// Default constructor
Bomb::Bomb()
{
	targetTerritory = NULL;
	playerTerritories = vector<Territory*>();
}

Bomb::Bomb(Territory* target, vector<Territory*> listOfplayerTerritory)
{
	targetTerritory = target;
	playerTerritories = listOfplayerTerritory;
}


// Assignment operator for the copy constructor
Bomb& Bomb::operator= (const Bomb& bomb)
{
	Orders::operator= (bomb);
	this->targetTerritory = new Territory(*(bomb.targetTerritory));
	for (int i = 0; i < playerTerritories.size(); i++) {

		this->playerTerritories[i] = new Territory(*(bomb.playerTerritories[i]));
	}
	return *this;
}

// Copy constructor
Bomb::Bomb(const Bomb& bomb) : Orders(bomb)
{
	this->targetTerritory = new Territory(*(bomb.targetTerritory));
	for (int i = 0; i < playerTerritories.size(); i++) {

		this->playerTerritories[i] = new Territory(*(bomb.playerTerritories[i]));
	}
}

// Destructor
Bomb::~Bomb()
{
	delete targetTerritory;

	for (auto p : playerTerritories)
	{
		delete p;
		p = NULL;
	}
	playerTerritories.clear();
}

Territory* Bomb::getTargetTerritory()
{
	return targetTerritory;
}

void Bomb::setTargetTerritory(Territory* changeTarget)
{
	targetTerritory = changeTarget;
}

// not needed but is defined becuase it's a virtual method
void Bomb::setSourceTerritory(Territory* changeSource)
{
	return;
}

// Returns the value for printing the appropriate description
int Bomb::getValue()
{
	return value4;
}
string Bomb::getOrderName() {
	return orderName;
}

/*
   Determines if the order is valid by checking the appropriate territories
   If it is valid, the data member valid will be true, otherwise, it's false.
*/
bool Bomb::validate()
{
	for (auto p : playerTerritories)
	{
		if (p->getName() == targetTerritory->getName())
		{
			return false;
		}
	}
	return true;
}

/*
  If valid = true, then the order's actions will be executed and complete data member will be true.
*/
void Bomb::execute()
{
	if (validate())
	{
		targetTerritory->setArmiesAmount((targetTerritory->getArmiesAmount() / 2));
		afterExecution(8);
	}
}

ostream& operator<< (ostream& out, const Bomb& copy)
{
	out << copy.orderName;
	return out;
}

                                                             /*Deploy Order*/
// Default constructor
Deploy::Deploy()
{
	amountDeploy = 0;
	targetTerritory = NULL;
	listOfplayerTerritories = vector<Territory*>();
}

// Parameterized constructor
Deploy::Deploy(Territory* target, int amount, vector<Territory*> listOfPlayerTerritory)
{
	amountDeploy = amount;
	targetTerritory = target;
	listOfplayerTerritories = listOfPlayerTerritory;
}

// Copy constructor
Deploy::Deploy(const Deploy& deploy) : Orders(deploy)
{
	Orders::operator = (deploy);
	this->amountDeploy = deploy.amountDeploy;
	this->targetTerritory = new Territory(*deploy.targetTerritory);

	for (int i = 0; i < deploy.listOfplayerTerritories.size(); i++) {

		this->listOfplayerTerritories[i] = new Territory(*(deploy.listOfplayerTerritories[i]));
	}
}

// Assignment operator for the copy constructor
Deploy& Deploy::operator= (const Deploy& deploy)
{
	Orders::operator = (deploy);
	this->amountDeploy = deploy.amountDeploy;
	this->targetTerritory = new Territory(*deploy.targetTerritory);

	for (int i = 0; i < deploy.listOfplayerTerritories.size(); i++) {

		this->listOfplayerTerritories[i] = new Territory(*(deploy.listOfplayerTerritories[i]));
	}

	return *this;
}

// Destructor
Deploy::~Deploy()
{
	delete targetTerritory;

	for (auto p : this->listOfplayerTerritories)
	{
		delete p;
		p = NULL;
	}
	this->listOfplayerTerritories.clear();
}

// returns the target territory
Territory* Deploy::getTargetTerritory()
{
	return targetTerritory;
}

// sets the new value for the target territory
void Deploy::setTargetTerritory(Territory* changeTarget)
{
	targetTerritory = changeTarget;
}

// not needed but must have as it is a virtual method
void Deploy::setSourceTerritory(Territory* changeSource)
{
	return;
}

// Returns the value for printing the appropriate description
int Deploy::getValue()
{
	return value5;
}
string Deploy::getOrderName() {
	return orderName;
}

/*
   Determines if the order is valid by checking the number of armies and appropriate territories
   If it is valid, the data member valid will be true, otherwise, it's false.
*/
bool Deploy::validate()
{
	for (auto p : listOfplayerTerritories)
	{
		if (targetTerritory->getName() == p->getName())
		{
			return true;
		}
	}
	return false;
}

/*
  If valid = true, then the order's actions will be executed and complete data member will be true.
*/
void Deploy::execute()
{
	if (this->validate())
	{
		targetTerritory->incrementArmiesAmount(amountDeploy);
		afterExecution(9);
	}
	else
	{
		cout << "not valid" << endl;
	}

}


ostream& operator<< (ostream& out, const Deploy& copy)
{
	out << copy.orderName;
	return out;
}


                                                              /*Negotiate Order*/

// Default constructor
Negotiate::Negotiate()
{
	myPlayer = nullptr;
	enemyPlayer = nullptr;
}

Negotiate::Negotiate(Player* myPlay, Player* enemyPlay)
{
	myPlayer = myPlay;
	enemyPlayer = enemyPlay;
}

// Copy constructor
Negotiate::Negotiate(const Negotiate& negotiate) : Orders(negotiate)
{
	Orders::operator = (negotiate);
	this->myPlayer = new Player(*negotiate.myPlayer);
	this->enemyPlayer = new Player(*negotiate.enemyPlayer);
}

// Assignment operator for the copy constructor
Negotiate& Negotiate::operator= (const Negotiate& negotiate)
{
	Orders::operator = (negotiate);
	this->myPlayer = new Player(*negotiate.myPlayer);
	this->enemyPlayer = new Player(*negotiate.enemyPlayer);
	return *this;
}

// Destructor
Negotiate::~Negotiate()
{
	delete myPlayer;
	delete enemyPlayer;
	myPlayer = nullptr;
	enemyPlayer = nullptr;
}

// not needed but is here because it is a virtual method
Territory* Negotiate::getTargetTerritory()
{
	return nullptr;
}

// not needed but is here becasue it is a virtual method
void Negotiate::setTargetTerritory(Territory* changeTarget)
{
	return;
}

// not needed but is here becasue it is a virtual method
void Negotiate::setSourceTerritory(Territory* changeSource)
{
	return;
}

// Returns the value for printing the appropriate description
int Negotiate::getValue()
{
	return value6;
}

string Negotiate::getOrderName() {
	return orderName;
}
/*
   Determines if the order is valid by checking the appropriate territories
   If it is valid, the data member valid will be true, otherwise, it's false.
*/
bool Negotiate::validate()
{
	if (myPlayer == enemyPlayer) // if both players are the same, then it is false
		return false;
	else
		return true; // if both players are not the same, then it is true
}

/*
  If valid = true, then the order's actions will be executed .
*/
void Negotiate::execute()
{
	if (validate())
	{
		const type_info& checkObject = typeid(enemyPlayer->getPlayerOrderList()->getList().at(0));
		const type_info& checkAdvance = typeid(Advance);
		const type_info& checkAirlift = typeid(Airlift);
		const type_info& checkBomb = typeid(Bomb);
		vector<Territory*> myPlayerTerritories = myPlayer->getAllTerritories();

		if (checkObject == checkAdvance || checkObject == checkAirlift) // if the enemy's order is of type Advance, then true
		{
			for (int x = 0; x < myPlayerTerritories.size(); x++) // loop through the myPlayer's territory list
			{
				if (enemyPlayer->getPlayerOrderList()->getList().at(0)->getTargetTerritory() == myPlayerTerritories[x]) // if enemy plans to target your territory, then order becomes invalid
				{
					enemyPlayer->getPlayerOrderList()->getList().at(0)->setSourceTerritory(myPlayerTerritories[x]); // order becomes invalid my changing the source territory to a territory belonging to another player
					break; // exits loop
				}
			}
		}
		else if (checkObject == checkBomb)
		{
			for (int x = 0; x < myPlayerTerritories.size(); x++) // loop through the myPlayer's territory list
			{
				if (enemyPlayer->getPlayerOrderList()->getList().at(0)->getTargetTerritory() == myPlayerTerritories[x]) // if enemy plans to target your territory, then order becomes invalid
				{
					enemyPlayer->getPlayerOrderList()->getList().at(0)->setTargetTerritory(enemyPlayer->getAllTerritories().at(0)); // order becomes invalid by replacing target territory with the enemy's own territory
					break; // exits loop
				}
			}
		}
		afterExecution(10);
	}
}

ostream& operator<< (ostream& out, const Negotiate& copy)
{
	out << copy.orderName;
	return out;
}


                                                              /*Orderlist Class*/
// Default constructor
Orderlist::Orderlist()
{
	list = {};
}

// The assignment operator for the copy constructor
Orderlist& Orderlist::operator= (const Orderlist& orderlist)
{
	for (int index = 0; index < list.size(); index++)
		this->list[index] = orderlist.list[index];
	return *this;
}

// The copy constructor
Orderlist::Orderlist(const Orderlist& orderlist)
{
	for (int index = 0; index < list.size(); index++)
		this->list[index] = orderlist.list[index];
}


// Destructor
Orderlist::~Orderlist()
{
	for (auto ptr : list)
	{
		delete ptr;
		ptr = NULL;
	}
	list.clear();

}

// Adds an order to the list at the end
void Orderlist::add_OrderToList(Orders& orderlist)
{
	list.push_back(&orderlist);
}

// Displays the order list
void Orderlist::displayOrders()
{
	int x = 0;

	for (auto index = list.begin(); index != list.end(); ++index)
	{
		cout << x;
		cout << ". ";
		cout << *index << endl;
		x++;
	}
	return;
}

// Removes an order from the order list
void Orderlist::remove(int index)
{
	if (index >= 0 && index < (int)list.size())
	{
		list.erase(list.begin() + index);
	}
	else
	{
		cout << "The order you wish to remove does not exist" << endl;
	}

}

// Swaps two orders into each other's resective container in the order list
void Orderlist::move(int one, int two)
{
	if (one >= 0 && two >= 0 && one < (int)list.size() && two < (int)list.size())
	{
		Orders* temp = list.at(one);
		list[one] = list[two];
		list[two] = temp;
	}
	else
	{
		cout << "The order you wish to move does not exist" << endl;
	}

}

// Ostream
ostream& operator<<(ostream& out, const Orderlist& copy)
{
	/*out << "Your orders as follows\n";
	return out;*/
	for (int x = 0; x < copy.list.size(); x++)
	{
		out << copy.list[x];
	}
	return out;
}

// Returns the vector list
vector <Orders*> Orderlist::getList()
{
	return list;
}



