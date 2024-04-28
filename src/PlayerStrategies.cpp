/*
 * PlayerStrategies.cpp
 *
 *  Created on: Apr. 26, 2024
 *      Author: mikenkie
 */

#include "PlayerStrategies.h"


Strategy::Strategy()
{

}

Strategy::~Strategy()
{

}

//  HumanPlayerStrategy

HumanPlayerStrategy::HumanPlayerStrategy()
{
}

HumanPlayerStrategy::HumanPlayerStrategy(const HumanPlayerStrategy &temp)
{
}

HumanPlayerStrategy::~HumanPlayerStrategy()
{
}

HumanPlayerStrategy &HumanPlayerStrategy::operator=(const HumanPlayerStrategy &temp)
{
    return *this;
}

ostream &operator<<(ostream &out, const HumanPlayerStrategy &copy)
{
    out << "Human Strategy" << endl;
    return out;
}

vector<Territory*> HumanPlayerStrategy::toAttack(Player *player, int from)
{
    return player->getAllTerritories().at(from)->getAllNeighbors();
}

vector<Territory*> HumanPlayerStrategy::toDefend(Player *player)
{
    return player->getAllTerritories();
}

void HumanPlayerStrategy::issueOrder(Player *player, Map *gameMap, vector <Player*> vectorOfplayerInGame, Deck* deck)
{
    cout<<endl<<"Human Strategy IssueOrder"<<endl<<endl;
    Orders *temp;
    cout << "STARTING DEPLOY PHASE...." << endl;
    cout << "the player: " << player->getPlayerId() << " has " << player->getReinformentPool() << " army in the reinforcement Pool" << endl;
    while (player->getReinformentPool() > 0)
    {
        int ichoice, amount;
        cout << "the player: " << player->getPlayerId() << " has " << player->getReinformentPool() << " army in the reinforcement Pool" << endl;
        cout << "player: " << player->getPlayerId() << endl;
        cout << "Territory own by player: " << endl;

        showList(toDefend(player));

        cout << "choose a territory to deploy army: ";
        ichoice = checkInput(player->getAllTerritories().size());
        cout << endl
             << "Enter an number of army you wish to deploy: ";
        amount = checkInput(player->getReinformentPool());

        temp = new Deploy(player->getAllTerritories().at(ichoice), amount, player->getAllTerritories());
        player->getPlayerOrderList()->add_OrderToList(*temp);
        player->subReinforment(amount);
    }

    cout << "Card Phase..........." << endl;
    char choice;
    bool okay = true;
    cout << "do you want to use card(y/n)?";

    cin >> choice;

    while (choice == 'y' && okay || choice == 'Y' && okay)
    {
        int ichoosecard;
        if (player->getPlayerHand().getAllCards().size() > 0)
        {
            cout << "Player: " << player->getPlayerId() << " hands :" << endl;
            player->getPlayerHand().showCard();
            cout << "choose a card: "<<endl;
            ichoosecard = checkInput(player->getPlayerHand().getAllCards().size());
            cout << endl;
            player->getCardUsed().push_back(new Cards(player->getPlayerHand().playCard(ichoosecard)));

            string cardType = player->getCardUsed().back()->getCardType();
            if (cardType == "Airlift")
            {
                Player *enemy;
                Territory *attack;
                Territory *source;
                int amount;
                int sourceTerritory = -1;
                int attackTerritory = -1;
                for (auto p : gameMap->getAllTerritories())
                {
                    attackTerritory++;
                    cout << attackTerritory << ". " << p->getName() << endl;
                }
                cout << "choose a target territory: ";
                attackTerritory = checkInput(gameMap->getAllTerritories().size());
                cout << endl;
                attack = enemy->getAllTerritories().at(0);

                showList(toDefend(player));
                cout << "Choose a source territory: ";
                sourceTerritory = checkInput(player->getAllTerritories().size());
                cout << endl;
                int availableAmount = player->getAllTerritories().at(sourceTerritory)->getArmiesAmount();

                cout << "Choose an # of armies to move: ";
                amount = checkInput(availableAmount);
                cout << endl;
                if (amount > availableAmount)
                {
                    cout << "invalid amount, card use already, order not created :( " << endl;
                }
                else
                {
                    temp = new Airlift(attack, source, player->getAllTerritories(), amount);
                }
            }
            else
            {
                if (cardType == "Blockade")
                {
                    int inum = 0;
                    int territoryToBlock = 0;
                    cout << "which territory you want to block" << endl;
                    showList(toDefend(player));
                    cout << "Choose a territory to block" << endl;
                    territoryToBlock = checkInput(player->getAllTerritories().size());
                    temp = new Blockade(player->getAllTerritories().at(territoryToBlock), player->getAllTerritories());
                }
                else
                {
                    if (cardType == "Bomb")
                    {
                        int bombTerritory = -1;
                        Territory *enemyTerritory;
                        showList(toDefend(player));
                        for (auto p : gameMap->getAllTerritories())
                        {
                            bombTerritory++;
                            cout << bombTerritory << ". " << p->getName();
                        }
                        cout << "Choose a territory: ";
                        bombTerritory = checkInput(gameMap->getAllTerritories().size());
                        cout << endl;
                        temp = new Bomb(gameMap->getAllTerritories().at(bombTerritory), player->getAllTerritories());
                    }
                    else
                    {
                        if (cardType == "Negotiate")
                        {
                            int playerCount = -1;
                            Player *enemyPlayer;
                            for (auto p : vectorOfplayerInGame)
                            {
                                playerCount++;
                                cout << playerCount << ". Player with ID: " << p->getPlayerId() << endl;
                            }
                            cout << "chose a player: ";
                            playerCount = checkInput(vectorOfplayerInGame.size());
                            enemyPlayer = new Player(*vectorOfplayerInGame.at(playerCount));
                            temp = new Negotiate(player, enemyPlayer);
                        }
                        else
                        {
                            cout << "Error, card type not found" << endl;
                            return;
                        }
                    }
                }
            }

            cout << endl
                 << "do you want to use another card(y/n)?";
            cin >> choice;
            player->getPlayerOrderList()->add_OrderToList(*temp);
        }
        else
        {
            cout << "Player " << player->getPlayerId() << " has no card on hand" << endl;
            okay = false;
        }
    }

    // Advace phase

    okay = true;
    cout << endl
         << "STARTING ADVANCE PHASE............" << endl;
    cout << "do you want to issue an advance order(y/n)?" << endl;
    cin >> choice;

    while (okay && choice == 'y' || okay && choice == 'Y')
    {
        int ito, ifrom, iamount, stationAmount;
        showList(toDefend(player));
        cout << endl << "choose a from: ";
        ifrom = checkInput(player->getAllTerritories().size());
        stationAmount = player->getAllTerritories().at(ifrom)->getArmiesAmount();
        if(stationAmount > 0)
        {
            showList(toAttack(player, ifrom));
            cout << endl<< "choose a to: ";
            ito = checkInput(player->getAllTerritories().at(ifrom)->getAllNeighbors().size());
            cout<<"Available armies to send: "<<stationAmount<<endl;
            cout << endl << "Enter an amount of army you wish to send: ";
            iamount = checkInput(stationAmount);

            Territory *souce = player->getAllTerritories().at(ifrom);
            Territory *target = player->getAllTerritories().at(ifrom)->getAllNeighbors().at(ito);

            temp = new Advance(souce, target, iamount, player->getAllTerritories(), player, deck);

            player->getPlayerOrderList()->add_OrderToList(*temp);
        }
        else
        {
            cout<<"selected territory doesnt have any army to advance"<<endl;
        }

        okay = false;

        cout << "do you want to issue another advance(y/n)?";
        cin >> choice;
        for (auto p : player->getAllTerritories())
        {
            if (p->getArmiesAmount() > 0) //if at least there is one territory with with army remain on them
            {
                okay = true;
            }
        }
    }
}

