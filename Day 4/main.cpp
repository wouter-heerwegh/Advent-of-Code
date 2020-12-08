#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

typedef struct passport
{
	int byr, iyr, eyr, hgt;
	std::string ecl, hcl, pid, cid;
	int metric = -1;
} Passport;

std::string getText(std::string line, int index)
{
	std::string text = "";
	for (int i = index + 4; i < line.length(); i++)
	{
		if (line[i] == ' ')
		{
			break;
		}
		text += line[i];
	}

	return text;
}

void find(std::string field, std::string line, Passport &pass)
{
	int index = line.find(field);

	if (index == std::string::npos)
	{
		return;
	}

	if (field == "byr")
	{
		pass.byr = stoi(getText(line, index));
	}
	else if (field == "iyr")
	{
		pass.iyr = stoi(getText(line, index));
	}
	else if (field == "eyr")
	{
		pass.eyr = stoi(getText(line, index));
	}
	else if (field == "hgt")
	{
		std::string text = getText(line, index);
		if (text.substr(text.length() - 2, text.length() - 1) == "in")
		{
			pass.metric = 0;
		}
		else if(text.substr(text.length() - 2, text.length() - 1) == "cm")
		{
			pass.metric = 1;
		}
		pass.hgt = stoi(text);
	}
	else if (field == "ecl")
	{
		pass.ecl = getText(line, index);
	}
	else if (field == "hcl")
	{
		pass.hcl = getText(line, index);
	}
	else if (field == "pid")
	{
		pass.pid = getText(line, index);
	}
}

std::vector<Passport> readInput(std::string filename)
{

	std::ifstream file;
	file.open(filename);
	std::string line = "";
	std::vector<Passport> passports;

	std::vector<std::string> fields = {"byr", "iyr", "eyr", "hgt", "ecl", "hcl", "pid"};

	Passport pass;
	while (std::getline(file, line))
	{
		if (line == "" || line == " ")
		{
			passports.push_back(pass);
			pass = Passport();
			continue;
		}

		for (std::string field : fields)
		{
			find(field, line, pass);
		}
	}
	

	passports.push_back(pass);

	file.close();

	return passports;
}

int validatePassports(std::vector<Passport> &passports)
{
	int validCounter = 0;
	for (Passport pass : passports)
	{
		if (pass.byr > 0 && pass.iyr > 0 && pass.eyr > 0 && pass.hgt > 0 && pass.ecl != "" && pass.hcl != "" && pass.pid != "")
		{
			validCounter++;
		}
	}
	return validCounter;
}

int validatePassports2(std::vector<Passport> &passports)
{
	std::vector<std::string> eyecolors{"amb", "blu", "brn", "gry", "grn", "hzl", "oth"};
	std::vector<char> lookupSet{'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};
	std::vector<char> numbers{'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};

	int validCounter = 0;
	for (Passport & pass : passports)
	{
		if (pass.byr < 1920 || pass.byr > 2002)
		{
			continue;
		}

		if (pass.iyr < 2010 || pass.iyr > 2020)
		{			
			continue;
		}

		if (pass.eyr < 2020 || pass.eyr > 2030)
		{
			continue;
		}

		if ((pass.metric == 1 && (pass.hgt < 150 || pass.hgt > 193)) || (pass.metric == 0 && (pass.hgt < 59 || pass.hgt > 76)) || pass.metric == -1)
		{
			continue;
		}

		if(pass.hcl.length() != 7 || pass.hcl[0] != '#'){
			continue;
		}
		bool valid = true;
		for (int i = 1; i < pass.hcl.length(); i++)
		{
			if (std::find(lookupSet.begin(), lookupSet.end(), pass.hcl[i]) == lookupSet.end())
			{
				valid = false;
				break;
			}
		}

		if(!valid){
			continue;
		}

		if (std::find(eyecolors.begin(), eyecolors.end(), pass.ecl) == eyecolors.end())
		{
			continue;
		}

		if(pass.pid.length() != 9){
			continue;
		}

		for(int i = 0; i < pass.pid.length(); i++){
			if (std::find(numbers.begin(), numbers.end(), pass.pid[i]) == numbers.end())
			{
				valid = false;
				break;
			}
		}

		if (!valid)
		{
			continue;
		}

		validCounter++;
	}
	return validCounter;
}

int main(int argc, char const *argv[])
{
	std::vector<Passport> passports = readInput("input.txt");
	int result = validatePassports(passports);

	std::cout << "Valid passports: " << result << std::endl;

	result = validatePassports2(passports);

	std::cout << "Valid passports: " << result << std::endl;
	return 0;
}
