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

static const std::string delimiterRules = "|";
static const std::string delimiterPages = ",";

typedef std::string Rule;
typedef std::set<std::string> Rules;

typedef std::vector<int> Update;
typedef std::vector<Update> Updates;

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


void readInputFile(std::string fileName, Rules& rulesForwards, Rules& rulesBackwards, Updates& updates)
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
                    // Read page orders
                    if (!line.empty())
                    {
                        auto rule = split(line, delimiterRules);

                        // Conversion to int and then back to string is unnecessary, but let's do that to eliminate
                        // any conversion issues
                        int page0 = stoi(rule[0]);
                        int page1 = stoi(rule[1]);

                        std::string page0AsString = std::to_string(page0);
                        std::string page1AsString = std::to_string(page1);

                        rulesForwards.insert(page0AsString + delimiterRules + page1AsString);
                        rulesBackwards.insert(page1AsString + delimiterRules + page0AsString);
                    }
                    else
                    {
                        state++;
                    }
                    break;
                }
                case 2:
                {
                    // Read Updates
                    auto pages = split(line, delimiterPages);
                    Update update;
                    for (auto& page : pages)
                    {
                        update.emplace_back(stoi(page));
                    }
                    updates.emplace_back(update);

                    break;
                }

            }
        }
        myFile.close();
    }
}

bool isUpdateOrderCorrect(Rules& rulesForwards, Rules& rulesBackwards, Update& update)
{
    for (int i = 0; i < update.size() - 1; i++)
    {
        for (int j = i + 1; j < update.size(); j++)
        {
            std::string pageA = std::to_string(update[i]);
            std::string pageB = std::to_string(update[j]);
            if (rulesForwards.find(pageA + delimiterRules + pageB) == rulesForwards.end())
            {
                return false;
            }
        }
    }

    return true;
}

int findFirst(Rules& rulesForwards, Update& update)
{
    if (update.size() == 1)
    {
        return update[0];
    }

    for (int i = 0; i < update.size(); i++)
    {
        bool foundIt = true;
        for (int j = 0; j < update.size(); j++)
        {
            if (i != j)
            {
                std::string pageA = std::to_string(update[i]);
                std::string pageB = std::to_string(update[j]);
                if (rulesForwards.find(pageA + delimiterRules + pageB) == rulesForwards.end())
                {
                    // not it
                    foundIt = false;
                    continue;
                }
            }
        }
        if (foundIt)
        {
            return update[i];
        }
    }

    // Should never get here!
    // return something so that the compiler does not complain
    return update[0];
}

void fixOrder(Rules& rulesForwards, Rules& rulesBackwards, Update& update)
{
    Update updateFixed;
    Update updateFixing = update;

    for (int i = 0; i < update.size(); i++)
    {
         int first = findFirst(rulesForwards, updateFixing);
         updateFixed.emplace_back(first);

         // Remove element from list of pages to process
         for (auto it = updateFixing.begin(); it < updateFixing.end(); it++)
         {
             if(*it == first)
             {
                 updateFixing.erase(it);
                 break;
             }
         }
    }
    update = updateFixed;
}

int main()
{
    std::cout << "Parsing input file..." << std::endl;
    Rules rulesForwards;
    Rules rulesBackwards;
    Updates updates;
    Updates updatesToFix;

    readInputFile(inputFileName, rulesForwards, rulesBackwards, updates);

    if (rulesForwards.empty() || rulesBackwards.empty())
    {
        std::cout << "Failed to read and parse input data!" << std::endl;
    }
    else
    {
        std::cout << "Solving problem..." << std::endl;
        std::cout << "Part 1:" << std::endl;
        {
            int sum = 0;

            for (int i = 0; i < updates.size(); i++)
            {
                if (isUpdateOrderCorrect(rulesForwards, rulesBackwards, updates[i]))
                {
                    sum += updates[i][updates[i].size() / 2];
                }
                else
                {
                    updatesToFix.push_back(updates[i]);
                }
            }

            std::cout << "Total: " << sum;
        }

        std::cout << std::endl;

        std::cout << "Part 2:" << std::endl;
        {
            int sum = 0;

            for (int i = 0; i < updatesToFix.size(); i++)
            {
                fixOrder(rulesForwards, rulesBackwards, updatesToFix[i]);
                sum += updatesToFix[i][updatesToFix[i].size() / 2];
            }

            std::cout << "Total: " << sum;
        }

        std::cout << std::endl;

    }
}
