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
//static const char inputFileName[] = "input_test2.txt";


typedef unsigned long long BigNumber;
typedef std::vector<int> Arrangement;

class Element
{
public:
    Element(std::string& pattern, Arrangement& arrangement)
        : pattern_(pattern)
        , arrangement_(arrangement)
    {
        numPlaces_ = 0;
        for (int i = 0; i < pattern_.size(); i++)
        {
            if (pattern_[i] == '?')
            {
                numPlaces_++;
            }
        }
    }

    Element(const Element& element)
    {
        pattern_ = element.pattern_;
        arrangement_ = element.arrangement_;
        numPlaces_ = element.numPlaces_;
    }

    std::string pattern_;
    Arrangement arrangement_;
    int numPlaces_;
};

typedef std::vector<Element> Data;

typedef std::map<std::string, BigNumber> Cache;

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


void readInputFile(std::string fileName, Data& data)
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
                        auto elementSplit = split(line, " ");
                        auto numbersSplit = split(elementSplit[1], ",");

                        Arrangement arrangement;

                        for (auto& number : numbersSplit)
                        {
                            arrangement.emplace_back(std::stoi(number));
                        }

                        data.emplace_back(elementSplit[0], arrangement);
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


bool isValidArrangement(Element& element)
{

    if (element.pattern_.find('?') != std::string::npos)
    {
        return false;
    }

    int count = 0;
    Arrangement arrangement;
    std::string pattern = element.pattern_;

    while (!pattern.empty())
    {
        switch (pattern[0])
        {
            case '#':
            {
                count++;
                break;
            }
            default:
            {
                if (count > 0)
                {
                    arrangement.emplace_back(count);
                }
                count = 0;
                break;
            }
        }
        pattern.erase(0, 1);
    }

    if (count > 0)
    {
        arrangement.emplace_back(count);
    }

    if (arrangement == element.arrangement_)
    {
        return true;
    }

    return false;
}


int getNumberOfArrangements(Element& element)
{
    int total = 0;
    unsigned long variations = 0;

    if (element.numPlaces_ == 0)
    {
        return total;
    }

    variations = 1 << element.numPlaces_;

    for (unsigned long pattern = 0; pattern < variations; pattern++)
    {
        unsigned int bits = pattern;
        Element testElement(element.pattern_, element.arrangement_);
        for (int i = 0; i < element.numPlaces_; i++)
        {
            unsigned int bit = bits & 1;

            auto tokenPosition = testElement.pattern_.find("?");
            if (tokenPosition != std::string::npos)
            {
                testElement.pattern_[tokenPosition] = (bit == 1 ? '#' : '.');
            }

            bits = bits >> 1;
        }

        if (isValidArrangement(testElement))
        {
            total++;
        }

    }

    return total;
}


