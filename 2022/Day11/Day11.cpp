#include <iostream>
#include <fstream>

#include <string>
#include <set>
#include <vector>
#include <map>
#include <deque>
#include <algorithm>

static const char inputFileName[] = "input.txt";
//static const char inputFileName[] = "input2.txt";
//static const char inputFileName[] = "inputTest.txt";

static const std::string delimiter0 = " ";
static const std::string delimiterMonkey = "Monkey ";
static const std::string delimiterColon = ":";
static const std::string delimiterComma = ", ";
static const std::string delimiterItems = "Starting items: ";
static const std::string delimiterOperation = "Operation: new = old ";
static const std::string delimiterDivisible = "Test: divisible by ";
static const std::string delimiterTrueMonkey = "If true: throw to monkey ";
static const std::string delimiterFalseMonkey = "If false: throw to monkey ";

class Monkey
{
public:
    Monkey(int id, std::vector<unsigned long long> items, char operation, unsigned long long operationBy, unsigned long long divisibleBy, int onTrueMonkey, int onFalseMonkey)
        : id_(id)
        , items_(items)
        , operation_(operation)
        , operationBy_(operationBy)
        , divisibleBy_(divisibleBy)
        , onTrueMonkey_(onTrueMonkey)
        , onFalseMonkey_(onFalseMonkey)
        , inspections_(0)
    {}

    int id_;
    std::vector<unsigned long long> items_;
    char operation_;
    unsigned long long operationBy_;
    unsigned long long divisibleBy_;
    int onTrueMonkey_;
    int onFalseMonkey_;
    unsigned long long inspections_;
};

typedef std::vector<Monkey> Monkeys;

void readInputFile(std::string fileName, Monkeys& monkeys)
{
    std::string line;
    std::ifstream myFile(fileName);
    int id;
    std::vector<unsigned long long> items;
    char operation;
    unsigned long long operationBy;
    unsigned long long divisibleBy;
    int onTrueMonkey;
    int onFalseMonkey;

    if (myFile.is_open())
    {
        int state = 1;
        while (getline(myFile, line))
        {
            if(!line.empty())
            {
                switch (state)
                {
                    case 1:
                    {
                        size_t tokenPosition = line.find(delimiterMonkey, 0);
                        if (tokenPosition != std::string::npos)
                        {
                            line.erase(0, tokenPosition + delimiterMonkey.length());

                            size_t tokenPosition = line.find(delimiterColon, 0);
                            if (tokenPosition != std::string::npos)
                            {
                                id = std::stoi(line.substr(0, tokenPosition));
                            }
                        }
                        state++;
                        break;
                    }
                    case 2:
                    {
                        size_t tokenPosition = line.find(delimiterItems, 0);
                        if (tokenPosition != std::string::npos)
                        {
                            items.clear();
                            line.erase(0, tokenPosition + delimiterItems.length());
                            while (!line.empty())
                            {
                                tokenPosition = line.find(delimiterComma, 0);
                                if (tokenPosition == std::string::npos && !line.empty())
                                {
                                    tokenPosition = line.length();
                                }

                                if (tokenPosition != std::string::npos)
                                {
                                    std::string item = line.substr(0, tokenPosition);
                                    items.emplace_back(std::stoi(item));
                                    line.erase(0, tokenPosition + delimiterComma.length());
                                }
                            }
                        }
                        state++;
                        break;
                    }
                    case 3:
                    {
                        size_t tokenPosition = line.find(delimiterOperation, 0);
                        if (tokenPosition != std::string::npos)
                        {
                            line.erase(0, tokenPosition + delimiterOperation.length());
                            operation = line[0];
                            line.erase(0, 2);

                            if (line == "old")
                            {
                                // special case!!!
                                operation = '^';
                                operationBy = 0;
                            }
                            else
                            {
                                operationBy = std::stoi(line);
                            }
                        }
                        state++;
                        break;
                    }
                    case 4:
                    {
                        size_t tokenPosition = line.find(delimiterDivisible, 0);
                        if (tokenPosition != std::string::npos)
                        {
                            line.erase(0, tokenPosition + delimiterDivisible.length());
                            divisibleBy = std::stoi(line);
                        }
                        state++;
                        break;
                    }
                    case 5:
                    {
                        size_t tokenPosition = line.find(delimiterTrueMonkey, 0);
                        if (tokenPosition != std::string::npos)
                        {
                            line.erase(0, tokenPosition + delimiterTrueMonkey.length());
                            onTrueMonkey = std::stoi(line);
                        }
                        state++;
                        break;
                    }
                    case 6:
                    {
                        size_t tokenPosition = line.find(delimiterFalseMonkey, 0);
                        if (tokenPosition != std::string::npos)
                        {
                            line.erase(0, tokenPosition + delimiterFalseMonkey.length());
                            onFalseMonkey = std::stoi(line);
                        }
                        monkeys.emplace_back(id, items, operation, operationBy, divisibleBy, onTrueMonkey, onFalseMonkey);
                        state = 1;
                        break;
                    }
                }
            }
        }
        myFile.close();
    }
}

