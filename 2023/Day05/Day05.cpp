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

static const std::string delimiterSeeds = "seeds: ";
static const std::string delimiterSeedToSoil = "seed-to-soil map:";
static const std::string delimiterSoilToFertilizer = "soil-to-fertilizer map:";
static const std::string delimiterFertilizerToWater = "fertilizer-to-water map:";
static const std::string delimiterWaterToLight = "water-to-light map:";
static const std::string delimiterLightToTemperature = "light-to-temperature map:";
static const std::string delimiterTemperatureToHumidity = "temperature-to-humidity map:";
static const std::string delimiterHumidityToLocation = "humidity-to-location map:";

static const std::string delimiterValues = " ";

typedef unsigned long long BigNumber;

class Range
{
public:
    Range(BigNumber dstStart, BigNumber srcStart, BigNumber length)
        : dstStart_(dstStart)
        , srcStart_(srcStart)
        , length_(length)
    {

    }
    BigNumber dstStart_;
    BigNumber srcStart_;
    BigNumber length_;
};

typedef std::vector<std::vector<Range>> Ranges;
typedef std::vector<BigNumber> Seeds;

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

void processRanges(std::ifstream& myFile, Ranges& ranges)
{
    std::string line;
    std::vector <Range> stepRange;

    if (myFile.is_open())
    {
        while (getline(myFile, line))
        {
            if (line.empty())
            {
                break;
            }
            std::vector<std::string> rangeAsString = split(line, delimiterValues);
            stepRange.emplace_back(std::stoull(rangeAsString[0]), std::stoull(rangeAsString[1]), std::stoull(rangeAsString[2]));
        }
    }

    if (!stepRange.empty())
    {
        ranges.emplace_back(stepRange);
    }

}


void readInputFile(std::string fileName, Seeds& seeds, Ranges& ranges)
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
                    // Read Seeds
                    size_t tokenPosition = 0;

                    tokenPosition = line.find(delimiterSeeds, 0);
                    if (tokenPosition != std::string::npos)
                    {
                        line.erase(0, tokenPosition + delimiterSeeds.length());

                        std::vector<std::string> seedsAsStrings = split(line, delimiterValues);
                        line.clear();

                        for (auto& seedAsString : seedsAsStrings)
                        {
                            seeds.emplace_back(std::stoull(seedAsString));
                        }

                        getline(myFile, line);
                    }
                    state++;
                    break;
                }
                case 2:
                {
                    // Read seed-to-soil map
                    size_t tokenPosition = 0;

                    tokenPosition = line.find(delimiterSeedToSoil, 0);
                    if (tokenPosition != std::string::npos)
                    {
                        line.erase(0, tokenPosition + delimiterSeedToSoil.length());
                        processRanges(myFile, ranges);
                    }
                    state++;
                    break;
                }
                case 3:
                {
                    // Read soil-to-fertilizer map
                    size_t tokenPosition = 0;

                    tokenPosition = line.find(delimiterSoilToFertilizer, 0);
                    if (tokenPosition != std::string::npos)
                    {
                        line.erase(0, tokenPosition + delimiterSoilToFertilizer.length());
                        processRanges(myFile, ranges);
                    }
                    state++;
                    break;
                }
                case 4:
                {
                    // Read fertilizer-to-water map
                    size_t tokenPosition = 0;

                    tokenPosition = line.find(delimiterFertilizerToWater, 0);
                    if (tokenPosition != std::string::npos)
                    {
                        line.erase(0, tokenPosition + delimiterFertilizerToWater.length());
                        processRanges(myFile, ranges);
                    }
                    state++;
                    break;
                }
                case 5:
                {
                    // Read water-to-light map
                    size_t tokenPosition = 0;

                    tokenPosition = line.find(delimiterWaterToLight, 0);
                    if (tokenPosition != std::string::npos)
                    {
                        line.erase(0, tokenPosition + delimiterWaterToLight.length());
                        processRanges(myFile, ranges);
                    }
                    state++;
                    break;
                }
                case 6:
                {
                    // Read light-to-temperature map
                    size_t tokenPosition = 0;

                    tokenPosition = line.find(delimiterLightToTemperature, 0);
                    if (tokenPosition != std::string::npos)
                    {
                        line.erase(0, tokenPosition + delimiterLightToTemperature.length());
                        processRanges(myFile, ranges);
                    }
                    state++;
                    break;
                }
                case 7:
                {
                    // Read temperature-to-humidity map
                    size_t tokenPosition = 0;

                    tokenPosition = line.find(delimiterTemperatureToHumidity, 0);
                    if (tokenPosition != std::string::npos)
                    {
                        line.erase(0, tokenPosition + delimiterTemperatureToHumidity.length());
                        processRanges(myFile, ranges);
                    }
                    state++;
                    break;
                }
                case 8:
                {
                    // Read humidity-to-location map
                    size_t tokenPosition = 0;

                    tokenPosition = line.find(delimiterHumidityToLocation, 0);
                    if (tokenPosition != std::string::npos)
                    {
                        line.erase(0, tokenPosition + delimiterHumidityToLocation.length());
                        processRanges(myFile, ranges);
                    }
                    state++;
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
    Seeds seeds;
    Ranges rangess;

    readInputFile(inputFileName, seeds, rangess);

    if (rangess.empty())
    {
        std::cout << "Failed to read and parse input data!" << std::endl;
    }
    else
    {
        std::cout << "Solving problem..." << std::endl;
        std::cout << "Part 1:" << std::endl;
        {
            BigNumber lowestLocation = ULLONG_MAX;
            for (auto& seed : seeds)
            {
                BigNumber place = seed;
                for (auto& ranges : rangess)
                {
                    bool isMapped = false;
                    for (auto& range : ranges)
                    {
                        if (range.srcStart_ <= place && place < range.srcStart_ + range.length_)
                        {
                            place = range.dstStart_ + place - range.srcStart_;
                            isMapped = true;
                            break;
                        }
                    }
                    if (!isMapped)
                    {
                        // place remains the same, no mapping is needed
                    }
                }
                if (place < lowestLocation)
                {
                    lowestLocation = place;
                }
            }

            std::cout << "Lowest location is: " << lowestLocation;
        }

        std::cout << std::endl;

        std::cout << "Part 2:" << std::endl;
        {
            BigNumber lowestLocation = ULLONG_MAX;
            for (BigNumber i = 0; i < seeds.size() / 2; i+=2)
            {
                BigNumber start = seeds[i];
                BigNumber length = seeds[i + 1];
                for (BigNumber seed = start; seed < start+length; seed++)
                {
                    BigNumber place = seed;
                    for (auto& ranges : rangess)
                    {
                        bool isMapped = false;
                        for (auto& range : ranges)
                        {
                            if (range.srcStart_ <= place && place < range.srcStart_ + range.length_)
                            {
                                place = range.dstStart_ + place - range.srcStart_;
                                isMapped = true;
                                break;
                            }
                        }
                        if (!isMapped)
                        {
                            // place remains the same, no mapping is needed
                        }
                    }
                    if (place < lowestLocation)
                    {
                        lowestLocation = place;
                    }
                }
            }
            std::cout << "Lowest location is: " << lowestLocation;
        }

        std::cout << std::endl;

    }
}
