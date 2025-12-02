#include <iostream>
#include <iomanip>
#include <fstream>

#include <string>
#include <set>
#include <vector>
#include <map>
#include <set>
#include <deque>
#include <queue>
#include <algorithm>
#include <numeric>
#include <bitset>

static const char inputFileName[] = "input.txt";
//static const char inputFileName[] = "input_test.txt";
//static const char inputFileName[] = "input_test1.txt";
//static const char inputFileName[] = "input_test2.txt";


typedef std::vector<std::string> Map;
typedef std::string Moves;

typedef long long BigNumber;
typedef std::vector<BigNumber> BigNumbers;

typedef std::map<std::string, BigNumber> ValueMap;

enum Type
{
    INV = -1,
    CONST = 0,
    AND = 1,
    OR = 2,
    XOR = 3
};

class Component
{
public:
    Component()
        : type_(INV)
    { }

    Component(std::string name, Type type, std::string input1, std::string input2)
        : name_(name)
        , type_(type)
        , output_(-1)
        , input1_(input1)
        , input2_(input2)
    { }

    int output() { return output_; }
    std::string name() { return name_; }
    Type type() { return type_; }

    std::string name_;
    Type type_;
    std::string input1_;
    std::string input2_;
    int output_;
};


typedef std::map<std::string, Component> Components;


// Split a line by delimiter
std::vector<std::string> split(std::string line, std::vector<std::string> delimiters)
{
    std::vector<std::string> result;

    size_t tokenPosition = 0;
    if (delimiters.empty())
    {
        return result;
    }

    int delimIndex = 0;

    while (!line.empty())
    {
        std::string delimiter = delimiters[delimIndex];
        
        // remove any leading tokens that might be present
        tokenPosition = line.find(delimiter);
        while (tokenPosition == 0)
        {
            if (tokenPosition != std::string::npos)
            {
                line.erase(0, tokenPosition + delimiter.length());

                delimIndex = delimIndex < delimiters.size() - 1 ? delimIndex + 1 : delimIndex;
                delimiter = delimiters[delimIndex];
                tokenPosition = line.find(delimiter);
            }
        }

        if (tokenPosition != std::string::npos)
        {
            result.emplace_back(line.substr(0, tokenPosition));
            line.erase(0, tokenPosition);
        }
        else
        {
            // Must be last element in the line
            if (!line.empty())
            {
                result.emplace_back(line);
                line.clear();
            }
        }
    }

    return result;
}


void readInputFile(std::string fileName, Components& components)
{
    std::string line;
    std::ifstream myFile(fileName);

    if (myFile.is_open())
    {
        int state = 1;
        while (getline(myFile, line))
        {
            switch(state)
            {
                case 1:
                {
                    // Read constants
                    if (!line.empty())
                    {
                        auto elements = split(line, { ": " });
                        Component component(elements[0], CONST, "", "");
                        component.output_ = stoi(elements[1]);

                        components.emplace(component.name(), component);
                    }
                    else
                    {
                        state++;
                    }
                    break;
                }
                case 2:
                {
                    if (!line.empty())
                    {
                        auto elements = split(line, { " ", " ", " -> "});

                        Type type = INV;
                        if (elements[1] == "OR")
                        {
                            type = OR;
                        }
                        else if (elements[1] == "AND")
                        {
                            type = AND;
                        }
                        else if (elements[1] == "XOR")
                        {
                            type = XOR;
                        }

                        Component component(elements[3], type, elements[0], elements[2]);
                        components.emplace(component.name(), component);
                    }
                    break;
                }
            }
        }
        myFile.close();
    }
}

void iterate(Components& components)
{
    bool done = false;
    while (!done)
    {
        done = true;
        for (auto& component : components)
        {
            int inp1 = -1;
            int inp2 = -1;

            if (component.second.type() != CONST)
            {
                auto input1It = components.find(component.second.input1_);
                if (input1It != components.end())
                {
                    inp1 = input1It->second.output();
                }
                else
                {
                    // should never get here
                    done = done;
                }
                auto input2It = components.find(component.second.input2_);
                if (input2It != components.end())
                {
                    inp2 = input2It->second.output();
                }
                else
                {
                    // should never get here
                    done = done;
                }
            }

            switch (component.second.type())
            {
                case CONST:
                {
                    // Do nothing, output value is known of this type
                    break;
                }
                case AND:
                {
                    if (inp1 > -1 && inp2 > -1)
                    {
                        component.second.output_ = (inp1 & inp2) & 1;
                    }
                    else
                    {
                        done = false;
                    }
                    break;
                }
                case OR:
                {
                    if (inp1 > -1 && inp2 > -1)
                    {
                        component.second.output_ = (inp1 | inp2) & 1;
                    }
                    else
                    {
                        done = false;
                    }
                    break;
                }
                case XOR:
                {
                    if (inp1 > -1 && inp2 > -1)
                    {
                        component.second.output_ = (inp1 ^ inp2) & 1;
                    }
                    else
                    {
                        done = false;
                    }
                    break;
                }
                default:
                    break;
            }
        }
    }
}