void turn(Monkeys& monkeys, bool isPart2, unsigned long long commonModulo)
{
    for(auto& monkey: monkeys)
    {
        //monkey.inspections_ += monkey.items_.size();
        if (!monkey.items_.empty())
        {
            //monkey.inspections_ += monkey.items_.size();
            for(auto it = monkey.items_.begin(); it != monkey.items_.end(); it++)
//            auto it = monkey.items_.begin();
//            while(it != monkey.items_.end())
            {
                monkey.inspections_++;
                unsigned long long newItem = 0;
                switch(monkey.operation_)
                {
                    case '*':
                    {
                        newItem = (*it) * monkey.operationBy_;
                        break;
                    }
                    case '+':
                    {
                        newItem = (*it) + monkey.operationBy_;
                        break;
                    }
                    case '^':
                    {
                        newItem = (*it) * (*it);
                        break;
                    }
                }
                if (isPart2)
                {
                    newItem = newItem % commonModulo;
                }
                else
                {
                    newItem = newItem / ((unsigned long long) 3);
                }
                if (newItem % monkey.divisibleBy_ == 0)
                {
                    monkeys[monkey.onTrueMonkey_].items_.emplace_back(newItem);
                }
                else
                {
                    monkeys[monkey.onFalseMonkey_].items_.emplace_back(newItem);
                }
                //it = monkey.items_.erase(it);
                //it++;
            }
            monkey.items_.clear();
        }
    }
}

int main()
{
    std::cout << "Parsing input file..." << std::endl;
    Monkeys monkeys1;
    Monkeys monkeys2;

    readInputFile(inputFileName, monkeys1);
    readInputFile(inputFileName, monkeys2);

    // Calculate the common modulo
    unsigned long long commonModulo = 1;
    for(int i = 0; i < monkeys1.size(); i++)
    {
        commonModulo = commonModulo * monkeys1[i].divisibleBy_;
    }

    if (monkeys1.empty() || monkeys2.empty())
    {
        std::cout << "Failed to read and parse input data!" << std::endl;
    }
    else
    {
        std::cout << "Solving problem..." << std::endl;
        std::cout << "Part 1:" << std::endl;
        {
            for(int i = 0; i < 20; i++)
            {
                turn(monkeys1, false, commonModulo);
            }

            std::vector<unsigned long long> inspections;
            for (int i = 0; i < monkeys1.size(); i++)
            {
                std::cout << "Monkey " << i << " : " << monkeys1[i].inspections_ << std::endl;
                inspections.emplace_back(monkeys1[i].inspections_);
            }
            std::sort(inspections.begin(), inspections.end());
            unsigned long long result = inspections[inspections.size() - 1] * inspections[inspections.size() - 2];
            std::cout << "Monkey business: " << result;

            std::cout << std::endl;
        }

        std::cout << "Part 2:" << std::endl;
        {
            for(int i = 0; i < 10000; i++)
            {
                turn(monkeys2, true, commonModulo);
            }

            std::vector<unsigned long long> inspections;
            for(int i = 0; i < monkeys2.size(); i++)
            {
                std::cout << "Monkey " << i << " : " << monkeys2[i].inspections_ <<std::endl;
                inspections.emplace_back(monkeys2[i].inspections_);
            }
            std::sort(inspections.begin(), inspections.end());
            unsigned long long result = inspections[inspections.size() - 1] * inspections[inspections.size() - 2];
            std::cout << "Monkey business: " << result;
        }

        std::cout << std::endl;

    }
}