int HumanPlayerStrategy::checkInput(int range)
{
    int input;
    cin >> input;
    while (true)
    {
        if (!cin)
        {
            cout << "wrong input type, try again;" << endl;
            cin.clear();
            cin.ignore(10000, '\n');
            cin >> input;
        }
        else
        {
            if (input <= range)
            {
                return input;
            }
            else
            {
                cout << "out of the range try again." << endl;
                cin >> input;
            }
        }
    }
}

void HumanPlayerStrategy::showList(vector<Territory*> display)
{
    int inum = 0;
    for(auto p : display)
    {
        cout<<inum<<". "<<p->getName()<<endl;
        inum++;
    }
}
// ------------------ AggressivePlayerStrategy -------------------------

AggressivePlayerStrategy::AggressivePlayerStrategy()
{
    myOrderedTerr;
}

AggressivePlayerStrategy::AggressivePlayerStrategy(const AggressivePlayerStrategy &temp)
{
     int index = 0;
    for (auto p : myOrderedTerr)
    {
        this->myOrderedTerr[index] = new Territory(*(temp.myOrderedTerr[index]));
        index++;
    }
}

AggressivePlayerStrategy::~AggressivePlayerStrategy()
{
    for (auto p : myOrderedTerr)
    {
        delete p;
        p = nullptr;
    }
    myOrderedTerr.clear();
}

