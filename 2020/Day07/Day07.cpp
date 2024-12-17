#include <iostream>
#include <fstream>

#include <string>
#include <vector>
#include <map>

static const char inputFileName[] = "input.txt";
static const std::string itemOfInterest = "shiny gold";

static const std::string fieldDescriptionDelimiter = " bags contain ";
static const std::string fieldItemsDelimiter = ", ";
static const std::string fieldItemsEndDelimiter = ".";
static const std::string fieldItem1Delimiter = " bag";
static const std::string fieldItemManyDelimiter = " bags";
static const std::string fieldNothingDelimiter = "no other bags.";
static const std::string fieldCountDelimiter = " ";


struct Item
{
    std::string description;
    int count;
};

struct Rule
{
    std::string bagDescription;
    std::map<std::string, int> items; // description, count
    //std::vector<struct Item> items;
};

// Format of input:
//
// dull blue bags contain 2 dotted green bags, 1 dull brown bag, 3 striped tomato bags, 5 muted blue bags.
// dotted cyan bags contain 2 faded lavender bags, 1 drab fuchsia bag, 5 bright blue bags.
// clear magenta bags contain 1 wavy salmon bag, 3 dull lime bags, 2 striped white bags.
// drab white bags contain 1 drab lavender bag, 1 plaid maroon bag.
// bright bronze bags contain 4 striped purple bags, 1 dull crimson bag, 4 dotted plum bags, 1 vibrant silver bag.
// striped tomato bags contain no other bags.

struct Rule parseRecord(std::string line)
{
    Rule rule;

    // Parse the description of the bag first
    size_t fieldDescriptionPosition = line.find(fieldDescriptionDelimiter, 0);
    if (fieldDescriptionPosition != std::string::npos)
    {
        rule.bagDescription = line.substr(0, fieldDescriptionPosition);
        line.erase(0, fieldDescriptionPosition + fieldDescriptionDelimiter.length());

        // Eliminate any special cases, i.e. no other bags ara allowed
        size_t fieldNothingPosition = line.find(fieldNothingDelimiter, 0);
        if (fieldNothingPosition != std::string::npos)
        {
            // This bag can hold nothing
        }
        else
        {
            // Parse through the list of items that can be in the bag
            while (!line.empty())
            {
                Item item;

                size_t fieldCountPosition = line.find(fieldCountDelimiter, 0);
                if (fieldCountPosition != std::string::npos)
                {
                    item.count = std::stoi(line.substr(0, fieldCountPosition));
                    line.erase(0, fieldCountPosition + fieldCountDelimiter.length());
                }

                size_t fieldItemPosition;
                if (item.count == 1)
                {
                    fieldItemPosition = line.find(fieldItem1Delimiter, 0);
                }
                else
                {
                    fieldItemPosition = line.find(fieldItemManyDelimiter, 0);
                }
                if (fieldItemPosition != std::string::npos)
                {
                    item.description = line.substr(0, fieldItemPosition);
                    line.erase(0, fieldItemPosition + fieldCountDelimiter.length());
                }

                // Add found item to our list
                rule.items.emplace(item.description, item.count);
                //rule.items.emplace(item);

                size_t fieldItemEndPosition;
                fieldItemEndPosition = line.find(fieldItemsDelimiter, 0);
                if (fieldItemEndPosition != std::string::npos)
                {
                    line.erase(0, fieldItemEndPosition + fieldItemsDelimiter.length());
                }
                else
                {
                    fieldItemEndPosition = line.find(fieldItemsEndDelimiter, 0);
                    if (fieldItemEndPosition != std::string::npos)
                    {
                        line.erase(0, fieldItemEndPosition + fieldItemsEndDelimiter.length());
                    }
                }
            }
        }
    }

    return rule;
}


std::map<std::string, struct Rule> readInputFile(std::string fileName)
{
    std::map<std::string, struct Rule> data; // Bag description, items (description, count)
    //std::map<std::string, std::vector<struct Item>> data; // Bag description, items
    std::string line;
    std::ifstream myFile(fileName);

    if (myFile.is_open())
    {
        while (getline(myFile, line))
        {
            struct Rule rule = parseRecord(line);
            data.emplace(rule.bagDescription, rule);
        }
        myFile.close();
    }

    return data;
}

void debugPrint(std::string message, int iteration)
{
/*
    std::string tabulation;
    for (int i = 0; i < iteration; i++)
    {
        tabulation += " ";
    }
    std::cout << std::endl << tabulation << message;
*/
}

int findShinyGoldBags(std::map<std::string, struct Rule> & data, struct Rule rule, int iteration)
{
    int bagCount = 0;

//    if (rule.items.empty())
//    {
//        std::cout << iteration << "->" << rule.bagDescription << " is empty." << std::endl;
//    }

    for (auto item : rule.items)
    {
        debugPrint(std::to_string(iteration) + std::string("->") + item.first, iteration);

        if (item.first == itemOfInterest)
        {
            // Found some shiny gold bags !!!
            bagCount += item.second;
            debugPrint(std::to_string(iteration) + std::string("->gold:") + std::to_string(item.second), iteration);
        }
        else
        {
            int goldCount = findShinyGoldBags(data, data[item.first], iteration + 1);  // Keep looking
            if (goldCount > 0)
            {
                debugPrint(std::to_string(iteration) + std::string("->gold:") + std::to_string(goldCount), iteration);
                bagCount = bagCount + item.second * goldCount;
            }
        }
    }

    return bagCount;
}

int findBagTotal(std::map<std::string, struct Rule>& data, struct Rule rule, int iteration)
{
    int bagCount = 0;

    for (auto item : rule.items)
    {
        debugPrint(std::to_string(iteration) + std::string("->") + item.first, iteration);

        int insideBagCount = item.second + item.second * findBagTotal(data, data[item.first], iteration + 1);  // Keep looking
        bagCount = bagCount + insideBagCount;
    }

    debugPrint(std::to_string(iteration) + std::string("->") + rule.bagDescription + "(" + std::to_string(bagCount) + ")", iteration);
    return bagCount;
}


int main()
{
    std::cout << "Parsing input file..." << std::endl;
    std::map<std::string, struct Rule> data = readInputFile(inputFileName); // bad description, items it can hold

    if (data.empty())
    {
        std::cout << "Failed to read and parse input data!" << std::endl;
    }
    else
    {
        std::cout << "Solving problem..." << std::endl;

        std::cout << "Part 1:" << std::endl;

        int shinyGoldBagCountType = 0;
        for (auto bag : data)
        {
            int shinyGoldBagCount = findShinyGoldBags(data, bag.second, 0);
            if (shinyGoldBagCount > 0)
            {
                ++shinyGoldBagCountType;
            }
            else
            {
                shinyGoldBagCountType = shinyGoldBagCountType;
            }
        }

        std::cout << std::endl;
        std::cout << "Found shiny gold bag holding colors: " << shinyGoldBagCountType << std::endl;
        std::cout << std::endl;

        std::cout << "Part 2:" << std::endl;

        int shinyGoldBagCount = findBagTotal(data, data["shiny gold"], 0);
        std::cout << std::endl;
        std::cout << "Found shiny gold bag holds : " << shinyGoldBagCount << std::endl;
        std::cout << std::endl;

    }
}
