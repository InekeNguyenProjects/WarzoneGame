//============================================================================
// Name        : GameEngine.cpp
// Author      : Ineke Nguyen
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include "GameEngine.h"

#include <algorithm>
#include<random>
#include <limits>
#include <stdio.h>
#include <string.h>
#include "dirent.h"


using namespace std;

GameEngine::GameEngine()
{
    gameMap = NULL;
    gameDeck = NULL;
    players = {};
    state = State::COND0;
    maploader = NULL;
}

GameEngine::GameEngine(const string mapDirectory)
{
    createGameMap(mapDirectory);
    createPlayers();
    createDeck();
    observerSettings();
    startupPhase();
    mainGameLoop();

}

GameEngine::GameEngine(const GameEngine& obj)
{
    this-> gameMap = new Map (*(obj.gameMap));
    this-> gameDeck = new Deck (*(obj.gameDeck));

    for (int index = 0; index < obj.players.size(); index++)
        this->players[index] = new Player(*(obj.players[index]));

    this->state = obj.state;
}

GameEngine::~GameEngine()
{
    delete gameMap;
    gameMap = NULL;

    delete gameDeck;
    gameMap = NULL;

    for (int i = 0; i < players.size(); ++i) {
        delete players[i];
        players[i] = NULL;
    }
    players.clear();

    delete maploader;
    maploader = NULL;
}

void GameEngine::createGameMap(const string directory) {
    if(directory.find("ConquestMap") != -1)
    {
        maploader = new ConquestFileReaderAdapter(new ConquestFileReader);
    }
    else
    {
        maploader = new MapLoader();
    }

    gameMap = maploader->selectMap(directory);
    cout << endl << "AMOUNT OF COUNTRIES:" <<gameMap->territoryCount()<< endl;

}

// Gets input from the user for the amount of players that should be created
void GameEngine::createPlayers() {

    cout << endl <<"---CHOOSING THE NUMBER OF PLAYERS---" << endl;
    cout << "Select amount of players (2 - 5): " << endl;
    int numPlayers = getUserInputInteger("Your choice (-1 to quit): ", 2, 5);
    string typeStrategy = "";                                                //// just added

    for (int i = 0; i < numPlayers; i++) {
        cout << "Created Player with ID: " << (i + 1) << "..." << endl;
        Player* thePlayer = new Player(i + 1);
        cout << "Choose what strategy you want your player to have " << endl;    //////// 74-76 just added
        cin >> typeStrategy;
        thePlayer->setStrategy(typeStrategy);
        players.push_back(thePlayer);
    }

    cout << endl << "---EACH PLAYER HAS ALSO BEEN ASSIGNED A EMPTY HAND---" << endl;
}

void GameEngine::createDeck() {
    int numberofcards = gameMap->territoryCount();
    gameDeck = new Deck(numberofcards);

    Cards Airlift( "Airlift") ;
    Cards Blockade( "Blockade") ;
    Cards Bomb( "Bomb") ;
    Cards Negotiate( "Negotiate");
    for(int inum = 0 ; inum < numberofcards/4; inum++)
    {
        gameDeck->addCard(Airlift) ;
        gameDeck->addCard(Blockade) ;
        gameDeck->addCard(Bomb) ;
        gameDeck->addCard(Negotiate) ;
    }
    gameDeck->shuffle();
    cout <<*gameDeck<<endl;
}

//return vector of randomly ordered players
void GameEngine::setPlayerOrder()
{
    cout << endl << "---ASSIGNING RANDOM PLAYER ORDER---" << endl;
    std::random_device rng;
    std::mt19937 urng(rng());
    shuffle(begin(players), end(players), urng);

    cout << endl << "Current player order: " << endl;
    for (int i = 0; i < players.size(); i++) {
        cout << "Player with ID " << (i + 1) << ": " << players[i]->getPlayerId() << endl;
    }

}

//assign territories to each player round robin style
void GameEngine::assignTerritories()
{
    cout << endl << "---ASSIGNING RANDOM TERRITORIES TO PLAYERS---" << endl;

    const int size = gameMap->territoryCount();
    vector<int> temp;

    for (int i = 0; i < size; i++) {
        temp.push_back(i);
    }

    std::random_device rng;
    std::mt19937 urng(rng());
    shuffle(begin(temp), end(temp), urng);


    vector<Territory*> allTerritories = gameMap->getAllTerritories();

    int count = 0;
    for (int i = 0; i < temp.size(); i++) {
        Territory* toBeAssigned = allTerritories[temp[i]];
        players.at(count % players.size())->addTerritory(toBeAssigned);
        count++;
    }


    // Pretty print all the territories assigned
    cout << endl << "Current territories assigned: " << endl;
    for (int i = 0; i < players.size(); i++)
    {
        cout << endl <<"Player with ID:"<<players[i]->getPlayerId() << " (has " << players[i]-> getAllTerritories().size() << " territories): " << endl;
        for (int j = 0; j < players[i]->getAllTerritories().size(); j++) {
            cout << players[i]->getAllTerritories()[j]->getName() << endl;
        }
    }

}

