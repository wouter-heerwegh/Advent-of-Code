#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <bitset>
#include <inttypes.h>

std::string globalMask = "";
std::unordered_map<unsigned long long, long long> memory;

typedef struct instruction{
	int type;
	unsigned long long location;
	std::string mask;
	std::bitset<36> val;
} Instruction;

std::vector<Instruction> readInput(std::string filename) {
    std::ifstream file(filename);
    std::vector<Instruction> instructions;
	std::string line = "";

    while (std::getline(file, line)) {
		char *buffer = (char *)malloc(200 * sizeof(char));
		unsigned long long location = 0;
		Instruction inst;
		if(line[1] == 'a'){
			inst.type = 1;
			std::sscanf(line.c_str(), "mask = %s", buffer);
			inst.mask = std::string(buffer);
		} else if(line[1] == 'e'){
			int val = 0;
			std::sscanf(line.c_str(), "mem[%llu] = %d", &location, &val);

			inst.type = 2;
			inst.location = location;
			inst.val = std::bitset<36>(val);
		}

		free(buffer);
        instructions.push_back(inst);
    }

    file.close();
    return instructions;
}

long long getValue(Instruction & inst){
	for(int i = 0; i < globalMask.size(); i++){
		if(globalMask[i] == '1'){
			inst.val.set(35-i);
		} else if(globalMask[i] == '0'){
			inst.val.reset(35-i);
		}
	}
	return inst.val.to_ullong();
}

long long calculateSum(std::vector<Instruction> instructions){
	for(Instruction & inst: instructions){
		if(inst.type == 1){
			globalMask = inst.mask;
		} else {
			if(memory.count(inst.location) >= 1){
				memory[inst.location] = getValue(inst);
			} else {
				memory.insert({inst.location, getValue(inst)});
			}
		}
	}

	long long sum = 0;
	for(auto & reg: memory){
		sum += reg.second;
	}

	return sum;
}

std::vector<std::bitset<36>> getAllLocations(Instruction & inst){
	std::vector<std::bitset<36>> bitsets;
	std::bitset<36> bits = std::bitset<36>();

	bitsets.push_back(bits);
	for(int i = 0; i < globalMask.size(); i++){
		if(globalMask[i] == '1'){
			for(auto & set: bitsets){
				set.set(35-i);
			}
		} else if(globalMask[i] == '0'){
			for(auto & set: bitsets){
				set.set(35-i, std::bitset<36>(inst.location)[35-i]);
			}
		} else if(globalMask[i] == 'X'){
			for(auto & set: bitsets){
				set.reset(35-i);
				std::bitset<36> setcopy = set;
				setcopy.set(35-i);
				bitsets.push_back(setcopy);
			}
		}
	}
	return bitsets;
}

unsigned long long calculateSum2(std::vector<Instruction> instructions){
	for(Instruction & inst: instructions){
		if(inst.type == 1){
			globalMask = inst.mask;
		} else {
			std::vector<std::bitset<36>> locations = getAllLocations(inst);

			for(auto & loc: locations){
				unsigned long long location = loc.to_ullong();

				if(memory.count(location) >= 1){
					memory[location] = inst.val.to_ullong();
				} else {
					memory.insert({location, inst.val.to_ullong()});
				}
			}
		}
	}

	unsigned long long sum = 0;
	for(auto & reg: memory){
		sum += reg.second;
	}

	return sum;
}


int main(int argc, char const *argv[])
{
	std::vector<Instruction> instructions = readInput("input.txt");
	unsigned long long result = calculateSum(instructions);

	std::cout << "Part 1: " << result << std::endl;

	memory.clear();
	globalMask = "";
	result = calculateSum2(instructions);

	std::cout << "Part 2: " << result << std::endl;
	return 0;
}
