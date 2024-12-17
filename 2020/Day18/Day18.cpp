#include <iostream>
#include <fstream>

#include <string>
#include <vector>

#include "calculator.hpp" // Manually tuned to meet criteria (long long int, precedence)

static const char inputFileName[] = "input.txt";

enum operands
{
    START = 'S',
    CONT = 'C',
    MULT = '*',
    ADD = '+',
};



long long int evaluate1(std::string& line, long long int lh, char operand)
{
    while (!line.empty())
    {
        char ch = line[0];
        line.erase(0, 1);
        switch (ch)
        {
        case '(':
        {
            long long int eval = evaluate1(line, lh, START);
            switch (operand)
            {
            case START:
                lh = eval;
                break;
            case MULT:
                lh = lh * eval;
                break;
            case ADD:
                lh = lh + eval;
                break;
            }
            break;
        }
        case ')':
        {
            return lh;
            break;
        }
        case '*':
        {
            operand = MULT;
            break;
        }
        case '+':
        {
            operand = ADD;
            break;
        }
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
        {
            long long int value = ch - '0';
            switch (operand)
            {
            case START:
                lh = value;
                break;
            case MULT:
                lh = lh * value;
                break;
            case ADD:
                lh = lh + value;
                break;
            }

            break;
        }
        default:
            break;
        }
    }

    return lh;
}

std::string removeSpaces(std::string input)
{
    std::string output;

    for (auto ch : input)
    {
        if (ch != ' ')
        {
            output += ch;
        }
    }

    return output;
}

long long int parseLine(std::string line)
{
    std::string processBuffer = removeSpaces(line);
//    std::cout << processBuffer;
    long long int val = evaluate1(processBuffer, 0, START);
//    std::cout << " = " << val << std::endl << std::endl;
    return val;
}

long long int readInputFile(std::string fileName)
{
    long long int total = 0;
    std::string line;
    std::ifstream myFile(fileName);

    if (myFile.is_open())
    {
        while (getline(myFile, line))
        {
            total += parseLine(line);
        }
        myFile.close();
    }

    return total;
}

unsigned long long int readInputFile2(std::string fileName)
{
    long long int total = 0;
    std::string line;
    std::ifstream myFile(fileName);

    if (myFile.is_open())
    {
        while (getline(myFile, line))
        {
            std::string processBuffer = removeSpaces(line);
//            std::cout << processBuffer;

            long long int val = calculator::eval(processBuffer);
//            std::cout << " = " << val << std::endl << std::endl;
            total += val;
        }
        myFile.close();
    }

    return total;
}


int main()
{
    std::cout << "Parsing input file..." << std::endl;

    std::cout << "Solving problem..." << std::endl;
    std::cout << "Part 1:" << std::endl;
    long long int result = readInputFile(inputFileName);

    std::cout << "Total: " << result << std::endl;

    std::cout << "Part 2:" << std::endl;
    long long int result2 = readInputFile2(inputFileName);

    std::cout << "Total: " << result2 << std::endl;

}
