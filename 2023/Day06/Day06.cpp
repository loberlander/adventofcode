#include <iostream>
#include <fstream>

#include <string>
#include <set>
#include <vector>
#include <map>
#include <set>
#include <deque>

static const char inputFileName[] = "input.txt";
//static const char inputFileName[] = "input_test.txt";

typedef unsigned long long BigNumber;

class Problem
{
public:
    Problem(BigNumber time, BigNumber distance)
        : time_(time)
        , distance_(distance)
    {
    }

    BigNumber time_;
    BigNumber distance_;
};

typedef std::vector<Problem> Problems;


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


void readInputFile(std::string fileName)
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

    //Test Problem
    //Time:      7  15   30
    //Distance : 9  40  200
    //Problems problems1 = { {7, 9}, {15, 40}, {30, 200} };
    //Problems problems2 = { {71530, 940200}};

    //Actual Problem
    //Time:       62     73     75     65
    //Distance : 644   1023   1240   1023
    Problems problems1 = { {62, 644}, {73, 1023}, {75, 1240}, {65, 1023} };
    Problems problems2 = { {62737565, 644102312401023} };

    //readInputFile(inputFileName);

    if (problems1.empty() || problems2.empty())
    {
        std::cout << "Failed to read and parse input data!" << std::endl;
    }
    else
    {
        std::cout << "Solving problem..." << std::endl;
        std::cout << "Part 1:" << std::endl;
        {
            // Brute force find
            BigNumber factor = 1;
            for (auto& problem : problems1)
            {
                BigNumber count = 0;
                for (BigNumber i = 1; i <= problem.time_; i++)
                {
                    if (i * (problem.time_ - i) > problem.distance_)
                    {
                        count++;
                    }
                }
                factor *= count;
            }

            std::cout << "Factor is " << factor;
        }

        std::cout << std::endl;

        std::cout << "Part 2:" << std::endl;
        {
            // Brute force find
            BigNumber factor = 1;
            for (auto& problem : problems2)
            {
                BigNumber count = 0;
                for (BigNumber i = 1; i <= problem.time_; i++)
                {
                    if (i * (problem.time_ - i) > problem.distance_)
                    {
                        count++;
                    }
                }
                factor *= count;
            }

            std::cout << "Factor is " << factor;
        }

        std::cout << std::endl;

    }
}
