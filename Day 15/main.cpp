#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>


std::vector<int> input = {9,6,0,10,18,2,1};


int calculateNumber(int limit){
	int counter = 0;
	int val = -1;
	int prevVal = -1;
	std::unordered_map<int, int> lookup;
	
	while(1){
		if(prevVal > -1){
			lookup[prevVal] = counter;
		}

		prevVal = val;

		if(counter < input.size()){
			val = input[counter];
		} else {
			if(lookup.count(val) >= 1){
				val = (counter + 1) - lookup[val];
			} else {
				val = 0;
			}
		}
		counter++;
		if(counter == limit){
			break;
		}
	}

	return val;
}


int main(int argc, char const *argv[])
{
	int result = calculateNumber(2020);
	std::cout << "Part 1: " << result << std::endl;

	result = calculateNumber(30000000);
	std::cout << "Part 2: " << result << std::endl;
	return 0;
}
