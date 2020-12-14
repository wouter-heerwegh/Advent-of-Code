#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <set>

typedef struct bag
{
    std::string color;
    std::unordered_map<std::string, int> canBeIn, canContain;
} Bag;

std::unordered_map<std::string, Bag> readInput(std::string filename)
{
    std::ifstream file;
    file.open(filename);
    std::string line = "";
    std::unordered_map<std::string, Bag> bags;

    int counter = 0;
    while (std::getline(file, line))
    {
        Bag bag;
        bag.color = line.substr(0, line.find(" bags contain"));

        if (bags.count(bag.color) == 1)
        {
            bag = bags[bag.color];
        }

        std::string contains = line.substr(line.find(" bags contain ") + 14);
        std::stringstream stream(contains);
        std::string baginfo;
        while (std::getline(stream, baginfo, ','))
        {
            if(baginfo.find("no other") != std::string::npos){
                break;
            }

            int amount = 0;
            char *buffer = (char *)malloc(200 * sizeof(char));
            std::sscanf(baginfo.c_str(), "%d %[^\n]s", &amount, buffer);
            std::string color(buffer);
            color = color.substr(0, color.find(" bag"));
            bag.canContain.insert(std::make_pair(color, amount));
            free(buffer);
        }

        bags.insert(std::make_pair(bag.color, bag));
    }

    file.close();

    for (auto &child : bags)
    {
        for (auto &parent : bags)
        {
            for (auto &contain : parent.second.canContain)
            {
                if (child.first == contain.first)
                {
                    child.second.canBeIn.insert(std::make_pair(parent.first, contain.second));
                }
            }
        }
    }
    return bags;
}

void calculateAllPossibleSolutions(std::string color, std::unordered_map<std::string, Bag> & bags, std::set<std::string> & uniqueColors){
    for(auto & possible: bags[color].canBeIn){
        uniqueColors.insert(possible.first);
        calculateAllPossibleSolutions(possible.first, bags, uniqueColors);
    }
}

int calculateAllPossibleSolutions2(std::string color, std::unordered_map<std::string, Bag> & bags){

    int nrOfBags = 0;
    for(auto & possible: bags[color].canContain){
        nrOfBags += possible.second * calculateAllPossibleSolutions2(possible.first, bags);
    }
    
    return ++nrOfBags;
}

int main(int argc, char const *argv[])
{
    std::unordered_map<std::string, Bag> bags = readInput("input.txt");

    std::cout << bags["shiny gold"].canBeIn.size() << ", " << bags["shiny gold"].canContain.size() << std::endl;
    std::set<std::string> uniqueColors;
    calculateAllPossibleSolutions("shiny gold", bags, uniqueColors);
    std::cout << "Part 1: " << uniqueColors.size() << std::endl;
    std::cout << "Part 2: " << calculateAllPossibleSolutions2("shiny gold", bags) - 1<< std::endl;
    return 0;
}
