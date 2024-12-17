#include <iostream>
#include <fstream>

#include <string>
#include <set>
#include <vector>

static const char inputFileName[] = "input.txt";

class Elf
{
public:
    Elf(std::vector<int> calories)
        : calories_(calories)
        , maxCalories_(0)
    {
        for (auto value : calories_)
        {
            maxCalories_ += value;
        }
    }

    std::vector<int> calories_;
    int maxCalories_;
};

struct elfComparator
{
    bool operator() (Elf lhs, Elf rhs) const { return lhs.maxCalories_ > rhs.maxCalories_; }
};

typedef std::set<Elf, elfComparator> Elves;

Elves readInputFile(std::string fileName)
{
    Elves data;
    std::string line;
    std::ifstream myFile(fileName);

    if (myFile.is_open())
    {
        std::vector<int> calories;
        while (getline(myFile, line))
        {
            if (line.empty())
            {
                // Add new elf's data
                data.emplace(calories);
                calories.clear();
            }
            else
            {
                calories.push_back(std::stoi(line));
            }
        }
        if (!calories.empty())
        {
            // Add last elf's data
            data.emplace(calories);
        }
        myFile.close();
    }

    return data;
}


int main()
{
    std::cout << "Parsing input file..." << std::endl;
    Elves data = readInputFile(inputFileName);

    if (data.empty())
    {
        std::cout << "Failed to read and parse input data!" << std::endl;
    }
    else
    {
        std::cout << "Solving problem..." << std::endl;
        std::cout << "Part 1:" << std::endl;
        std::cout << "Highest calories:" << data.begin()->maxCalories_ << std::endl;

        std::cout << std::endl;
        std::cout << "Part 2:" << std::endl;
        int sum = 0;
        std::cout << "Top calories:" << std::endl;;
        auto it = data.begin();
        sum += it->maxCalories_;
        std::cout << it->maxCalories_ << std::endl;

        it++;
        sum += it->maxCalories_;
        std::cout << it->maxCalories_ << std::endl;

        it++;
        sum += it->maxCalories_;
        std::cout << it->maxCalories_ << std::endl;

        std::cout << "Sum of highest 3 calories:  " << sum << std::endl;
    }
}
