#include <fstream>
#include <iostream>
#include <list>
#include <string>
#include <vector>

int preambleSize = 25;

std::vector<int> readInput(std::string filename) {
    std::ifstream file;
    file.open(filename);
    std::string line = "";
    std::vector<int> numbers;

    while (std::getline(file, line)) {
        int number = 0;
        std::sscanf(line.c_str(), "%d\n", &number);

        numbers.push_back(number);
    }

    file.close();
    return numbers;
}

int solvePart1(std::vector<int> &numbers) {
    for (int i = preambleSize; i < numbers.size(); i++) {
        bool valid = false;
        for (int j = i - preambleSize; j < i; j++) {
            for (int k = j + 1; k < i; k++) {
                if (numbers[j] + numbers[k] == numbers[i]) {
                    valid = true;
                }
            }
        }

        if (!valid) {
            return numbers[i];
        }
    }
}

std::list<int> findContiguousSet(std::vector<int> &numbers, int sum) {
    std::list<int> contiguousSet;
    int runningSum = 0;
    int index = 0;
    while (runningSum != sum) {
        if (index >= numbers.size()) {
            std::cout << "INDEX OUT OF BOUNDS" << std::endl;
            break;
        }

        if (runningSum > sum) {
            runningSum -= contiguousSet.front();
            contiguousSet.pop_front();
        } else if (runningSum < sum) {
            runningSum += numbers[index];
            contiguousSet.push_back(numbers[index]);
            index++;
        }

        if (runningSum == sum) {
            return contiguousSet;
        }
    }

    std::cout << "Didn't find solution" << std::endl;
    return contiguousSet;
}

int main(int argc, char const *argv[]) {
    std::vector<int> numbers = readInput("input.txt");

    int result = solvePart1(numbers);

    std::cout << "Part 1:" << result << std::endl;

    std::list<int> set = findContiguousSet(numbers, result);
    set.sort();
    result = set.front() + set.back();

    std::cout << "Part 2:" << result << std::endl;

    return 0;
}
