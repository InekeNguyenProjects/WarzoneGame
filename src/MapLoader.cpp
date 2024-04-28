/*
 * MapLoader.cpp
 *
 *  Created on: Apr. 26, 2024
 *      Author: mikenkie
 */

#include "Map.h"
#include <iostream>


//Territory class

Territory::Territory()
{
	name = "";
	continentName = "";
	armiesAmount = 0;
	ownerId = 0;

}

Territory::Territory(string tName, string cName)
{
	name = tName;
	continentName = cName;
	ownerId = 0;
}

Territory::Territory(const Territory& copy) {

	name = copy.name;
	continentName = copy.continentName;
	ownerId = copy.ownerId;
}

Territory& Territory::operator=(const Territory& copy)
{
	this->name = copy.name;
	this->continentName = copy.continentName;
	this->ownerId = copy.ownerId;
	return *this;
}

ostream& operator<<(ostream& out, const Territory& copy)
{
	out << copy.name;

	return out;
}

istream& operator>>(istream& in, Territory& copy)
{
	cout << "Enter territory name: ";
	in >> copy.name;

	return in;
}

Territory::~Territory()
{

	for (auto p : neighbors)
	{
		delete p;
		p = NULL;
	}
	neighbors.clear();

}

string Territory::getName()
{
	return name;
};

string Territory::getContinentName()
{
	return continentName;
}

void Territory::setArmiesAmount(int amount)
{
	armiesAmount = amount;
};

int Territory::getArmiesAmount()
{
	return armiesAmount;
}


void Territory::incrementArmiesAmount(int newSoldiers)
{
	armiesAmount += newSoldiers;
}

void Territory::decrementArmiesAmount(int lostSoldiers)
{
	armiesAmount -= lostSoldiers;
}

bool Territory::hasNeighbors()
{
	return !neighbors.empty();
};

void Territory::addNeighbor(Territory& territory)
{
	neighbors.push_back(&territory);
}

int Territory::getNeighborAmount()
{
	return (int)neighbors.size();
};

vector<Territory*> Territory::getAllNeighbors() {

	vector<Territory*> territories = vector<Territory*>();

	for (auto& temp : neighbors) {
		territories.push_back(temp);
	}
	return territories;
}

vector<string> Territory::getAllNeighborsName()
{
	vector<string> neighborsName = vector<string>();
	for (auto temp : neighbors)
	{
		neighborsName.push_back(temp->getName());
	}
	return neighborsName;
}

void Territory::setOwnerId(int inum) // set the ownerId
{
	ownerId = inum;
}



//Continent class

Continent::Continent()
{
	name = "";
	peace = false;
}

Continent::Continent(string Cname, int bonus)
{
	name = Cname;
	armiesBonus = bonus;
	peace = false;
}

Continent::Continent(const  Continent& copy) {

	name = copy.name;
	armiesBonus = copy.armiesBonus;
}

Continent& Continent::operator=(const Continent& copy)
{
	this->name = copy.name;
	this->armiesBonus = copy.armiesBonus;

	return *this;
}

ostream& operator<<(ostream& out, const Continent& copy)
{
	out << "Continent name: " << copy.name << ", Armies bonus due to possesion: " << copy.armiesBonus << endl;

	return out;
}

istream& operator>>(istream& in, Continent& copy)
{
	cout << "Enter continent name: ";
	in >> copy.name;

	return in;
}

Continent::~Continent()
{
	for (auto p : myTerritory)
	{
		delete p;
		p = NULL;
	}
	myTerritory.clear();
}

string Continent::getName()
{
	return name;
};

int Continent::getArmiesBonus()
{
	return armiesBonus;
};

void Continent::addContinentTerritory(Territory tempTerritory)
{
	myTerritory.push_back(new Territory(tempTerritory));
}

int Continent::numberOfTerritory()
{
	return myTerritory.size();
}

int Continent::checkOwnership()// : all territory are own by one person. 0 there are multiple or there are unoccupied territory
{
	const int inum = 0; // no "0" is use as of no owner using var incase future change is needed in the value
	int owner = 0;
	for (auto p : myTerritory)
	{
		int itemp = p->getOwnerId();
		if (itemp != inum) // if p as a owner enter
		{
			if (itemp != owner)// there are different owner for the territory therefore no one own the continent
			{
				return inum;//not owner;
			}
			owner = itemp;//  same value will be save over and over again
		}
		else // at least 1 of the territory in the continent is not own by anyone
		{
			return inum;
		}
	}
	// when it reach here we know that all the territory in the continent are own by one person
	return owner;
}

bool Continent::getTreaty()
{
	return peace;
}

void Continent::armistice(bool status)
{
	peace = status;
}



//Map class

Map::Map()
{

}

Map::~Map()
{
	for (map<string, Territory*>::iterator i = territories.begin(); i != territories.end(); ++i) {

		delete i->second;
		i->second = NULL;
	}

	for (map<string, Continent*>::iterator j = continents.begin(); j != continents.end(); ++j) {

		delete j->second;
		j->second = NULL;
	}
}

Map::Map(const Map& copy) {

}

Map& Map::operator=(const Map& copy)
{

	return *this;
}

