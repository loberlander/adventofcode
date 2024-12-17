#include <iostream>
#include <fstream>

#include <string>
#include <set>
#include <vector>
#include <map>

static const char inputFileName[] = "input.txt";

static const std::string delimiter1 = ",";
static const std::string delimiter2 = "-";

struct Range
{
    int lowerLimit;
    int upperLimit;
};

class Job
{
public:
    Job(std::string line)
    {
        size_t tokenPosition = 0;
        size_t rangePosition = 0;

        // Parse 1st parameter
        tokenPosition = line.find(delimiter1, 0);
        if (tokenPosition != std::string::npos)
        {
            std::string r1 = line.substr(0, tokenPosition);
            line.erase(0, tokenPosition + delimiter1.length());

            std::string r2 = line;

            rangePosition = r1.find(delimiter2, 0);
            if (rangePosition != std::string::npos)
            {
                range1_.lowerLimit = std::stoi(r1.substr(0, rangePosition));
                r1.erase(0, rangePosition + delimiter2.length());

                range1_.upperLimit = std::stoi(r1);
            }

            rangePosition = r2.find(delimiter2, 0);
            if (rangePosition != std::string::npos)
            {
                range2_.lowerLimit = std::stoi(r2.substr(0, rangePosition));
                r2.erase(0, rangePosition + delimiter2.length());

                range2_.upperLimit = std::stoi(r2);
            }
        }
    }

    bool isFullyContained()
    {
        if ((range1_.lowerLimit <= range2_.lowerLimit && range1_.upperLimit >= range2_.upperLimit) 
            ||(range2_.lowerLimit <= range1_.lowerLimit && range2_.upperLimit >= range1_.upperLimit))
        {
            return true;
        }

        return false;
    }

    bool isOverlapping()
    {
        if ((range1_.lowerLimit <= range2_.lowerLimit && range1_.upperLimit >= range2_.lowerLimit)
            || (range2_.lowerLimit <= range1_.lowerLimit && range2_.upperLimit >= range1_.lowerLimit))
        {
            return true;
        }

        return false;
    }

    Range range1_;
    Range range2_;
};


typedef std::vector<Job> Jobs;

void readInputFile(std::string fileName, Jobs& data)
{
    std::string line;
    std::ifstream myFile(fileName);

    if (myFile.is_open())
    {
        while (getline(myFile, line))
        {
            if (!line.empty())
            {
                // Add new elf's data
                data.emplace_back(line);
            }
        }
        myFile.close();
    }
}


int main()
{
    std::cout << "Parsing input file..." << std::endl;
    Jobs data;
    readInputFile(inputFileName, data);

    if (data.empty())
    {
        std::cout << "Failed to read and parse input data!" << std::endl;
    }
    else
    {
        std::cout << "Solving problem..." << std::endl;
        std::cout << "Part 1:" << std::endl;
        int sum1 = 0;
        for (auto& job: data)
        {
            if (job.isFullyContained())
            {
                sum1++;
            }
        }
        std::cout << "Sum is: " << sum1 << std::endl;
        std::cout << std::endl;

        std::cout << "Part 2:" << std::endl;
        int sum2 = 0;
        for (auto& job: data)
        {
            if (job.isOverlapping())
            {
                sum2++;
            }
        }
        std::cout << "Sum is: " << sum2 << std::endl;



    }
}
