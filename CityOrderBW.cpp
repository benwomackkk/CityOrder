// Benjamin Womack
// COSC 2436
// PROGRAM SET 3, PROBLEM 2
// CITYORDER

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>
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

//-------proccess data---------

void processData(ifstream& input, int dataSet) {

    int numberOfCities;
    input >> numberOfCities;

    vector<string> cities(numberOfCities);
    unordered_map<string, int> cityNumber;

    for (int i = 0; i < numberOfCities; i++) {
        input >> cities[i];
        cityNumber[cities[i]] = i;
    }

    //---------graph-----------
    
    graph.assign(numberOfCities, vector<Flight>());

    int numberOfFlights;
    input >> numberOfFlights;

    for (int i = 0; i < numberOfFlights; i++) {
        string city1, city2;
        int distance;

        input >> city1 >> city2 >> distance;
        int first = cityNumber[city1];
        int second = cityNumber[city2];

        graph[first].push_back({second, distance});
        graph[second].push_back({first, distance});
    }
    
    visited.assign(numberOfCities, false);
    currentRoute.clear();
    bestRoute.clear();
    bestDistance = LLONG_MAX;

    visited[0] = true;
    currentRoute.push_back(0);
    
    findRoute(0, 1, 0);

    //---------display route-------------

    cout << "Route " << dataSet << ": ";

    for (int i = 0; i < bestRoute.size(); i++) {

        if (i > 0) {
            cout << " -> ";
        }
        cout << cities[bestRoute[i]];
    }
    cout << endl;

//-------------main-------------

int main() {

    char again = 'Y';

    while (toupper(again) == 'Y') {

        string filename;

        cout << "Enter filename: ";
        cin >> filename;

        ifstream input(filename);

        if (!input) {
            cout << "Error opening file." << endl;
        }
        else {

            int numberOfDataSets;
            input >> numberOfDataSets;

            for (int i = 0; i < numberOfDataSets; i++) {
                processData(input, i + 1);
            }
        
            input.close();
        }

        cout << "Run Again? (Y/N): ";
        cin >> again;
    }
    return 0;
}

    
}