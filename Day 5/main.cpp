#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

std::vector<std::string> readInput(std::string filename){
    std::ifstream file;
	file.open(filename);
    std::string line = "";
	std::vector<std::string> passes;
	while(std::getline(file, line)){
		passes.push_back(line);
    }

	file.close();

	return passes;
}

int calculateBinary(std::string & pass){
	unsigned int val = 0;
	for(char c: pass){
		val <<= 1;
		switch (c)
		{
		case 'F':
		case 'L':
			val |= 0;
			break;
		
		case 'B':
		case 'R':
			val |= 1;
			break;
		default:
			break;
		}
	}
	return val;
}

int calculateSeat(std::string pass){
	int row = 0;
	int col = 0;
	std::string rowString = pass.substr(0, 7);
	std::string colString = pass.substr(7, std::string::npos);
	row = calculateBinary(rowString);
	col = calculateBinary(colString);
	//std::cout << rowString << colString << ": row:" << row << ", col: " << col << " answer: " << row * 8 + col<< std::endl;
	return row * 8 + col;
}

int main(int argc, char const *argv[])
{
	std::vector<std::string> passes = readInput("input.txt");
	std::vector<int> seats;
	int maxSeat = 0;
	for(std::string pass: passes){
		int seatId = calculateSeat(pass);
		maxSeat = std::max(maxSeat, seatId);
		seats.push_back(seatId);
	}
	std::cout << "Part 1: " << maxSeat << std::endl;

	std::sort(seats.begin(), seats.end());
	
	std::cout << "Part 2: ";
	std::vector<int> possibleSeats;
	for(int i = 0; i + 1 < seats.size(); i++){
		if(seats[i] + 1 == seats[i + 1] - 1){
			std::cout << seats[i] + 1 << std::endl;
		}
	}

	return 0;
}