#include <iostream>
#include <fstream>

#include <algorithm>
#include <string>
#include <set>
#include <vector>
#include <map>
#include <set>
#include <deque>

static const char inputFileName[] = "input.txt";
//static const char inputFileName[] = "input_test.txt";


typedef std::vector<int> Report;

typedef std::vector<Report> Reports;


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


void readInputFile(std::string fileName, Reports& reports)
{
    std::string line;
    std::ifstream myFile(fileName);

    if (myFile.is_open())
    {
        int state = 1;
        while (getline(myFile, line))
        {
            switch (state)
            {
                case 1:
                {
                    // Read Reports
                    if (!line.empty())
                    {
                        auto lineSplit = split(line, " ");

                        Report report;
                        for (auto& value : lineSplit)
                        {
                            report.emplace_back(std::stoi(value));
                        }

                        reports.emplace_back(report);
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

bool isReportSafeIncreasing(Report report)
{
    bool isSafe = true;
    int lastValue = -1;
    for (auto& value : report)
    {
        if (lastValue == -1)
        {
            // do nothing, reading the first value
        }
        else
        {
            if ((lastValue < value) && (value - lastValue >= 1) && (value - lastValue <= 3))
            {
                // keep going
            }
            else
            {
                isSafe = false;
                break;
            }
        }

        lastValue = value;
    }

    return isSafe;
}


bool isReportSafeDecreasing(Report report)
{
    bool isSafe = true;
    int lastValue = -1;
    for (auto& value : report)
    {
        if (lastValue == -1)
        {
            // do nothing, reading the first value
        }
        else
        {
            if ((lastValue > value) && (lastValue - value >= 1) && (lastValue - value <= 3))
            {
                // keep going
            }
            else
            {
                isSafe = false;
                break;
            }
        }

        lastValue = value;
    }

    return isSafe;
}


bool isReportSafe(Report report)
{
    return isReportSafeIncreasing(report) || isReportSafeDecreasing(report);
}


int main()
{
    std::cout << "Parsing input file..." << std::endl;
    Reports reports;

    readInputFile(inputFileName, reports);

    if (reports.empty())
    {
        std::cout << "Failed to read and parse input data!" << std::endl;
    }
    else
    {
        std::cout << "Solving problem..." << std::endl;
        std::cout << "Part 1:" << std::endl;
        {
            int sum = 0;

            for (auto& report : reports)
            {
                if (isReportSafe(report))
                {
                    sum++;
                }
            }

            std::cout << "Total: " << sum;
        }

        std::cout << std::endl;

        std::cout << "Part 2:" << std::endl;
        {
            int sum = 0;

            for (auto& report : reports)
            {
                if (isReportSafe(report))
                {
                    sum++;
                    continue;
                }
                else
                {
                    for (int i = 0; i < report.size(); i++)
                    {
                        Report reportTry = report;
                        reportTry.erase(reportTry.begin() + i);
                        if (isReportSafe(reportTry))
                        {
                            sum++;
                            break;
                        }
                    }
                }
            }

            std::cout << "Total: " << sum;
        }

        std::cout << std::endl;

    }
}