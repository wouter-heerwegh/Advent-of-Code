#include <iostream>
#include <fstream>
#include <string>
#include <vector>

typedef struct slope{
    int row, col;
} Slope;

std::vector<std::vector<char>> readInput(std::string filename){
	std::cout << "started" << std::endl;
    std::ifstream file;
	file.open(filename);
    std::string line = "";
    
    std::vector<std::vector<char>> map;

    int lineCounter = 0;

	while(std::getline(file, line)){
        map.push_back(std::vector<char>());
		for(char c: line){
            map[lineCounter].push_back(c);
        }
        lineCounter++;
    }

	file.close();

	return map;
}

int checkMap(std::vector<std::vector<char>> & map, Slope slope){
    int line = 0;
    int col = 0;
    int treeCounter = 0;
    while(line < map.size()){
        if(map[line][col] == '#'){
            treeCounter++;
        }
        col += slope.col;
        col = col % map[0].size();
        line += slope.row;
    }

    return treeCounter;
}


int main(int argc, char const *argv[])
{
    std::vector<Slope> slopes = {{1,1},{1,5},{1,7},{2,1}};
    std::vector<std::vector<char>> map = readInput("input.txt");

    long trees = checkMap(map, {1,3});

    std::cout << trees << " number of trees" << std::endl;

    for(Slope slope: slopes){
        int result = checkMap(map, slope);

        std::cout << result << std::endl;
        trees *= result;
    }

    std::cout << trees << " number of trees (Part 2)" << std::endl;

    return 0;
}
