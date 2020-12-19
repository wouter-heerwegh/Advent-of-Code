#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <tuple>
#include <sstream>
#include <algorithm>

bool foundSolution = false;

typedef struct rule{
	std::string rule;
	std::map<int, int> data;
} Rule;

typedef struct ticket{
	std::vector<int> values;
} Ticket;

Ticket myTicket;

std::tuple<std::vector<Rule>, std::vector<Ticket>> readInput(std::string filename) {
    std::ifstream file(filename);
    std::vector<Rule> rules;
	std::vector<Ticket> tickets;
	std::string line = "";
	bool ruleLines = true;

    while (std::getline(file, line)) {
		if(line == ""){
			continue;
		}

		if(line.find("your ticket:") != std::string::npos){
			std::getline(file, line);
			std::string val = "";
			std::stringstream stream(line);
			while(std::getline(stream, val, ',')){
				myTicket.values.push_back(stoi(val));
			}
			ruleLines = false;
			continue;
		}

		if(ruleLines){
			char *buffer = (char *)malloc(200 * sizeof(char));
			std::pair<int,int> range1;
			std::pair<int,int> range2;
			std::sscanf(line.c_str(), "%[^:]: %d-%d or %d-%d", buffer, &range1.first, &range1.second, &range2.first, &range2.second);

			Rule rule;
			rule.rule = std::string(buffer);
			//std::cout << buffer << ": " << range1.first << "-" << range1.second << ", " << range2.first << '-' << range2.second << std::endl;
			rule.data.insert(range1);
			rule.data.insert(range2);

			rules.push_back(rule);
			free(buffer);

		} else {
			if(line.find("nearby tickets") != std::string::npos){
				continue;
			}

			Ticket tick;
			std::string val = "";
			std::stringstream stream(line);
			while(std::getline(stream, val, ',')){
				tick.values.push_back(stoi(val));
			}
			tickets.push_back(tick);
		}

    }

    file.close();
    return std::make_tuple(rules, tickets);
}

std::tuple<int, std::vector<Ticket>> solvePart1(std::vector<Rule> & rules, std::vector<Ticket> & tickets){
	int sum = 0;
	std::vector<Ticket> goodTickets;
	for(Ticket &tick: tickets){
		bool goodTicket = true;
		for(int & val: tick.values){
			bool good = false;
			for(Rule & rule: rules){
				for(auto & pair: rule.data){
					//std::cout << val << ", " << pair.first << "-" << pair.second;
					if(val >= pair.first && val <= pair.second){
						good = true;
						//std::cout << ": GOOD";
					}
					//std::cout << std::endl;
				}
			}

			if(!good){
				goodTicket = false;
				sum += val;
			}
		}

		if(goodTicket){
			goodTickets.push_back(tick);
		}
		
	}
	return std::tuple(sum, goodTickets);
}

int chooseFields(std::map<int, std::string> & lookup, std::vector<std::pair<std::string, std::map<int, int>>> & possibilities, int tickSize, int ruleSize){
	if(lookup.size() == ruleSize){
		foundSolution = true;
		std::cout << "Solution found!" << std::endl;
		return 0;
	}

	for(int i = 0; i < possibilities.size(); i++){
		for(auto & field: possibilities[i].second){
			//std::cout <<field.first << ", " << field.second << std::endl;
			if(field.second >= tickSize && !(lookup.count(field.first) >= 1)){
				lookup.insert(std::pair<int,std::string>(field.first, possibilities[i].first));
				//std::cout << "trying: " << field.first << ", " << pos.first << std::endl;
				auto copy(possibilities);
				copy.erase(copy.begin() + i);
				chooseFields(lookup, copy, tickSize, ruleSize);
				if(foundSolution){
					return 0;
				} else {
					lookup.erase(field.first);
				}
			}
		}
	}

	//std::cout << "No solution found" << std::endl;
}

long long solvePart2(std::vector<Rule> & rules, std::vector<Ticket> & tickets){
	std::map<std::string, std::map<int, int>> possibilities;

	for(Rule & rule: rules){
		for(Ticket & tick: tickets){
			for(int i = 0; i < tick.values.size(); i++){
				bool passed = false;
				for(auto & pair: rule.data){
					if(tick.values[i] >= pair.first && tick.values[i] <= pair.second){
						passed = true;
					}
				}

				//std::cout << tick.values[i] << ", " << passed << std::endl;
				if(passed){
					//std::cout << "Inserting: " << rule.rule << ": " << i << std::endl;
					if(possibilities.count(rule.rule) >= 1){
						if(possibilities[rule.rule].count(i) >= 1){
							possibilities[rule.rule][i]++;
						} else {
							std::map<int, int> newmap = possibilities[rule.rule];
							newmap.insert(std::pair<int,int>(i, 1));
							possibilities[rule.rule] = newmap;
						}
					} else {
						std::map<int,int> newmap;
						newmap.insert(std::pair<int,int>(i, 1));
						possibilities.insert(std::pair<std::string, std::map<int,int>>(rule.rule, newmap));
					}
				}
			}
		}
		
	}

	std::map<int,std::string> lookup;
	std::vector<std::pair<std::string, std::map<int,int>>> vec;
 
    std::copy(possibilities.begin(),
            possibilities.end(),
            std::back_inserter<std::vector<std::pair<std::string, std::map<int,int>>>>(vec));
 
	for(auto & pair: vec){
		std::map<int,int> newmap(pair.second);
		for(auto & pos: pair.second){
			if(pos.second < tickets.size()){
				newmap.erase(pos.first);
			}
		}
		pair.second = newmap;
		std::cout << pair.first << ", " << pair.second.size() << std::endl;
	}

    std::sort(vec.begin(), vec.end(),
            [](const std::pair<std::string, std::map<int,int>> & l, const std::pair<std::string, std::map<int,int>>& r) {
                return l.second.size() < r.second.size();
            });
	
	chooseFields(lookup, vec, tickets.size(), possibilities.size());
	long long result = 1; 
	for(auto & val: lookup){
		if(val.second.find("departure") != std::string::npos){
			result *= myTicket.values[val.first];
		}
		std::cout << val.second << ": " << myTicket.values[val.first] << std::endl;
	}
	return result;
 }

int main(int argc, char const *argv[])
{
	auto [rules, tickets] = readInput("input.txt");

	auto [result, tickets2] = solvePart1(rules, tickets);

	std::cout << "Part 1: " << result << std::endl;

	long long result2 = solvePart2(rules, tickets2);

	std::cout << "Part 2: " << result2 << std::endl; 
	return 0;
}
