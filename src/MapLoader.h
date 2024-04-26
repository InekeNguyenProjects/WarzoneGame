/*
 * MapLoader.h
 *
 *  Created on: Apr. 26, 2024
 *      Author: mikenkie
 */
#pragma once

#include "Map.h"
#include "dirent.h"

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <cstdlib>
#include <sstream>

using namespace std;

#ifndef MAPLOADER_H_
#define MAPLOADER_H_

class MapLoader {
private:

	vector<Continent*> ListOfContinent;
	vector<Territory*> ListOfTerritory;
	vector<string>displayAllMaps(const string mapDirectory);
	vector<string> split(const string& str, char delimeter);
	vector<string> split(const string& str);
	int getUserInputInteger(string, int, int);
	string getUserInputString(string, string, string);
public:
	MapLoader();
	virtual ~MapLoader();
	MapLoader(const MapLoader& copy);
	MapLoader& operator = (const MapLoader& copy);
	virtual Map* selectMap(const string mapDirectory);
	virtual Map* loadMap(string fileName);

};


// Adaptee
class ConquestFileReader {
public:
	ConquestFileReader();
	~ConquestFileReader();
	ConquestFileReader(const ConquestFileReader& copy);
	ConquestFileReader& operator =(const ConquestFileReader& copy);
	friend ostream& operator <<(ostream& strm, const  ConquestFileReader& conquest);
	vector<string> displayAllMaps(const string mapDirectory);
	int getUserInputInteger(string, int, int);
	string getUserInputString(string, string, string);
	Map* selectMapConquest(const string mapDirectory);
	Map* loadMap(string fileName);
private:
	vector<string> split(const string& str, char delimeter);
	vector<string> splitCont(const string& str);
	vector<string> splitTerr(const string& str);
	vector<Continent*> ListOfContinent;
	vector<Territory*> ListOfTerritory;
};


// Adapter
class ConquestFileReaderAdapter : public MapLoader {
private:
	ConquestFileReader* conquestReader;
public:
	ConquestFileReaderAdapter();
	~ConquestFileReaderAdapter();
	ConquestFileReaderAdapter(ConquestFileReader* conquest);
	ConquestFileReaderAdapter(const ConquestFileReaderAdapter& copy);
	friend ostream& operator << (ostream& strm, const ConquestFileReaderAdapter& adapter); //stream insertion operator
	ConquestFileReaderAdapter& operator=(const ConquestFileReaderAdapter& copy);
	virtual Map* selectMap(string fileName);
};




#endif /* MAPLOADER_H_ */
