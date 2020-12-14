#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

typedef struct seat{
	int surrSeat;
	bool isSeat, occupied, changed;
} Seat;

using Map = std::vector<std::vector<Seat>>;


Map readInput(std::string filename) {
    std::ifstream file(filename);
    Map map;
	std::string line = "";

    while (std::getline(file, line)) {
		std::vector<Seat> row;
		for(char c: line){
			if(c == '#'){
				row.push_back({0, true, true, false}); // occupied
			} else if(c == '.'){
				row.push_back({0, false, false, false}); // no seat
			} else {
				row.push_back({0, true, false, false}); // empty
			}
		}

        map.push_back(row);
    }

    file.close();
    return map;
}

int checkSurr(Map & map, int row, int col){
	int seatCounter = 0;
	for(int i = -1; i < 2; i++){
		for(int j = -1; j < 2; j++){
			//std::cout << "SURR: " << i << ", " << j << ", " << row + i << ", " << col + j << std::endl;
			if(i == 0 && j == 0 || (row + i < 0 || row + i >= map.size()) || (col + j < 0 || col + j >= map[row].size())){
				continue;
			} else if(map[row + i][col + j].isSeat && map[row + i][ col + j].occupied){
				seatCounter++;
			}
		}
	}
	return seatCounter;
}

int checkSurr2(Map & map, int row, int col){
	int seatCounter = 0;
	for(int i = -1; i < 2; i++){
		for(int j = -1; j < 2; j++){
			int currRow = row + i;
			int currCol = col + j;
			//std::cout << "SURR: " << i << ", " << j << ", " << currRow << ", " << currCol << std::endl;

			if(currRow == row && currCol == col || (currRow < 0 || currRow >= map.size()) || (currCol < 0 || currCol >= map[row].size())){
				continue;
			}

			bool seat = map[currRow][currCol].isSeat;
			while(1){
				if(currRow == row && currCol == col || (currRow < 0 || currRow >= map.size()) || (currCol < 0 || currCol >= map[row].size())){
					break;
				} else {
					if(map[currRow][currCol].isSeat){
						seat = map[currRow][currCol].occupied;
						break;
					}
					currRow += i;
					currCol += j;
				}
			}

			if(seat){
				seatCounter++;
			}
		}
	}
	return seatCounter;
}

bool updateSeatCounts(Map & map, bool part1){
	Map nextMap = map;
	bool changed = false;
	for(int i = 0; i < map.size(); i++){
		for(int j = 0; j < map[i].size(); j++){
			//std::cout << i << ", " << j << std::endl;
			if(part1){
				map[i][j].surrSeat = checkSurr(nextMap, i, j);
			} else {
				map[i][j].surrSeat = checkSurr2(nextMap, i, j);
			}
			if(map[i][j].changed){
				changed = true;
				map[i][j].changed = false;
			}
		}
	}

	return changed;
}

void printMap(Map & map) {
	for(std::vector<Seat> & row: map){
		for(Seat & seat: row){
			if(seat.isSeat && !seat.occupied){
				std::cout << "L";
			} else if(seat.isSeat && seat.occupied){
				std::cout << '#';
			} else {
				std::cout << ".";
			}
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

void applyRules(Map & map, int max){
	for(int i = 0; i < map.size(); i++){
		for(int j = 0; j < map[i].size(); j++){
			if(map[i][j].isSeat && !map[i][j].occupied && map[i][j].surrSeat == 0){
				map[i][j].occupied = true;
				map[i][j].changed = true;
			} else if(map[i][j].isSeat && map[i][j].occupied && map[i][j].surrSeat >= max){
				map[i][j].occupied = false;
				map[i][j].changed = true;
			}
		}
	}
}

int countSeats(Map & map){
	int counter = 0;
	for(std::vector<Seat> & row: map){
		for(Seat & seat: row){
			if(seat.isSeat && seat.occupied){
				counter++;
			}
		}
	}
	return counter;
}

void doIter(Map & map){
	Map initialmap = map;
	bool changed = true;
	int counter = 0;
	while(changed){
		applyRules(map, 4);

		bool result = updateSeatCounts(map, true);
		if(!result){
			changed = false;
		}

		counter++;
	}
	std::cout << "Iter: " << counter << std::endl;
	std::cout << "Part 1: " << countSeats(map) << std::endl;

	changed = true;
	
	map = initialmap;

	while(changed){
		applyRules(map, 5);
		bool result = updateSeatCounts(map, false);
		if(!result){
			changed = false;
		}
	}

	std::cout << "Part 2: " << countSeats(map) << std::endl;
}
int main(int argc, char const *argv[])
{
	Map map = readInput("input.txt");
	std::cout << "Read data" << std::endl;
	doIter(map);

	return 0;
}