AggressivePlayerStrategy &AggressivePlayerStrategy::operator=(const AggressivePlayerStrategy&temp)
{
    int index = 0;
    for (auto p : myOrderedTerr)
    {
        this->myOrderedTerr[index] = new Territory(*(temp.myOrderedTerr[index]));
        index++;
    }
    return *this;
}

ostream &operator<<(ostream &out, const AggressivePlayerStrategy &copy)
{
    out << "Agressive Strategy" << endl;
    return out;
}

vector<Territory*> AggressivePlayerStrategy::toAttack(Player *player, int from)
{
    vector<Territory *> territoryToAttack;
    for(auto p : player->getAllTerritories().at(from)->getAllNeighbors())
    {
        if(p->getOwnerId() != player->getPlayerId())
        {
            territoryToAttack.push_back(p);
        }
    }
    return territoryToAttack;
}

vector<Territory*> AggressivePlayerStrategy::toDefend(Player *player)
{
    myOrderedTerr = player->getAllTerritories();
    for (int index = 0; index < myOrderedTerr.size() - 1; index++)
    {
        for (int nextIndex = index + 1; nextIndex < myOrderedTerr.size(); nextIndex++)
        {
            if (myOrderedTerr.at(index)->getArmiesAmount() < myOrderedTerr.at(nextIndex)->getArmiesAmount())
            {
                Territory *temp = myOrderedTerr.at(index);
                myOrderedTerr.at(index) = myOrderedTerr.at(nextIndex);
                myOrderedTerr.at(nextIndex) = temp;
            }
        }
    }
    return myOrderedTerr;
}