//assign armies to each territories round robin style
void GameEngine::assignArmies()
{
    cout << endl << "---ASSIGNING ARMIES TO PLAYERS---" << endl;
    //Players are given a number of armies (A), to be placed one by one in a round-robin fashion*/
    int numArmies = 0;

    switch (players.size()) {
    case 2: numArmies = 40;
        break;
    case 3: numArmies = 35;
        break;
    case 4: numArmies = 30;
        break;
    case 5: numArmies = 25;
        break;
    }

    cout << endl << "There will be "<<numArmies<<" armies added to each players reinforcement pool " << endl;

    // sets the armie to player reinformcent pool
    for (int i = 0; i < players.size(); i++) {
        players[i]->setReinformentPool(numArmies);
    }

    cout << endl << "Current player armies: " << endl;
    for (int i = 0; i < players.size(); i++) {
        cout <<"Player with ID " << (i + 1) << ": " << players[i]->getReinformentPool()<<endl;
    }
}

// create new player objectinitializing its ID and pushing it into vector players
void GameEngine::startupPhase ()
{
    setPlayerOrder();
    assignTerritories();
    assignArmies();
    setState(State::CONDSTAT);
};

void GameEngine::observerSettings() {

    cout << endl << "---ASSIGNING ARMIES TO PLAYERS---" << endl;

    string input = getUserInputString("Do you want to enable the Phase observer? (y/n):", "y", "n");

    if (input == "y") {
        PhaseObserver* myPhase = new PhaseObserver(this);
        cout << "\nPhase Observer has been ENABLED" << endl;
    }
    else {
        cout << "\nPhase Observer has been DISABLED" << endl;
    }
    string input2 = getUserInputString("Do you want to enable the Stat observer? (y/n):", "y", "n");

    if (input2 == "y") {
        StatisticObserver* myStatistic = new StatisticObserver(this);
        cout << "\nGame stats Observer has been ENABLED " << endl;
    }
    else {
        cout << "\nGame stats Observer has been  DISABLED" << endl;
    }
}

Deck* GameEngine::getDeck()
{
    return gameDeck;
}

vector<Player*> GameEngine::getPlayers()
{
    return players;
}

Map* GameEngine::getMap()
{
    return gameMap;
}

// Helper function for getting user integer input
int GameEngine::getUserInputInteger(string output, int min, int max) {
    string inputString;
    int input;
    bool failFlag;

    cout << output;
    cin >> inputString;

    try {
        input = stoi(inputString);
        failFlag = false;
    }
    catch (invalid_argument e) {
        failFlag = true;
    }

    // While the input is invalid
    while (input < min || input > max || cin.fail() || failFlag || inputString.find(".") != string::npos) {
        if (input == -1) {
            cout << "Quitting..." << endl;
            exit(1);
        }

        cin.clear();
        //cin.ignore(numeric_limits<streamsize> std::max, '\n');
        cout << "Invalid input" << endl;

        if (inputString.find(".") == string::npos) {
            cout << "Must be greater than " << min - 1 << " and less than " << max + 1 << " and must be an integer" << endl;
        }
        else {
            cout << "Must be an integer" << endl;
        }
        cout << "Try again (-1 to quit): ";
        cin >> inputString;

        try {
            input = stoi(inputString);
            failFlag = false;
        }
        catch (invalid_argument e) {
            failFlag = true;
        }
    }

    return input;
}

// Helper function for getting user string input
string GameEngine::getUserInputString(string output, string choice1, string choice2) {

    string input;

    cout << output;
    cin >> input;

    // While the input is invalid
    while ((input != choice1 && input != choice2) || cin.fail() || input == "q" || input == "Q") {
        if (input == "q" || input == "Q") {
            cout << "Quitting..." << endl;
            exit(1);
        }

        // Clear the stream
        cin.clear();
        //cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input" << endl;
        cout << "Must be " << choice1 << " or " << choice2 << " and must be a string" << endl;
        cout << "Try again ('q' to quit): ";
        cin >> input;
    }

    return input;
}

GameEngine& GameEngine::operator=(const GameEngine& copy)
{
    this->gameMap = new Map(*(copy.gameMap));
    this->gameDeck = new Deck(*(copy.gameDeck));

    for (int index = 0; index < copy.players.size(); index++)
        this->players[index] = new Player(*(copy.players[index]));
    this->state = copy.state;
    return *this;
}

