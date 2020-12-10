#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <set>
#include <tuple>

typedef struct operation
{
    std::string instruction;
    int parameter;
    std::tuple<int, int> doOperation(int accumulator, int index){        
        if(instruction == "acc"){
            accumulator += parameter;
            index++;
        } else if(instruction == "jmp"){
            index += parameter;
        } else {
            index++;
        }

        return std::make_tuple(accumulator, index);
    }
} Operation;

std::vector<Operation> readInput(std::string filename)
{
    std::ifstream file;
    file.open(filename);
    std::string line = "";
    std::vector<Operation> operations;

    char *buffer = (char *)malloc(200 * sizeof(char));
    
    while (std::getline(file, line))
    {
        Operation op;
        std::sscanf(line.c_str(), "%s %d\n", buffer, &op.parameter);
        op.instruction = std::string(buffer);

        operations.push_back(op);
    }

    file.close();
    return operations;
}

std::tuple<bool, int> getValueAtLoop(std::vector<Operation> & operations){
    std::set<int> lookupSet;
    int accumulator = 0;
    int i = 0;
    bool loop = false;
    while(i < operations.size()){
        if(lookupSet.count(i) == 1){
            loop = true;
            break;
        }

        lookupSet.insert(i);
        Operation op = operations[i];
        std::tie(accumulator, i) = op.doOperation(accumulator, i);
    }

    return std::make_tuple(loop, accumulator);
}

std::vector<Operation> fixBootLoop(std::vector<Operation> operations, int index){
    if(operations[index].instruction == "jmp"){
        operations[index].instruction = "nop";
    } else if(operations[index].instruction == "nop"){
        operations[index].instruction = "jmp";
    }

    return operations;
}


int main(int argc, char const *argv[])
{
    std::vector<Operation> operations = readInput("input.txt");
    bool loop;
    int result;
    std::tie(loop, result) = getValueAtLoop(operations);
    std::cout << "Part 1: " << result << std::endl;

    std::vector<Operation> fixed = operations;
    int index = operations.size() - 1;
    while(loop && !(index < 0)){
        fixed = fixBootLoop(operations, index);
        std::tie(loop, result) = getValueAtLoop(fixed);
        if(loop){
            index--;
        }
    }

    if(loop){
        std::cout << "DIDN'T FIND SOLUTION" << std::endl;
    } else {
        std::cout << "Part 2: " << result << std::endl;

    }


    return 0;
}
