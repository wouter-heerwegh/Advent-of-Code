#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <math.h>

typedef struct boat
{
	boat() : N(0), E(0), heading(0.0f) {}
	int N;
	int E;
	float heading;
	friend std::ostream &operator<<(std::ostream &os, const boat &boat)
	{
		os << boat.N << ", " << boat.E << ", " << boat.heading;
		return os;
	}
} Boat;

typedef struct waypoint{
	waypoint() : N(1), E(10){}
	int N;
	int E;
	friend std::ostream &operator<<(std::ostream &os, const waypoint &wp)
	{
		os << wp.N << ", " << wp.E;
		return os;
	}
} Waypoint;

typedef struct direction
{
	char instruction;
	int amount;
} Direction;

std::vector<Direction> readInput(std::string filename)
{
	std::ifstream file(filename);
	std::vector<Direction> dirs;
	std::string line = "";

	while (std::getline(file, line))
	{
		Direction dir;
		std::sscanf(line.c_str(), "%c%d\n", &dir.instruction, &dir.amount);
		dirs.push_back(dir);
	}

	file.close();
	return dirs;
}

int getBoatPosition(std::vector<Direction> &dirs)
{
	Boat boat;
	for (Direction &dir : dirs)
	{
		switch (dir.instruction)
		{
		case 'F':
			boat.E += dir.amount * round(cos(boat.heading));
			boat.N += dir.amount * round(sin(boat.heading));
			break;
		case 'L':
			boat.heading += dir.amount * M_PI / 180;
			break;
		case 'R':
			boat.heading -= dir.amount * M_PI / 180;
			break;
		case 'N':
			boat.N += dir.amount;
			break;
		case 'E':
			boat.E += dir.amount;
			break;
		case 'S':
			boat.N -= dir.amount;
			break;
		case 'W':
			boat.E -= dir.amount;
			break;
		default:
			break;
		}
	}

	return abs(boat.N) + abs(boat.E);
}

Waypoint rotateWP(Waypoint wp, Boat & boat, Direction & dir){
	int x = wp.E;
	int y = wp.N;
	float theta = 0.0f;
	if(dir.instruction == 'R'){
		dir.amount *= -1;
	}

	theta += dir.amount * M_PI / 180;

	wp.E = x * round(cos(theta)) - y * round(sin(theta));
	wp.N = y * round(cos(theta)) + x * round(sin(theta));
	return wp;
}

int getBoatPosition2(std::vector<Direction> &dirs)
{
	Boat boat;
	Waypoint wp;
	for (Direction &dir : dirs)
	{
		switch (dir.instruction)
		{
		case 'F':
			boat.E += dir.amount * wp.E;
			boat.N += dir.amount * wp.N;
			break;
		case 'L':
		case 'R':
			wp = rotateWP(wp, boat, dir);
			break;
		case 'N':
			wp.N += dir.amount;
			break;
		case 'E':
			wp.E += dir.amount;
			break;
		case 'S':
			wp.N -= dir.amount;
			break;
		case 'W':
			wp.E -= dir.amount;
			break;
		default:
			break;
		}

		std::cout << dir.instruction << dir.amount << ", " << boat << ", " << wp << std::endl;
	}

	return abs(boat.N) + abs(boat.E);
}

int main(int argc, char const *argv[])
{
	std::vector<Direction> dirs = readInput("input.txt");

	int result = getBoatPosition(dirs);

	std::cout << "Part 1: " << result << std::endl;

	result = getBoatPosition2(dirs);
	std::cout << "Part 2: " << result << std::endl;
	return 0;
}
