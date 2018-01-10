#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <set>
#include <iterator>
#include <string>
#include <stdio.h>
#include <map>
#include <queue>

using namespace std;

struct coin {
    int coinId;

    coin(int _coinId) : coinId(_coinId) {}
};

struct traveller1 {
    int nowResiding;
    string history = "";
    double distance = 0;
    set<int> coinIds;
    vector<int> coins;

    traveller1();

    traveller1(int _nowResiding, string townId) : nowResiding(_nowResiding) {
        coinIds.insert(0);
        coins.push_back(0);
        history += townId;
    }

    traveller1(const traveller1 &oldTraveller) {
        this->nowResiding = oldTraveller.nowResiding;
        this->distance = oldTraveller.distance;
        this->history = oldTraveller.history;
        this->coinIds = oldTraveller.coinIds;
        this->coins = oldTraveller.coins;


    }
};

struct thief {
    int thiefId;

    thief(int _thiefId) : thiefId(_thiefId) {}
};

struct jeweler {
    int townId;
    vector<coin> coins;

    jeweler(int _townId) : townId(_townId) {}

};

struct road {
    int fromTown;
    int toTown;
    int length;
    vector<thief> thiefs;
    //road(int _fromTown, int _toTown, int _length): fromTown(_fromTown), toTown(_toTown), length(_length) {}
};

struct town {
    map<int, int> shortestDistance;
    map<int, string> shortestPath;
    vector<road *> roads;
    vector<jeweler> jewelers;
};

void print(town *towns, int _numberOfTowns, road *roads, int _numberOfRoads) {

    for (int i = 1; i <= _numberOfTowns; i++) {

        cout << i << ": ";

        for (int j = 0; j < towns[i].jewelers.size(); j++) {

            cout << j << "- ";

            for (int k = 0; k < towns[i].jewelers[j].coins.size(); k++) {

                cout << towns[i].jewelers[j].coins[k].coinId << " ";

            }

            cout << "/ ";
        }

        cout << endl;
    }

    cout << endl;

    for (int i = 0; i < _numberOfRoads; i++) {

        cout << i << ": " << roads[i].fromTown << " " << roads[i].toTown << " " << roads[i].length << " / ";

        for (int j = 0; j < roads[i].thiefs.size(); j++) {

            cout << roads[i].thiefs[j].thiefId << " - ";

        }

        cout << endl;

    }

}

template<class Container>
void _split(const string &str, Container &cont) {
    istringstream iss(str);
    copy(istream_iterator<string>(iss),
         istream_iterator<string>(),
         back_inserter(cont));
}


