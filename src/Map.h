/*
 * Map.h
 *
 *  Created on: Apr. 26, 2024
 *      Author: mikenkie
 */

#ifndef MAP_H_
#define MAP_H_

#include <map>
#include <vector>
#include <string>
#include<iostream>

using namespace std;

class Territory {

private:

	string name;
	string continentName;
	int armiesAmount;
	vector<Territory*> neighbors;
	int ownerId; // no owner;

public:

	Territory();
	Territory(string tName, string cName);
	Territory(const Territory& copy);
	Territory& operator=(const Territory& copy);
	friend ostream& operator << (ostream& out, const Territory& copy);
	friend istream& operator >> (istream& in, Territory& copy);
	~Territory();
	string getName();
	string getContinentName();
	void setArmiesAmount(int amount);
	int getArmiesAmount();
	void incrementArmiesAmount(int newSoldiers);
	void decrementArmiesAmount(int lostSoldiers);
	bool hasNeighbors();
	void addNeighbor(Territory& territory);
	int getNeighborAmount();
	vector<Territory*> getAllNeighbors();
	vector<string> getAllNeighborsName();

	void setOwnerId(int);
	int getOwnerId() {return ownerId; };

};

class Continent {

private:

	string name;
	int armiesBonus;
	vector<Territory* > myTerritory;
	bool peace;

public:

	Continent();
	Continent(string name,int bonus);
	Continent(const  Continent& copy);
	Continent& operator=(const  Continent& copy);
	friend ostream& operator << (ostream& out, const Continent& copy);
	friend istream& operator >> (istream& in, Continent& copy);
	~Continent();
	string getName();
	int getArmiesBonus();
	void addContinentTerritory(Territory);
	int numberOfTerritory();
	int checkOwnership(); // check if all the territory are own by one person
	bool getTreaty();
	void armistice(bool status);
};

class Map {

private:
	map<string, Territory*> territories;
	map<string, Continent*> continents;
	void addEdge(Territory& from, Territory& to);
	void addEdgeAll(Territory& from, vector<string>& neighborNames);
	void territoryTraverseHelper(Territory* territory, map<string, bool>& visited, int& count, bool isDebug);
	void continentTraverseHelper(Territory* territory, map<string, bool>& visited, string continent, int& count, bool isDebug);


public:
	Map();
	~Map();
	Map(const Map& copy);
	Map& operator=(const Map& copy);
	friend ostream& operator << (ostream& out, const Map& copy);
	friend istream& operator >> (istream& in, Map& copy);
	int territoryCount();
	int continentCount();
	void addTerritory(string territoryName, string continentName, vector<string> neighborNames);
	void addContinent(Continent *continent);
	Territory* getTerritory(string TName);
	Continent* getContinent(string CName);
	vector<Territory*> getAllTerritories();
	vector<Territory*> getTerritoriesByContinent(string continentName);
	vector<Continent*> getAllContinents();
	int traverseAll(bool isDebug = false);
	int traverseContinent(string continent, bool isDebug);
	bool validate();

};




#endif /* MAP_H_ */
