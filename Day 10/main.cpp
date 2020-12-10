#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <tuple>
#include <vector>

int neededJolts;

std::vector<int> readInput(std::string filename) {
    std::ifstream file(filename);
    int jolt = 0;
    std::vector<int> adapters;

    while (file >> jolt) {
        adapters.push_back(jolt);
    }

    file.close();
    return adapters;
}

std::pair<bool, std::vector<int>> recurse(std::vector<int> &adapters, std::vector<int> needed, int index, int prevJolt) {
    if (prevJolt + 3 == neededJolts) {
        needed.push_back(neededJolts);
        return std::make_pair(true, needed);
    }

    for (int i = index; adapters[i] <= prevJolt + 3; i++) {
        if ((adapters[i] == prevJolt + 3 || adapters[i] == prevJolt + 1) && adapters[i] - prevJolt > 0) {
            std::vector<int> next = needed;
            next.push_back(adapters[i]);
            bool result = false;

            std::tie(result, next) = recurse(adapters, next, i + 1, adapters[i]);
            if (result) {
                return std::make_pair(true, next);
            }
        }
    }

    return std::make_pair(false, std::vector<int>());
}


long long calculateAllPaths(std::vector<int> &adapters) {
    std::map<int, long long> possiblePaths;
    possiblePaths.insert({0, 1});
    for (int i = 0; i < adapters.size(); i++) {
        possiblePaths.insert({adapters[i], 0});
        if (possiblePaths.count(adapters[i] - 1) == 1) {
            possiblePaths[adapters[i]] += possiblePaths[adapters[i] - 1];
        }
		
		if (possiblePaths.count(adapters[i] - 2) == 1) {
			possiblePaths[adapters[i]] += possiblePaths[adapters[i] - 2];
        }
		
		if (possiblePaths.count(adapters[i] - 3) == 1) {
			possiblePaths[adapters[i]] += possiblePaths[adapters[i] - 3];
        }
    }

    return possiblePaths[neededJolts];
}

int main(int argc, char const *argv[]) {
    std::vector<int> adapters = readInput("input.txt");
    sort(adapters.begin(), adapters.end());
    neededJolts = adapters.back() + 3;

    std::cout << "Needed Jolts: " << neededJolts << std::endl;

    std::vector<int> needed;
    needed.push_back(0);
    bool resultFound = false;
    std::tie(resultFound, needed) = recurse(adapters, needed, 0, 0);

    if (!resultFound) {
        std::cout << "Didn't find a solution" << std::endl;
    } else {
        sort(needed.begin(), needed.end());
        int threes = 0;
        int ones = 0;

        for (int i = 0; i < needed.size() - 1; i++) {
            if (needed[i] == needed[i + 1] - 1) {
                ones++;
            } else if (needed[i] == needed[i + 1] - 3) {
                threes++;
            }
        }

        std::cout << "Part 1: " << ones * threes << " Ones: " << ones << " Threes: " << threes << std::endl;
    }

    std::vector<int> allJolts = adapters;

    allJolts.push_back(neededJolts);

    sort(allJolts.begin(), allJolts.end());

    std::cout << "Part 2: " << calculateAllPaths(allJolts) << std::endl;
    return 0;
}
