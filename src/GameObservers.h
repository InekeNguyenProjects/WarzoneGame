/*
 * GameObservers.h
 *
 *  Created on: Apr. 26, 2024
 *      Author: mikenkie
 */

#pragma once
#include <iomanip>
#include <iostream>
#include <list>

using namespace  std;

class GameEngine;
class Observer;
class Observable;

#ifndef GAMEOBSERVERS_H_
#define GAMEOBSERVERS_H_

// Observer
class Observer {
public:
	~Observer();
	virtual void Update() = 0;    //pure virtual method
	virtual void display() = 0;   //pure virtual method

protected:
	Observer();
};

// Subject
class Observable {
public:
	Observable();
	~Observable();
	virtual void Detach(Observer* observe);
	virtual void Attach(Observer* observe);
	virtual void Notify();

private:
	list <Observer*>* observerList;
};


//view
class PhaseObserver: public Observer {
public:
	PhaseObserver();
	PhaseObserver(GameEngine* game);
	~PhaseObserver();
	PhaseObserver(const PhaseObserver& observe);
	PhaseObserver& operator = (const PhaseObserver& oo);
	friend ostream& operator<< (ostream& out, const PhaseObserver& copy);
	void Update();
	void display();
	void displayReinforcementPhase();
	void displayIssuePhase();
	void displayExecutionPhase();

private:
	GameEngine* subject;
};

//view
class StatisticObserver: public Observer {
public:
	StatisticObserver();
	~StatisticObserver();
	StatisticObserver(GameEngine* game);
	StatisticObserver(const StatisticObserver& observe);
	StatisticObserver& operator = (const StatisticObserver& oo);
	friend ostream& operator<< (ostream& out, const StatisticObserver& copy);
	void Update();
	void display();

private:
	GameEngine* subject;
};




#endif /* GAMEOBSERVERS_H_ */
