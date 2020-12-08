#include <iostream>
#include <fstream>
#include <cstdio>
#include <string>
#include <vector>

class Password
{
public:
    Password();
    ~Password();

	void checkPass();
	void checkPass2();

    int min;
    int max;
    char character;
    std::string password;
	bool valid;
};

Password::Password(): min(0), max(0), character(' '), password(""), valid(false)
{
}

Password::~Password()
{
}

void Password::checkPass(){
	int charCounter = 0;
	for(char c: password){
		if(c == character){
			charCounter++;
		}
	}

	if(charCounter >= min && charCounter <= max){
		valid = true;
	}
}

void Password::checkPass2(){
	if(password[min - 1] != password[max - 1] && (password[min - 1] == character || password[max - 1] == character)){
		valid = true;
	}
}



std::vector<Password> readInput(std::string filename){
	std::cout << "started" << std::endl;
    std::ifstream file;
	file.open(filename);
    std::string line = "";
    std::vector<Password> passwords = std::vector<Password>();

	while(std::getline(file, line)){
		Password pass;

		char * buffer = (char *) malloc(200 * sizeof(char));
        std::sscanf(line.c_str(), "%d-%d %c: %s", &pass.min, &pass.max, &pass.character, buffer);
		pass.password = buffer;
		passwords.push_back(pass);
		free(buffer);
    }

	file.close();

	return passwords;
}

int main(int argc, char const *argv[])
{
    std::vector<Password> passwords = readInput("input.txt");
	std::cout << "Finished reading..." << std::endl;

	int validCounter = 0;
	for(Password pass: passwords){
		pass.checkPass();
		if(pass.valid){
			validCounter++;
		}
	}


	std::cout << "Valid passwords: " << validCounter << std::endl;

	validCounter = 0;
	for(Password pass: passwords){
		pass.checkPass2();
		if(pass.valid){
			validCounter++;
		}
	}

	std::cout << "Valid passwords(Part 2): " << validCounter << std::endl;


    return 0;
}