int main(int argc, char *argv[]) {

    if (argc != 3) {
        cout << "Run the code with the following command: ./project1 [input_file] [output_file]" << endl;
        return 1;
    }
    /*
    cout << "input file: " << argv[1] << endl;
    cout << "output file: " << argv[2] << endl;
    */
    ifstream infile(argv[1]);
    string line;

    vector<string> inputs;

    getline(infile, line);

    _split(line, inputs);

    int numberOfTowns = stoi(inputs[0]);
    int numberOfRoads = stoi(inputs[1]);
    int numberOfThiefGroups = stoi(inputs[2]);
    int numberOfJewelers = stoi(inputs[3]);
    /*
    cout << endl;

    cout << "numberOfTowns: " << numberOfTowns << endl;
    cout << "numberOfRoads: " << numberOfRoads << endl;
    cout << "numberOfThiefGroups: " << numberOfThiefGroups << endl;
    cout << "numberOfJewelers: " << numberOfJewelers << endl;

    cout << endl;
    */
    town *towns = new town[numberOfTowns + 1];

    for (int i = 0; i < numberOfJewelers; i++) {

        inputs.clear();
        getline(infile, line);
        _split(line, inputs);

        int townId = stoi(inputs[0]);
        int numberOfCoins = stoi(inputs[1]);

        jeweler newJeweler(townId);

        for (int j = 2; j < numberOfCoins + 2; j++) {

            int coinId = stoi(inputs[j]);

            coin newCoin(coinId);

            newJeweler.coins.push_back(newCoin);

        }

        towns[townId].jewelers.push_back(newJeweler);
    }

    road *roads = new road[numberOfRoads];

    for (int i = 0; i < numberOfRoads; i++) {

        inputs.clear();
        getline(infile, line);
        _split(line, inputs);

        int fromTown = stoi(inputs[0]);
        int toTown = stoi(inputs[1]);
        int length = stoi(inputs[2]);
        int numberOfThieves = stoi(inputs[3]);

        road *newRoad = new road;

        road *newRoadR = new road;

        newRoad->fromTown = fromTown;
        newRoad->toTown = toTown;
        newRoad->length = length;

        newRoadR->fromTown = toTown;
        newRoadR->toTown = fromTown;
        newRoadR->length = length;

        /*
        roads[i].fromTown = fromTown;
        roads[i].toTown = toTown;
        roads[i].length = length;
        */

        for (int j = 4; j < numberOfThieves + 4; j++) {

            int thiefId = stoi(inputs[j]);

            thief newThief(thiefId);

            thief newThiefR(thiefId);

            newRoad->thiefs.push_back(newThief);

            newRoadR->thiefs.push_back(newThiefR);

        }

        //towns[fromTown].roads.push_back(&roads[i]);

        towns[fromTown].roads.push_back(newRoad);
        towns[toTown].roads.push_back(newRoadR);


    }

    //print(towns, numberOfTowns, roads, numberOfRoads);

    //delete [] towns;
    //delete [] roads;

    infile.close();
    //create the queue
    queue<traveller1> travellers;
    // create ali baba
    traveller1 firstTraveller(1, to_string(1));
    // take coins from the first town
    for (int i = 0; towns[1].jewelers.size(); i++) {

        for (int j = 0; towns[1].jewelers[i].coins.size(); j++) {

            firstTraveller.coinIds.insert(towns[1].jewelers[i].coins[j].coinId);
        }

    }
    // copy coin set into coin vector
    copy(firstTraveller.coinIds.begin(), firstTraveller.coinIds.end(), back_inserter(firstTraveller.coins));
    //put Ali Baba into queue
    travellers.push(firstTraveller);
    // Update first town's information
    towns[1].shortestDistance[0] = 0;
    towns[1].shortestPath[0] = "1 ";

    //cout << "Target City:" << numberOfTowns << endl;

    while (!travellers.empty()) {

        traveller1 _traveller = travellers.front(); // Get the traveler
        travellers.pop(); // Remove the traveler from the queue

        //cout << "Traveller: " << _traveller.history << "-" << _traveller.distance << endl;

        int townId = _traveller.nowResiding; // Get Town Id which traveler is residing at the moment

        //Check for each town that is reachable from the current town
        for (int i = 0; i < towns[townId].roads.size(); i++) {

            traveller1 traveller(_traveller);

            int nextTownId = towns[townId].roads[i]->toTown; // Get the next town Id

            bool canContinue = true; // A variable to hold a path is okay to pass

            //Check is there any thief on the road
            if (!towns[townId].roads[i]->thiefs.empty()) {

                //Check for different thief types and whether we have the relevant coin
                for (int j = 0; j < towns[townId].roads[i]->thiefs.size(); j++) {

                    // Check whether we have at least one coin to use against the thief
                    if (!traveller.coinIds.count(towns[townId].roads[i]->thiefs[j].thiefId)) {
                        canContinue = false;
                    }
                }
            }

            if (!canContinue) {
                continue;
            }

            // Traveler can pass the way without facing security issues

            bool deserveExisting = false;
            //Check for every type of coins whether it provides the shortest path
            for (int k = 0; k < traveller.coins.size(); k++) {
                // if no one visit here before
                if (!towns[nextTownId].shortestDistance.count(traveller.coins[k])) {

                    towns[nextTownId].shortestDistance[traveller.coins[k]] =
                            traveller.distance + towns[townId].roads[i]->length;

                    towns[nextTownId].shortestPath[traveller.coins[k]] =
                            traveller.history + " " + to_string(nextTownId);

                    deserveExisting = true;

                } else {
                    // if it was visited but traveller has smaller distance
                    if ((traveller.distance + towns[townId].roads[i]->length) < towns[nextTownId].shortestDistance[traveller.coins[k]]) {

                        towns[nextTownId].shortestDistance[traveller.coins[k]] =
                                traveller.distance + towns[townId].roads[i]->length;

                        towns[nextTownId].shortestPath[traveller.coins[k]] =
                                traveller.history + " " + to_string(nextTownId);

                        deserveExisting = true;

                    }

                }

            }
            //If traveller deserve existing, update its info and put him in the queue
            if (deserveExisting) {

                traveller.nowResiding = nextTownId; // Update residing town as the next town

                traveller.history = traveller.history + " " + to_string(nextTownId); // Updata town history

                traveller.distance = traveller.distance + towns[townId].roads[i]->length; // Update distance

                //Loop through for the jeweleries in the next town, make sure traveller takes all of those coins
                for (int l = 0; l < towns[nextTownId].jewelers.size(); l++) {

                    for (int m = 0; m < towns[nextTownId].jewelers[l].coins.size(); m++) {
                        //put all coins in a set  to prevent getting duplicate coins
                        traveller.coinIds.insert(towns[nextTownId].jewelers[l].coins[m].coinId);

                    }
                    // clear coins vector to copy from the new set
                    traveller.coins.clear();

                    /*
                    set<int>::iterator it;
                    for (it = traveller.coinIds.begin(); it != traveller.coinIds.end(); ++it)
                    {
                        traveller.coins.push_back(*it);
                    }
                     */
                    // copy set to vector
                    copy(traveller.coinIds.begin(), traveller.coinIds.end(), back_inserter(traveller.coins));

                }

                // put it in a queue
                travellers.push(traveller);

            }


        }


    }
    // Print it into the output file
    ofstream output(argv[2]);
    //Check for shortest path for all type of coins if it exists
    if (!towns[numberOfTowns].shortestPath.empty()) {

        map<int, int>::iterator it = towns[numberOfTowns].shortestDistance.begin();

        int minIndex = it->first;
        int minDistace = it->second;

        for (; it != towns[numberOfTowns].shortestDistance.end(); ++it) {
            if (it->second < minDistace) {
                minDistace = it->second;
                minIndex = it->first;
            }
        }
        cout << towns[numberOfTowns].shortestDistance[minIndex] << endl;
        cout << towns[numberOfTowns].shortestPath[minIndex] << endl;

        output << towns[numberOfTowns].shortestPath[minIndex];


        for (map<int, string>::iterator it = towns[numberOfTowns].shortestPath.begin();
             it != towns[numberOfTowns].shortestPath.end(); ++it) {
            cout << it->first << " - " << it->second << endl;
        }


    } else {
        output << -1;

        cout << -1 << endl;
    }

    output.close();

    delete[] towns;

    return 0;
}