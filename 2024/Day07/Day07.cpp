#include <iostream>
#include <fstream>

#include <string>
#include <set>
#include <vector>
#include <map>
#include <set>
#include <deque>
#include <algorithm>
#include <numeric>

static const char inputFileName[] = "input.txt";
//static const char inputFileName[] = "input_test.txt";

static const std::string delimiterValue = ":";
static const std::string delimiterNumber = " ";

typedef long long BigNumber;

typedef std::vector<BigNumber> Numbers;

enum Operation
{
    Add = '+',
    Multiply = '*',
    Concat = '|'
};

typedef std::vector<Operation> Operations;

class Equation
{
public:
    Equation(BigNumber value, Numbers numbers)
        : value_(value)
        , numbers_(numbers)
    { }

    BigNumber value_;
    Numbers numbers_;
};

typedef std::vector<Equation> Equations;


// Split a line by delimiter
std::vector<std::string> split(std::string line, std::string delimiter)
{
    std::vector<std::string> result;

    size_t tokenPosition = 0;

    while (!line.empty())
    {
        // remove any leading tokens that might be present
        tokenPosition = line.find(delimiter);
        while (tokenPosition == 0)
        {
            if (tokenPosition != std::string::npos)
            {
                line.erase(0, tokenPosition + delimiter.length());
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


void readInputFile(std::string fileName, Equations& equations)
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
                    // Read Map
                    if (!line.empty())
                    {
                        auto elements = split(line, delimiterValue);
                        BigNumber value = stoll(elements[0]);

                        auto nums = split(elements[1], delimiterNumber);

                        Numbers numbers;

                        for (auto& num : nums)
                        {
                            numbers.emplace_back(stoll(num));
                        }
                        equations.emplace_back(value, numbers);
                    }
                    else
                    {
                        
                    }
                    break;
                }
            }
        }
        myFile.close();
    }
}

BigNumber calculate(Equation equation, Operations operations)
{
    BigNumber result = 0;

    if (equation.numbers_.size() - 1 == operations.size())
    {
        result = equation.numbers_[0];
        for (int i = 0; i < operations.size(); i++)
        {
            switch (operations[i])
            {
            case Add:
                result += equation.numbers_[i + 1];
                break;
            case Multiply:
                result *= equation.numbers_[i + 1];
                break;
            case Concat:
                result = stoll(std::to_string(result) + std::to_string(equation.numbers_[i + 1]));
                break;
            default:
                // should never get here
                break;
            }
        }
    }

    return result;
}

bool isEquationSolved1(Equation& equation, Operations operations)
{
    auto numOperationsNeeded = equation.numbers_.size() - 1;

    if (numOperationsNeeded == operations.size())
    {
        if (calculate(equation, operations) == equation.value_)
        {
            return true;
        }
        return false;
    }
    else
    {
        auto operations1 = operations;

        operations1.emplace_back(Add);
        bool isSolved1 = isEquationSolved1(equation, operations1);

        if (isSolved1)
        {
            return true;
        }
        else
        {
            auto operations2 = operations;
            operations2.emplace_back(Multiply);
            return isEquationSolved1(equation, operations2);
        }
    }

    return false;
}

bool isEquationSolved2(Equation& equation, Operations operations)
{
    auto numOperationsNeeded = equation.numbers_.size() - 1;

    if (numOperationsNeeded == operations.size())
    {
        if (calculate(equation, operations) == equation.value_)
        {
            return true;
        }
        return false;
    }
    else
    {
        auto operations1 = operations;

        operations1.emplace_back(Add);
        bool isSolved1 = isEquationSolved2(equation, operations1);

        if (isSolved1)
        {
            return true;
        }
        else
        {
            auto operations2 = operations;
            operations2.emplace_back(Multiply);
            bool isSolved2 = isEquationSolved2(equation, operations2);
            if (isSolved2)
            {
                return true;
            }
            else
            {
                auto operations3 = operations;
                operations3.emplace_back(Concat);
                return isEquationSolved2(equation, operations3);
            }
        }
    }

    return false;
}


int main()
{
    std::cout << "Parsing input file..." << std::endl;
    Equations equations;

    readInputFile(inputFileName, equations);

    if (equations.empty())
    {
        std::cout << "Failed to read and parse input data!" << std::endl;
    }
    else
    {
        std::cout << "Solving problem..." << std::endl;
        std::cout << "Part 1:" << std::endl;
        {
            BigNumber sum = 0;

            for (auto& equation : equations)
            {
                Operations operations;
                if (isEquationSolved1(equation, operations))
                {
                    sum += equation.value_;
                }
            }

            std::cout << "Total: " << sum;
        }

        std::cout << std::endl;

        std::cout << "Part 2:" << std::endl;
        {
            BigNumber sum = 0;

            for (auto& equation : equations)
            {
                Operations operations;
                if (isEquationSolved2(equation, operations))
                {
                    sum += equation.value_;
                }
            }

            std::cout << "Total: " << sum;
        }

        std::cout << std::endl;

    }
}