void AggressivePlayerStrategy::issueOrder(Player *player, Map *gameMap, vector <Player*> vectorOfplayerInGame, Deck* deck)
{
    cout<<endl<<"Aggressive Strategy Player"<<endl<<endl;
    Orders *temp;
    cout << "STARTING DEPLOY PHASE...." << endl;
    cout << "the player: " << player->getPlayerId() << " has " << player->getReinformentPool() << " army in the reinforcement Pool" << endl;
    while (player->getReinformentPool() > 0)
    {
        int amount = rand() % player->getReinformentPool();
        if(amount == 0 )
        {
            amount = player->getReinformentPool();
        }
        cout<<"amount to deploy: "<<amount<<endl;
        Orders *temp = new Deploy(toDefend(player).front(), amount, player->getAllTerritories());
        player->getPlayerOrderList()->add_OrderToList(*temp);
        player->subReinforment(amount);
    }
    cout << "player " << player->getPlayerId() << " finish deploying" << endl;

    if (player->getPlayerHand().getAllCards().size() > 0)
    {
        cout << "STARTING Card Usage PHASE...." << endl;
        player->getCardUsed().push_back(new Cards(player->getPlayerHand().playCard())); // take first card in player hands

        string cardType = player->getCardUsed().back()->getCardType();
        Orders *temp;
        if (cardType == "Airlift")
        {
            Player *enemy;
            vector<Territory *> territoryToAttack;
            vector<Territory *> territoryToDefend;

            int sourceTerritory;
            int amount = 0;
            int attackTerritory = 0;
            int attackTerritorySize = 0;
            do
            {
                territoryToAttack = gameMap->getAllTerritories();
                attackTerritorySize = territoryToAttack.size();
                attackTerritory = rand() % attackTerritorySize;

            } while (territoryToAttack.at(attackTerritory)->getOwnerId() == player->getPlayerId());

            territoryToDefend = toDefend(player);

            sourceTerritory = rand() % territoryToDefend.size();
            amount = rand() % territoryToDefend.at(sourceTerritory)->getArmiesAmount();

            temp = new Airlift(territoryToAttack.at(attackTerritory), territoryToDefend.at(sourceTerritory), player->getAllTerritories(), amount);
            player->getPlayerOrderList()->add_OrderToList(*temp);
        }
        else
        {
            if (cardType == "Blockade")
            {
                vector<Territory *> playerTerritory = player->getAllTerritories();
                int randomTerritory = playerTerritory.size();
                temp = new Blockade(playerTerritory.at(rand() % randomTerritory), playerTerritory);
            }
            else
            {
                if (cardType == "Bomb")
                {
                    Territory *enemyTerritory;
                    for (auto p : vectorOfplayerInGame)
                    {
                        if (p->getPlayerId() != player->getPlayerId())
                        {
                            int inum = p->getAllTerritories().size();
                            enemyTerritory = p->getAllTerritories().at(rand() % inum);
                        }
                    }
                    temp = new Bomb(enemyTerritory, player->getAllTerritories());
                }
                else
                {
                    if (cardType == "Negotiate")
                    {
                        Player *enemyPlayer;
                        for (auto p : vectorOfplayerInGame)
                        {
                            if (p->getPlayerId() != player->getPlayerId())
                            {
                                enemyPlayer = p;
                                break;
                            }
                        }
                        temp = new Negotiate(player, enemyPlayer);
                    }
                }
            }
            player->getPlayerOrderList()->add_OrderToList(*temp);
        }
    }
        // Advace phase
        cout<<endl<<"STARTING ADVANCE PHASE............"<<endl;
        vector<Territory *> territotyToDefend = toDefend(player);
        vector<Territory *> territotyToAttack = toAttack(player,0);
        if(territotyToAttack.size() != 0 )
        {
            //int attackTerritory =  rand() % territotyToAttack.size(); // for testing

            int amount = rand() % territotyToDefend.front()->getArmiesAmount();

            if(amount == 0)
            {
                amount = territotyToDefend.front()->getArmiesAmount();
            }
            temp = new Advance(territotyToDefend.at(0), territotyToAttack.at(0), amount, player->getAllTerritories(), player, deck);
            player->getPlayerOrderList()->add_OrderToList(*temp);
        }

        cout<<"Player finish Advancing"<<endl;

}



vector<Territory *> AggressivePlayerStrategy::getOrders()
{
    return myOrderedTerr;
}

BenevolentPlayerStrategy::BenevolentPlayerStrategy()
{
    myOrderedTerr;
}

BenevolentPlayerStrategy::BenevolentPlayerStrategy(const BenevolentPlayerStrategy &benevolent)
{
    int index = 0;
    for (auto p : myOrderedTerr)
    {
        this->myOrderedTerr[index] = new Territory(*(benevolent.myOrderedTerr[index]));
        index++;
    }
}

BenevolentPlayerStrategy::~BenevolentPlayerStrategy()
{
    for (auto p : myOrderedTerr)
    {
        delete p;
        p = nullptr;
    }
    myOrderedTerr.clear();
}

BenevolentPlayerStrategy &BenevolentPlayerStrategy::operator=(const BenevolentPlayerStrategy &benevolent)
{
    int index = 0;
    for (auto p : myOrderedTerr)
    {
        this->myOrderedTerr[index] = new Territory(*(benevolent.myOrderedTerr[index]));
        index++;
    }
    return *this;
}

ostream &operator<<(ostream &out, const BenevolentPlayerStrategy &copy)
{
    out << "Benevolent strategy";
    return out;
}

vector<Territory *> BenevolentPlayerStrategy::toAttack(Player *lplayer, int x)
{
    //never gets used by the BenevolentPlayerStrategy because in Assignment 3, this type of player does not attack any enemies
    cout << "Benevolent Player Strategy does not attack any territories. \n";

    return vector<Territory *>();
}

