#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <tuple>
#include <sstream>
#include <regex>

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
			std::sscanf(line.c_str(), "%s %d-%d or %d-%d", buffer, &range1.first, &range1.second, &range2.first, &range2.second);

			Rule rule;
			rule.rule = std::string(buffer);
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

std::tuple<int, std::vector<Ticket>> solvePart1(auto & rules, auto & tickets){
	int sum = 0;
	std::vector<Ticket> goodTickets;
	for(Ticket &tick: tickets){
		bool goodTicket = false;
		for(int & val: tick.values){
			bool good = false;
			for(Rule & rule: rules){
				for(auto & pair: rule.data){
					//std::cout << val << ", " << pair.first << "-" << pair.second;
					if(val >= pair.first && val <= pair.second){
						good = true;
						goodTicket = true;
						//std::cout << ": GOOD";
					}
					//std::cout << std::endl;
				}
			}

			if(!good){
				sum += val;
			}
		}

		if(goodTicket){
			goodTickets.push_back(tick);
		}
		
	}
	return std::tuple(sum, goodTickets);
}

int main(int argc, char const *argv[])
{
	auto [rules, tickets] = readInput("input.txt");

	auto [result, tickets2] = solvePart1(rules, tickets);

	std::cout << result << std::endl;
	return 0;
}
