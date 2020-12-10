#include <iostream>
#include <fstream>
#include <string>
#include <set>
#include <unordered_map>
#include <vector>

typedef struct person{
    std::set<char> answers;
} Person;

typedef struct group{
    std::vector<Person> persons;
    std::unordered_map<char,int> uniqueAnswers;
} Group;



std::vector<Group> readInput(std::string filename){
    std::ifstream file;
	file.open(filename);
    std::string line = "";
    std::vector<Group> groups;
	Group group;

    char c = ' ';
	while(std::getline(file, line)){
        Person person;
        if(line.size() == 0){
            if(group.uniqueAnswers.size() > 0){
                groups.push_back(group);
            }
            group = Group();

            continue;
        }

        for(char c : line){
            if(c != '\n' && c != ' '){
                if(group.uniqueAnswers.count(c) == 1){
                    group.uniqueAnswers[c]++;
                } else {
                    group.uniqueAnswers.insert({c,1});
                }
                person.answers.insert(c);
            }
        }

        group.persons.push_back(person);
    }

    groups.push_back(group);

	file.close();

	return groups;
}

int main(int argc, char const *argv[])
{
    std::vector<Group> answers = readInput("input.txt");
    long sum = 0;
    for(Group & group: answers){
        sum += group.uniqueAnswers.size();
    }

    std::cout << "Part 1: " << sum << std::endl;

    sum = 0;
    for(Group & group: answers){
        int unanimous = 0;
        for(std::pair<char,int> p: group.uniqueAnswers){
            if(p.second >= group.persons.size()){
                unanimous++;
            }
        }
        sum += unanimous;
    }

    std::cout << "Part 2: " << sum << std::endl;

    return 0;
}