void GameEngine:: mainGameLoop()
{
    int turns= 0 ;
    bool keepPlaying = true;
    string changeStrategy = "";
    string yesOrNo = "";

   do
   {
       cout<<"TURN: "<<turns<<endl<<endl;
        reinforcementPhase();
        issueOrdersPhase();
        executeOrdersPhase();
        for(auto p : players)
        {
            cout << "Do you want to change your strategy? Yes or No: ";    /////// 326-343 just added
            cin >> yesOrNo;
            if (yesOrNo == "Yes" || yesOrNo == "yes")
            {
                cout << "State your new strategy: ";
                cin >> changeStrategy;
                p->setStrategy(changeStrategy);
            }

            cout<<"checking players status"<<endl;
            if(p->getAllTerritories().size() == gameMap->getAllTerritories().size())
            {
                keepPlaying = false;
            }

            if(p->getAllTerritories().size() == 0)
            {
                delete p;
            }

        }
        turns++;
      //system("pause");
    }
    while(keepPlaying && players.size() >= 1);
}

void GameEngine::reinforcementPhase() // addreinforment  =  bonus + (territory/3)<~ should be greater than 3
{
    for(auto rein: players) // add armies
    {
        double armies = rein->getAllTerritories().size();// get amount of territory a player has
        armies = floor(armies/3);// divide by 3 and round down

        if(armies < 3) // minimum should be 3 armies
        {
            armies = 3;
        }

        rein->addReinforment(armies);
    }
    cout << "\nAdding a bonus armies" << endl;
    for(auto p : gameMap->getAllContinents()) // add any bonus if to the corresponding player according to continent owner
    {
        int playerId = p->checkOwnership();
        if(playerId) // return a int(however in c++ 0 is consider false any other number is consider true)
        {
            string ContinentName = p->getName();
            int bonus = p->getArmiesBonus();
            for(auto rein : players)
            {
                if(rein->getPlayerId() == playerId)
                {
                    rein->addReinforment(bonus);
                }
            }
        }
    }
    setState(State::COND1);
}

void GameEngine::issueOrdersPhase()
{
    cout<<"STARTING ISSUE ORDER PHASE"<<endl;

        for(auto p: players)
        {
            p->issueOrder(this->getMap(), this->getPlayers(), this->getDeck());
            cout<<"player "<<p->getPlayerId()<<" Done"<<endl;
        }
     setState(State::COND2);
}

//sorts the player's order list from least to highest priority (deploy, airlift, blockade, then the rest)
void GameEngine::sortListByPriority(Player* player)
{
    for (int index = 0; index < (player->getPlayerOrderList()->getList().size() - 1); index++)
    {
        for (int nextIndex = index + 1; nextIndex < player->getPlayerOrderList()->getList().size(); nextIndex++)
        {
            if (player->getPlayerOrderList()->getList().at(index)->getValue() > player->getPlayerOrderList()->getList().at(nextIndex)->getValue())
                player->getPlayerOrderList()->move(index, nextIndex);
        }
    }
}

void GameEngine::executeOrdersPhase()
{
    setState(State::COND3);
    setState(State::CONDSTAT);
    cout<<"<<<<<<<<<<<executionOrderPhase>>>>>>>>>>"<<endl;
    int index = 0;
    int checkforemptyorderlist = 0;

    // Reorganizes the orders in order of priority for each player's orderlist
    for (auto p : players)
    {
        sortListByPriority(p);
    }

    //executes orders in a round-robin fashion
    while (checkforemptyorderlist < players.size())
    {
        for (int thisPlayerIsPlayingNow = 0; thisPlayerIsPlayingNow < players.size(); thisPlayerIsPlayingNow++)
        {
            if (!players[thisPlayerIsPlayingNow]->getPlayerOrderList()->getList().empty())
            {
                players[thisPlayerIsPlayingNow]->getPlayerOrderList()->getList().at(0)->execute();
                players[thisPlayerIsPlayingNow]->getPlayerOrderList()->remove(0);
                checkforemptyorderlist = 0;
            }
            else
            {
                checkforemptyorderlist++;
            }
        }
    }
}

vector<Player*> GameEngine::getGamePlayer()
{
    return this->players;
}
GameEngine::State GameEngine::getState() {
    return state;
}
void GameEngine::setState(State state) {
    this->state = state;
    this->Notify();
}


int main(int argc, const char* argv[]) {

    const string mapDirectory = ".\\ConquestMap\\";
    //const string mapDirectory = ".\\Maps\\";
    GameEngine* game = new GameEngine(mapDirectory);

    delete game;
    return 0;
}