vector<Territory *> BenevolentPlayerStrategy::toDefend(Player *player)
{
    myOrderedTerr = player->getAllTerritories();
    //prioritizes the territories from least armies to most armies
    for (int index = 0; index < myOrderedTerr.size() - 1; index++)
    {
        for (int nextIndex = index + 1; nextIndex < myOrderedTerr.size(); nextIndex++)
        {
            if (myOrderedTerr.at(index)->getArmiesAmount() > myOrderedTerr.at(nextIndex)->getArmiesAmount())
            {
                Territory *temp = myOrderedTerr.at(index);
                myOrderedTerr.at(index) = myOrderedTerr.at(nextIndex);
                myOrderedTerr.at(nextIndex) = temp;
            }
        }
    }

    return myOrderedTerr;
}

void BenevolentPlayerStrategy::issueOrder(Player* player, Map* gameMap, vector<Player*> enemyPlayers, Deck* deck)
{
    cout<<endl<<"Benevolent Strategy IssueOrder"<<endl<<endl;
    //Deploy all the armies in the reinforcement pool to the weakest terrirtories
    cout << "STARTING DEPLOY PHASE...." << endl;
    cout << "The player: " << player->getPlayerId() << " has " << player->getReinformentPool() << " armies in the Reinforcement Pool." << endl;

    int index = 0;
    int numArmiesLeft = player->getReinformentPool();
    int deploy = 0;

    Orders* temp = nullptr;

    toDefend(player);

    if (player->getReinformentPool() != 0)
    {
        if (player->getAllTerritories().size() == 1)
        {
            temp = new Deploy(myOrderedTerr.at(0), numArmiesLeft, player->getAllTerritories());
            player->getPlayerOrderList()->add_OrderToList(*temp);
        }
        else // first two weakest countries get reinforcement armies
        {
            deploy = numArmiesLeft / 2;
            numArmiesLeft -= deploy;

            temp = new Deploy(myOrderedTerr.at(0), deploy, player->getAllTerritories());
            player->getPlayerOrderList()->add_OrderToList(*temp);

            temp = new Deploy(myOrderedTerr.at(1), numArmiesLeft, player->getAllTerritories());
            player->getPlayerOrderList()->add_OrderToList(*temp);
        }

        player->setReinformentPool(0);
    }
    else
        cout << "You have no armies in the reinforcement pool" << endl;

    // SINCE PLAYER CANT ADVANCE TO A ENEMY TERRIOTY THE PLAYER WILL NEVER GET CARDS THEREFORE CARD USAGE WAS NOT IMPLEMENTED
    cout<<"Advance Order"<<endl;
    // for advance order to a target territory owned by player
    for (auto p : myOrderedTerr)
    {
        for (auto lol : p->getAllNeighbors())
        {
            if (lol->getOwnerId() == p->getOwnerId())
            {
                temp = new Advance(p, lol, (p->getArmiesAmount() / 2), player->getAllTerritories(), player, deck);
                player->getPlayerOrderList()->add_OrderToList(*temp);
                return;
            }
        }
    }
}

vector<Territory *> BenevolentPlayerStrategy::getOrders()
{
    return myOrderedTerr;
}

//  NeutralPlayerStrategy

NeutralPlayerStrategy::NeutralPlayerStrategy()
{
}

NeutralPlayerStrategy::NeutralPlayerStrategy(const NeutralPlayerStrategy &neutral)
{
}

NeutralPlayerStrategy &NeutralPlayerStrategy::operator=(const NeutralPlayerStrategy &neutral)
{
    return *this;
}

NeutralPlayerStrategy::~NeutralPlayerStrategy()
{
}

vector<Territory*> NeutralPlayerStrategy::toAttack(Player *player, int x)
{
	return vector<Territory*>();
}

vector<Territory*> NeutralPlayerStrategy::toDefend(Player *player)
{
	return player->getAllTerritories();
}

void NeutralPlayerStrategy::issueOrder(Player *player, Map *gameMap, vector <Player*> vectorOfplayerInGame, Deck* deck)
{
     cout<<endl<<" Neutral Strategy IssueOrder"<<endl<<endl;
}