unsigned long long calculate(Components &components, unsigned long long x, unsigned long long y)
{
    if(x != LLONG_MAX)
    {
        bool done = false;
        int i = 0;
        while (!done)
        {
            std::string key = std::to_string(i);
            if (key.size() == 1)
            {
                key = "0" + key;
            }
            key = "x" + key;
            auto compIt = components.find(key);
            if (compIt == components.end())
            {
                done = true;
            }
            else
            {
                compIt->second.output_ = (x >> i) & 0x1;
            }
            i++;
        }
    }

    if (y != LLONG_MAX)
    {
        bool done = false;
        int i = 0;
        while (!done)
        {
            std::string key = std::to_string(i);
            if (key.size() == 1)
            {
                key = "0" + key;
            }
            key = "y" + key;
            auto compIt = components.find(key);
            if (compIt == components.end())
            {
                done = true;
            }
            else
            {
                compIt->second.output_ = (y >> i) & 0x1;
            }
            i++;
        }
    }

    iterate(components);

    unsigned long long result = 0;
    bool done = false;
    int i = 0;
    while (!done)
    {
        std::string key = std::to_string(i);
        if (key.size() == 1)
        {
            key = "0" + key;
        }
        key = "z" + key;
        auto compIt = components.find(key);
        if (compIt == components.end())
        {
            done = true;
        }
        else
        {
            result += ((unsigned long long)compIt->second.output()) << i;
        }
        i++;
    }

    return result;
}


std::set<std::string> findDependents(Components& components, Component& component)
{
    std::set<std::string> dependents;

    if (component.type() != CONST)
    {
        dependents.emplace(component.input1_);
        dependents.emplace(component.input2_);
        dependents.merge(findDependents(components, components[component.input1_]));
        dependents.merge(findDependents(components, components[component.input2_]));
    }
    return dependents;
}


int main()
{
    std::cout << "Parsing input file..." << std::endl;

    Components components;

    readInputFile(inputFileName, components);

    if (components.empty())
    {
        std::cout << "Failed to read and parse input data!" << std::endl;
    }
    else
    {
        std::cout << "Solving problem..." << std::endl;
        std::cout << "Part 1:" << std::endl;
        { 
            BigNumber sum = 0;
            unsigned long long result = calculate(components, LLONG_MAX, LLONG_MAX);

            std::cout << "Total: " << result;
        }

        std::cout << std::endl;

        std::cout << "Part 2:" << std::endl;
        {
            BigNumber sum = 0;
            size_t bitErrorCount = 0;
            std::set<std::string> dependents;

            std::cout << "X = 0, Y = 0" << std::endl;
            {
                unsigned long long x = 0;
                unsigned long long y = 0;
                unsigned long long z = calculate(components, x, y);
                if (x + y != z)
                {
                    std::bitset<45> zBitset(z);
                    bitErrorCount += zBitset.count() == 0 ? 1 : zBitset.count();
                    std::cout << "XY0" << ":" << zBitset << std::endl;
                }
            }
            std::cout << std::endl;

            std::cout << "X = 1, Y = 0" << std::endl;
            for (int i = 0; i < 44; i++)
            {
                unsigned long long x = (unsigned long long) 1 << i;
                unsigned long long y = 0;
                unsigned long long z = calculate(components, x, y);
                if (x + y != z)
                {
                    std::bitset<45> zBitset(z);
                    bitErrorCount += zBitset.count() == 0 ? 1 : zBitset.count();
                    std::cout << "X" << std::setfill('0') << std::setw(2) << i << ":" << zBitset << std::endl;

                    std::string key = "z" + ((i < 10) ? std::string("0") : std::string("")) + std::to_string(i);
                    dependents.merge(findDependents(components, components[key]));
                }
            }
            std::cout << std::endl;

            std::cout << "X = 0, Y = 1" << std::endl;
            for (int i = 0; i < 44; i++)
            {
                unsigned long long x = 0;
                unsigned long long y = (unsigned long long) 1 << i;
                unsigned long long z = calculate(components, x, y);
                if (x + y != z)
                {
                    std::bitset<45> zBitset(z);
                    bitErrorCount += zBitset.count() == 0 ? 1 : zBitset.count();
                    std::cout << "Y" << std::setfill('0') << std::setw(2) << i << ":" << zBitset << std::endl;

                    std::string key = "z" + ((i < 10) ? std::string("0") : std::string("")) + std::to_string(i);
                    dependents.merge(findDependents(components, components[key]));
                }
            }
            std::cout << std::endl;

            std::cout << "X = 1, Y = 1" << std::endl;
            for (int i = 0; i < 44; i++)
            {
                unsigned long long x = (unsigned long long) 1 << i;
                unsigned long long y = (unsigned long long) 1 << i;
                unsigned long long z = calculate(components, x, y);
                if (x + y != z)
                {
                    std::bitset<45> zBitset(z);
                    bitErrorCount += zBitset.count() == 0 ? 1 : zBitset.count();
                    std::cout << "XY" << std::setfill('0') << std::setw(2) << i << ":" << zBitset << std::endl;

                    std::string key = "z" + ((i < 10) ? std::string("0") : std::string("")) + std::to_string(i);
                    dependents.merge(findDependents(components, components[key]));
                }
            }
            std::cout << std::endl;

            std::cout << "Total bit errors: " << bitErrorCount;
            std::cout << std::endl;

            for (auto& conn : dependents)
            {
                std::cout << " " << conn;
            }
            std::cout << std::endl;

        }

        std::cout << std::endl;
    }
}