BigNumber count(Cache& cache, Element element, bool inABlock = false)
{
    auto countWithoutCache = [&](Cache& cache, Element element, bool inABlock = false) -> BigNumber
    {
        if (element.pattern_.empty())
        {
            // Used up all the characters from the pattern
            if (element.arrangement_.size() == 0 || (element.arrangement_.size() == 1 && element.arrangement_[0] == 0))
            {
                // Found a working arrangement
                return 1;
            }
            return 0;
        }

        if (element.arrangement_.size() == 0 || (element.arrangement_.size() == 1 && element.arrangement_[0] == 0))
        {
            // More characters are left but ran out of numbers
            if (element.pattern_.find('#') == std::string::npos)
            {
                // Found a working arrangement
                return 1;
            }
            return 0;
        }

        if (element.pattern_[0] == '#')
        {
            // A block of #s is starting or continuing
            if (inABlock && element.arrangement_[0] == 0)
            {
                // Ran out of needed counts
                return 0;
            }
            Element newElement(element);
            newElement.pattern_.erase(0, 1);
            newElement.arrangement_[0]--;
            return count(cache, newElement, true); // definitely in a block now!
        }

        if (element.pattern_[0] == '.')
        {
            if (inABlock && element.arrangement_[0] > 0)
            {
                // cannot continue, no more #s but count is not zero
                return 0;
            }

            Element newElement(element);
            newElement.pattern_.erase(0, 1);
            if (newElement.arrangement_[0] == 0)
            {
                // remove used up counter
                newElement.arrangement_.erase(newElement.arrangement_.begin());
            }
            return count(cache, newElement, false); // definitely not in block 
        }

//        if (element.pattern_[0] == '?')
//        {
            // Must be a ?
            if (inABlock)
            {
                // Already in a block
                if (element.arrangement_[0] == 0)
                {
                    // Just reached the end of a block that must end, so this ? must be a .
                    Element newElement(element);
                    newElement.pattern_.erase(0, 1);
                    // remove used up counter
                    newElement.arrangement_.erase(newElement.arrangement_.begin());
                    return count(cache, newElement, false); // definitely not in block anymore
                }

                // ? must be a #, since counts was not zero
                Element newElement(element);
                newElement.pattern_.erase(0, 1);
                newElement.arrangement_[0]--;
                return count(cache, newElement, true); // definitely in a block now!
            }
//        }

        // Must branch on a new ?
        // Assume ? is a .  not a block starting
        Element newElement1(element);
        newElement1.pattern_.erase(0, 1);

        // A block is starting
        // Assume ? is a # and a bock is starting
        Element newElement2(element);
        newElement2.pattern_.erase(0, 1);
        newElement2.arrangement_[0]--;

        // branch
        return count(cache, newElement1, false) + count(cache, newElement2, true);
    };

    std::string vectorAsString;
    for (auto& number : element.arrangement_)
    {
        vectorAsString += ":" + std::to_string(number);
    }

    // Create a key for the cache (std::map)
    std::string key = element.pattern_ + ":" + vectorAsString + ":" + (inABlock ? "1" : "0");

    auto cacheHitIt = cache.find(key);
    if (cacheHitIt != cache.end())
    {
        // Already in the cache, easy peasy
        return cacheHitIt->second;
    }

    // find the count recursively and then save it in the cache
    BigNumber cnt = countWithoutCache(cache, element, inABlock);
    cache.emplace(key, cnt);

    return cnt;
}


int main()
{
    std::cout << "Parsing input file..." << std::endl;
    Data data1;

    readInputFile(inputFileName, data1);

    if (data1.empty())
    {
        std::cout << "Failed to read and parse input data!" << std::endl;
    }
    else
    {
        std::cout << "Solving problem..." << std::endl;
        std::cout << "Part 1:" << std::endl;
        {
            BigNumber total = 0;
            for (auto& element : data1)
            {
                Cache cache;
                //total += getNumberOfArrangements(element);
                total += count(cache, element);
            }
            
            std::cout << "The total is: " << total;
            
        }

        std::cout << std::endl;
        std::cout << "Part 2:" << std::endl;
        {
            BigNumber total = 0;

            Data data2;

            for (auto& element : data1)
            {
                std::string newPattern;
                Arrangement newArrangement;
                for (int i = 0; i < 5; i++)
                {
                    newPattern += ((i== 0 ? "" : "?") + element.pattern_);
                    for (auto& arrangement : element.arrangement_)
                    {
                        newArrangement.emplace_back(arrangement);
                    }
                }

                data2.emplace_back(newPattern, newArrangement);
            }

            int loopcount = 0;
            for (auto& element : data2)
            {
                Cache cache;
                total += count(cache, element);
                loopcount++;
                //std::cout << loopcount << " : total: " << total << std::endl;
            }

/*
            for (auto& element : data1)
            {
                BigNumber total1 = getNumberOfArrangements(element);
                total += total1 * total1 * total1 * total1 * total1;
            }
*/
            std::cout << "The total is: " << total;
        }

        std::cout << std::endl;

    }
}
