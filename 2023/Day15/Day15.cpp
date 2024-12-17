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
//static const char inputFileName[] = "input_test1.txt";
//static const char inputFileName[] = "input_test2.txt";
//static const char inputFileName[] = "input_test3.txt";
//static const char inputFileName[] = "input_test4.txt";
//static const char inputFileName[] = "input_test5.txt";

static const std::string delimiterRemove = "-";
static const std::string delimiterReplace = "=";


typedef std::vector<std::string> Steps;

typedef unsigned long long BigNumber;

class Lens
{
public:
    Lens(std::string label, int focalLength)
        : label_(label)
        , focalLength_(focalLength)
    {
    }
    std::string label_;
    int focalLength_;
};

class Box
{
public:
    Box()
    {
    }

    void operation(char command, std::string label, int focalLength)
    {
        switch (command)
        {
            case '-': // remove lense with label
            {
                // Find lense
                for (auto it=lenses_.begin(); it < lenses_.end(); it++)
                {
                    if (it->label_ == label)
                    {
                        lenses_.erase(it);
                        break;
                    }
                }
                break;
            }
            case '=':
            {
                bool done = false;

                // Find lens
                for (auto it = lenses_.begin(); it < lenses_.end(); it++)
                {
                    if (it->label_ == label)
                    {
                        //replace it
                        it->focalLength_ = focalLength;
                        done = true;
                        break;
                    }
                }

                if (!done)
                {
                    // Add new lens at the beginning
                    lenses_.emplace_back(label, focalLength);
                }

                break;
            }
        }
    }

    std::string label_;
    std::vector<Lens> lenses_;
};

typedef std::vector<Box> Boxes;

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


unsigned char hash(std::string& step, unsigned char& current)
{
    for (auto& c : step)
    {
        unsigned int currentLocal = current;
        currentLocal += c;
        currentLocal *= 17;
        currentLocal %= 256;
        current = (unsigned char)currentLocal;
    }
    return current;
}


void readInputFile(std::string fileName, Steps& steps)
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
                    // Read Instructions
                    if (!line.empty())
                    {
                        steps = split(line, ",");
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


int main()
{
    std::cout << "Parsing input file..." << std::endl;
    Steps steps;
    Boxes boxes;

    //create empty boxes
    for (int i = 0; i < 256; i++)
    {
        boxes.emplace_back();
    }


    readInputFile(inputFileName, steps);

    if (steps.empty())
    {
        std::cout << "Failed to read and parse input data!" << std::endl;
    }
    else
    {
        std::cout << "Solving problem..." << std::endl;
        std::cout << "Part 1:" << std::endl;
        {
            BigNumber total = 0;

            for (auto& step : steps)
            {
                unsigned char currentValue = 0;
                currentValue = hash(step, currentValue);
                total += currentValue;
            }

            std::cout << "Total: " << total << std::endl;
        }

        std::cout << std::endl;

        std::cout << "Part 2:" << std::endl;
        {
            BigNumber total = 0;

            for (auto& step : steps)
            {
                std::string opCommand = step;
                char command = ' ';
                std::string label;
                int focalLength = 0;

                size_t tokenPosition;

                tokenPosition = opCommand.find(delimiterRemove, 0);
                if (tokenPosition != std::string::npos)
                {
                    command = '-';
                    label = opCommand.substr(0, tokenPosition);
                    opCommand.erase(0, tokenPosition + delimiterRemove.length());
                }
                else
                {
                    tokenPosition = opCommand.find(delimiterReplace, 0);
                    if (tokenPosition != std::string::npos)
                    {
                        command = '=';
                        label = opCommand.substr(0, tokenPosition);
                        opCommand.erase(0, tokenPosition + delimiterReplace.length());
                        focalLength = std::stoi(opCommand);
                    }
                }

                unsigned char currentValue = 0;
                int boxNumber = hash(label, currentValue);

                boxes[boxNumber].operation(command, label, focalLength);
            }

            for (BigNumber i = 0; i < 256; i++)
            {
                size_t numLenses = boxes[i].lenses_.size();
                for (BigNumber j = 0; j < numLenses; j++)
                {
                    BigNumber focusingPower = (i + 1)* (j + 1)* boxes[i].lenses_[j].focalLength_;
                    total += focusingPower;
                }
            }

            //Let's score the result
            std::cout << "Total: " << total << std::endl;
        }

        std::cout << std::endl;

    }
}
