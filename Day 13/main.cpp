#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

int earliest = 0;
long long mult = 1;

std::vector<int> readInput(std::string filename)
{
	std::ifstream file(filename);
	std::vector<int> busIDs;
	std::string line = "";

	std::getline(file, line);
	earliest = stoi(line);
	std::getline(file, line);

	std::stringstream stream(line);
	std::string idstring;
	while(std::getline(stream, idstring, ',')){
		if(idstring == "x"){
			busIDs.push_back(-1);
			continue;
		}
		busIDs.push_back(stoi(idstring));
		mult *= stoi(idstring);
	}

	file.close();
	return busIDs;
}

int calculateTime(std::vector<int> & busIDs){
	int min = __INT_MAX__;
	int minId = -1;
	for(int & id: busIDs){
		if(id == -1){
			continue;
		}

		int calctime = id - earliest % id;
		min = std::min(min, calctime);
		//std::cout << id << ", " << calctime << ", " << earliest << ", " << earliest / id << ", " << earliest % id << ", " << min << std::endl;
		if(min == calctime){
			minId = id;
		}
	}

	return minId * min;
}

long long modInverse(long long a, long long m) 
{ 
    long long m0 = m; 
    long long y = 0, x = 1; 
  
    if (m == 1) 
        return 0; 
  
    while (a > 1) {
        long long q = a / m; 
        long long t = m; 

        m = a % m;
		a = t; 
        t = y; 
        y = x - q * y; 
        x = t; 
    } 

    if (x < 0) 
        x += m0; 
  
    return x; 
} 

long long calculateTime2(std::vector<int> & busIDs){
	long long time = 0;
	for(int i = 0; i < busIDs.size(); i++){
		if(busIDs[i] <= 0){
			continue;
		}
		long long m = mult/busIDs[i];
		long long mi = modInverse(m, busIDs[i]);
		time += m * mi * (busIDs[i] - i);
	}

	return time % mult;
}


int main(int argc, char const *argv[])
{
	std::vector<int> busIDs = readInput("input.txt");

	int result = calculateTime(busIDs);
	std:: cout << mult << std::endl;
	std::cout << "Part 1: " << result << std::endl;

	long long result2 = calculateTime2(busIDs);
	std::cout << "Part 2: " << result2 << std::endl;

	return 0;
}