ostream& operator<<(ostream& out, const Map& copy)
{

	return out;
}

istream& operator>>(istream& in, Map& copy)
{

	return in;
}

int Map::territoryCount()
{
	return (int)territories.size();
}

int Map::continentCount() {

	return (int)continents.size();
}

// Add territory and creates edges
void  Map::addTerritory(string territoryName, string continentName, vector<string> neighborNames)
{
	Territory* temp1 = getTerritory(territoryName);
	if (temp1 != nullptr) {
		addEdgeAll(*temp1, neighborNames);

	}
	else {
		Territory* temp2 = new Territory(territoryName, continentName);
		territories[territoryName] = temp2;
		addEdgeAll(*temp2, neighborNames);
	}
}

// Add Continent
void Map::addContinent(Continent* continent)
{
	continents[continent->getName()] = continent;
};

Territory* Map::getTerritory(string TName)
{
	return territories[TName];
}

Continent* Map::getContinent(string CName)
{
	return continents[CName];
}

//Returns all the territories
vector<Territory*> Map::getAllTerritories()
{
	vector<Territory*> tempTerritories;

	for (map<string, Territory*>::iterator i = territories.begin(); i != territories.end(); i++) {
		tempTerritories.push_back(i->second);
	}

	return tempTerritories;
}

// Returns all the continents
vector<Continent*> Map::getAllContinents()
{
	vector<Continent*> tempContinents;

	for (std::map<string, Continent*>::iterator i = continents.begin(); i != continents.end(); i++) {
		tempContinents.push_back(i->second);
	}

	return tempContinents;
}

// Returns all the territories by continents
vector<Territory*>  Map::getTerritoriesByContinent(string continentName)
{
	vector<Territory*> tempTerritories;

	for (std::map<string, Territory*>::iterator i = territories.begin(); i != territories.end(); i++) {
		if (i->second->getContinentName() == continentName) {
			tempTerritories.push_back(i->second);
		}
	}

	return tempTerritories;
}

//Performs a depth - first - search traversal of the graph and return the numberof nodes visited
int Map::traverseAll(bool isDebug) {
	map<string, bool> visited;

	for (map<string, Territory*>::iterator it = territories.begin(); it != territories.end(); it++) {
		visited[it->first] = false;
	}

	int count = 0;
	map<string, Territory*>::iterator it = territories.begin();
	if (it->second != NULL) {
		territoryTraverseHelper(it->second, visited, count, isDebug);
	}

	return count;
}

//Perform a depth-first-search traversal of a continent subgraph
int Map::traverseContinent(string continent, bool isDebug = false) {
	map<string, bool> visited;

	for (map<string, Territory*>::iterator it = territories.begin(); it != territories.end(); it++) {
		visited[it->first] = false;
	}

	int count = 0;
	vector<Territory*> temp = getTerritoriesByContinent(continent);
	Territory* firstTerritory = temp[0];
	continentTraverseHelper(firstTerritory, visited, continent, count, isDebug);

	return count;
}

void Map::territoryTraverseHelper(Territory* territory, map<string, bool>& visited, int& count, bool isDebug = false) {

	visited[territory->getName()] = true;
	count++;
	vector<Territory*> neighbors = territory->getAllNeighbors();

	if (isDebug) {
		cout << territory->getName() << endl;

		for (int i = 0; i < neighbors.size(); i++) {
			cout << "-" << neighbors[i]->getName() << endl;
		}

		cout << endl;
	}

	for (int i = 0; i < neighbors.size(); i++) {
		if (!visited[neighbors[i]->getName()]) {
			territoryTraverseHelper(neighbors[i], visited, count, isDebug);
		}
	}
}

void Map::continentTraverseHelper(Territory* territory, map<string, bool>& visited, string continent, int& count, bool isDebug) {

	visited[territory->getName()] = true;
	count++;
	vector<Territory*> neighbors = territory->getAllNeighbors();

	if (isDebug) {
		cout << "[ P: " << territory->getName() << ", " << territory->getContinentName() << " ]\t---> " << endl;

		for (int i = 0; i < neighbors.size(); i++) {
			cout << "{ P: " << neighbors[i]->getName() << ", " << neighbors[i]->getContinentName() << endl;
		}

		cout << endl << endl;
	}

	for (int i = 0; i < neighbors.size(); i++) {
		if (!visited[neighbors[i]->getName()] && neighbors[i]->getContinentName() == continent) {
			continentTraverseHelper(neighbors[i], visited, continent, count, isDebug);
		}
	}
}

//Creates an edge in the graph between two territories
void Map::addEdge(Territory& from, Territory& to)
{
	from.addNeighbor(to);
	to.addNeighbor(from);
}

//Create edges from one given territory to all the other neighboring territories
void Map::addEdgeAll(Territory& from, vector<string>& neighborNames) {

	for (int i = 0; i < neighborNames.size(); ++i) {
		Territory* neighbors = getTerritory(neighborNames[i]);
		if (neighbors != nullptr) {
			addEdge(from, *neighbors);
		}
	}
}

//Return true if this map is a connected graph
bool Map::validate() {
	if (territories.size() > 0) {
		return traverseAll() == territories.size();
	}

	return false;
}


