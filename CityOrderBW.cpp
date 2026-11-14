// Benjamin Womack
// COSC 2436
// PROGRAM SET 3, PROBLEM 2
// CITYORDER

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <climits>
#include <cctype>

using namespace std;

//-------flight---------

struct Flight {
    int city;
    int distance;
};

vector<vector<Flight>> graph;
vector<bool> visited;

vector<int> currentRoute;
vector<int> bestRoute;

long long bestDistance;

//-------shortest route---------

void findRoute(int current, int count, long long distance) {

    if (count == graph.size()) {
        if (distance < bestDistance) {
            bestDistance = distance;
            bestRoute = currentRoute;
        }
        return;
    }

    if (distance >= bestDistance) {
        return;
    }

    for (Flight flight : graph[current]) {

        int next = flight.city;

        if (!visited[next]) {

            visited[next] = true;
            currentRoute.push_back(next);

            findRoute(next, count + 1, distance + flight.distance);

            currentRoute.pop_back();
            visited[next] = false;
        }
    }
}

//-------process data---------

bool processData(ifstream& input, int dataSet) {

    int numberOfCities;

    if (!(input >> numberOfCities) || numberOfCities < 2 || numberOfCities > 30) {
        cout << "Invalid number of cities or file format." << endl;
        return false;
    }

    vector<string> cities(numberOfCities);
    
    unordered_map<string, int> cityNumber;

    //---------numberOfCities---------

    for (int i = 0; i < numberOfCities; i++) {

        if (!(input >> cities[i])) {
            cout << "Error reading cities." << endl;
            return false;
        }

        cityNumber[cities[i]] = i;
    }

    //---------graph-----------

    graph.assign(numberOfCities, vector<Flight>());

    int numberOfFlights;

    if (!(input >> numberOfFlights) || numberOfFlights < 1 || numberOfFlights > 82) {
        cout << "Invalid number of flights." << endl;
        return false;
    }

    for (int i = 0; i < numberOfFlights; i++) {

        string city1, city2;
       
        int distance;

        if (!(input >> city1 >> city2 >> distance)) {
            cout << "Error reading flights." << endl;
            return false;
        }

        
        if (cityNumber.find(city1) == cityNumber.end() || cityNumber.find(city2) == cityNumber.end()) {
            cout << "Flight contains an unknown city." << endl;
            return false;
        }

        
        int first = cityNumber[city1];
        int second = cityNumber[city2];

        graph[first].push_back({second, distance});
        graph[second].push_back({first, distance});
    }

    //---------find route---------

    visited.assign(numberOfCities, false);
    currentRoute.clear();
    bestRoute.clear();
    bestDistance = LLONG_MAX;

    visited[0] = true;
    currentRoute.push_back(0);

    findRoute(0, 1, 0);

    //---------display route-------------

    if (bestRoute.empty()) {
        cout << "No valid route found." << endl;
        return false;
    }

    cout << "Route " << dataSet << ": ";

    for (int i = 0; i < bestRoute.size(); i++) {

        if (i > 0) {
            cout << " => ";
        }

        cout << cities[bestRoute[i]];
    }

    cout << endl;

    return true;
}

//-------------main-------------

int main() {

    char again = 'Y';

    while (toupper(static_cast<unsigned char>(again)) == 'Y') {

        string filename;

        cout << "Enter filename: ";
        cin >> filename;

        ifstream input(filename);

        if (!input) {
            cout << "Error opening file." << endl;
        }
        else {

            int numberOfDataSets;

            if (!(input >> numberOfDataSets) || numberOfDataSets < 1 || numberOfDataSets > 11) {
                cout << "Invalid data file." << endl;
            }
            else {

                for (int i = 0; i < numberOfDataSets; i++) {

                    if (!processData(input, i + 1)) {
                        break;
                    }
                }
            }

            input.close();
        }

        cout << "Run Again? (Y/N): ";
        cin >> again;
    }

    return 0;
}